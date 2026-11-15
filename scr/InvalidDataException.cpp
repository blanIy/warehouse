#include "InvalidDataException.h"
#include <format>

InvalidDataException::InvalidDataException(std::string_view message, std::string_view fieldName)
    : WarehouseException(fieldName.empty()
        ? std::format("Некорректные данные: {}", message)
        : std::format("Некорректные данные: {} [Поле: {}]", message, fieldName)),
    fieldName(fieldName) {
}

std::string InvalidDataException::getFieldName() const {
    return fieldName;
}