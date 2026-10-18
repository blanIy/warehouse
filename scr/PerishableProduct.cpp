#include "PerishableProduct.h"
#include <iostream>

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

std::string PerishableProduct::getProductType() const {
    return "Скоропортящийся груз (Холодильная цепь)";
}

void PerishableProduct::printDetailedReport() const {
    std::cout << "[ПАСПОРТ СКОРОПОРТЯЩЕГОСЯ ГРУЗА]\n"
        << "Артикул: " << getSku() << " | Название: " << getName() << "\n"
        << "Масса: " << getWeight() << " кг | Объем: " << getVolume() << " м3\n"
        << "Срок реализации до: " << expiryDate << "\n"
        << "Температура в камере: " << targetTemperature << " C\n"
        << "Суточный тариф хранения: " << calculateDailyStorageCost() << " руб.\n";
}

double PerishableProduct::calculateDailyStorageCost() const {
    return (getWeight() * 0.5) + 15.0;
}

std::unique_ptr<Product> PerishableProduct::clone() const {
    return std::make_unique<PerishableProduct>(*this);
}

double PerishableProduct::calculateCoolingCost(double ambientTemp) const {
    if (ambientTemp > targetTemperature) {
        return (ambientTemp - targetTemperature) * 3.5;
    }
    return 0.0;
}