#pragma once
#include "WarehouseException.h"

class InvalidDataException : public WarehouseException {
private:
    std::string fieldName;

public:
    explicit InvalidDataException(std::string_view message, std::string_view fieldName = "");
    [[nodiscard]] std::string getFieldName() const;
};
