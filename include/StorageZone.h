#pragma once
#include "Product.h"
#include <string>
#include <vector>
#include <string_view>
#include <iostream>

class StorageZone {
private:
    std::string name;
    double maxWeight = 0.0;
    double maxVolume = 0.0;
    double currentWeight = 0.0;
    double currentVolume = 0.0;
    std::vector<Product> products;

public:
    StorageZone(std::string_view name, double maxWeight, double maxVolume);

    std::string getName() const;
    double getMaxWeight() const;
    double getMaxVolume() const;
    double getCurrentWeight() const;
    double getCurrentVolume() const;
    int getProductCount() const;

    const Product* getProduct(int index) const;
    Product* getProduct(int index);

    bool canAccommodate(const Product& product) const;
    bool addProduct(const Product& product);
    bool removeProduct(std::string_view sku);

    StorageZone& operator+=(const Product& product);
    StorageZone& operator-=(std::string_view sku);

    friend std::ostream& operator<<(std::ostream& os, const StorageZone& zone) {
        os << "--- " << zone.name << " ---\n"
            << "Загрузка по массе: " << zone.currentWeight << " / " << zone.maxWeight << " кг\n"
            << "Загрузка по объему: " << zone.currentVolume << " / " << zone.maxVolume << " м3\n"
            << "Товаров в зоне: " << zone.products.size() << "\n";
        if (zone.products.empty()) {
            os << "  (Зона свободна)\n";
        }
        else {
            for (const auto& product : zone.products) {
                os << "  * " << product << "\n";
            }
        }
        os << "----------------------------------------\n";
        return os;
    }
};