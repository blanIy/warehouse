#pragma once
#include "Warehouse.h"
#include <string>
#include <string_view>

class FileStorage {
public:
    static bool exists(std::string_view path);
    static void writeAtomic(std::string_view path, std::string_view content);
    static void save(std::string_view path, const Warehouse& warehouse);
    static void loadAndMerge(std::string_view path, Warehouse& warehouse);
};
