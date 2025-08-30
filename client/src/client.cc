/**
 * @file client.cc
 *
 * @brief UnaryRPCClient represents gRPC client with mutual TLS (mTLS) authentication.
 *
 * @author Hovsep Papoyan
 * Contact: papoyanhovsep93@gmail.com
 * @Date 2025-08-30
 *
 */

#include <grpcpp/grpcpp.h>

#include <iostream>
#include <memory>
#include <string>

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "helper/helper.h"

#include "unary_rpc_service.grpc.pb.h"

ABSL_FLAG(::std::string, target, "localhost:50051", "Server address");

class UnaryRPCClient
{
public:
    UnaryRPCClient(::std::shared_ptr<::grpc::Channel> channel)
        : m_stub(::example::UnaryRPCService::NewStub(channel))
    { }

    // Assembles the client's payload, sends it and presents the response back from the server.
    ::std::string UnaryRPC(const ::std::string& user)
    {
        // Data we are sending to the server.
        ::example::Request request;
        request.set_payload(user);

        // Container for the data we expect from the server.
        ::example::Response response;

        // Context for the client. It could be used to convey extra
        // information to the server and/or tweak certain RPC behaviors.
        ::grpc::ClientContext context;

        // Set 1 second timeout
        context.set_deadline(::std::chrono::system_clock::now() + ::std::chrono::seconds(1));

        // The actual RPC.
        const ::grpc::Status status = m_stub->UnaryRPC(&context, request, &response);

        // Act upon its status.
        if (status.ok())
        {
            return response.payload();
        }
        else
        {
            ::std::cerr << status.error_code() << ": " << status.error_message() << '\n';
            return "RPC failed";
        }
    }

private:
    ::std::unique_ptr<::example::UnaryRPCService::Stub> m_stub;
};

int main(int argc, char** argv)
{
    ::absl::ParseCommandLine(argc, argv);

    // Load SSL credentials and build a SSL options for the channel
    const ::std::string keyPath = ::std::getenv("GRPC_SSL_KEY_PATH");
    const ::std::string crtPath = ::std::getenv("GRPC_SSL_CRT_PATH");
    ::grpc::SslCredentialsOptions sslOptions;
    sslOptions.pem_private_key = ::helper::loadStringFromFile(keyPath);
    sslOptions.pem_cert_chain = ::helper::loadStringFromFile(crtPath);

    // NOTE: Do NOT set `pem_root_certs` here — it will be loaded from the env var
    // sslOptions.pem_root_certs = ""; // <-- leave unset

    // Instantiate the client. It requires a channel, out of which the actual RPCs
    // are created. This channel models a connection to an endpoint specified by
    // the argument "--target=" which is the only expected argument.
    const ::std::string targetStr = ::absl::GetFlag(FLAGS_target);

    // Create a channel with SSL credentials
    UnaryRPCClient client(::grpc::CreateChannel(targetStr, ::grpc::SslCredentials(sslOptions)));
    const ::std::string response = client.UnaryRPC("Request from client");
    ::std::cout << "Client received: " << response << '\n';
}

