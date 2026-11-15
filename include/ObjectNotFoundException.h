#pragma once
#include "WarehouseException.h"

class ObjectNotFoundException : public WarehouseException {
private:
    std::string identifier;

public:
    explicit ObjectNotFoundException(std::string_view message, std::string_view identifier = "");
    [[nodiscard]] std::string getIdentifier() const;
};