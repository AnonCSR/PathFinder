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
- [Results](#results)
- [Other Semantics](#other-semantics)

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
- By default, the file contains a query that retrieves the first five direct/indirect friends of a specific user in the `Pokec` social network, as well as the paths to each of these
- The query is written as follows:

```
PREFIX : <http://p.db/> SELECT * WHERE {:696585 ANY (:Knows* as ?p) ?x} LIMIT 5
```

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
python scripts/sparql_query.py query.sparql > out.txt
```

Results
================================================================================

- The results will be saved to a file named `out.txt`
- The paths are shown in a nested fashion, as part of the `JSON` response
- A better way of displaying these paths, for the `SPARQL` support of our engine, is still in development
- The contents of `out.txt` after the query execution are the following:

```
PREFIX : <http://p.db/> SELECT * WHERE {:696585 ANY (:Knows* as ?p) ?x} LIMIT 5

-----------------

{
  "head": {
    "vars": [
      "p",
      "x"
    ]
  },
  "results": {
    "bindings": [
      {
        "p": {
          "type": "path",
          "value": [
            {
              "node": {
                "type": "uri",
                "value": "http://p.db/696585"
              }
            }
          ]
        },
        "x": {
          "type": "uri",
          "value": "http://p.db/696585"
        }
      },
      {
        "p": {
          "type": "path",
          "value": [
            {
              "node": {
                "type": "uri",
                "value": "http://p.db/696585"
              }
            },
            {
              "edge": {
                "type": "uri",
                "value": "http://p.db/Knows"
              },
              "inverse": false
            },
            {
              "node": {
                "type": "uri",
                "value": "http://p.db/63119"
              }
            }
          ]
        },
        "x": {
          "type": "uri",
          "value": "http://p.db/63119"
        }
      },
      {
        "p": {
          "type": "path",
          "value": [
            {
              "node": {
                "type": "uri",
                "value": "http://p.db/696585"
              }
            },
            {
              "edge": {
                "type": "uri",
                "value": "http://p.db/Knows"
              },
              "inverse": false
            },
            {
              "node": {
                "type": "uri",
                "value": "http://p.db/158464"
              }
            }
          ]
        },
        "x": {
          "type": "uri",
          "value": "http://p.db/158464"
        }
      },
      {
        "p": {
          "type": "path",
          "value": [
            {
              "node": {
                "type": "uri",
                "value": "http://p.db/696585"
              }
            },
            {
              "edge": {
                "type": "uri",
                "value": "http://p.db/Knows"
              },
              "inverse": false
            },
            {
              "node": {
                "type": "uri",
                "value": "http://p.db/819930"
              }
            }
          ]
        },
        "x": {
          "type": "uri",
          "value": "http://p.db/819930"
        }
      },
      {
        "p": {
          "type": "path",
          "value": [
            {
              "node": {
                "type": "uri",
                "value": "http://p.db/696585"
              }
            },
            {
              "edge": {
                "type": "uri",
                "value": "http://p.db/Knows"
              },
              "inverse": false
            },
            {
              "node": {
                "type": "uri",
                "value": "http://p.db/123865"
              }
            }
          ]
        },
        "x": {
          "type": "uri",
          "value": "http://p.db/123865"
        }
      }
    ]
  }
}
Results: 5
```

Other Semantics
================================================================================

- The example above uses the `ANY WALKS` semantics
- For other path semantics, simply replace `ANY` with the keywords for each semantic
- Some examples are shown below:

```
PREFIX : <http://p.db/> SELECT * WHERE {:696585 ANY WALKS (:Knows* as ?p) ?x} LIMIT 5
PREFIX : <http://p.db/> SELECT * WHERE {:696585 ALL SHORTEST WALKS (:Knows* as ?p) ?x} LIMIT 5
PREFIX : <http://p.db/> SELECT * WHERE {:696585 ANY SIMPLE (:Knows* as ?p) ?x} LIMIT 5
PREFIX : <http://p.db/> SELECT * WHERE {:696585 ALL SHORTEST SIMPLE (:Knows* as ?p) ?x} LIMIT 5
```