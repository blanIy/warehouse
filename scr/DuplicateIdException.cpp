#include "DuplicateIdException.h"
#include <format>

DuplicateIdException::DuplicateIdException(std::string_view message, std::string_view duplicateId)
    : WarehouseException(duplicateId.empty()
        ? std::format("Дублирование ID: {}", message)
        : std::format("Дублирование ID: {} [Артикул: {}]", message, duplicateId)),
    duplicateId(duplicateId) {
}

std::string DuplicateIdException::getDuplicateId() const {
    return duplicateId;
}