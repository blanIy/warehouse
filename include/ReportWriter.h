#pragma once
#include "Warehouse.h"
#include <string>
#include <string_view>

class ReportWriter {
public:
    static std::string build(const Warehouse& warehouse);
    static void write(std::string_view path, const Warehouse& warehouse);
};
