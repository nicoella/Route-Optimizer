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

std::vector<std::string_view> param_parser(string_view_converter target) {
    std::size_t query_start = std::string_view(target.data, target.size).find('?');
    std::string_view query_string = std::string_view(target.data + query_start + 1, target.size - query_start - 1);
    std::vector<std::string_view> param_pairs;

    size_t start_pos = 0;
    while (start_pos != std::string_view::npos) {
        size_t end_pos = query_string.find('&', start_pos);
        param_pairs.push_back(query_string.substr(start_pos, end_pos - start_pos));
        start_pos = (end_pos == std::string_view::npos) ? end_pos : end_pos + 1;
    }

    return param_pairs;
}