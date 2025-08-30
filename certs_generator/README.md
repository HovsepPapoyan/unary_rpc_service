# Scripts for generate and clean certificates

## Generate

1.
    ca.key and ca.pem inside generated_ca_certs directory based on ca_openssl.conf config
    ```bash
    ./generate_ca_certs.sh
    ```

2.
    server.crt, server.csr and server.key inside generated_server_certs directory based on server_san.conf config
    ```bash
    ./generate_server_certs.sh
    ```

3.
    client.crt, client.csr and client.key inside generated_client_certs directory based on client_san.conf config
    ```bash
    ./generate_client_certs.sh
    ```

## Clean

1.
    ca.key and ca.pem
    ```bash
    ./clean_ca_certs.sh
    ```

2.
    server.crt, server.csr and server.key
    ```bash
    ./clean_server_certs.sh
    ```

3.
    client.crt, client.csr and client.key
    ```bash
    ./clean_client_certs.sh
    ```

## Sequence of running generate scripts

1.
    generate_ca_certs.sh
2.
    generate_server_certs.sh and generate_client_certs.sh (or vice versa)

