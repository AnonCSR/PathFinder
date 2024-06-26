from SPARQLWrapper import SPARQLWrapper, JSON
from socket import timeout
import multiprocessing
import os
import re
import subprocess
import sys
import time
import traceback

# Usage:
# python benchmark.py <ENGINE> <QUERIES_FILE_ABSOLUTE_PATH> <LIMIT> <PREFIX_NAME>
# LIMIT = 0 will not add a limit

# Db engine that will execute queries
ENGINE       = sys.argv[1]
QUERIES_FILE = sys.argv[2]
LIMIT        = sys.argv[3]
PREFIX_NAME  = sys.argv[4]

BENCHMARK_ROOT = os.path.normpath(os.path.join(os.getcwd(), '..'))

# Path to needed output and input files
RESUME_FILE = f'{BENCHMARK_ROOT}/results/sparql/wdbench/{PREFIX_NAME}_{ENGINE}_limit_{LIMIT}.csv'
ERROR_FILE  = f'{BENCHMARK_ROOT}/results/sparql/wdbench/errors/{PREFIX_NAME}_{ENGINE}_limit_{LIMIT}.log'

SERVER_LOG_FILE  = f'{BENCHMARK_ROOT}/results/sparql/wdbench/log/{PREFIX_NAME}_{ENGINE}_limit_{LIMIT}.log'

###################### EDIT THIS PARAMETERS ######################
TIMEOUT = 60 # Max time per query in seconds

VIRTUOSO_LOCK_FILE = f'{BENCHMARK_ROOT}/virtuoso/wdbench/virtuoso.lck'

# use absolute paths to avoid problems with current directory
ENGINES_PATHS = {
    'BLAZEGRAPH': f'{BENCHMARK_ROOT}/blazegraph/service',
    'JENA':       f'{BENCHMARK_ROOT}/jena/apache-jena-fuseki-4.7.0',
    'VIRTUOSO':   f'{BENCHMARK_ROOT}/virtuoso',
}

ENGINES_PORTS = {
    'BLAZEGRAPH': 9999,
    'JENA':       3030,
    'VIRTUOSO':   1111,
}

ENDPOINTS = {
    'BLAZEGRAPH': 'http://localhost:9999/bigdata/namespace/wdq/sparql',
    'JENA':       'http://localhost:3030/jena/sparql',
    'VIRTUOSO':   'http://localhost:8890/sparql',
}

SERVER_CMD = {
    'BLAZEGRAPH': ['./runBlazegraph.sh'],
    'JENA': f'java -Xmx64g -jar fuseki-server.jar --loc=wikidata --timeout={TIMEOUT*1000} /jena'.split(' '),
    'VIRTUOSO': ['bin/virtuoso-t', '-c', 'wikidata.ini', '+foreground'],
}
#######################################################

PORT = ENGINES_PORTS[ENGINE]

server_log = open(SERVER_LOG_FILE, 'w')
server_process = None

# Check if output file already exists
if os.path.exists(RESUME_FILE):
    print(f'File {RESUME_FILE} already exists.')
    sys.exit()

