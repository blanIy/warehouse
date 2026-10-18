#include "PerishableProduct.h"

PerishableProduct::PerishableProduct(std::string_view sku, std::string_view name, std::string_view category,
    double weight, double volume, std::string_view expiryDate, double targetTemperature)
    : Product(sku, name, category, weight, volume), expiryDate(expiryDate), targetTemperature(targetTemperature) {
}

std::string PerishableProduct::getExpiryDate() const {
    return expiryDate;
}

double PerishableProduct::getTargetTemperature() const {
    return targetTemperature;
}

void PerishableProduct::setExpiryDate(std::string_view newExpiryDate) {
    expiryDate = newExpiryDate;
}

void PerishableProduct::setTargetTemperature(double newTemperature) {
    targetTemperature = newTemperature;
}

double PerishableProduct::calculateDailyStorageCost() const {
    return (getWeight() * 0.5) + 15.0;
}

double PerishableProduct::calculateCoolingCost(double ambientTemp) const {
    if (ambientTemp > targetTemperature) {
        return (ambientTemp - targetTemperature) * 3.5;
    }
    return 0.0;
}