#!/bin/bash

# Make for sure
make

# Build the Docker image
docker build -t hexchat-docker .


# Run the Docker container
docker run -it hexchat-docker

# Add -p port instead -it for run with a special port

#run exec ircserv

./ircserv 9999 manger