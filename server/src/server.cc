/**
 * @file server.cc
 *
 * @brief Represents gRPC server with UnaryRPCService registered with mutual TLS (mTLS) authentication.
 *
 * @author Hovsep Papoyan
 * Contact: papoyanhovsep93@gmail.com
 * @Date 2025-08-30
 *
 */

#include <grpcpp/grpcpp.h>

#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "absl/log/initialize.h"
#include "absl/strings/str_format.h"
#include "helper/helper.h"

#include "unary_rpc_service.grpc.pb.h"

ABSL_FLAG(uint16_t, port, 50051, "Server port for the service");

// Logic and data behind the server's behavior.
class UnaryRPCServiceImpl final : public ::example::UnaryRPCService::Service
{
    ::grpc::Status UnaryRPC(::grpc::ServerContext* const context,
        const ::example::Request* const request,
        ::example::Response* const response) override
    {
        ::std::string idStr;
        const ::std::shared_ptr<const ::grpc::AuthContext> authCtx = context->auth_context();
        if (authCtx && authCtx->IsPeerAuthenticated())
        {
            const ::std::string identityProperty = authCtx->GetPeerIdentityPropertyName();
            for (const auto& identity : authCtx->GetPeerIdentity())
            {
                idStr = ::std::string(identity.data(), identity.size());
                ::std::cout << "Peer identity (" << identityProperty << "): " << idStr << '\n';
            }
        }
        else
        {
            // This should never happen to the server and client
            return ::grpc::Status(::grpc::StatusCode::UNAUTHENTICATED, "Peer not authenticated");
        }

        ::std::cout << "Server received: " << request->payload() << '\n';
        response->set_payload("Response from Server");
        return ::grpc::Status::OK;
    }
};

void runServer(const uint16_t port)
{
    const ::std::string keyPath = ::std::getenv("GRPC_SSL_KEY_PATH");
    const ::std::string crtPath = ::std::getenv("GRPC_SSL_CRT_PATH");
    const ::std::string caPath = ::std::getenv("GRPC_SSL_CA_PATH");

    // Load SSL credentials and build a SSL credential options
    ::grpc::SslServerCredentialsOptions::PemKeyCertPair keyCertPair =
        { ::helper::loadStringFromFile(keyPath), ::helper::loadStringFromFile(crtPath) };
    ::grpc::SslServerCredentialsOptions sslOptions;
    sslOptions.pem_key_cert_pairs.emplace_back(keyCertPair);
    sslOptions.pem_root_certs = ::helper::loadStringFromFile(caPath);

    // Require and verify the client's certificate (mutual TLS)
    sslOptions.client_certificate_request = GRPC_SSL_REQUEST_AND_REQUIRE_CLIENT_CERTIFICATE_AND_VERIFY;

    const ::std::string serverAddress = ::absl::StrFormat("0.0.0.0:%d", port);
    UnaryRPCServiceImpl service;

    ::grpc::ServerBuilder builder;

    // Listen on the given address with SSL credentials
    builder.AddListeningPort(serverAddress, ::grpc::SslServerCredentials(sslOptions));

    // Register "service" as the instance through which we'll communicate with clients.
    // In this case it corresponds to an *synchronous* service.
    builder.RegisterService(&service);
  
    // Finally assemble the server.
    const ::std::unique_ptr<::grpc::Server> server(builder.BuildAndStart());
    ::std::cout << "Server listening on " << serverAddress << '\n';

    // Wait for the server to shutdown. Note that some other thread must be
    // responsible for shutting down the server for this call to ever return.
    server->Wait();
}

int main(int argc, char** argv)
{
    ::absl::ParseCommandLine(argc, argv);
    ::absl::InitializeLog();
    runServer(::absl::GetFlag(FLAGS_port));
}

