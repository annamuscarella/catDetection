#!/bin/bash
git fetch
git pull
cd build
make
cd ..