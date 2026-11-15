#include "OutOfRangeException.h"
#include <format>

OutOfRangeException::OutOfRangeException(std::string_view message, size_t requestedIndex, size_t collectionSize)
    : WarehouseException(std::format("Выход за пределы диапазона: {} [Индекс: {}, Размер: {}]", message, requestedIndex, collectionSize)),
    requestedIndex(requestedIndex),
    collectionSize(collectionSize) {
}

size_t OutOfRangeException::getRequestedIndex() const noexcept {
    return requestedIndex;
}

size_t OutOfRangeException::getCollectionSize() const noexcept {
    return collectionSize;
}