#!/bin/bash
cd "etapa-$1"
mkdir -p out

make main

./out/main