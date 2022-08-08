#include <chrono>
#include <thread>

#include "../common/logging.h"

#include <asio.hpp>

using namespace std::chrono_literals;
using           std::chrono::system_clock;

using namespace boost;

int main( void )
{
    auto console   = logging::setup( );

    auto max_size  = 200 * 1'024 * 1'024;
    auto max_files = 1;
    auto logger = spdlog::rotating_logger_mt( "run.log", "D:/Build/Runs/run.log", max_size, max_files );

    // Step 1. Assume that the client application has already obtained the IP-address and the protocol port
    // number.
    std::string raw_ip_address = "127.0.0.1";
    unsigned short port_number = 3333;

    // Used to store information about error that happens while parsing the raw IP-address.
    system::error_code ec;

    // Step 2. Using IP protocol version independent address representation
    asio::ip::address ip_address = asio::ip::address::from_string(raw_ip_address, ec);
    if (ec.value() != 0) 
    {
        // Provided IP address is invalid. Breaking execution.
        console->error("Failed to parse the IP address. Error code = {}. Message: {}", ec.value(), ec.message());

        return ec.value();
    }

    // Step 3.
    asio::ip::tcp::endpoint ep( ip_address,
                                port_number
                              );
    uint64_t i = 0;
    for( ;; )
    {
        std::cout << ".";

        std::this_thread::sleep_for  ( 10ns );
        std::this_thread::sleep_until( system_clock::now( ) + 1s );

        if ( ++i % 80 == 0 )
        {
            console->debug( "Here's the endpoint: {}", ep );
            logger ->debug( "Here's the endpoint: {}", ep ); logger->flush( );
        }
    }

  // Step 4. The endpoint is ready and can be used to specify a particular server in the network the client
  // wants to communicate with.

  return 0;
}
