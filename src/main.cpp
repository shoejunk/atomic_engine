#include "bayou.h"
#include "network_game_manager.h"
#include "utility.h"

#include <iostream>
#include <string>
#include <memory>

using namespace atom;

void print_usage()
{
	logln("Usage: bayou [options]");
	logln("Options:");
	logln("  -s, --server         Run as server");
	logln("  -c, --client <ip>    Run as client and connect to the specified IP address");
	logln("  -h, --help           Display this help message");
}

int main(int argc, char* argv[])
{
	// Default values
	bool is_server = false;
	bool is_client = false;
	std::string server_ip = "127.0.0.1"; // Default to localhost
	
	// Parse command line arguments
	for (int i = 1; i < argc; ++i)
	{
		std::string arg = argv[i];
		
		if (arg == "-s" || arg == "--server")
		{
			is_server = true;
		}
		else if (arg == "-c" || arg == "--client")
		{
			is_client = true;
			
			// Check if there's a next argument for the IP address
			if (i + 1 < argc)
			{
				server_ip = argv[++i];
			}
		}
		else if (arg == "-h" || arg == "--help")
		{
			print_usage();
			return 0;
		}
		else
		{
			errorln("Unknown option: {}", arg);
			print_usage();
			return 1;
		}
	}
	
	// Create the bayou instance
	c_bayou bayou;
	
	// Create the network game manager with the game state
	auto network_manager = std::make_shared<c_network_game_manager>(bayou.get_game_state());
	
	// Set the network manager in the bayou instance first
	bayou.set_network_manager(network_manager);
	
	// Configure network mode
	if (is_server)
	{
		logln("Starting in server mode...");
		network_manager->start_server();
	}
	else if (is_client)
	{
		logln("Starting in client mode, connecting to {}...", server_ip);
		network_manager->start_client(server_ip);
	}
	else
	{
		logln("Starting in standalone mode...");
	}
	
	// Start the game
	return bayou.go();
}
