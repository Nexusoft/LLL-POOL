#ifndef NEXUSPOOL_POOL_MANAGER_HPP
#define NEXUSPOOL_POOL_MANAGER_HPP

#include "network/socket_factory.hpp"
#include "network/types.hpp"
#include <spdlog/spdlog.h>
#include "chrono/timer_factory.hpp"
#include "session.hpp"
#include "block.hpp"

#include <memory>
#include <mutex>

namespace asio { class io_context; }

namespace nexuspool
{
namespace config { class Config; }
class Wallet_connection;

class Pool_manager : public std::enable_shared_from_this<Pool_manager>
{
public:

    Pool_manager(std::shared_ptr<asio::io_context> io_context, config::Config& config, network::Socket_factory::Sptr socket_factory);

    void start();
    void stop();

    // Methods towards wallet_connection
    void set_current_height(std::uint32_t height);

    // Methods towards miner_connection
    using Get_block_handler = std::function<void(LLP::CBlock const& block)>;
    void get_block(Session_key key, Get_block_handler handler);


private:

    chrono::Timer::Handler session_registry_maintenance_handler(std::uint16_t session_registry_maintenance_interval);

    std::shared_ptr<::asio::io_context> m_io_context;
    config::Config& m_config;
    chrono::Timer_factory::Sptr m_timer_factory;
    network::Socket_factory::Sptr m_socket_factory;
    std::shared_ptr<spdlog::logger> m_logger;

    std::shared_ptr<Wallet_connection> m_wallet_connection;     // connection to nexus wallet
    network::Socket::Sptr m_listen_socket;

    Session_registry m_session_registry;    // holds all sessions -> each session contains a miner_connection
    chrono::Timer::Uptr m_session_registry_maintenance;

    // connection variables
    std::atomic<std::uint32_t> m_current_height;
    LLP::CBlock m_block;
};
}

#endif
