import os
import sys
import time
from nebula3.common import *
from nebula3.Config import SessionPoolConfig
from nebula3.Exception import IOErrorException
from nebula3.gclient.net.SessionPool import SessionPool
from subprocess import run

"""
Run: python run_nebula.py [DB_NAME] [MODE] [TIMEOUT_MAX]
    [DB_NAME] Name of the database
    [MODE] Semantic mode (0: ALL TRAILS, 1: ANY SHORTEST TRAILS, 2: ALL SHORTEST TRAILS, 3: ALL WALKS, 4: ENDPOINTS, 5: ALL FIXED)
    [TIMEOUT_MAX] Max amount of consecutive timeouts (diamond: 1, pokec: 11, wdbench: inf, endpoints: 2, default: 1)
"""

# Relevant paths
ROOT_DIR = os.path.normpath(os.path.join(os.getcwd(), '..'))
ENGINE_DIR = os.path.join(ROOT_DIR, 'nebula')
QUERY_DIR = os.path.join(ROOT_DIR, 'queries/nebula')
RESULTS_DIR = os.path.join(ROOT_DIR, 'results/nebula')

# Timeout settings
TIMEOUT = 60  # Timeout in seconds
TIMEOUT_COUNT = 0  # Current number of timeouts
TIMEOUT_MAX = 1  # Max amount of consecutive timeouts

# Networking settings
IP = '127.0.0.1'
PORT = 9669
SERVICE_PORTS = [9559, 9669, 9779]

# Start client
def start_client(db_name):
    config = SessionPoolConfig()
    config.timeout = int(TIMEOUT * 1000)
    client = SessionPool('root', 'nebula', db_name, [(IP, PORT)])
    client.init(config)
    return client

# Close client and sessions
def close_client(client):
    client.execute('SHOW SESSIONS | KILL SESSIONS $-.SessionId;')
    client.close()

# Check if server ports are listening
def is_listening(ports):
    status = []
    for port in ports:
        command = run(['sudo', 'lsof', '-t', f'-i:{port}'], capture_output=True, text=True)
        listeners = [pid for pid in command.stdout.split('\n') if pid not in ('', str(os.getpid()))]
        status.append(bool(listeners))
    if True not in status:
        return 'none'
    if False not in status:
        return 'all'
    return 'some'

# Start server
def start_server():
    if is_listening(SERVICE_PORTS) == 'all':  # Bypass server start
        time.sleep(5)
        return
    wait_count = 0  # Amount of waiting before retrying
    run(['sudo', '/usr/local/nebula/scripts/nebula.service', 'start', 'all'], capture_output=True)
    while is_listening(SERVICE_PORTS) != 'all':
        if wait_count == 10:
            wait_count = 0
            close_server()  # To avoid infinite loop when start fails to awaken all services
            run(['sudo', '/usr/local/nebula/scripts/nebula.service', 'start', 'all'], capture_output=True)
        time.sleep(1)
        wait_count += 1
    time.sleep(5)  # Wait to avoid problems with Nebula's latency system

# Close server
def close_server():
    wait_count = 0  # Amount of waiting before retrying
    run(['sudo', '/usr/local/nebula/scripts/nebula.service', 'stop', 'all'], capture_output=True)
    while is_listening(SERVICE_PORTS) != 'none':
        if wait_count == 10:
            wait_count = 0
            run(['sudo', '/usr/local/nebula/scripts/nebula.service', 'stop', 'all'], capture_output=True)
        time.sleep(1)
        wait_count += 1
    time.sleep(5)  # Wait to avoid problems with Nebula's latency system

# Write timeout to results file
def query_timeout(query, query_number, results_file):
    with open(results_file, 'a') as out_file:
        out_file.write(f'{query_number},{query[:-1].replace(",", "")},0,TIMEOUT,-1,-1\n')

# Execute a single query
def execute_query(client, query_line, results_file):
    split_query = query_line.strip('\n').split(',')
    query_number = split_query[0]
    query = ','.join(split_query[1:])

    # If already timed out multiple consecutive times, assume timeout
    if TIMEOUT_COUNT >= TIMEOUT_MAX:
        query_timeout(query, query_number, results_file)
        return True

    # Execute new query
    try:
        start = time.time()
        results = client.execute(query)
        exec_time = int((time.time() - start) * 1000)
        if not results.is_succeeded():  # Debug errors
            print(results.error_msg())
            raise IOErrorException
        n_results = results.row_size()
        enum_time = int((time.time() - start) * 1000)
        with open(results_file, 'a') as out_file:
            out_file.write(f'{query_number},{query[:-1].replace(",", "")},{n_results},OK,{exec_time},{enum_time}\n')
        return False
    except IOErrorException:  # Timeout
        close_client(client)
        close_server()
        query_timeout(query, query_number, results_file)
        return True

# Run benchmark for NebulaGraph with a specific mode on a database
if __name__ == '__main__':
    try:
        database_name = sys.argv[1]
        chosen_mode = int(sys.argv[2])
        if len(sys.argv) == 4:
            if sys.argv[3] == 'inf':
                TIMEOUT_MAX = float('inf')
            else:
                TIMEOUT_MAX = int(sys.argv[3])
        modes = ['all_trails', 'any_shortest_trails', 'all_shortest_trails', 'all_walks', 'endpoints', 'all_fixed']
        mode = modes[chosen_mode]
        print(f'Engine: NebulaGraph\nMode: {mode}\nDatabase: {database_name}\n')
        query_path = os.path.join(QUERY_DIR, f'{database_name}_{mode}.txt')
        results_path = os.path.join(RESULTS_DIR, f'{database_name}_{mode}.csv')
        print('Loading Database...')
        start_server()
        client_process = start_client(database_name)
        print('Starting Benchmark...\n')
        with open(results_path, 'w') as out_file:
            out_file.write('query_number,query,results,status,exec_time_ms,enum_time_ms\n')
        with open(query_path, 'r') as query_file:
            queries = query_file.readlines()
        for idx, query_line in enumerate(queries):
            print(f'Processing Query... {idx + 1}/{len(queries)}', end='', flush=True)
            timed_out = execute_query(client_process, query_line, results_path)
            if timed_out:
                print('   TIMEOUT')
                TIMEOUT_COUNT += 1
                if TIMEOUT_COUNT < TIMEOUT_MAX:  # Restart server/client if max timeouts is not reached
                    start_server()
                    client_process = start_client(database_name)
            else:
                print('   OK')
                TIMEOUT_COUNT = 0
        if TIMEOUT_COUNT < TIMEOUT_MAX:  # Close server/client only if necessary
            close_client(client_process)
            close_server()
        print('\nBenchmark Finished!')
    except (IndexError, ValueError):
        print('Args are either missing or wrong!')
