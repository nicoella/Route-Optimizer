#include <cstdlib>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/asio.hpp>

struct string_view_converter
{
    const char* data;
    std::size_t size;

    string_view_converter(boost::beast::string_view view) : data(view.data()), size(view.size()) {}

    explicit string_view_converter(std::string_view view) : data(view.data()), size(view.size()) {}
};