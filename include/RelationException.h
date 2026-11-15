#pragma once
#include "WarehouseException.h"

class RelationException : public WarehouseException {
private:
    std::string relatedEntity;

public:
    explicit RelationException(std::string_view message, std::string_view relatedEntity = "");
    [[nodiscard]] std::string getRelatedEntity() const;
};