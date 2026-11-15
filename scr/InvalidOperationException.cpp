#include "InvalidOperationException.h"
#include <format>

InvalidOperationException::InvalidOperationException(std::string_view message, std::string_view operationName)
    : WarehouseException(operationName.empty()
        ? std::format("Недопустимая операция: {}", message)
        : std::format("Недопустимая операция: {} [Операция: {}]", message, operationName)),
    operationName(operationName) {
}

std::string InvalidOperationException::getOperationName() const {
    return operationName;
}