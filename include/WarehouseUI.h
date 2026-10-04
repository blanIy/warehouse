#pragma once
#include "Warehouse.h"
#include <vector>

void printMenu();
void handlePlacement(Warehouse& warehouse, const std::vector<Product>& catalog);
void handleIssue(Warehouse& warehouse);
void handleModifyProduct(Warehouse& warehouse);
void handleRegisterProduct(std::vector<Product>& catalog);
void handleLogisticsAnalysis(const std::vector<Product>& catalog);