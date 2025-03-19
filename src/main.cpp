#include "utility.h"

#include "bayou_client.h"

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
	
	// Configure network mode
	if (is_server)
	{
		logln("Starting in server mode...");
	}
	else if (is_client)
	{
		logln("Starting in client mode, connecting to {}...", server_ip);
		c_bayou_client client;
		client.go();
	}
	else
	{
		logln("Starting in standalone mode...");
	}
	
	// Start the game
	return 0;
}
