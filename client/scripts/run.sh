#!/bin/bash

export GRPC_DEFAULT_SSL_ROOTS_FILE_PATH=$PWD/../certs/ca.pem # PEM file to load SSL roots from
export GRPC_SSL_CRT_PATH=$PWD/../certs/client.crt
export GRPC_SSL_KEY_PATH=$PWD/../certs/client.key

../cmake/build/client --target localhost:50051

