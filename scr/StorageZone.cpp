#include "StorageZone.h"
#include <iostream>

StorageZone::StorageZone(std::string_view name, double maxWeight, double maxVolume)
    : name(name), maxWeight(maxWeight), maxVolume(maxVolume) {
}

StorageZone::StorageZone(const StorageZone& other)
    : name(other.name), maxWeight(other.maxWeight), maxVolume(other.maxVolume),
    currentWeight(other.currentWeight), currentVolume(other.currentVolume) {
    for (size_t i = 0; i < other.products.getSize(); ++i) {
        if (other.products[i] != nullptr) {
            products.add(other.products[i]->clone());
        }
    }
}

StorageZone& StorageZone::operator=(const StorageZone& other) {
    if (this == &other) {
        return *this;
    }
    name = other.name;
    maxWeight = other.maxWeight;
    maxVolume = other.maxVolume;
    currentWeight = other.currentWeight;
    currentVolume = other.currentVolume;
    products.clear();
    for (size_t i = 0; i < other.products.getSize(); ++i) {
        if (other.products[i] != nullptr) {
            products.add(other.products[i]->clone());
        }
    }
    return *this;
}

StorageZone::~StorageZone() {
    products.clear();
}

std::string StorageZone::getName() const { return name; }
double StorageZone::getMaxWeight() const { return maxWeight; }
double StorageZone::getMaxVolume() const { return maxVolume; }
double StorageZone::getCurrentWeight() const { return currentWeight; }
double StorageZone::getCurrentVolume() const { return currentVolume; }
int StorageZone::getProductCount() const { return static_cast<int>(products.getSize()); }

const Product* StorageZone::getProduct(int index) const {
    if (index >= 0 && static_cast<size_t>(index) < products.getSize()) {
        return products[static_cast<size_t>(index)].get();
    }
    return nullptr;
}

Product* StorageZone::getProduct(int index) {
    if (index >= 0 && static_cast<size_t>(index) < products.getSize()) {
        return products[static_cast<size_t>(index)].get();
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
    products.add(product.clone());
    currentWeight += product.getWeight();
    currentVolume += product.getVolume();
    return true;
}

bool StorageZone::removeProduct(std::string_view sku) {
    for (size_t i = 0; i < products.getSize(); ++i) {
        if (products[i]->getSku() == sku) {
            currentWeight -= products[i]->getWeight();
            currentVolume -= products[i]->getVolume();
            products.removeAt(i);
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
        std::cout << "\n[УСПЕХ] Товар принят на ответственное хранение в \"" << name << "\".\n";
    }
    return *this;
}

StorageZone& StorageZone::operator-=(std::string_view sku) {
    if (!removeProduct(sku)) {
        std::cout << "\n[ОШИБКА ОТГРУЗКИ] Товар с артикулом " << sku << " не числится в зоне \"" << name << "\"!\n";
    }
    else {
        std::cout << "\n[УСПЕХ] Товар с артикулом " << sku << " успешно списан и выдан со склада.\n";
    }
    return *this;
}