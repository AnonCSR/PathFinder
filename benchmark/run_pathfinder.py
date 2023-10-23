import os
import sys
import time
from subprocess import run, Popen, DEVNULL

"""
Run: python run_pathfinder.py [DB_NAME] [MODE] [TIMEOUT_MAX] [SEARCH_STRATEGY]
    [DB_NAME] Name of the database
    [MODE] Semantic mode (0: ALL TRAILS, 1: ANY SHORTEST WALKS, 2: ALL SHORTEST WALKS, 3: ANY TRAILS, 4: ENDPOINTS, 5: ALL FIXED)
    [TIMEOUT_MAX] Max amount of consecutive timeouts (diamond: 1, pokec: 11, wdbench: inf, endpoints: 2, default: 1)
    [SEARCH_STRATEGY] Search algorithm to use when exploring the graph: bfs/dfs (default: bfs)
"""

# Relevant paths
ROOT_DIR = os.path.normpath(os.path.join(os.getcwd(), '..'))
ENGINE_DIR = os.path.join(ROOT_DIR, 'pathfinder')
QUERY_DIR = os.path.join(ROOT_DIR, 'queries/pathfinder')
RESULTS_DIR = os.path.join(ROOT_DIR, 'results/pathfinder')

# Timeout settings
TIMEOUT = 60  # Timeout in seconds
TIMEOUT_COUNT = 0  # Current number of timeouts
TIMEOUT_MAX = 1  # Max amount of consecutive timeouts

# Networking settings
PORT = 8080

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
def start_server(db_name, path_mode):
    os.chdir(ENGINE_DIR)
    database = f'dbs/{db_name}'
    server = Popen(['build/Release/bin/pf-server', database, '-t', f'{TIMEOUT}', '--path-mode', path_mode, '--shared-buffer', '64GB'],
                   stdin=DEVNULL, stdout=DEVNULL, stderr=DEVNULL)
    while is_listening([PORT]) != 'all':
        time.sleep(1)
    return server

# Close server
def close_server(server):
    run(['rm', 'results.tmp'])
    server.terminate()
    server.wait()

# Write timeout to results file
def query_timeout(query, query_number, results_file):
    with open(results_file, 'a') as out_file:
        out_file.write(f'{query_number},{query.replace(",", "..")},0,TIMEOUT,-1,-1\n')

# Execute a single query
def execute_query(query_line, results_file):
    split_query = query_line.strip('\n').split(',')
    query_number = split_query[0]
    query = ','.join(split_query[1:])

    # If already timed out multiple consecutive times, assume timeout
    if TIMEOUT_COUNT >= TIMEOUT_MAX:
        query_timeout(query, query_number, results_file)
        return True

    # Execute new query
    with open('results.tmp', 'w') as out_file:
        start = time.time()
        run(['build/Release/bin/pf-query-mql'], input=query, stdout=out_file, stderr=DEVNULL, text=True)
        exec_time = int((time.time() - start) * 1000)
        enum_time = exec_time
    if exec_time <= TIMEOUT * 1000:
        line_count = run(['wc', '-l', 'results.tmp'], capture_output=True, text=True)
        n_results = int(line_count.stdout.strip('\n').split()[0]) - 1
        with open(results_file, 'a') as out_file:
            out_file.write(f'{query_number},{query.replace(",", "..")},{n_results},OK,{exec_time},{enum_time}\n')
        return False
    query_timeout(query, query_number, results_file)  # Timeout
    return True

# Run benchmark for PathFinder with a specific mode on a database
if __name__ == '__main__':
    try:
        database_name = sys.argv[1]
        chosen_mode = int(sys.argv[2])
        if len(sys.argv) >= 4:
            if sys.argv[3] == 'inf':
                TIMEOUT_MAX = float('inf')
            else:
                TIMEOUT_MAX = int(sys.argv[3])
        path_mode = 'bfs'
        if len(sys.argv) == 5 and sys.argv[4] == 'dfs':
            path_mode = 'dfs'
        modes = ['all_trails', 'any_shortest_walks', 'all_shortest_walks', 'any_trails', 'endpoints', 'all_fixed']
        mode = modes[chosen_mode]
        print(f'Engine: PathFinder\nMode: {mode} ({path_mode})\nDatabase: {database_name}\n')
        query_path = os.path.join(QUERY_DIR, f'{database_name}_{mode}.txt')
        results_path = os.path.join(RESULTS_DIR, f'{database_name}_{mode}_{path_mode}.csv')
        print('Loading Database...')
        server_process = start_server(database_name, path_mode)
        print('Starting Benchmark...\n')
        with open(results_path, 'w') as out_file:
            out_file.write('query_number,query,results,status,exec_time_ms,enum_time_ms\n')
        with open(query_path, 'r') as query_file:
            queries = query_file.readlines()
        for idx, query_line in enumerate(queries):
            print(f'Processing Query... {idx + 1}/{len(queries)}', end='', flush=True)
            timed_out = execute_query(query_line, results_path)
            if timed_out:
                print('   TIMEOUT')
                TIMEOUT_COUNT += 1
            else:
                print('   OK')
                TIMEOUT_COUNT = 0
        close_server(server_process)
        print('\nBenchmark Finished!')
    except (IndexError, ValueError):
        print('Args are either missing or wrong!')
