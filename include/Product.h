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
    Product(const Product&) = default;
    Product& operator=(const Product&) = default;
    Product(Product&&) noexcept = default;
    Product& operator=(Product&&) noexcept = default;
    virtual ~Product() = default;

    std::string getSku() const;
    std::string getName() const;
    std::string getCategory() const;
    double getWeight() const;
    double getVolume() const;

    void setSku(std::string_view newSku);
    void setName(std::string_view newName);
    void setCategory(std::string_view newCategory);
    void setWeight(double newWeight);
    void setVolume(double newVolume);

    virtual double calculateDailyStorageCost() const;

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
        std::cout << "Введите название: ";
        is >> product.name;
        std::cout << "Введите категорию: ";
        is >> product.category;
        std::cout << "Введите массу (кг): ";
        is >> product.weight;
        std::cout << "Введите объем (м3): ";
        is >> product.volume;
        return is;
    }

    friend double calculateDensity(const Product& product);
};