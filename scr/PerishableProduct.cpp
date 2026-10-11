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