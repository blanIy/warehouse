#pragma once
#include "WarehouseException.h"

class DuplicateIdException : public WarehouseException {
private:
    std::string duplicateId;

public:
    explicit DuplicateIdException(std::string_view message, std::string_view duplicateId = "");
    [[nodiscard]] std::string getDuplicateId() const;
};
