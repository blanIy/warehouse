#include "LimitExceededException.h"
#include <format>

LimitExceededException::LimitExceededException(std::string_view message, double currentValue, double limitValue)
    : WarehouseException(std::format("Превышено ограничение: {} [Текущее: {:.2f}, Лимит: {:.2f}]", message, currentValue, limitValue)),
    currentValue(currentValue),
    limitValue(limitValue) {
}

double LimitExceededException::getCurrentValue() const noexcept {
    return currentValue;
}

double LimitExceededException::getLimitValue() const noexcept {
    return limitValue;
}