#include "ObjectNotFoundException.h"
#include <format>

ObjectNotFoundException::ObjectNotFoundException(std::string_view message, std::string_view identifier)
    : WarehouseException(identifier.empty()
        ? std::format("Объект не найден: {}", message)
        : std::format("Объект не найден: {} [ID: {}]", message, identifier)),
    identifier(identifier) {
}

std::string ObjectNotFoundException::getIdentifier() const {
    return identifier;
}