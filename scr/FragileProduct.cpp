#include "FragileProduct.h"

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

double FragileProduct::calculateDailyStorageCost() const {
    return (getWeight() * 0.5) + 8.0;
}

double FragileProduct::calculateMaxStackWeight() const {
    if (maxStackLayers > 1) {
        return getWeight() * (maxStackLayers - 1);
    }
    return 0.0;
}