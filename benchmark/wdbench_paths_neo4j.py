from neo4j import GraphDatabase
import time
import sys
import os

if len(sys.argv) < 2:
    print('usage:')
    print('python3 wdbench_paths_neo4j.py <QUERIES_FILE_PATH> <FUNCTION_NAME?>')
    exit(1)

LIMIT = 100000
QUERIES_FILE = sys.argv[1]

RESUME_FILE = '../results/neo4j/wdbench_all_trails.csv'
NEO4J_DATABASE = 'wdbench'
FUNCTION_NAME = ''

if len(sys.argv) >= 3:
    FUNCTION_NAME  = sys.argv[2]
    if FUNCTION_NAME != 'shortestPath' and FUNCTION_NAME != 'allShortestPaths':
        print(f"Path function '{FUNCTION_NAME}' not recognized. Supported are 'shortestPath' and 'allShortestPaths'.")
        exit(1)
    RESUME_FILE = f'results/PATHS_NEO4J_{FUNCTION_NAME}.csv'


# Check if output file already exists
if os.path.exists(RESUME_FILE):
    print(f'File {RESUME_FILE} already exists.')
    sys.exit()
else:
    with open(RESUME_FILE, 'w', encoding='UTF-8') as file:
        file.write('query_number,results,status,time\n')


def execute_query(session, cypher_pattern, query_number):
    if FUNCTION_NAME == '':
        cypher_query = f'MATCH {cypher_pattern} RETURN *'
    else:
        cypher_query = f'MATCH p = {FUNCTION_NAME}( {cypher_pattern} ) RETURN p'

    if LIMIT:
        cypher_query += f' LIMIT {LIMIT}'

    result_count = 0
    start_time = time.time()
    try:
        print("executing query", query_number, cypher_query)
        results = session.execute_read(lambda tx: tx.run(cypher_query).data())
        for _ in results:
            result_count += 1
        elapsed_time = int((time.time() - start_time) * 1000) # Truncate to milliseconds
        with open(RESUME_FILE, 'a', encoding='UTF-8') as output_file:
            output_file.write(f'{query_number},{result_count},OK,{elapsed_time}\n')

    except Exception as e:
        elapsed_time = int((time.time() - start_time) * 1000) # Truncate to milliseconds
        with open(RESUME_FILE, 'a', encoding='UTF-8') as output_file:
            output_file.write(f'{query_number},{result_count},ERROR({type(e).__name__}),{elapsed_time}\n')
        print(e)



with open(QUERIES_FILE, 'r', encoding='UTF-8') as queries_file:
    driver = GraphDatabase.driver('bolt://127.0.0.1:7687')
    with driver.session(database=NEO4J_DATABASE) as session:
        for line in queries_file:
            splitted_by_comma = line.strip().split(',')
            query_number = splitted_by_comma[0]
            query = ','.join(splitted_by_comma[1:])
            execute_query(session, query, query_number)
    driver.close()