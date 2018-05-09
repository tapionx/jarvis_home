#!/bin/bash

# build the Docker image
docker build -t jarvis_home .

# run the Docker container
docker run -p 8000:8000 -v $(pwd):/code jarvis_home
