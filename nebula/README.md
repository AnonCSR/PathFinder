NebulaGraph <!-- omit in toc -->
================================================================================
The following steps explain how to setup the engine and import the data needed to replicate the experiments.
When reading the instructions, assume that your working directory is where this `README` file is located.

Table of Contents <!-- omit in toc -->
================================================================================
- [Setup](#setup)
    - [Install NebulaGraph](#install-nebulagraph)
    - [Edit Configuration](#edit-configuration)
    - [Nebula Importer](#nebula-importer)
    - [Client API](#client-api)
- [Importing Data](#importing-data)
    - [Pokec](#pokec)
    - [Diamond](#diamond)

Setup
================================================================================

### Install NebulaGraph

- Download, install and configure `NebulaGraph 3.5.0` by following their quick start guide: [NebulaGraph Quick Start](https://docs.nebula-graph.io/3.5.0/2.quick-start/2.install-nebula-graph/)

### Edit Configuration

- Custom configuration files are available inside the `config` folder
- Go to the Nebula configuration folder: `cd /usr/local/nebula/etc`
- Replace the corresponding configuration files with the custom ones we provide in the `config` folder

### Nebula Importer

- Download the Nebula Importer tool from here: [Nebula Importer Executable](https://github.com/vesoft-inc/nebula-importer/releases/download/v4.0.0/nebula-importer_4.0.0_Linux_x86_64)
- Place the executable in the same folder as this `README` file, and rename it to `nebula-importer`

### Client API

- Install the Nebula python API: `pip3 install nebula3-python==3.4.0`

Importing Data
================================================================================

- Before importing data, the service needs to be running: `sudo /usr/local/nebula/scripts/nebula.service start all`
- After importing the data, stop the service with: `sudo /usr/local/nebula/scripts/nebula.service stop all`

### Pokec

- Execute the node data import (automatically imports into the correct database): `./nebula-importer --config pokec/config_pokec_nodes.yaml`

- Execute the edge data import (automatically imports into the correct database): `./nebula-importer --config pokec/config_pokec_edges.yaml`

### Diamond

- Execute the data import (automatically imports into the correct database): `./nebula-importer --config diamond/config_diamond.yaml`