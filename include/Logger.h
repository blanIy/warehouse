#pragma once
#include <string>
#include <string_view>

class Logger {
public:
    static std::string timestamp();
    static void append(std::string_view path, std::string_view message);
    static std::string readAll(std::string_view path);
};
