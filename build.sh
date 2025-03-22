#!/bin/bash
#sudo rm -R build
echo -e "\nGenerating"
cmake -B ../Assets/Plugins/build -S .

echo -e "\nBuilding"
cmake --build ../Assets/Plugins/build

# echo -e "\nDeleting some files"
# bash cleanup.sh

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

echo -e "\nRunning"
./../Assets/Plugins/build/Bootstrap/bootstrap
