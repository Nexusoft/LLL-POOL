#ifndef NEXUSPOOL_TIMER_MANAGER_HPP
#define NEXUSPOOL_TIMER_MANAGER_HPP

#include "chrono/timer_factory.hpp"
#include "chrono/timer.hpp"

#include <memory>
#include <vector>

namespace nexuspool 
{
namespace network
{
    class Endpoint;
    class Connection;
}

class Wallet_connection;

class Timer_manager
{
public:

    Timer_manager(chrono::Timer_factory::Sptr timer_factory);

    void start_connection_retry_timer(std::uint16_t timer_interval, std::weak_ptr<Wallet_connection> worker_manager, 
        network::Endpoint const& wallet_endpoint);
    // collect also data from the workers
    void start_get_height_timer(std::uint16_t timer_interval, std::weak_ptr<network::Connection> connection);

    void stop();

private:

    chrono::Timer::Handler connection_retry_handler(std::weak_ptr<Wallet_connection> worker_manager,
        network::Endpoint const& wallet_endpoint);
    chrono::Timer::Handler get_height_handler(std::uint16_t get_height_interval, std::weak_ptr<network::Connection> connection);

    chrono::Timer_factory::Sptr m_timer_factory;
    chrono::Timer::Uptr m_connection_retry_timer;
    chrono::Timer::Uptr m_get_height_timer;
};
}

#endif
