#pragma once
#include <stdexcept>
#include <string>
#include <string_view>

class WarehouseException : public std::runtime_error {
public:
    explicit WarehouseException(std::string_view message)
        : std::runtime_error(std::string(message)) {}

    WarehouseException(const WarehouseException&) = default;
    WarehouseException& operator=(const WarehouseException&) = default;
    WarehouseException(WarehouseException&&) noexcept = default;
    WarehouseException& operator=(WarehouseException&&) noexcept = default;
    virtual ~WarehouseException() = 0;
};

inline WarehouseException::~WarehouseException() = default;