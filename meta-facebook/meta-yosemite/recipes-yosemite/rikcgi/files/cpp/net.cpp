

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

#include "json.hpp"
using json = nlohmann::json;
using namespace std::chrono_literals;



int main(int argc, char const *argv[])
{
    openlog("rikcgi-net", LOG_CONS, LOG_USER);


	if((argc > 1) && (std::strcmp(argv[1], "--store") == 0))
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
	else
	{ // --get
		if((argc > 1) && (std::strcmp(argv[1], "--get") == 0))
			syslog(LOG_INFO, " ~~~ GET command");
		else if(argc > 1)
			syslog(LOG_INFO, " ~~~ command is %s", argv[1]);
		std::cout << "{\"mac\":\"aa-bb-cc-dd-ee-ff\",\"hostname\":\"testtest\",\"dhcp\":\"yes\",\"addr\":\"10.10.0.222\",\"mask\":\"255.255.255.0\",\"gateway\":\"10.10.0.1\",\"dns\":\"\",\"ip6_addr\":\"\",\"ip6_com\":\"add\",\"ip6_auto_conf\":\"on\",\"ip6_dhcp\":\"stateless\"}";
	}

	return 0;
}


