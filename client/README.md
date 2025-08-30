# Build, run and clean client

## Build

1.
    Prerequisites
    The solution was tested on
        Ubuntu 22.04.5 LTS
    with
        cmake version 3.22.1
        clang version 17.0.6

2.
    Build
    ```bash
        cd ./scripts && ./build.sh
    ```

## Run

1.
    Generate ca and client certificates and put inside certs directory (ca.pem, client.crt and client.key).
    For more info about how to generate certificates refer to certs_generator/README.md.
    For production environment avoid placing certs directory with certificates in the application source code directory.
    Instead:
        1. Place certs directory into e.g. /etc/yourapp/client/
        2. Set ownership (e.g., sudo chown -R user:user /etc/yourapp/client && sudo chown -R user:user /etc/yourapp/client/*)
        3. Set strict permissions:
            ```bash
            sudo chmod 644 /etc/yourapp/client/certs/ca.pem
            sudo chmod 644 /etc/yourapp/client/certs/client.crt
            sudo chmod 600 /etc/yourapp/client/certs/client.key
            ```
        4. Modify ./scripts/run.sh script to point correct locations:
            ```bash
            export GRPC_DEFAULT_SSL_ROOTS_FILE_PATH=/etc/yourapp/client/certs/ca.pem
            export GRPC_SSL_CRT_PATH=/etc/yourapp/client/certs/client.crt
            export GRPC_SSL_KEY_PATH=/etc/yourapp/client/certs/client.key
            ```

2.
    ```bash
    cd ./scripts && ./run.sh
    ```

## Clean

1.
    ```bash
    cd ./scripts && ./clean.sh
    ```

