#include "Help.hpp"
#include <iostream>

void SR2Server::Help::PrintHelp()
{
	std::cout << "Usage: sr2server [options]" << std::endl << std::endl;
	std::cout << "Options:" << std::endl;
	std::cout << "  -h, --help                               Print this help message" << std::endl;
	std::cout << "  -r, --runserver <port>                   Set the server to run on <port>, 8000 by default" << std::endl;
	std::cout << "  -mm, --make-migrations <app>             Call make migrations on TinyORM on <app>, all apps by default" << std::endl;
	std::cout << "  -m, --migrate                            Call migrate on TinyORM" << std::endl;
	std::cout << "  -d, --drop                               Call drop on TinyORM" << std::endl;
	std::cout << "  -c, --command                            Call a custom command" << std::endl;
}
