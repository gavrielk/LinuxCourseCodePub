#!/bin/bash

sudo docker build -t zipper-build:1.0 -f Dockerfile.build .

sudo docker run -v `pwd`/app:/root/build_dir zipper-build:1.0

sudo docker build -f Dockerfile.prod -t zipper:1.0 ./
