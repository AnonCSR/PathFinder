PathFinder <!-- omit in toc -->
================================================================================
PathFinder is a graph oriented database management system.
This project aims for a fully functional and easy-to-extend DBMS that serves as the basis for testing new techniques and algorithms related to databases and graphs.
The following steps explain how to setup the engine and import the data needed to replicate the experiments.
When reading the instructions, assume that your working directory is where this `README` file is located.

Table of Contents <!-- omit in toc -->
================================================================================
- [Setup](#setup)
    - [Install Dependencies](#install-dependencies)
    - [Install Boost](#install-boost)
    - [Build Project](#build-project)
- [Importing Data](#importing-data)
    - [Pokec](#pokec)
    - [Diamond](#diamond)
    - [Wikidata](#wikidata)

Setup
================================================================================
PathFinder should be able to compile on any x86-64 Linux distribution.
On windows, Windows Subsystem for Linux (WSL) can be used.

### Install Dependencies

PathFinder needs the following dependencies:
- GCC >= 8.1
- CMake >= 3.12
- Git
- libssl
- ncursesw and less for the CLI

On current Debian and Ubuntu based distributions they can be installed by running:
```bash
sudo apt update && sudo apt install git g++ cmake libssl-dev libncurses-dev locales less
```

The `en_US.UTF-8` locale also needs to be generated.
On Ubuntu based distributions this can be done as follows:
```bash
sudo locale-gen en_US.UTF-8
```

On distributions without a patched locale-gen you can run:
```bash
sudo sed -i '/en_us.utf-8/Is/^# //g' /etc/locale.gen
sudo locale-gen
```

Or manually uncomment `en_US.UTF-8` in `/etc/locale.gen` and run:
```bash
sudo locale-gen
```

### Install Boost

Set the `PF_HOME` env variable, while inside the same directory as this `README` file:
```
export PF_HOME=$(pwd)
```

Download [`boost_1_81_0.tar.gz`](https://boostorg.jfrog.io/artifactory/main/release/1.81.0/source/boost_1_81_0.tar.gz) using a browser or wget:
```bash
wget -q --show-progress https://boostorg.jfrog.io/artifactory/main/release/1.81.0/source/boost_1_81_0.tar.gz
```

and run the following in the directory where boost was downloaded:
```bash
tar -xf boost_1_81_0.tar.gz
mkdir -p $PF_HOME/third_party/boost_1_81/include
mv boost_1_81_0/boost $PF_HOME/third_party/boost_1_81/include
rm -r boost_1_81_0.tar.gz boost_1_81_0
```

### Build Project

Execute this command to build PathFinder:
```
cmake -B build/Release -D CMAKE_BUILD_TYPE=Release && cmake --build build/Release/
```

Importing Data
================================================================================

### Pokec

- Execute the data import:

```
build/Release/bin/pf-import ../dbs/pokec_pathfinder.txt dbs/pokec -m quad
```

### Diamond

- Execute the data import:

```
build/Release/bin/pf-import ../dbs/diamond_pathfinder.txt dbs/diamond -m quad
```

### Wikidata

- Execute the data import:

```
build/Release/bin/pf-import ../dbs/wdbench_pathfinder.txt dbs/wdbench -m quad
```