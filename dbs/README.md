Data Preparation <!-- omit in toc -->
================================================================================
In this section we provide instructions for downloading and formatting the graph data that the benchmark works with.
For all these steps, your working directory must be the same as the location of this `README` file (inside the `dbs` folder).

Table of Contents <!-- omit in toc -->
================================================================================
- [Pokec](#pokec)
  - [Download and Process Dataset](#download-and-process-dataset)
  - [Convert to PathFinder format](#convert-to-pathfinder-format)
  - [Convert to RDF format](#convert-to-rdf-format)
- [Diamond](#diamond)
  - [Data Files](#data-files)
- [Wikidata](#wikidata)
  - [Download and Process Dataset](#download-and-process-dataset-1)
  - [Convert to PathFinder format](#convert-to-pathfinder-format-1)
  - [Convert to Neo4j format](#convert-to-neo4j-format)

Pokec
================================================================================

Download and Process Dataset
--------------------------------------------------------------------------------

- Get the data with: `wget https://snap.stanford.edu/data/soc-pokec-relationships.txt.gz`
- Extract it: `gzip -d soc-pokec-relationships.txt.gz`
- Generate files for nodes and edges: `python process_pokec.py soc-pokec-relationships.txt`
- The previous command should generate two files: `pokec_nodes.csv` and `pokec_edges.csv` (these are required)

Convert to PathFinder format
--------------------------------------------------------------------------------

- Run the following command: `python pokec_to_pathfinder.py pokec_edges.csv Knows`
- This generates a `pokec_pathfinder.txt` file that is necessary for importing the data into PathFinder

Convert to RDF format
--------------------------------------------------------------------------------

- Run the following command: `python pokec_to_rdf.py pokec_edges.csv Knows`
- This generates a `pokec_rdf.nt` file that is necessary for importing the data into SPARQL engines

Diamond
================================================================================

Data Files
--------------------------------------------------------------------------------

We already provide all necessary files for the diamond database experiments present in the paper.
They are located in this same folder.

Wikidata
================================================================================

Download and Process Dataset
--------------------------------------------------------------------------------

- Download dataset from [here](https://figshare.com/s/50b7544ad6b1f51de060)
- Extract it: `bzip2 -d truthy_direct_properties.nt.bz2`
- This original file, `truthy_direct_properties.nt`, is ready to be imported into SPARQL engines
- For loading this data into SPARQL engines, use the instructions provided by the authors of `WDBench`: [WDBench Instructions](https://github.com/MillenniumDB/WDBench/blob/remove_cross_product/README.md)

Convert to PathFinder format
--------------------------------------------------------------------------------

- Run the following command: `python wdbench_nt_to_pathfinder.py truthy_direct_properties.nt wdbench_pathfinder.txt`
- This generates a `wdbench_pathfinder.txt` file that is necessary for importing the data into PathFinder

Convert to Neo4j format
--------------------------------------------------------------------------------

- Run the following command: `python wdbench_nt_to_neo4j.py truthy_direct_properties.nt`
- This generates three files (`entities.csv`, `literals.csv`, `edges.csv`), which are necessary for importing the data into Neo4j