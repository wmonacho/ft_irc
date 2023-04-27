#!/bin/bash

# Make for sure
make

# Build the Docker image
docker build -t weechat-docker .


# Run the Docker container
docker run -it weechat-docker

# Add -p port instead -it for run with a special port