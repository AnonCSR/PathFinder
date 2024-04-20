PathFinder Repository <!-- omit in toc -->
================================================================================
This repository contains the source code, scripts and instructions for the experiments in the paper:

**PathFinder: Returning Paths in Graph Queries**

The extended version of the paper can be found here:

[PathFinder](Paper_extended.pdf)
  - Contains pipelined pseudo-code for all the algorithms realized through linear iterators (AppendixA); and
  - Additional experiments that allow us to highlight different aspects of the proposed algorithms (AppendixB)

Given that `AnonymousDB`, which is the foundation `PathFinder` is built on, supports both property graphs with a `GQL`-like language
 and graphs with `RDF (SPARQL 1.1)`, we also provide a small demonstration of how the proposed approach allows extending `SPARQL`
 with the ability to return paths: [Returning Paths in SPARQL](pathfinder/SPARQL.md)

We remark that to apply our approach to `SPARQL`, we can use the same algorithm (see the implementation [here](pathfinder/src/query/executor/binding_iter/paths/any_walks/bfs_enum.cc)), but processing the data using the `RDF` index instead of the property paths one.

We next explain how one can replicate the experiments in the paper (including the ones from the extended version).

**IMPORTANT:** All steps for running the experiments require `python 3.8` to be installed.

Table of Contents <!-- omit in toc -->
================================================================================
- [Data Preparation](#data-preparation)
- [Engines Setup](#engines-setup)
- [Running the Experiments](#running-the-experiments)
- [PathFinder](#pathfinder)
    - [Pokec](#pokec)
    - [Diamond](#diamond)
    - [Wikidata](#wikidata)
- [Neo4j](#neo4j)
    - [Pokec](#pokec-1)
    - [Diamond](#diamond-1)
    - [Wikidata](#wikidata-1)
- [Kuzu](#kuzu)
    - [Pokec](#pokec-2)
    - [Diamond](#diamond-2)
- [NebulaGraph](#nebulagraph)
    - [Pokec](#pokec-3)
    - [Diamond](#diamond-3)
- [SPARQL](#sparql)
    - [Pokec](#pokec-4)
    - [Wikidata](#wikidata-2)

Data Preparation
================================================================================

Follow the instructions in the [Data Preparation Section](dbs/README.md).

Engines Setup
================================================================================

For each engine that you want to test, follow the corresponding steps to prepare them for the benchmark:

[PathFinder](pathfinder/README.md)

[Neo4j](neo4j/README.md)

[Kuzu](kuzu/README.md)

[NebulaGraph](nebula/README.md)

If you also want to test the `SPARQL` engines `(Jena, Blazegraph, Virtuoso)`, refer to their respective websites for instructions on installing and importing the data.

For details on how to generate and load the `RDF` data into these `SPARQL` engines, refer to the [Data Preparation Section](dbs/README.md).

Running the Experiments
================================================================================

From here on, execute all commands from inside the `benchmark` folder in the repository.

PathFinder
================================================================================

### Pokec

- Run Benchmark: `python run_pathfinder.py pokec [MODE] [TIMEOUT_MAX] [SEARCH_STRATEGY]`

- The `[MODE]` parameter is an integer that represents which semantic mode to test, it can take the following values:
  - `0` ALL TRAILS
  - `1` ANY SHORTEST WALKS
  - `2` ALL SHORTEST WALKS
  - `3` ANY TRAILS
  - `4` ENDPOINTS

- The `[TIMEOUT_MAX]` parameter controls how many timeouts in a row should be encountered before stopping the benchmark
- The `[SEARCH_STRATEGY]` parameter controls the search algorithm to use when exploring the graph, can be either `bfs` or `dfs`
- Results are stored inside the `results/pathfinder` folder in the repository

### Diamond

- Run Benchmark: `python run_pathfinder.py diamond [MODE] [TIMEOUT_MAX] [SEARCH_STRATEGY]`

- The `[MODE]` parameter is an integer that represents which semantic mode to test, it can take the following values:
  - `0` ALL TRAILS
  - `1` ANY SHORTEST WALKS
  - `2` ALL SHORTEST WALKS
  - `3` ANY TRAILS

- The `[TIMEOUT_MAX]` and `[SEARCH_STRATEGY]` parameters are the same as before
- Results are stored inside the `results/pathfinder` folder in the repository

### Wikidata

- Run Benchmark: `python wdbench-quad-paths.py ../queries/pathfinder/wdbench.txt 100000 [MODE] [SEARCH_STRATEGY]`

- The `[MODE]` parameter is a string which indicates the semantic mode to test, it can take the following values:
  - `any` ANY WALKS
  - `all_shortest` ALL SHORTEST WALKS (only allows `bfs` search strategy)
  - `any_trails` ANY TRAILS
  - `all_trails` ALL TRAILS
  - `all_shortest_trails` ALL SHORTEST TRAILS (only allows `bfs` search strategy)

- The `[SEARCH_STRATEGY]` parameter controls the search algorithm to use when exploring the graph, can be either `bfs` or `dfs`
- Results are stored inside the `results/pathfinder/wdbench` folder in the repository

Neo4j
================================================================================

### Pokec

- Run Benchmark: `python run_neo4j.py pokec [MODE] [TIMEOUT_MAX]`

- The `[MODE]` parameter is an integer that represents which semantic mode to test, it can take the following values:
  - `0` ALL TRAILS
  - `1` ANY SHORTEST WALKS
  - `2` ALL SHORTEST WALKS
  - `3` ENDPOINTS

- The `[TIMEOUT_MAX]` parameter is the same as with `PathFinder`
- Results are stored inside the `results/neo4j` folder in the repository

### Diamond

- Run Benchmark: `python run_neo4j.py diamond [MODE] [TIMEOUT_MAX]`

- The `[MODE]` parameter is an integer that represents which semantic mode to test, it can take the following values:
  - `0` ALL TRAILS
  - `1` ANY SHORTEST WALKS
  - `2` ALL SHORTEST WALKS

- The `[TIMEOUT_MAX]` parameter is the same as with `PathFinder`
- Results are stored inside the `results/neo4j` folder in the repository

### Wikidata

- Run Benchmark: `python wdbench_paths_neo4j.py ../queries/neo4j/wdbench.txt`

- This will run with the `ALL TRAILS` path semantic
- Results are stored inside the `results/neo4j` folder in the repository

Kuzu
================================================================================

### Pokec

- Run Benchmark: `python run_kuzu.py pokec [MODE] [TIMEOUT_MAX]`

- The `[MODE]` parameter is an integer that represents which semantic mode to test, it can take the following values:
  - `0` ALL WALKS
  - `1` ANY SHORTEST WALKS
  - `2` ALL SHORTEST WALKS
  - `3` ENDPOINTS

- The `[TIMEOUT_MAX]` parameter is the same as with `PathFinder`
- Results are stored inside the `results/kuzu` folder in the repository

### Diamond

- Run Benchmark: `python run_kuzu.py diamond [MODE] [TIMEOUT_MAX]`

- The `[MODE]` parameter is an integer that represents which semantic mode to test, it can take the following values:
  - `0` ALL WALKS
  - `1` ANY SHORTEST WALKS
  - `2` ALL SHORTEST WALKS

- The `[TIMEOUT_MAX]` parameter is the same as with `PathFinder`
- Results are stored inside the `results/kuzu` folder in the repository

NebulaGraph
================================================================================

### Pokec

- Run Benchmark: `python run_nebula.py pokec [MODE] [TIMEOUT_MAX]`

- The `[MODE]` parameter is an integer that represents which semantic mode to test, it can take the following values:
  - `0` ALL TRAILS
  - `1` ANY SHORTEST TRAILS
  - `2` ALL SHORTEST TRAILS
  - `3` ALL WALKS
  - `4` ENDPOINTS

- The `[TIMEOUT_MAX]` parameter is the same as with `PathFinder`
- Results are stored inside the `results/nebula` folder in the repository

### Diamond

- Run Benchmark: `python run_nebula.py diamond [MODE] [TIMEOUT_MAX]`

- The `[MODE]` parameter is an integer that represents which semantic mode to test, it can take the following values:
  - `0` ALL TRAILS

- The `[TIMEOUT_MAX]` parameter is the same as with `PathFinder`
- Results are stored inside the `results/nebula` folder in the repository

SPARQL
================================================================================

### Pokec

- The queries can be run manually (there are only 12 for the endpoints experiment)
- They are contained [here](queries/sparql/pokec_endpoints.txt)

### Wikidata

- We provide a benchmark script for SPARQL engines, named `wdbench_sparql.py`
- This script requires installing the following python library: `pip3 install SPARQLWrapper`
- Before using it, you need to edit the parameters marked inside the script, indicating necessary details such as the path to each of the engines in your machine

- After editing the parameters, run the benchmark: `python wdbench_sparql.py [ENGINE] ../queries/sparql/wdbench.txt 100000 [OUT_NAME]`

- The `[ENGINE]` parameter can be one of the following:
  - `JENA`
  - `BLAZEGRAPH`
  - `VIRTUOSO`

- The `[OUT_NAME]` parameter is any prefix name you want to give to the output file
- Results are stored inside the `results/sparql/wdbench` folder in the repository