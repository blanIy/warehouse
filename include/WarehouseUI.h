#pragma once
#include "Warehouse.h"
#include "StandardProduct.h"
#include "PerishableProduct.h"
#include "FragileProduct.h"
#include "ChemicalProduct.h"
#include "Collection.h"
#include <memory>

void printMenu();
void handlePlacement(Warehouse& warehouse, const Collection<std::shared_ptr<Product>>& catalog);
void handleIssue(Warehouse& warehouse);
void handleRegisterProduct(Collection<std::shared_ptr<Product>>& catalog,
    Collection<PerishableProduct>& perishables,
    Collection<FragileProduct>& fragiles,
    Collection<ChemicalProduct>& chemicals);
void handleLogisticsAnalysis(const Collection<std::shared_ptr<Product>>& catalog);
void showInheritedFields(const Collection<PerishableProduct>& perishables,
    const Collection<FragileProduct>& fragiles,
    const Collection<ChemicalProduct>& chemicals);
void showDerivedFields(const Collection<PerishableProduct>& perishables,
    const Collection<FragileProduct>& fragiles,
    const Collection<ChemicalProduct>& chemicals);
void demonstrateInheritedSetter(Collection<PerishableProduct>& perishables,
    Collection<FragileProduct>& fragiles,
    Collection<ChemicalProduct>& chemicals);
void demonstrateInheritedGetter(const Collection<PerishableProduct>& perishables,
    const Collection<FragileProduct>& fragiles,
    const Collection<ChemicalProduct>& chemicals);
void demonstratePolymorphicReporting(const Collection<std::shared_ptr<Product>>& catalog);
void demonstrateTemplateCollections();