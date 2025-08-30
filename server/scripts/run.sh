#!/bin/bash

export GRPC_SSL_CA_PATH=$PWD/../certs/ca.pem
export GRPC_SSL_CRT_PATH=$PWD/../certs/server.crt
export GRPC_SSL_KEY_PATH=$PWD/../certs/server.key

../cmake/build/server

