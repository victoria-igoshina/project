#!/usr/bin/env bash

echo "Start"

echo "Building sort_file..."
g++ sort_file/sort_file.cpp -o sort_file.bin
#cmake .
#make

echo "Building generator..."
g++ generator/generator.cpp -o generator.bin

echo "Running generator..."
./generator.bin -o input -n 1000000 -l 100

SECONDS=0
echo "Running sort_file..."
./sort_file.bin -i input -o result -t tmp

echo $SECONDS

echo "Checking result..."
python3.6 check_output.py

echo "Finish!"
