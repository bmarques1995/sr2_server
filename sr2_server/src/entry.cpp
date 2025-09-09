#include <iostream>
#include "Sample.hpp" 
#include "crow.h"

int main(int argc, char** argv)
{
    SR2Server::Sample::PrintHello();
    std::cout << "Sample cmake\n";
    
    crow::SimpleApp app; //define your crow application

    //define your endpoint at the root directory
    CROW_ROUTE(app, "/").methods(crow::HTTPMethod::POST)([]() {
        return "Hello world";
        });

    //set the port, set the app to run on multiple threads, and run the app
    try
    {
		app.port(7700).multithreaded().run();
	}
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    return 0;
}