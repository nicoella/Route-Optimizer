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

std::vector<std::vector<std::string>> destination_parser(std::string destinations) {
    std::vector<std::vector<std::string>> data_array;

    std::istringstream iss(destinations);
    std::string part;

    while (std::getline(iss, part, '%')) {
        std::istringstream innerIss(part);
        std::string value;
        std::vector<std::string> row;

        while (std::getline(innerIss, value, ',')) {
            row.push_back(value);
        }

        data_array.push_back(row);
    }

    return data_array;
}

std::vector<int> group_count_parser(std::string group_count) {
    std::vector<int> numbers;

    std::istringstream iss(group_count);
    std::string part;

    while (std::getline(iss, part, ',')) {
        numbers.push_back(std::stoi(part));
    }

    return numbers;
}