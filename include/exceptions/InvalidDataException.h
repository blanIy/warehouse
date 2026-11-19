#pragma once
#include "WarehouseException.h"
#include <format>
#include <string>
#include <string_view>

class InvalidDataException : public WarehouseException {
private:
    std::string fieldName;

public:
    explicit InvalidDataException(std::string_view message, std::string_view fieldName = "")
        : WarehouseException(fieldName.empty()
            ? std::format("Некорректные данные: {}", message)
            : std::format("Некорректные данные: {} [Поле: {}]", message, fieldName)),
        fieldName(fieldName) {
    }

    [[nodiscard]] std::string getFieldName() const {
        return fieldName;
    }
};