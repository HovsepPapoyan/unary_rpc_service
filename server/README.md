# Build, run and clean server

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
    Generate ca and server certificates and put inside certs directory (ca.pem, server.crt and server.key).
    For more info about how to generate certificates refer to certs_generator/README.md.
    For production environment avoid placing certs directory with certificates in the application source code directory.
    Instead:
        1. Place certs directory into e.g. /etc/yourapp/server/
        2. Set ownership (e.g., sudo chown -R user:user /etc/yourapp/server && sudo chown -R user:user /etc/yourapp/server/*)
        3. Set strict permissions:
            ```bash
            sudo chmod 644 /etc/yourapp/server/certs/ca.pem
            sudo chmod 644 /etc/yourapp/server/certs/server.crt
            sudo chmod 600 /etc/yourapp/server/certs/server.key
            ```
        4. Modify ./scripts/run.sh script to point correct locations:
            ```bash
            export GRPC_SSL_CA_PATH=/etc/yourapp/server/certs/ca.pem
            export GRPC_SSL_CRT_PATH=/etc/yourapp/server/certs/server.crt
            export GRPC_SSL_KEY_PATH=/etc/yourapp/server/certs/server.key
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

