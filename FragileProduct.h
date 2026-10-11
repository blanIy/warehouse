#pragma once
#include "Product.h"

class FragileProduct : public Product {
private:
    int maxStackLayers = 1;
    std::string paddingType;

public:
    FragileProduct(std::string_view sku, std::string_view name, std::string_view category,
        double weight, double volume, int maxStackLayers, std::string_view paddingType);

    int getMaxStackLayers() const;
    std::string getPaddingType() const;

    void setMaxStackLayers(int newLayers);
    void setPaddingType(std::string_view newPadding);
};
