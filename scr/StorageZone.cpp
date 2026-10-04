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

StorageZone& StorageZone::operator+=(const Product& product) {
    if (!addProduct(product)) {
        std::cout << "\n[ОТКАЗ В РАЗМЕЩЕНИИ] Превышена грузоподъемность или полезный объем зоны \"" << name << "\"!\n";
    }
    else {
        std::cout << "\n[УСПЕШНО] Товар принят на ответственное хранение в \"" << name << "\".\n";
    }
    return *this;
}

StorageZone& StorageZone::operator-=(std::string_view sku) {
    if (!removeProduct(sku)) {
        std::cout << "\n[ОШИБКА ОТГРУЗКИ] Товар с артикулом " << sku << " не числится в зоне \"" << name << "\"!\n";
    }
    else {
        std::cout << "\n[УСПЕШНО] Товар с артикулом " << sku << " успешно списан и выдан со склада.\n";
    }
    return *this;
}