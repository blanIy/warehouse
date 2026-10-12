#include "ChemicalProduct.h"

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

double ChemicalProduct::calculateDailyStorageCost() const {
    return (weight * 0.5) + 25.0;
}