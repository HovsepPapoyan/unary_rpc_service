#!/bin/bash

echo "Generating CA..."

openssl genrsa -out generated_ca_certs/ca.key 2048
openssl req -x509 -new -nodes -key generated_ca_certs/ca.key -sha256 -days 3650 -out generated_ca_certs/ca.pem -subj "/CN=grpc-ca" -extensions v3_ca -config configs/ca_openssl.conf

echo "Done!"

