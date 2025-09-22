#include "RunServer.hpp"
#include <crow.h>
#include <crow/query_string.h>
#include <orm/db.hpp>

void SR2Server::RunServer::Run(uint16_t port)
{
    crow::SimpleApp app; //define your crow application

    auto manager = Orm::DB::create({
        {"driver",          "QMYSQL"},
        {"host",            qEnvironmentVariable("DB_HOST", "127.0.0.1")},
        {"port",            qEnvironmentVariable("DB_PORT", "3306")},
        {"database",        qEnvironmentVariable("DB_DATABASE", "sr2testdb")},
        {"username",        qEnvironmentVariable("DB_USERNAME", "sr2_sample")},
        {"password",        qEnvironmentVariable("DB_PASSWORD", "sample_sr2")},
        {"charset",         qEnvironmentVariable("DB_CHARSET", "utf8mb4")},
        {"collation",       qEnvironmentVariable("DB_COLLATION", "utf8mb4_0900_ai_ci")},
        {"timezone",        "+00:00"},
        /* Specifies what time zone all QDateTime-s will have, the overridden default is
        the QTimeZone::UTC, set to the QTimeZone::LocalTime or
        QtTimeZoneType::DontConvert to use the system local time. */
        {"qt_timezone",     QVariant::fromValue(QTimeZone::UTC)},
        {"prefix",          ""},
        {"prefix_indexes",  false},
        {"strict",          true},
        {"engine",          "InnoDB"},
        {"options",         QVariantHash()},
    });

    std::cout << "Database connection successful on: " << manager->hostName().toStdString() << "!" << std::endl;
    std::cout << "Database name: " << manager->databaseName().toStdString() << std::endl;
    std::cout << "Database user: " << manager->connection().getName().toStdString() << std::endl;
    
    //define your endpoint at the root directory
    CROW_ROUTE(app, "/").methods(crow::HTTPMethod::GET)
    ([](const crow::request& req)
    {
        return crow::response(200, "text/plain", "Hello error");
    });

    CROW_ROUTE(app, "/sample/<uint>").methods(crow::HTTPMethod::GET)
    ([](const crow::request& req, uint32_t index)
    {
        return crow::response(200, "text/plain", "Hello success");
    });

    CROW_ROUTE(app, "/sample_query").methods(crow::HTTPMethod::GET)
    ([](const crow::request& req)
    {
        auto url_params = crow::query_string(req.url_params);
        std::string coder = req.url_params.get("coder") == nullptr ? "" : req.url_params.get("coder");

        std::cout << "url_params: " << url_params << "\n";
        std::cout << "challenge: " << coder << "\n";

        return crow::response(200, "text/html; charset=utf-8", coder);
    });

    //set the port, set the app to run on multiple threads, and run the app
    try
    {
        app.port(port).concurrency(8).run();
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}