#pragma once
#include "WarehouseException.h"
#include <format>
#include <string>
#include <string_view>

class DuplicateIdException : public WarehouseException {
private:
    std::string duplicateId;

public:
    explicit DuplicateIdException(std::string_view message, std::string_view duplicateId = "")
        : WarehouseException(duplicateId.empty()
            ? std::format("Дублирование ID: {}", message)
            : std::format("Дублирование ID: {} [Артикул: {}]", message, duplicateId)),
        duplicateId(duplicateId) {
    }

    [[nodiscard]] std::string getDuplicateId() const {
        return duplicateId;
    }
};