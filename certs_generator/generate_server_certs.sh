#!/bin/bash

echo "Generating server cert..."

openssl genrsa -out generated_server_certs/server.key 2048

openssl req -new -key generated_server_certs/server.key -out generated_server_certs/server.csr -config configs/server_san.conf

openssl x509 -req -in generated_server_certs/server.csr -CA generated_ca_certs/ca.pem -CAkey generated_ca_certs/ca.key -CAcreateserial \
    -out generated_server_certs/server.crt -days 3650 -sha256 -extensions req_ext -extfile configs/server_san.conf

echo "Done!"

