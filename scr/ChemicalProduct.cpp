#include "ChemicalProduct.h"
#include <iostream>

ChemicalProduct::ChemicalProduct(std::string_view sku, std::string_view name, std::string_view category,
    double weight, double volume, std::string_view hazardClass, bool requiresVentilation)
    : Product(sku, name, category, weight, volume), hazardClass(hazardClass), requiresVentilation(requiresVentilation) {
}

std::string ChemicalProduct::getHazardClass() const {
    return hazardClass;
}

bool ChemicalProduct::getRequiresVentilation() const {
    return requiresVentilation;
}

void ChemicalProduct::setHazardClass(std::string_view newHazardClass) {
    hazardClass = newHazardClass;
}

void ChemicalProduct::setRequiresVentilation(bool newVentilation) {
    requiresVentilation = newVentilation;
}

std::string ChemicalProduct::getProductType() const {
    return "Опасный химический груз (ADR)";
}

void ChemicalProduct::printDetailedReport() const {
    std::cout << "[ПАСПОРТ ОПАСНОГО ГРУЗА]\n"
        << "Артикул: " << getSku() << " | Название: " << getName() << "\n"
        << "Масса: " << getWeight() << " кг | Объем: " << getVolume() << " м3\n"
        << "Класс опасности ADR: " << hazardClass << "\n"
        << "Принудительная вытяжка: " << (requiresVentilation ? "Обязательна" : "Не требуется") << "\n"
        << "Суточный тариф хранения: " << calculateDailyStorageCost() << " руб.\n";
}

double ChemicalProduct::calculateDailyStorageCost() const {
    return (getWeight() * 0.5) + 25.0;
}

std::unique_ptr<Product> ChemicalProduct::clone() const {
    return std::make_unique<ChemicalProduct>(*this);
}

double ChemicalProduct::calculateVentilationRequirement() const {
    if (requiresVentilation) {
        return getVolume() * 40.0;
    }
    return 0.0;
}