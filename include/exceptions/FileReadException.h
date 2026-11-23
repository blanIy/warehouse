#pragma once
#include "WarehouseException.h"
#include <string>

class FileReadException : public WarehouseException {
public:
    explicit FileReadException(const std::string& msg = "Ошибка чтения файла.")
        : WarehouseException(msg) {
    }
};
