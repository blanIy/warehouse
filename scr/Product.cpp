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

bool Product::operator==(const Product& other) const {
    return sku == other.sku;
}

bool Product::operator!=(const Product& other) const {
    return !(*this == other);
}

bool Product::operator<(const Product& other) const {
    return weight < other.weight;
}

bool Product::operator>(const Product& other) const {
    return other < *this;
}

std::ostream& operator<<(std::ostream& os, const Product& product) {
    os << "[" << product.sku << "] " << product.name << " | Категория: " << product.category
        << " | Масса: " << product.weight << " кг | Объем: " << product.volume << " м3";
    return os;
}

std::istream& operator>>(std::istream& is, Product& product) {
    std::cout << "Введите артикул: ";
    is >> product.sku;
    std::cout << "Введите название (одним словом): ";
    is >> product.name;
    std::cout << "Введите категорию (одним словом): ";
    is >> product.category;
    std::cout << "Введите массу (кг): ";
    is >> product.weight;
    std::cout << "Введите объем (м3): ";
    is >> product.volume;
    return is;
}

double calculateDensity(const Product& product) {
    if (product.volume <= 0.0) {
        return 0.0;
    }
    return product.weight / product.volume;
}