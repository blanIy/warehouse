#pragma once
#include "WarehouseException.h"
#include <format>
#include <string_view>

class LimitExceededException : public WarehouseException {
private:
    double currentValue = 0.0;
    double limitValue = 0.0;

public:
    explicit LimitExceededException(std::string_view message, double currentValue = 0.0, double limitValue = 0.0)
        : WarehouseException(std::format("Превышено ограничение: {} [Текущее: {:.2f}, Лимит: {:.2f}]", message, currentValue, limitValue)),
        currentValue(currentValue),
        limitValue(limitValue) {
    }

    [[nodiscard]] double getCurrentValue() const noexcept {
        return currentValue;
    }

    [[nodiscard]] double getLimitValue() const noexcept {
        return limitValue;
    }
};