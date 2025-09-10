#include <iostream>
#include <orm/db.hpp>
#include "Help.hpp"
#include "RunServer.hpp"
#include <unordered_map>
#include <string>

const std::unordered_map<std::string, uint32_t> standalone_cmd_option = {
    {"--help", 1},
    {"-h", 1},
    {"--runserver", 2},
    {"-r", 2},
    // Add more error codes and messages as needed
};

uint16_t arg_to_uint16(const char* arg);
void RunServerWrapper(int argc, char** argv);

int main(int argc, char** argv)
{
    if(argc == 1)
        SR2Server::Help::PrintHelp();
    else
    {
        std::string arg = argv[1];
        auto it = standalone_cmd_option.find(arg);
        if (it != standalone_cmd_option.end()) {
            switch (it->second)
            {
                case 1:
                    SR2Server::Help::PrintHelp();
                    break;
                case 2:
                    RunServerWrapper(argc, argv);
                    break;
                default:
                    SR2Server::Help::PrintHelp();
                    break;
            }
        } 
        
        else
        {
            SR2Server::Help::PrintHelp();
        }
    }
    return 0;
}

void RunServerWrapper(int argc, char** argv)
{
    uint16_t port = 8000;
    if(argc > 2)
    {
        std::string port_arg = argv[2];
        try
        {
            port = arg_to_uint16(port_arg.c_str());
        }
        catch(std::invalid_argument e)
        {
            std::cerr << "Invalid port, must be a number (0-65535)\n";
            exit(65);
        }
        catch(std::out_of_range e)
        {
            std::cerr << "Invalid port, must be a number (0-65535)\n";
            exit(65);
        }
    }
    SR2Server::RunServer::Run(port);
}

uint16_t arg_to_uint16(const char* arg)
{
    char* end;
    unsigned long value = std::strtoul(arg, &end, 10);

    if (*end != '\0') {
        throw std::invalid_argument("Invalid number format.");
    }
    if (value > UINT16_MAX) {
        throw std::out_of_range("Value exceeds uint16_t range.");
    }

    return static_cast<uint16_t>(value);
}
