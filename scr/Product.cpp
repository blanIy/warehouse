#include "Product.h"
#include <iostream>

Product::Product(std::string_view sku, std::string_view name, std::string_view category, double weight, double volume)
    : sku(sku), name(name), category(category), weight(weight), volume(volume) {
}

std::string Product::getSku() const { return sku; }
std::string Product::getName() const { return name; }
std::string Product::getCategory() const { return category; }
double Product::getWeight() const { return weight; }
double Product::getVolume() const { return volume; }

void Product::setCategory(std::string_view newCategory) {
    category = newCategory;
}

void Product::setWeight(double newWeight) {
    if (newWeight > 0.0) {
        weight = newWeight;
    }
}

void Product::printInfo() const {
    std::cout << "[" << sku << "] " << name << " | Категория: " << category
        << " | Масса: " << weight << " кг | Объем: " << volume << " м3\n";
}