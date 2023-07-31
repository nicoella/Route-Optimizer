#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/asio.hpp>
#include <iostream>

#include "http_server.hpp" 

int main()
{
    start_server();
    return 0;
}