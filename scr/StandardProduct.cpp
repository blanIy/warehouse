#include "StandardProduct.h"
#include <iostream>

StandardProduct::StandardProduct(std::string_view sku, std::string_view name, std::string_view category,
    double weight, double volume, std::string_view packageType)
    : Product(sku, name, category, weight, volume), packageType(packageType) {
}

std::string StandardProduct::getPackageType() const {
    return packageType;
}

void StandardProduct::setPackageType(std::string_view newPackageType) {
    packageType = newPackageType;
}

std::string StandardProduct::getProductType() const {
    return "Стандартный груз (Общего назначения)";
}

void StandardProduct::printDetailedReport() const {
    std::cout << "[ПАСПОРТ СТАНДАРТНОГО ГРУЗА]\n"
        << "Артикул: " << getSku() << " | Название: " << getName() << "\n"
        << "Масса: " << getWeight() << " кг | Объем: " << getVolume() << " м3\n"
        << "Тип тары: " << packageType << "\n"
        << "Спецусловия: Не требуются\n"
        << "Суточный тариф хранения: " << calculateDailyStorageCost() << " руб.\n";
}

double StandardProduct::calculateDailyStorageCost() const {
    return getWeight() * 0.5;
}

std::unique_ptr<Product> StandardProduct::clone() const {
    return std::make_unique<StandardProduct>(*this);
}

double StandardProduct::calculateHandlingFee() const {
    return getWeight() * 0.15;
}