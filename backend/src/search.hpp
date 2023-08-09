#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <iostream>
#include "json.hpp"

namespace http = boost::beast::http;
namespace net = boost::asio;
using tcp = net::ip::tcp;
namespace ssl = boost::asio::ssl;
namespace beast = boost::beast;


std::string text_search(const std::string& query, double latitude, double longitude, int radius, const std::string& api_key) {
    try {
        const std::string host = "maps.googleapis.com";
        const std::string target = "/maps/api/place/textsearch/json?query=" + query + "&location=" + std::to_string(latitude) + "," + std::to_string(longitude) + "&radius=" + std::to_string(radius) + "&maxResultCount=20&key=" + api_key;

        net::io_context io_context;
        ssl::context ssl_context(ssl::context::sslv23_client);
        tcp::resolver resolver(io_context);
        ssl::stream<tcp::socket> socket(io_context, ssl_context);

        // Connect to the API server
        tcp::resolver::results_type endpoints = resolver.resolve(host, "https");
        net::connect(socket.next_layer(), endpoints.begin(), endpoints.end());
        socket.handshake(ssl::stream_base::client);

        // Create the HTTPS request
        http::request<http::string_body> req{http::verb::get, target, 11};
        req.set(http::field::host, host);
        req.set(http::field::user_agent, "RouteOptimizer/1.0");

        // Send the HTTPS request
        http::write(socket, req);

        // Read the HTTPS response
        beast::flat_buffer buffer;
        http::response<http::string_body> res;
        http::read(socket, buffer, res);

        // Process the HTTPS response
        if (res.result() != http::status::ok) {
            throw std::runtime_error("HTTP request failed with status code: " + std::to_string(res.result_int()));
        }

        // Close the socket
        beast::error_code ec;
        socket.shutdown(ec);
        socket.next_layer().close();

        return res.body();
    } catch (std::exception const& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return "{\"status\", \"BAD\"}";
    }
}

std::string find_place(const std::string& query, const std::string& api_key) {
    try {
        const std::string host = "maps.googleapis.com";
        const std::string target = "/maps/api/place/findplacefromtext/json?input=" + query + "&inputtype=textquery&fields=geometry,formatted_address,name&key=" + api_key;

        net::io_context io_context;
        ssl::context ssl_context(ssl::context::sslv23_client);
        tcp::resolver resolver(io_context);
        ssl::stream<tcp::socket> socket(io_context, ssl_context);

        // Connect to the API server
        tcp::resolver::results_type endpoints = resolver.resolve(host, "https");
        net::connect(socket.next_layer(), endpoints.begin(), endpoints.end());
        socket.handshake(ssl::stream_base::client);

        // Create the HTTPS request
        http::request<http::string_body> req{http::verb::get, target, 11};
        req.set(http::field::host, host);
        req.set(http::field::user_agent, "RouteOptimizer/1.0");

        // Send the HTTPS request
        http::write(socket, req);

        // Read the HTTPS response
        beast::flat_buffer buffer;
        http::response<http::string_body> res;
        http::read(socket, buffer, res);

        // Process the HTTPS response
        if (res.result() != http::status::ok) {
            throw std::runtime_error("HTTP request failed with status code: " + std::to_string(res.result_int()));
        }

        // Close the socket
        beast::error_code ec;
        socket.shutdown(ec);
        socket.next_layer().close();

        return res.body();
    } catch (std::exception const& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return "{\"status\", \"BAD\"}";
    }
}

int distance(std::string lat1, std::string lng1, std::string lat2, std::string lng2, std::string api_key) {
    try {
        const std::string host = "maps.googleapis.com";
        const std::string target = "/maps/api/directions/json?destination=" + lat2 + "," + lng2 + "&origin=" + lat1 + "," + lng1 + "&key=" + api_key;

        net::io_context io_context;
        ssl::context ssl_context(ssl::context::sslv23_client);
        tcp::resolver resolver(io_context);
        ssl::stream<tcp::socket> socket(io_context, ssl_context);

        // Connect to the API server
        tcp::resolver::results_type endpoints = resolver.resolve(host, "https");
        net::connect(socket.next_layer(), endpoints.begin(), endpoints.end());
        socket.handshake(ssl::stream_base::client);

        // Create the HTTPS request
        http::request<http::string_body> req{http::verb::get, target, 11};
        req.set(http::field::host, host);
        req.set(http::field::user_agent, "RouteOptimizer/1.0");

        // Send the HTTPS request
        http::write(socket, req);

        // Read the HTTPS response
        beast::flat_buffer buffer;
        http::response<http::string_body> res;
        http::read(socket, buffer, res);

        // Process the HTTPS response
        if (res.result() != http::status::ok) {
            throw std::runtime_error("HTTP request failed with status code: " + std::to_string(res.result_int()));
        }

        // Close the socket
        beast::error_code ec;
        socket.shutdown(ec);
        socket.next_layer().close();

        nlohmann::json json_data = nlohmann::json::parse(res.body());

        int value = json_data["routes"][0]["legs"][0]["duration"]["value"];
        
        return value;
    } catch (std::exception const&e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
}