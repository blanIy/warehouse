#pragma once
#include "Product.h"

class FragileProduct : public Product {
private:
    int maxStackLayers = 1;
    std::string paddingType;

public:
    FragileProduct(std::string_view sku, std::string_view name, std::string_view category,
        double weight, double volume, int maxStackLayers, std::string_view paddingType);
    FragileProduct(const FragileProduct&) = default;
    FragileProduct& operator=(const FragileProduct&) = default;
    FragileProduct(FragileProduct&&) noexcept = default;
    FragileProduct& operator=(FragileProduct&&) noexcept = default;
    ~FragileProduct() override = default;

    int getMaxStackLayers() const;
    std::string getPaddingType() const;

    void setMaxStackLayers(int newLayers);
    void setPaddingType(std::string_view newPadding);

    std::string getProductType() const override;
    void printDetailedReport() const override;
    double calculateDailyStorageCost() const override;
    std::unique_ptr<Product> clone() const override;

    double calculateMaxStackWeight() const;
};