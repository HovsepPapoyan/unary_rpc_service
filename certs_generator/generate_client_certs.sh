#!/bin/bash

echo "Generating client cert..."

openssl genrsa -out generated_client_certs/client.key 2048

openssl req -new -key generated_client_certs/client.key -out generated_client_certs/client.csr -config configs/client_san.conf

openssl x509 -req -in generated_client_certs/client.csr -CA generated_ca_certs/ca.pem -CAkey generated_ca_certs/ca.key -CAcreateserial \
    -out generated_client_certs/client.crt -days 3650 -sha256 -extensions req_ext -extfile configs/client_san.conf

echo "Done!"

