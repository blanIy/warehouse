#include "FragileProduct.h"
#include <iostream>

FragileProduct::FragileProduct(std::string_view sku, std::string_view name, std::string_view category,
    double weight, double volume, int maxStackLayers, std::string_view paddingType)
    : Product(sku, name, category, weight, volume), maxStackLayers(maxStackLayers), paddingType(paddingType) {
}

int FragileProduct::getMaxStackLayers() const {
    return maxStackLayers;
}

std::string FragileProduct::getPaddingType() const {
    return paddingType;
}

void FragileProduct::setMaxStackLayers(int newLayers) {
    if (newLayers > 0) {
        maxStackLayers = newLayers;
    }
}

void FragileProduct::setPaddingType(std::string_view newPadding) {
    paddingType = newPadding;
}

std::string FragileProduct::getProductType() const {
    return "Хрупкий груз (Ограничение штабелирования)";
}

void FragileProduct::printDetailedReport() const {
    std::cout << "[ПАСПОРТ ХРУПКОГО ГРУЗА]\n"
        << "Артикул: " << getSku() << " | Название: " << getName() << "\n"
        << "Масса: " << getWeight() << " кг | Объем: " << getVolume() << " м3\n"
        << "Предел штабелирования: " << maxStackLayers << " яруса\n"
        << "Амортизация: " << paddingType << "\n"
        << "Суточный тариф хранения: " << calculateDailyStorageCost() << " руб.\n";
}

double FragileProduct::calculateDailyStorageCost() const {
    return (getWeight() * 0.5) + 8.0;
}

std::unique_ptr<Product> FragileProduct::clone() const {
    return std::make_unique<FragileProduct>(*this);
}

double FragileProduct::calculateMaxStackWeight() const {
    if (maxStackLayers > 1) {
        return getWeight() * (maxStackLayers - 1);
    }
    return 0.0;
}