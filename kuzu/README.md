Kuzu <!-- omit in toc -->
================================================================================
The following steps explain how to setup the engine and import the data needed to replicate the experiments.
When reading the instructions, assume that your working directory is where this `README` file is located.

Table of Contents <!-- omit in toc -->
================================================================================
- [Setup](#setup)
    - [Install Kuzu](#install-kuzu)
    - [Client API](#client-api)
- [Importing Data](#importing-data)
    - [Pokec](#pokec)
    - [Diamond](#diamond)

Setup
================================================================================

### Install Kuzu

- Download `Kuzu 0.0.6` from their github: [Kuzu Executable](https://github.com/kuzudb/kuzu/releases/download/v0.0.6/kuzu_cli-linux-x86_64.zip/)
- Unzip the downloaded file and place the `kuzu` executable in the same folder as this `README`
- Set permissions for the executable: `chmod +x kuzu`

### Client API

- Install the Kuzu python API: `pip3 install kuzu==0.0.6`

Importing Data
================================================================================

### Pokec

- Connect to Kuzu and create the empty database: `./kuzu ./pokec`

- Inside the console, create the schema and import the data:

```
CREATE NODE TABLE Person(id STRING, PRIMARY KEY (id));
COPY Person FROM "../dbs/pokec_nodes.csv";
CREATE REL TABLE Knows(FROM Person TO Person);
COPY Knows FROM "../dbs/pokec_edges.csv";
```

### Diamond

- Connect to Kuzu and create the empty database: `./kuzu ./diamond`

- Inside the console, create the schema and import the data:

```
CREATE NODE TABLE N(id STRING, PRIMARY KEY (id));
COPY N FROM "../dbs/diamond_nodes.csv";
CREATE REL TABLE E(FROM N TO N);
COPY E FROM "../dbs/diamond_edges.csv";
```