from __future__ import annotations

import multiprocessing
import os
import re
import subprocess
import sys
import time
from subprocess import Popen

# import traceback
# from socket import timeout

###################### EDIT THIS PARAMETERS ######################
TIMEOUT = 60  # Max time per query in seconds
##################################################################

ROOT_DIR = os.path.normpath(os.path.join(os.getcwd(), '..'))
PF_ROOT = os.path.join(ROOT_DIR, 'pathfinder')
PF_OUT = os.path.join(ROOT_DIR, 'results/pathfinder/wdbench')

PORT = 8080


def lsof(pid: int) -> str:
    process = subprocess.Popen(
        ["lsof", "-a", f"-p{pid}", f"-i:{PORT}", "-t"], stdout=subprocess.PIPE, stderr=subprocess.PIPE
    )
    out, _ = process.communicate()
    return out.decode("UTF-8").rstrip()


def lsof_any() -> str:
    process = subprocess.Popen(["lsof", "-t", f"-i:{PORT}"], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    out, _ = process.communicate()
    return out.decode("UTF-8").rstrip()


def start_server() -> Popen[bytes]:
    print("starting server...")

    server_log.write("[start server]\n")
    server_process = subprocess.Popen(SERVER_CMD, stdout=server_log, stderr=server_log)

    # Sleep to wait server start
    while not lsof(server_process.pid):
        time.sleep(1)

    print("done")
    return server_process


def kill_server(server_process: Popen[bytes]) -> None:
    print(f"killing server[{server_process.pid}]...")
    server_log.write("[kill server]\n")

    server_process.kill()
    server_process.wait()

    while lsof(server_process.pid):
        time.sleep(1)

    print("done")


def parse_to_mql(query: str) -> None:
    query_parts = query.strip().split(" ")
    from_string = query_parts[0]
    property_path = " ".join(query_parts[1 : len(query_parts) - 1])
    end_string = query_parts[len(query_parts) - 1]

    pf_semantic = PATH_SEMANTIC.replace("_", " ")

    # Parse subject
    if from_string[0] == "?":
        from_string = f"({from_string})=[{pf_semantic} ?p "
    else:
        from_string = "(" + from_string.split("/")[-1].replace(">", "") + f")=[{pf_semantic} ?p "

    # Parse object
    if end_string[0] == "?":
        end_string = f"]=>({end_string})"
    else:
        end_string = "]=>(" + end_string.split("/")[-1].replace(">", "") + ")"

    # Parse SPARQL property path
    pattern = r"\<[a-zA-Z0-9\/\.\:\#]*\>"
    path_edges = re.findall(pattern, property_path)
    clean_property_path = property_path

    for path in path_edges:
        clean_path = ":" + path.split("/")[-1].replace(">", "")
        clean_property_path = re.sub(path, clean_path, clean_property_path, flags=re.MULTILINE)

    print(query.strip())
    print(f"MATCH {from_string}{clean_property_path}{end_string} RETURN ?p LIMIT {LIMIT}")
    print("")

    with open(PF_QUERY_FILE, "w", encoding="utf-8") as file:
        file.write(f"MATCH {from_string}{clean_property_path}{end_string} RETURN ?p")
        if LIMIT:
            file.write(f" LIMIT {LIMIT}")


# Send query to server
def execute_queries(server_process: Popen[bytes]) -> Popen[bytes]:
    with open(QUERIES_FILE, encoding="utf-8") as queries_file:
        for line in queries_file:
            query_number, query = line.split(",")
            print(f"Executing query {query_number}")
            server_process = query_pf(server_process, query, query_number)
    return server_process


def execute_mql_query(query_pattern: str, query_number: str, pid: int) -> None:
    parse_to_mql(query_pattern)
    start_time = time.time()
    with open(PF_RESULTS_FILE, "w", encoding="utf-8") as results_file, open(
        PF_QUERY_FILE, "r", encoding="utf-8"
    ) as query_file:
        query_execution = subprocess.Popen(
            ["./build/Release/bin/pf-query-mql"], stdin=query_file, stdout=results_file, stderr=subprocess.DEVNULL
        )
    exit_code = query_execution.wait()
    elapsed_time = int((time.time() - start_time) * 1000)
    p = subprocess.Popen(["wc", "-l", PF_RESULTS_FILE], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    result, _ = p.communicate()
    results_count = int(result.strip().split()[0]) - 1  # 1 line from header

    mem_cmd = f"grep ^VmRSS /proc/{pid}/status".split(" ")
    process = subprocess.Popen(mem_cmd, universal_newlines=True, stdout=subprocess.PIPE)
    out_cmd, _ = process.communicate()

    if len(out_cmd.strip().split()) > 1:
        out_cmd = out_cmd.strip().split()[1]

    with open(RESUME_FILE, "a", encoding="utf-8") as file:
        if exit_code == 0:
            file.write(f"{query_number},{results_count},OK,{elapsed_time},{out_cmd}\n")
        else:
            if elapsed_time >= TIMEOUT:
                file.write(f"{query_number},{results_count},TIMEOUT,{elapsed_time},{out_cmd}\n")
            else:
                file.write(f"{query_number},0,ERROR,{elapsed_time},{out_cmd}\n")


def query_pf(server_process: Popen[bytes], query_pattern: str, query_number: str) -> Popen[bytes]:
    start_time = time.time()

    try:
        p = multiprocessing.Process(target=execute_mql_query, args=[query_pattern, query_number, server_process.pid])
        p.start()
        # Give 2 more seconds for a chance to graceful timeout or enumerate the results
        p.join(TIMEOUT + 2)
        if p.is_alive():
            p.kill()
            p.join()
            raise Exception("PROCESS_TIMEOUT")
        return server_process

    except Exception as e:
        elapsed_time = int((time.time() - start_time) * 1000)  # Truncate to milliseconds
        with open(RESUME_FILE, "a", encoding="utf-8") as file:
            file.write(f"{query_number},,PROCESS TIMEOUT({type(e).__name__}),{elapsed_time}\n")

        with open(ERROR_FILE, "a", encoding="utf-8") as file:
            file.write(f"Exception in query {query_number} [{type(e).__name__}]: {str(e)}\n")

        kill_server(server_process)
        return start_server()


if __name__ == "__main__":
    if len(sys.argv) != 5:
        print("usage:")
        print(f"python3 {os.path.basename(__file__)} <QUERIES_FILE> <LIMIT> <PATH_SEMANTIC> <bfs|dfs>")
        print("LIMIT = 0 will not add a limit")
        sys.exit(1)

    # Db engine that will execute queries
    QUERIES_FILE = os.path.abspath(sys.argv[1])
    LIMIT = sys.argv[2]
    PATH_SEMANTIC = sys.argv[3]
    PATH_MODE = sys.argv[4]

    # Path to needed output and input files
    RESUME_FILE = f"{PF_OUT}/{PATH_SEMANTIC}_{PATH_MODE}_limit{LIMIT}.csv"
    ERROR_FILE = f"{PF_OUT}/errors/{PATH_SEMANTIC}_{PATH_MODE}_limit{LIMIT}.log"
    SERVER_LOG_FILE = f"{PF_OUT}/log/{PATH_SEMANTIC}_{PATH_MODE}_limit{LIMIT}.log"

    PF_QUERY_FILE = os.path.abspath(".wdbench_query.tmp")
    PF_RESULTS_FILE = os.path.abspath(".wdbench_results.tmp")
    SERVER_CMD = f"build/Release/bin/pf-server dbs/wdbench --path-mode {PATH_MODE} -t {TIMEOUT} --load-strings 10GB --shared-buffer 64GB".split()

    if lsof_any():
        raise Exception(f"other server already running in port {PORT}")

    # Check if output file already exists
    if os.path.exists(RESUME_FILE):
        print(f"File {RESUME_FILE} already exists.")
        sys.exit()

    server_log = open(SERVER_LOG_FILE, "w", encoding="utf-8")
    os.chdir(PF_ROOT)

    with open(RESUME_FILE, "w", encoding="utf-8") as file:
        file.write("query_number,results,status,time,mem[kB]\n")

    with open(ERROR_FILE, "w", encoding="utf-8") as file:
        file.write("")  # to replace the old error file

    print("benchmark is starting. TIMEOUT:", TIMEOUT, "seconds")
    server_process = start_server()
    server_process = execute_queries(server_process)

    # Delete temp file
    subprocess.Popen(["rm", PF_RESULTS_FILE], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    subprocess.Popen(["rm", PF_QUERY_FILE], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    kill_server(server_process)

    server_log.close()