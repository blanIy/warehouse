#pragma once
#include "WarehouseException.h"
#include <format>
#include <string>
#include <string_view>

class InvalidOperationException : public WarehouseException {
private:
    std::string operationName;

public:
    explicit InvalidOperationException(std::string_view message, std::string_view operationName = "")
        : WarehouseException(operationName.empty()
            ? std::format("Недопустимая операция: {}", message)
            : std::format("Недопустимая операция: {} [Операция: {}]", message, operationName)),
        operationName(operationName) {
    }

    [[nodiscard]] std::string getOperationName() const {
        return operationName;
    }
};