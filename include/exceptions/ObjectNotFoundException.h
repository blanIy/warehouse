#pragma once
#include "WarehouseException.h"
#include <format>
#include <string>
#include <string_view>

class ObjectNotFoundException : public WarehouseException {
private:
    std::string identifier;

public:
    explicit ObjectNotFoundException(std::string_view message, std::string_view identifier = "")
        : WarehouseException(identifier.empty()
            ? std::format("Объект не найден: {}", message)
            : std::format("Объект не найден: {} [ID: {}]", message, identifier)),
        identifier(identifier) {
    }

    [[nodiscard]] std::string getIdentifier() const {
        return identifier;
    }
};