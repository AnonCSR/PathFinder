Neo4j <!-- omit in toc -->
================================================================================
The following steps explain how to setup the engine and import the data needed to replicate the experiments.
When reading the instructions, assume that your working directory is where this `README` file is located.

Table of Contents <!-- omit in toc -->
================================================================================
- [Setup](#setup)
    - [Install Neo4j](#install-neo4j)
    - [Edit Configuration](#edit-configuration)
    - [Client API](#client-api)
- [Importing Data](#importing-data)
    - [Pokec](#pokec)
    - [Diamond](#diamond)
    - [Wikidata](#wikidata)

Setup
================================================================================

### Install Neo4j

- Download `Neo4j 5.12.0 Community Edition` from their website: [Neo4j Download Center](https://neo4j.com/deployment-center/). Select the option to download as a `TAR` file.
- Extract the downloaded file: `tar -xf neo4j-community-5.12.0-unix.tar.gz`
- Set the environment variable `$NEO4J_HOME` to point to the extracted Neo4j folder (using `export` and adding it to your `.bashrc`/`.zshrc` file)

### Edit Configuration

- Replace the contents of `neo4j-community-5.12.0/conf/neo4j.conf` with `config/neo4j.conf` (contained in this repository).

### Client API

- Install the Neo4j python API: `pip3 install neo4j==5.13.0`

Importing Data
================================================================================

### Pokec

- Navigate to the Neo4j folder: `cd neo4j-community-5.12.0`

- Edit `conf/neo4j.conf`, replacing the line that sets the default database:
```
initial.dbms.default_database=pokec
```

- Execute the data import:

```
bin/neo4j-admin database import full \
--nodes=Person="../../dbs/pokec_nodes_header.csv,../../dbs/pokec_nodes.csv" \
--relationships=Knows="../../dbs/pokec_edges_header.csv,../../dbs/pokec_edges.csv" \
--delimiter "," --array-delimiter ";" --skip-bad-relationships true pokec
```

- Create necessary indexes:

  - Start the server: `bin/neo4j console`
    - This process won't end until you interrupt it. Let this execute until the index creation is finished.
    - Run the index creation commands (shown below) in another terminal.

  - Open the cypher console: `bin/cypher-shell`
      - Inside the console, run the command: `CREATE INDEX FOR (n:Person) ON (n.id);`
      - Even though the above command returns immediately, you have to wait until the index is finished before interrupting the server. You can see the status of the index creation with the command `SHOW INDEXES;`

### Diamond

- Navigate to the Neo4j folder: `cd neo4j-community-5.12.0`

- Edit `conf/neo4j.conf`, replacing the line that sets the default database:
```
initial.dbms.default_database=diamond
```

- Execute the data import:

```
bin/neo4j-admin database import full \
--nodes=N="../../dbs/diamond_nodes_header.csv,../../dbs/diamond_nodes.csv" \
--relationships=E="../../dbs/diamond_edges_header.csv,../../dbs/diamond_edges.csv" \
--delimiter "," --array-delimiter ";" --skip-bad-relationships true diamond
```

- Create necessary indexes:

  - Start the server: `bin/neo4j console`
    - This process won't end until you interrupt it. Let this execute until the index creation is finished.
    - Run the index creation commands (shown below) in another terminal.

  - Open the cypher console: `bin/cypher-shell`
      - Inside the console, run the command: `CREATE INDEX FOR (n:N) ON (n.id);`
      - Even though the above command returns immediately, you have to wait until the index is finished before interrupting the server. You can see the status of the index creation with the command `SHOW INDEXES;`

### Wikidata

- Navigate to the Neo4j folder: `cd neo4j-community-5.12.0`

- Edit `conf/neo4j.conf`, replacing the line that sets the default database:
```
initial.dbms.default_database=wdbench
```

- Execute the data import:

```
bin/neo4j-admin database import full \
--nodes=Entity=../../dbs/entities.csv \
--nodes=../../dbs/literals.csv \
--relationships=../../dbs/edges.csv \
--delimiter "," --array-delimiter ";" --skip-bad-relationships true wdbench
```

- Create necessary indexes:

  - Start the server: `bin/neo4j console`
    - This process won't end until you interrupt it. Let this execute until the index creation is finished.
    - Run the index creation commands (shown below) in another terminal.

  - Open the cypher console: `bin/cypher-shell`
      - Inside the console, run the command: `CREATE INDEX FOR (n:Entity) ON (n.id);`
      - Even though the above command returns immediately, you have to wait until the index is finished before interrupting the server. You can see the status of the index creation with the command `SHOW INDEXES;`