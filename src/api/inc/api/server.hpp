#ifndef NEXUSPOOL_API_SERVER_HPP
#define NEXUSPOOL_API_SERVER_HPP

#include "network/socket_factory.hpp"
#include "network/types.hpp"
#include "network/endpoint.hpp"
#include "persistance/data_access.hpp"
#include "api/parser.hpp"

#include <memory>
#include <vector>
#include <string>

namespace spdlog { class logger; }
namespace nexuspool
{
namespace api
{
    class Connection;
    class Shared_data_access;

class Server 
{
public:

    Server(std::shared_ptr<spdlog::logger> logger,
        persistance::Data_access::Uptr data_access,
        std::string local_ip, 
        std::uint16_t api_listen_port, 
        network::Socket_factory::Sptr socket_factory);

    void start();
    void stop();
    network::Endpoint const& local_endpoint() const { return m_listen_socket->local_endpoint(); }

private:

    std::shared_ptr<spdlog::logger> m_logger;
    std::shared_ptr<Shared_data_access> m_shared_data_access;
    std::string m_local_ip;
    std::uint16_t m_api_listen_port;
    network::Endpoint m_local_endpoint;
    network::Socket_factory::Sptr m_socket_factory;
    network::Socket::Sptr m_listen_socket;
    Parser::Sptr m_parser;
    std::vector<std::shared_ptr<api::Connection>> m_api_connections;
};

}
}

#endif
