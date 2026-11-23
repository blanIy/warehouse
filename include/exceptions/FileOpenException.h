#pragma once
#include "WarehouseException.h"
#include <string>

class FileOpenException : public WarehouseException {
public:
    explicit FileOpenException(const std::string& msg = "Не удалось открыть файл.")
        : WarehouseException(msg) {
    }
};
