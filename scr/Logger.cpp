#include "Logger.h"
#include "exceptions/FileOpenException.h"
#include "exceptions/FileReadException.h"
#include "exceptions/FileWriteException.h"
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <sstream>

std::string Logger::timestamp() {
    const std::time_t now = std::time(nullptr);
    std::tm local{};
#ifdef _WIN32
    localtime_s(&local, &now);
#else
    localtime_r(&now, &local);
#endif
    std::ostringstream os;
    os << std::put_time(&local, "%Y-%m-%d %H:%M:%S");
    return os.str();
}

void Logger::append(std::string_view path, std::string_view message) {
    const std::filesystem::path filePath(path);
    if (filePath.has_parent_path()) {
        std::error_code ec;
        std::filesystem::create_directories(filePath.parent_path(), ec);
    }

    std::ofstream out(filePath, std::ios::app);
    if (!out.is_open()) {
        throw FileOpenException("Не удалось открыть файл журнала для записи: " + std::string(path));
    }

    out << "[" << timestamp() << "] " << message << "\n";
    out.flush();
    if (!out) {
        throw FileWriteException("Ошибка записи в файл журнала: " + std::string(path));
    }
}

std::string Logger::readAll(std::string_view path) {
    std::ifstream in{ std::filesystem::path(path) };
    if (!in.is_open()) {
        throw FileOpenException("Не удалось открыть файл журнала для чтения: " + std::string(path));
    }

    std::ostringstream buffer;
    buffer << in.rdbuf();
    if (in.bad()) {
        throw FileReadException("Ошибка чтения файла журнала: " + std::string(path));
    }
    return buffer.str();
}