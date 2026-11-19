#pragma once
#include "WarehouseException.h"
#include <format>
#include <cstddef>
#include <string_view>

class OutOfRangeException : public WarehouseException {
private:
    size_t requestedIndex = 0;
    size_t collectionSize = 0;

public:
    explicit OutOfRangeException(std::string_view message, size_t requestedIndex = 0, size_t collectionSize = 0)
        : WarehouseException(std::format("Выход за пределы диапазона: {} [Индекс: {}, Размер: {}]", message, requestedIndex, collectionSize)),
        requestedIndex(requestedIndex),
        collectionSize(collectionSize) {
    }

    [[nodiscard]] size_t getRequestedIndex() const noexcept {
        return requestedIndex;
    }

    [[nodiscard]] size_t getCollectionSize() const noexcept {
        return collectionSize;
    }
};