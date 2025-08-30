# Unary rpc service implementation with gRPC based server and client

## License

This project is licensed under the MIT License - see the [LICENSE](./LICENSE) file for details.

## Build and run server and client

1. Clone the repository
    ```bash
    git clone https://github.com/HovsepPapoyan/unary_rpc_service.git
    cd polycircuit
    ```

2. Build and run server
    Refer to server/README.md

3. Build and run client
    Refer to client/README.md

## Build and install OpenSSL and gRPC on x86-64 architecture and put installed artifacts to the third_party directories of the server and client (optional)

1.
    Prerequisites
    The solution was tested on
        Ubuntu 22.04.5 LTS
    with
        cmake version 3.22.1
        clang version 17.0.6

## Build and install OpenSSL
    ```bash
    cd /
    sudo mkdir openssl && cd openssl
    sudo wget https://www.openssl.org/source/openssl-3.0.2.tar.gz && sudo tar -xzf openssl-3.0.2.tar.gz && cd openssl-3.0.2
    sudo ./Configure --prefix=/openssl --openssldir=/openssl linux-x86_64
    sudo make -j$(nproc)
    sudo make install_sw

    cd .. && sudo rm -rf openssl-3.0.2*

    cd lib64
    sudo patchelf --remove-rpath libssl.so.3
    sudo patchelf --add-rpath '$ORIGIN' libssl.so.3
    ```

