#pragma once
#include "WarehouseException.h"
#include <cstddef>

class OutOfRangeException : public WarehouseException {
private:
    size_t requestedIndex = 0;
    size_t collectionSize = 0;

public:
    explicit OutOfRangeException(std::string_view message, size_t requestedIndex = 0, size_t collectionSize = 0);
    [[nodiscard]] size_t getRequestedIndex() const noexcept;
    [[nodiscard]] size_t getCollectionSize() const noexcept;
};
