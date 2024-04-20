Returning Paths in SPARQL <!-- omit in toc -->
================================================================================
The following steps explain how to manually execute `SPARQL` queries in `PathFinder`.

Table of Contents <!-- omit in toc -->
================================================================================
- [Setup](#setup)
- [Importing Data](#importing-data)
- [Query File](#query-file)
- [Server](#server)
- [Query Execution](#query-execution)

Setup
================================================================================

- Follow all the steps to prepare the benchmark, mentioned in the main [README](../README.md)
- This will prepare the necessary data, as well as build/setup the engine to start receiving queries
- Make sure to execute all the commands below **from the same working directory** as this `README` file

Importing Data
================================================================================

- Import the `RDF` data to be used (in this case `Pokec`):

```
build/Release/bin/pf-import ../dbs/pokec_rdf.nt dbs/pokec_rdf
```

Query File
================================================================================

- Use the query file present [here](./query.sparql) to define a query of interest
- By default, the file contains a query that retrieves all the direct friends of a specific user in the `Pokec` social network

Server
================================================================================

- Start the `PathFinder` server over the database of interest (in this case `Pokec`):

```
build/Release/bin/pf-server dbs/pokec_rdf -p 8084
```

Query Execution
================================================================================

- Make sure that the `SPARQLWrapper` python library is installed: `pip3 install SPARQLWrapper`
- While the server is running in a **different** terminal window, execute the query using the following `python` script:

```
python scripts/sparql_query.py query.sparql
```

- The results will be displayed in the terminal