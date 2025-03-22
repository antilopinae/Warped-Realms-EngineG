#!/bin/bash
#sudo rm -R build

cmake -B build -S .
cmake --build build

#echo -e "\nTesting"
#cd build
#ctest
#cd ..

# echo -e "\nGraphviz"
# cd build
# mkdir graph
# cmake --graphviz=graph/test.dot .
# dot -Tpdf graph/test.dot >> deps.pdf
# cd ..
