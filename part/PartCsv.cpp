#include "PartCsv.h"
#include <charconv>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cerrno>
namespace bufman {


namespace {
bool parse_integer(const std::string& text, int& value) {
    if (text.empty()) {
        return false;
    }
    const char* first = text.data();
    const char* last = first + text.size();
    const auto result = std::from_chars(first, last, value);
    return result.ec == std::errc{} && result.ptr == last;
}

bool parse_float(const std::string& text, float& value) {

    if (text.empty()|| text.front()== ' ' || text.front() =='\t'){
        return false;
    }
    errno =0;

    char* parse_end = nullptr;
    const float parsed = std::strtof(text.c_str(), &parse_end);

    if (parse_end != text.c_str() +text.size()|| errno == ERANGE) {
        return false;
        }
    
    value = parsed;
    return true;

}


bool parse_line(const std::string& line, Part& part, std::string& error) {
    std::stringstream input(line);
    std::string pid_text;
    std::string name;
    std::string part_weight;
    std::string part_color;
    std::string part_price;
    std::string part_material;
    std::string extra;

    if (!std::getline(input, pid_text, ',') ||
        !std::getline(input, name, ',') ||
        !std::getline(input, part_weight, ',') ||
        !std::getline(input, part_color, ',') ||
        !std::getline(input, part_price, ',') || !std::getline(input, part_material, ',') || std::getline(input, extra, ','))  {
        error = "expected exactly six comma-separated fields";
        return false;
    }

    int pid = 0;
    float weight = 0;
    int color=0;
    float price=0;
    if (!parse_integer(pid_text, pid) || pid <= 0) {
        error = "part_id must be a positive integer";
        return false;
    }
    if (!parse_float(part_weight, weight) || weight < 0) {
        error = "weight must be a nonnegative float";
        return false;
    }
    if (name.size() > 9) {
        error = "name must contain at most 9 characters";
        return false;
    }
    if (part_material.size() > 9) {
        error = "material must contain at most 9 characters";
        return false;
    }
    if (!parse_integer(part_color, color) || color < 0 || color > 5 ) {
        error = "color must be  a number between 0 to 5";
        return false;
    }
    if (!parse_float(part_price, price) || price < 0) {
        error = "price must be a nonnegative float";
        return false;
    }


    part = Part{};
    part.part_id=pid;
    part.part_weight = weight;
    part.part_color = color;
    part.part_price=price;

    name.copy(part.part_name, name.size());
    part_material.copy(part.part_material, part_material.size());
    return true;
}
}


PartLoadResult  load_parts(const std::string& path, std::ostream& diagnostics) {
    PartLoadResult result;
    std::ifstream input(path);
    if (!input) {
        diagnostics << "cannot open CSV file: " << path << '\n';
        result.skipped = 1;
        return result;
    }

    std::string line;
    std::size_t line_number = 0;
    while (std::getline(input, line)) {
        ++line_number;
        if (line.empty()) {
            ++result.skipped;
            diagnostics << "line " << line_number << ": blank line\n";
            continue;
        }
        Part part{};
        std::string error;
        if (!parse_line(line, part, error)) {
            ++result.skipped;
            diagnostics << "line " << line_number << ": " << error << '\n';
            continue;
        }
        result.parts.push_back(part);
    }
    return result;}

}   