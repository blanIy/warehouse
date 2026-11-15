#pragma once
#include "WarehouseException.h"

class LimitExceededException : public WarehouseException {
private:
    double currentValue = 0.0;
    double limitValue = 0.0;

public:
    explicit LimitExceededException(std::string_view message, double currentValue = 0.0, double limitValue = 0.0);
    [[nodiscard]] double getCurrentValue() const noexcept;
    [[nodiscard]] double getLimitValue() const noexcept;
};