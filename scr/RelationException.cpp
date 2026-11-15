#include "RelationException.h"
#include <format>

RelationException::RelationException(std::string_view message, std::string_view relatedEntity)
    : WarehouseException(relatedEntity.empty()
        ? std::format("Нарушение связи: {}", message)
        : std::format("Нарушение связи: {} [Объект: {}]", message, relatedEntity)),
    relatedEntity(relatedEntity) {
}

std::string RelationException::getRelatedEntity() const {
    return relatedEntity;
}