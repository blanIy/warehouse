#pragma once
#include "WarehouseException.h"
#include <format>
#include <string>
#include <string_view>

class RelationException : public WarehouseException {
private:
    std::string relatedEntity;

public:
    explicit RelationException(std::string_view message, std::string_view relatedEntity = "")
        : WarehouseException(relatedEntity.empty()
            ? std::format("Нарушение связи: {}", message)
            : std::format("Нарушение связи: {} [Объект: {}]", message, relatedEntity)),
        relatedEntity(relatedEntity) {
    }

    [[nodiscard]] std::string getRelatedEntity() const {
        return relatedEntity;
    }
};