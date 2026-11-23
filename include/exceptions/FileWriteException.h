#pragma once
#include "WarehouseException.h"
#include <string>

class FileWriteException : public WarehouseException {
public:
    explicit FileWriteException(const std::string& msg = "Ошибка записи в файл.")
        : WarehouseException(msg) {
    }
};