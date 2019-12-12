# websocket-serve

A simple Websocket server written in C++ using Boost.Axios and Boost.Beast. You can connect to it using any Websocket client and post messages. Messages will be broadcasted to all connected sockets. This repository is a good starting point for building up application-specific servers that support HTTP and Websocket protocols using C++ and Boost.

## Set up a development environment using Docker

#### Build the image
```
docker build -t websocket-serve .
```

#### Run the container
```
docker run -d -P --name websocket-serve-dev websocket-serve
```

#### Run bash to set up a password for root
```
docker exec -it websocket-serve-dev /bin/bash
$ passwd root
$ exit
```

#### Log in using SSH
```
ssh root@localhost -p {$PORT}
```