## Build and install gRPC with OpenSSL

    ```bash
    cd /
    export MY_INSTALL_DIR=/grpc_install
    sudo mkdir -p $MY_INSTALL_DIR
    export PATH="$MY_INSTALL_DIR/bin:$PATH"

    sudo git clone --recurse-submodules -b v1.74.0 --depth 1 --shallow-submodules https://github.com/grpc/grpc

    cd grpc && sudo mkdir -p cmake/build && cd cmake/build

    sudo cmake \
        -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_C_FLAGS_RELEASE="-O3 -DNDEBUG -g0" \
        -DCMAKE_CXX_FLAGS_RELEASE="-O3 -DNDEBUG -g0" \
        -DgRPC_INSTALL=ON \
        -DgRPC_BUILD_TESTS=OFF \
        -DBUILD_SHARED_LIBS=ON \
        -DCMAKE_CXX_STANDARD=20 \
        -DCMAKE_C_COMPILER=clang-17 \
        -DCMAKE_CXX_COMPILER=clang++-17 \
        -DgRPC_SSL_PROVIDER=package \
        -DOPENSSL_ROOT_DIR=/openssl \
        -DOPENSSL_INCLUDE_DIR=/openssl/include \
        -DOPENSSL_SSL_LIBRARY=/openssl/lib64/libssl.so \
        -DOPENSSL_CRYPTO_LIBRARY=/openssl/lib64/libcrypto.so \
        -DCMAKE_INSTALL_PREFIX=$MY_INSTALL_DIR \
        -DCMAKE_BUILD_WITH_INSTALL_RPATH=OFF \
        -DCMAKE_INSTALL_RPATH=/grpc_install/lib \
        ../..

    sudo make -j$(nproc)

    export LD_LIBRARY_PATH=/openssl/lib64:$LD_LIBRARY_PATH

    sudo make install

    sudo patchelf --remove-rpath /grpc_install/bin/ahost
    sudo patchelf --remove-rpath /grpc_install/bin/adig
    sudo patchelf --remove-rpath /grpc_install/bin/acountry
    sudo patchelf --remove-rpath /grpc_install/lib/libutf8_range.so.31.1.0
    sudo patchelf --remove-rpath /grpc_install/lib/libre2.so.9.0.0
    sudo patchelf --remove-rpath /grpc_install/lib/libz.so.1.3.1.1-motley
    sudo patchelf --remove-rpath /grpc_install/lib/libaddress_sorting.so.49.0.0
    sudo patchelf --remove-rpath /grpc_install/lib/libupb_base_lib.so.49.0.0
    sudo patchelf --remove-rpath /grpc_install/lib/libupb_lex_lib.so.49.0.0
    sudo patchelf --remove-rpath /grpc_install/lib/libupb_mem_lib.so.49.0.0
    sudo patchelf --remove-rpath /grpc_install/lib/libutf8_range_lib.so.49.0.0
    sudo patchelf --set-rpath '$ORIGIN/../lib' /grpc_install/bin/grpc_cpp_plugin
    sudo patchelf --set-rpath '$ORIGIN/../lib' /grpc_install/bin/grpc_csharp_plugin
    sudo patchelf --set-rpath '$ORIGIN/../lib' /grpc_install/bin/grpc_node_plugin
    sudo patchelf --set-rpath '$ORIGIN/../lib' /grpc_install/bin/grpc_objective_c_plugin
    sudo patchelf --set-rpath '$ORIGIN/../lib' /grpc_install/bin/grpc_php_plugin
    sudo patchelf --set-rpath '$ORIGIN/../lib' /grpc_install/bin/grpc_python_plugin
    sudo patchelf --set-rpath '$ORIGIN/../lib' /grpc_install/bin/grpc_ruby_plugin
    sudo patchelf --set-rpath '$ORIGIN' /grpc_install/lib/libutf8_validity.so.31.1.0
    sudo patchelf --set-rpath '$ORIGIN' /grpc_install/lib/libgpr.so.49.0.0
    sudo patchelf --set-rpath '$ORIGIN' /grpc_install/lib/libgrpc.so.49.0.0
    sudo patchelf --set-rpath '$ORIGIN' /grpc_install/lib/libgrpc_unsecure.so.49.0.0
    sudo patchelf --set-rpath '$ORIGIN' /grpc_install/lib/libupb_hash_lib.so.49.0.0
    sudo patchelf --set-rpath '$ORIGIN' /grpc_install/lib/libupb_json_lib.so.49.0.0
    sudo patchelf --set-rpath '$ORIGIN' /grpc_install/lib/libupb_message_lib.so.49.0.0
    sudo patchelf --set-rpath '$ORIGIN' /grpc_install/lib/libupb_mini_descriptor_lib.so.49.0.0
    sudo patchelf --set-rpath '$ORIGIN' /grpc_install/lib/libupb_mini_table_lib.so.49.0.0
    sudo patchelf --set-rpath '$ORIGIN' /grpc_install/lib/libupb_reflection_lib.so.49.0.0
    sudo patchelf --set-rpath '$ORIGIN' /grpc_install/lib/libupb_textformat_lib.so.49.0.0
    sudo patchelf --set-rpath '$ORIGIN' /grpc_install/lib/libupb_wire_lib.so.49.0.0
    sudo patchelf --set-rpath '$ORIGIN' /grpc_install/lib/libgrpc++.so.1.74.0
    sudo patchelf --set-rpath '$ORIGIN' /grpc_install/lib/libgrpc++_alts.so.1.74.0
    sudo patchelf --set-rpath '$ORIGIN' /grpc_install/lib/libgrpc++_error_details.so.1.74.0
    sudo patchelf --set-rpath '$ORIGIN' /grpc_install/lib/libgrpc++_reflection.so.1.74.0
    sudo patchelf --set-rpath '$ORIGIN' /grpc_install/lib/libgrpc++_unsecure.so.1.74.0
    sudo patchelf --set-rpath '$ORIGIN' /grpc_install/lib/libgrpc_authorization_provider.so.1.74.0
    sudo patchelf --set-rpath '$ORIGIN' /grpc_install/lib/libgrpc_plugin_support.so.1.74.0
    sudo patchelf --set-rpath '$ORIGIN' /grpc_install/lib/libgrpcpp_channelz.so.1.74.0

    sudo patchelf --set-rpath '$ORIGIN/../../openssl/lib64' /grpc_install/lib/libgrpc++.so.1.74.0
    sudo patchelf --set-rpath '$ORIGIN/../../openssl/lib64' /grpc_install/lib/libgrpc++_alts.so.1.74.0
    sudo patchelf --set-rpath '$ORIGIN/../../openssl/lib64' /grpc_install/lib/libgrpc++_error_details.so.1.74.0
    sudo patchelf --set-rpath '$ORIGIN/../../openssl/lib64' /grpc_install/lib/libgrpc++_reflection.so.1.74.0
    sudo patchelf --set-rpath '$ORIGIN/../../openssl/lib64' /grpc_install/lib/libgrpc.so.49.0.0
    sudo patchelf --set-rpath '$ORIGIN/../../openssl/lib64' /grpc_install/lib/libgrpcpp_channelz.so.1.74.0
    ```

## Put installed OpenSSL and gRPC artifacts to the third_party directories of the server and client
    ```bash
    sudo cp -r /openssl /path/to/unary_rpc_service/server/third_party/
    mkdir /path/to/unary_rpc_service/server/third_party/grpc
    sudo cp -r /grpc_install/* /path/to/unary_rpc_service/server/third_party/grpc/
    sudo chown -R user:user /path/to/unary_rpc_service/server && sudo chown -R user:user /path/to/unary_rpc_service/server/*

    sudo cp -r /openssl /path/to/unary_rpc_service/client/third_party/
    mkdir /path/to/unary_rpc_service/client/third_party/grpc
    sudo cp -r /grpc_install/* /path/to/unary_rpc_service/client/third_party/grpc/
    sudo chown -R user:user /path/to/unary_rpc_service/client && sudo chown -R user:user /path/to/unary_rpc_service/client/*
    ```

