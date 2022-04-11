# 🏘 Girvan-Newman Community Detection
This is a C++ implementation of the Girvan-Newman community detection algorithm
as proposed [here](https://www.pnas.org/doi/full/10.1073/pnas.122653799) and 
refined [here](https://journals.aps.org/pre/abstract/10.1103/PhysRevE.69.026113) with use of 
the Boost Graph Library for C++.
The algorithm involves repeated calculations of edge betweeness, in which the edge with 
the highest score is removed and a modularity is calculated, which determines the density of
the community structure is. This is repeated until all edges are removed the partition with 
the highest modularity is chosen.

This implementation will show its functionality with use of the Football 2000 Dataset, which
can be found [here](http://www-personal.umich.edu/~mejn/netdata/)

## Installation
This project can be run in the commandline via g++, or by compiling in CLion using the CMakeLists.txt file with your 
chosen compiler. Below I will show you how to install g++.

First, check if g++ is installed.
```text
$ g++ --version
```
If it is not installed, install it.
```text
$ sudo apt-get install g++
```

## Usage
First, navigate to the project directory.
```bash
$ cd FullFilePathHere
```
Next, compile all the relevant files using g++
```bash
$ g++ src/main.cpp 
```
Run the program, this program takes 1 command line argument: the relative path to the
graphml dataset.

```bash
$ ./a.out data/dataset_1.graphml
```

## Datasets
The program uses files formatted using graphml, which is a way to organize vertices, edges, and their
respective properties. It is formatted with properties defined on the top of file, nodes listed below
with distincive keys, and edges defined underneath.

An example of a small Graphml file with 2 nodes and 1 edge is shown below:
```text
<?xml version="1.0" encoding="UTF-8"?>
<graphml xmlns="http://graphml.graphdrawing.org/xmlns"  
    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
    xsi:schemaLocation="http://graphml.graphdrawing.org/xmlns
    http://graphml.graphdrawing.org/xmlns/1.0/graphml.xsd">
  <key id="d0" for="node" attr.name="community" attr.type="int"/>
  <graph id="G" edgedefault="undirected">
  
    <node id="n0">
      <data key="d0">1</data>
    </node>
    <node id="n1">
      <data key="d0">1</data>
    </node>
    
    <edge source="n0" target="n1"/> 
  </graph>
</graphml>
```


Randomly generated .graphml graphs were generated using the NetworkX python library 
and tested using the algorithm. The final testing dataset was converted from gml to graphml
using the same library.