# ================== Auxiliars ===============================
def lsof(pid):
    process = subprocess.Popen(['lsof', '-a', f'-p{pid}', f'-i:{PORT}', '-t'], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    if ENGINE == 'RDF4J':
        process = subprocess.Popen(['lsof', '-i', f'-i:{PORT}'], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    out, _ = process.communicate()
    return out.decode('UTF-8').rstrip()

def lsofany():
    process = subprocess.Popen(['lsof', '-t', f'-i:{PORT}'], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    out, _ = process.communicate()
    return out.decode('UTF-8').rstrip()


# ================== Parsers =================================
def parse_to_sparql(query):
    if not LIMIT:
        return f'SELECT * WHERE {{ {query} }}'
    return f'SELECT * WHERE {{ {query} }} LIMIT {LIMIT}'


def start_server():
    global server_process
    os.chdir(ENGINES_PATHS[ENGINE])
    print('starting server...')

    server_log.write("[start server]\n")
    server_process = subprocess.Popen(SERVER_CMD[ENGINE], stdout=server_log, stderr=server_log)

    # Sleep to wait server start
    while not lsof(server_process.pid):
        time.sleep(1)

    print(f'done')


def kill_server():
    global server_process
    print(f'killing server[{server_process.pid}]...')
    server_log.write("[kill server]\n")
    if ENGINE == 'VIRTUOSO':
        kill_process = subprocess.Popen([f'{ENGINES_PATHS[ENGINE]}/bin/isql', f'localhost:{PORT}', '-K'])
        kill_process.wait()
    elif ENGINE == 'RDF4J':
        kill_process = subprocess.Popen([f'{ENGINES_PATHS[ENGINE]}/bin/shutdown.sh'])
        kill_process.wait()
    else:
        server_process.kill()
        server_process.wait()

    while lsof(server_process.pid):
        time.sleep(1)

    if ENGINE == 'VIRTUOSO':
        kill_process = subprocess.Popen(['rm', VIRTUOSO_LOCK_FILE], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
        kill_process.wait()
    print('done')


# Send query to server
def execute_queries():
    with open(QUERIES_FILE) as queries_file:
        for line in queries_file:
            query_number, query = line.split(',')
            print(f'Executing query {query_number}')
            query_sparql(query, query_number)


def execute_sparql_wrapper(query_pattern, query_number):
    query = parse_to_sparql(query_pattern)

    sparql_wrapper = SPARQLWrapper(ENDPOINTS[ENGINE])
    # sparql_wrapper.setTimeout(TIMEOUT+10) # Give 10 more seconds for a chance to graceful timeout
    sparql_wrapper.setReturnFormat(JSON)
    sparql_wrapper.setQuery(query)

    count = 0
    start_time = time.time()

    try:
        # Compute query
        results = sparql_wrapper.query()
        json_results = results.convert()
        for _ in json_results["results"]["bindings"]:
            count += 1

        print(count)
        elapsed_time = int((time.time() - start_time) * 1000) # Truncate to milliseconds

        with open(RESUME_FILE, 'a') as file:
            file.write(f'{query_number},{count},OK,{elapsed_time}\n')

    except Exception as e:
        elapsed_time = int((time.time() - start_time) * 1000) # Truncate to milliseconds
        with open(RESUME_FILE, 'a') as file:
            file.write(f'{query_number},,ERROR({type(e).__name__}),{elapsed_time}\n')

        with open(ERROR_FILE, 'a') as file:
            file.write(f'Exception in query {str(query_number)} [{type(e).__name__}]: {str(e)}\n')


def query_sparql(query_pattern, query_number):
    start_time = time.time()

    try:
        p = multiprocessing.Process(target=execute_sparql_wrapper, args=[query_pattern, query_number])
        p.start()
        # Give 2 more seconds for a chance to graceful timeout or enumerate the results
        p.join(TIMEOUT + 2)
        if p.is_alive():
            p.kill()
            p.join()
            raise Exception("PROCESS_TIMEOUT")

    except Exception as e:
        elapsed_time = int((time.time() - start_time) * 1000) # Truncate to milliseconds
        with open(RESUME_FILE, 'a') as file:
            file.write(f'{query_number},,TIMEOUT({type(e).__name__}),{elapsed_time}\n')

        with open(ERROR_FILE, 'a') as file:
            file.write(f'Exception in query {str(query_number)} [{type(e).__name__}]: {str(e)}\n')

        kill_server()
        start_server()


with open(RESUME_FILE, 'w') as file:
    file.write('query_number,results,status,time\n')

with open(ERROR_FILE, 'w') as file:
    file.write('') # to replaces the old error file

if lsofany():
    raise Exception("other server already running")

print('benchmark is starting. TIMEOUT', TIMEOUT, 'seconds')
start_server()
execute_queries()

if server_process is not None:
    kill_server()

server_log.close()