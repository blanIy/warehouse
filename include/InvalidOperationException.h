#pragma once
#include "WarehouseException.h"

class InvalidOperationException : public WarehouseException {
private:
    std::string operationName;

public:
    explicit InvalidOperationException(std::string_view message, std::string_view operationName = "");
    [[nodiscard]] std::string getOperationName() const;
};
