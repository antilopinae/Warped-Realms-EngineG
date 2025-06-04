#!/bin/bash

echo -e "\nGraphviz"
cd build
mkdir graph
cmake --graphviz=graph/test.dot .
dot -Tpdf graph/test.dot >> deps.pdf
cd ..
