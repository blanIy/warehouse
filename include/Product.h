#pragma once
#include <string>
#include <string_view>
#include <iosfwd>

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
    bool operator!=(const Product& other) const;
    bool operator<(const Product& other) const;
    bool operator>(const Product& other) const;

    friend std::ostream& operator<<(std::ostream& os, const Product& product);
    friend std::istream& operator>>(std::istream& is, Product& product);
    friend double calculateDensity(const Product& product);
};