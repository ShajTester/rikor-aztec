

// #define _GNU_SOURCE     /* To get defns of NI_MAXSERV and NI_MAXHOST */

#include <iostream>
#include <fstream>
#include <iomanip>
#include <dirent.h>
#include <exception>
#include <string>
#include <cstring>
#include <random>
#include <chrono>
#include <ctime>

#include <set>

#include <syslog.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <ifaddrs.h>
#include <sys/socket.h>
#include <netdb.h>

#include "json.hpp"

extern "C"
{
#include <rikor-fru.h>
}

using json = nlohmann::json;
using namespace std::chrono_literals;


int main(int argc, char const *argv[])
{
    openlog("rikcgi-net", LOG_CONS, LOG_USER);


	if(argc > 1)
	{
		if(std::strcmp(argv[1], "--store") == 0)
		{
			syslog(LOG_INFO, " ~~~ STORE command");
		    std::string str;
		    std::getline(std::cin, str);
			syslog(LOG_INFO, " %s", str.c_str());
			json jin;
			try
			{
				jin = json::parse(str);
			}
			catch (std::exception& e)
			{
				syslog(LOG_ERR, "exception: %s", e.what());
			}

			syslog(LOG_INFO, " ~~~ IN %s", jin.dump().c_str());
		}
		else if(std::strcmp(argv[1], "--testlib") == 0)
		{
			int rc = 0;
			rikor_fru_t data;
			char eeprom_path[256];
			
			get_fru_device(eeprom_path);
			rc += fru_buf_init(&data);
			rc += read_fru(eeprom_path, &data);

			json jout;
			jout["ip1"] = data.ip1;
			jout["ip2"] = data.ip2;
			// jout["dhcp1"] = data.dhcp1;
			// jout["dhcp2"] = data.dhcp2;
			jout["psw1"] = data.psw1;
			jout["psw2"] = data.psw2;
			std::cout << std::setw(4) << jout << std::endl;
		}
		else
		{ // --get
			if((argc > 1) && (std::strcmp(argv[1], "--get") == 0))
				syslog(LOG_INFO, " ~~~ GET command");
			std::cout << "{\"mac\":\"aa-bb-cc-dd-ee-ff\",\"hostname\":\"testtest\",\"dhcp\":\"yes\",\"addr\":\"10.10.0.222\",\"mask\":\"255.255.255.0\",\"gateway\":\"10.10.0.1\",\"dns\":\"\",\"ip6_addr\":\"\",\"ip6_com\":\"add\",\"ip6_auto_conf\":\"on\",\"ip6_dhcp\":\"stateless\"}";
		}
	}
	else
	{
		struct ifaddrs *ifaddr, *ifa;
		int family, s, n;
		char host[NI_MAXHOST];

		if (getifaddrs(&ifaddr) == -1) 
		{
			perror("getifaddrs");
			exit(EXIT_FAILURE);
		}

		/* Walk through linked list, maintaining head pointer so we can free list later */
		for (ifa = ifaddr, n = 0; ifa != NULL; ifa = ifa->ifa_next, n++) 
		{
			if (ifa->ifa_addr == NULL)
				continue;

			family = ifa->ifa_addr->sa_family;
			if(family == AF_INET)
			{
				s = getnameinfo(ifa->ifa_addr, sizeof(struct sockaddr_in), host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
				if(s != 0)
					std::cout << "getnameinfo() failed: " << gai_strerror(s) << std::endl;
				else
					std::cout << ifa->ifa_name << "\taddress: <" << host << ">" << std::endl;
			}
			else if(family == AF_INET6)
			{
				s = getnameinfo(ifa->ifa_addr, sizeof(struct sockaddr_in6), host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
				if(s != 0)
					std::cout << "getnameinfo() failed: " << gai_strerror(s) << std::endl;
				else
					std::cout << ifa->ifa_name << "\t\taddress: <" << host << ">" << std::endl;
			}
		}
		freeifaddrs(ifaddr);
	}
	return 0;
}


