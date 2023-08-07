#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/asio.hpp>
#include <iostream>
#include <cstdlib>
#include <string>
#include <sstream>
#include "search.hpp"
#include "utils.hpp"

boost::asio::io_context ioc;
namespace http = boost::beast::http;
using tcp = boost::asio::ip::tcp;

void handle_request(http::request<http::string_body>& req, http::response<http::string_body>& res)
{
    if (req.method() == http::verb::get) { // handle GET requests
        boost::beast::string_view path = req.target().substr(0, req.target().find('?'));
        if (path == "/hello") { // HELLO request
            res.result(http::status::ok);
            res.set(http::field::content_type, "text/plain");
            res.body() = "Hello, Boost.Beast!";
            res.prepare_payload();
            return;
        } else if (path == "/text_search") { // Google Maps Places API text search
            try {
                if (req.target().find('?') != std::string_view::npos) {
                    // params
                    std::string api_key = "";
                    std::string query = "";
                    double radius = 0; 
                    double latitude = 0;
                    double longitude = 0;

                    // parsing
                    string_view_converter target = req.target();
                    std::vector<std::string_view> param_pairs = param_parser(target);

                    for (const auto& param_pair : param_pairs) {
                        size_t equal_pos = param_pair.find('=');
                        if (equal_pos != std::string_view::npos) {
                            std::string_view key = param_pair.substr(0, equal_pos);
                            std::string_view value = param_pair.substr(equal_pos + 1);
                            
                            if (key == "query") {
                                query = value;
                            } else if (key == "radius") {
                                std::stringstream ss(value.data());
                                ss >> radius;
                            } else if (key == "latitude") {
                                std::stringstream ss(value.data());
                                ss >> latitude;
                            } else if (key == "longitude") {
                                std::stringstream ss(value.data());
                                ss >> longitude;
                            } else if (key == "api_key") {
                                api_key = value;
                            }
                        }
                    }
                    std::string result = text_search(query, latitude, longitude, radius, api_key);
                    res.result(http::status::ok);
                    res.set(http::field::content_type, "application/json");
                    res.content_length(result.size());
                    res.body() = result;

                    res.prepare_payload();
                    return;
                } 
            }  catch (std::exception const& e) {
                std::cerr << "Error: " << e.what() << std::endl;
            }
            res.result(http::status::ok);
            res.set(http::field::content_type, "text/plain");
            res.body() = "{\"status\", \"BAD\"}";
            res.prepare_payload();
            return;
        } else if (path == "/find_place") {
            try {
                if (req.target().find('?') != std::string_view::npos) {
                    // params
                    std::string api_key = "";
                    std::string query = "";

                    // parsing
                    string_view_converter target = req.target();
                    std::vector<std::string_view> param_pairs = param_parser(target);

                    for (const auto& param_pair : param_pairs) {
                        size_t equal_pos = param_pair.find('=');
                        if (equal_pos != std::string_view::npos) {
                            std::string_view key = param_pair.substr(0, equal_pos);
                            std::string_view value = param_pair.substr(equal_pos + 1);
                            
                            if (key == "query") {
                                query = value;
                            } else if (key == "api_key") {
                                api_key = value;
                            }
                        }
                    }
                    std::string result = find_place(query, api_key);
                    res.result(http::status::ok);
                    res.set(http::field::content_type, "application/json");
                    res.content_length(result.size());
                    res.body() = result;

                    res.prepare_payload();
                    return;
                } 
            }  catch (std::exception const& e) {
                std::cerr << "Error: " << e.what() << std::endl;
            }
            res.result(http::status::ok);
            res.set(http::field::content_type, "text/plain");
            res.body() = "{\"status\", \"BAD\"}";
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
    tcp::acceptor acceptor(ioc, tcp::endpoint(tcp::v4(), 3000)); 

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