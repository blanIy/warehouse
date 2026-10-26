#pragma once
#include "Warehouse.h"
#include "StandardProduct.h"
#include "PerishableProduct.h"
#include "FragileProduct.h"
#include "ChemicalProduct.h"
#include "Collection.h"
#include <vector>
#include <memory>

void printMenu();
void handlePlacement(Warehouse& warehouse, const std::vector<std::unique_ptr<Product>>& catalog);
void handleIssue(Warehouse& warehouse);
void handleRegisterProduct(std::vector<std::unique_ptr<Product>>& catalog,
    std::vector<PerishableProduct>& perishables,
    std::vector<FragileProduct>& fragiles,
    std::vector<ChemicalProduct>& chemicals);
void handleLogisticsAnalysis(const std::vector<std::unique_ptr<Product>>& catalog);
void showInheritedFields(const std::vector<PerishableProduct>& perishables,
    const std::vector<FragileProduct>& fragiles,
    const std::vector<ChemicalProduct>& chemicals);
void showDerivedFields(const std::vector<PerishableProduct>& perishables,
    const std::vector<FragileProduct>& fragiles,
    const std::vector<ChemicalProduct>& chemicals);
void demonstrateInheritedSetter(std::vector<PerishableProduct>& perishables,
    std::vector<FragileProduct>& fragiles,
    std::vector<ChemicalProduct>& chemicals);
void demonstrateInheritedGetter(const std::vector<PerishableProduct>& perishables,
    const std::vector<FragileProduct>& fragiles,
    const std::vector<ChemicalProduct>& chemicals);
void demonstratePolymorphicReporting(const std::vector<std::unique_ptr<Product>>& catalog);
void demonstrateTemplateCollections();