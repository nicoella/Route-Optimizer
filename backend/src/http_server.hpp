#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/asio.hpp>
#include <iostream>

boost::asio::io_context ioc;
namespace http = boost::beast::http;
using tcp = boost::asio::ip::tcp;


void handle_request(http::request<http::string_body>& req, http::response<http::string_body>& res)
{
    if (req.method() == http::verb::get) { // handle GET requests
        if (req.target() == "/hello") {
            res.result(http::status::ok);
            res.set(http::field::content_type, "text/plain");
            res.body() = "Hello, Boost.Beast!";
            res.prepare_payload();
            return;
        }
    } else if (req.method() == http::verb::post) { // handle POST requests
        if (req.target() == "/test") {
            res.result(http::status::ok);
            res.set(http::field::content_type, "text/plain");
            res.body() = "Hello, Boost.eee!";
            res.prepare_payload();
            return;
        }
    } 
    // default
    res.body() = "Boost.Beast API";
    res.prepare_payload();
}

void start_server()
{
    tcp::acceptor acceptor(ioc, tcp::endpoint(tcp::v4(), 8080)); 

    while (true)
    {
        tcp::socket socket(ioc);
        acceptor.accept(socket);

        boost::beast::flat_buffer buffer;
        http::request<http::string_body> req;
        http::response<http::string_body> res;

        http::read(socket, buffer, req);
        handle_request(req, res);
        http::write(socket, res);
    }
}