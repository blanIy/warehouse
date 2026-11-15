#include "WarehouseException.h"

WarehouseException::WarehouseException(std::string_view message)
    : std::runtime_error(std::string(message)) {
}

WarehouseException::~WarehouseException() = default;