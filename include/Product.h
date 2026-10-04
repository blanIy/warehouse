#pragma once
#include <string>
#include <string_view>
#include <iostream>
#include <compare>

class Product {
private:
    std::string sku;
    std::string name;
    std::string category;
    double weight = 0.0;
    double volume = 0.0;

public:
    Product() = default;
    Product(std::string_view sku, std::string_view name, std::string_view category, double weight, double volume);

    std::string getSku() const;
    std::string getName() const;
    std::string getCategory() const;
    double getWeight() const;
    double getVolume() const;

    void setCategory(std::string_view newCategory);
    void setWeight(double newWeight);

    bool operator==(const Product& other) const;
    auto operator<=>(const Product& other) const {
        return weight <=> other.weight;
    }

    friend std::ostream& operator<<(std::ostream& os, const Product& product) {
        os << "[" << product.sku << "] " << product.name << " | Категория: " << product.category
            << " | Масса: " << product.weight << " кг | Объем: " << product.volume << " м3";
        return os;
    }

    friend std::istream& operator>>(std::istream& is, Product& product) {
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

    friend double calculateDensity(const Product& product);
};