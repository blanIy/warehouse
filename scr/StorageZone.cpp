#include "StorageZone.h"
#include <iostream>

StorageZone::StorageZone(std::string_view name, double maxWeight, double maxVolume)
    : name(name), maxWeight(maxWeight), maxVolume(maxVolume) {
}

std::string StorageZone::getName() const { return name; }
double StorageZone::getMaxWeight() const { return maxWeight; }
double StorageZone::getMaxVolume() const { return maxVolume; }
double StorageZone::getCurrentWeight() const { return currentWeight; }
double StorageZone::getCurrentVolume() const { return currentVolume; }
int StorageZone::getProductCount() const { return static_cast<int>(products.size()); }

const Product* StorageZone::getProduct(int index) const {
    if (index >= 0 && index < static_cast<int>(products.size())) {
        return &products[static_cast<size_t>(index)];
    }
    return nullptr;
}

Product* StorageZone::getProduct(int index) {
    if (index >= 0 && index < static_cast<int>(products.size())) {
        return &products[static_cast<size_t>(index)];
    }
    return nullptr;
}

bool StorageZone::canAccommodate(const Product& product) const {
    return (currentWeight + product.getWeight() <= maxWeight) &&
        (currentVolume + product.getVolume() <= maxVolume);
}

bool StorageZone::addProduct(const Product& product) {
    if (!canAccommodate(product)) {
        return false;
    }
    products.push_back(product);
    currentWeight += product.getWeight();
    currentVolume += product.getVolume();
    return true;
}

bool StorageZone::removeProduct(std::string_view sku) {
    for (auto it = products.begin(); it != products.end(); ++it) {
        if (it->getSku() == sku) {
            currentWeight -= it->getWeight();
            currentVolume -= it->getVolume();
            products.erase(it);
            return true;
        }
    }
    return false;
}

void StorageZone::printStatus() const {
    std::cout << "--- " << name << " ---\n"
        << "Загрузка по массе: " << currentWeight << " / " << maxWeight << " кг\n"
        << "Загрузка по объему: " << currentVolume << " / " << maxVolume << " м3\n"
        << "Товаров в зоне: " << products.size() << "\n";
    if (products.empty()) {
        std::cout << "  (Зона пуста)\n";
    }
    else {
        for (const auto& product : products) {
            std::cout << "  * ";
            product.printInfo();
        }
    }
    std::cout << "----------------------------------------\n";
}