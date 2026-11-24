#include "FileStorage.h"
#include "ChemicalProduct.h"
#include "FragileProduct.h"
#include "PerishableProduct.h"
#include "StandardProduct.h"
#include "exceptions/DuplicateIdException.h"
#include "exceptions/FileOpenException.h"
#include "exceptions/FileReadException.h"
#include "exceptions/FileWriteException.h"
#include "exceptions/InvalidDataException.h"
#include "exceptions/RelationException.h"
#include <filesystem>
#include <fstream>
#include <locale>
#include <memory>
#include <sstream>
#include <vector>
#include <set>

namespace {

    const char* const MAGIC_HEADER = "WMSSTATE";
    const int FORMAT_VERSION = 1;

    std::vector<std::string> splitLine(const std::string& line, char delimiter) {
        std::vector<std::string> tokens;
        std::stringstream ss(line);
        std::string item;
        while (std::getline(ss, item, delimiter)) {
            tokens.push_back(item);
        }
        if (!line.empty() && line.back() == delimiter) {
            tokens.emplace_back("");
        }
        return tokens;
    }

    double parseDouble(const std::string& str, const std::string& fieldName) 
    {
        std::istringstream iss(str);
        iss.imbue(std::locale::classic());
        double val = 0.0;
        if (!(iss >> val) || !iss.eof()) {
            throw InvalidDataException("Некорректное числовое значение поля: " + fieldName);
        }
        return val;
    }

    int parseInt(const std::string& str, const std::string& fieldName) {
        try {
            size_t pos = 0;
            const int val = std::stoi(str, &pos);
            if (pos != str.size()) {
                throw InvalidDataException("Некорректное целочисленное значение поля: " + fieldName);
            }
            return val;
        }
        catch (const InvalidDataException&) {
            throw;
        }
        catch (...) {
            throw InvalidDataException("Ошибка разбора целого числа для поля: " + fieldName);
        }
    }

    bool parseBool(const std::string& str, const std::string& fieldName) {
        if (str == "1" || str == "true") {
            return true;
        }
        if (str == "0" || str == "false") {
            return false;
        }
        throw InvalidDataException("Некорректное логическое значение поля: " + fieldName);
    }

    void writeProduct(std::ostringstream& os, std::string_view zoneName, const Product& p) {
        if (const auto* stdProd = dynamic_cast<const StandardProduct*>(&p)) {
            os << "PRODUCT;STANDARD;" << zoneName << ";"
                << p.getSku() << ";" << p.getName() << ";" << p.getCategory() << ";"
                << p.getWeight() << ";" << p.getVolume() << ";"
                << stdProd->getPackageType() << "\n";
        }
        else if (const auto* perProd = dynamic_cast<const PerishableProduct*>(&p)) {
            os << "PRODUCT;PERISHABLE;" << zoneName << ";"
                << p.getSku() << ";" << p.getName() << ";" << p.getCategory() << ";"
                << p.getWeight() << ";" << p.getVolume() << ";"
                << perProd->getExpiryDate() << ";" << perProd->getTargetTemperature() << "\n";
        }
        else if (const auto* fragProd = dynamic_cast<const FragileProduct*>(&p)) {
            os << "PRODUCT;FRAGILE;" << zoneName << ";"
                << p.getSku() << ";" << p.getName() << ";" << p.getCategory() << ";"
                << p.getWeight() << ";" << p.getVolume() << ";"
                << fragProd->getMaxStackLayers() << ";" << fragProd->getPaddingType() << "\n";
        }
        else if (const auto* chemProd = dynamic_cast<const ChemicalProduct*>(&p)) {
            os << "PRODUCT;CHEMICAL;" << zoneName << ";"
                << p.getSku() << ";" << p.getName() << ";" << p.getCategory() << ";"
                << p.getWeight() << ";" << p.getVolume() << ";"
                << chemProd->getHazardClass() << ";" << (chemProd->getRequiresVentilation() ? "1" : "0") << "\n";
        }
        else {
            throw InvalidDataException("Неизвестный тип товара при сохранении: " + p.getSku());
        }
    }

    std::shared_ptr<Product> buildProduct(const std::vector<std::string>& tokens) {
        if (tokens.size() < 9) {
            throw InvalidDataException("Недостаточно полей в записи товара.");
        }
        const std::string& type = tokens[1];
        const std::string& sku = tokens[3];
        const std::string& name = tokens[4];
        const std::string& category = tokens[5];
        const double weight = parseDouble(tokens[6], "масса");
        const double volume = parseDouble(tokens[7], "объем");

        if (type == "STANDARD") {
            if (tokens.size() != 9) {
                throw InvalidDataException("Неверное число полей для StandardProduct.");
            }
            return std::make_shared<StandardProduct>(sku, name, category, weight, volume, tokens[8]);
        }
        if (type == "PERISHABLE") {
            if (tokens.size() != 10) {
                throw InvalidDataException("Неверное число полей для PerishableProduct.");
            }
            const double temp = parseDouble(tokens[9], "целевая температура");
            return std::make_shared<PerishableProduct>(sku, name, category, weight, volume, tokens[8], temp);
        }
        if (type == "FRAGILE") {
            if (tokens.size() != 10) {
                throw InvalidDataException("Неверное число полей для FragileProduct.");
            }
            const int layers = parseInt(tokens[8], "макс. ярусов");
            return std::make_shared<FragileProduct>(sku, name, category, weight, volume, layers, tokens[9]);
        }
        if (type == "CHEMICAL") {
            if (tokens.size() != 10) {
                throw InvalidDataException("Неверное число полей для ChemicalProduct.");
            }
            const bool vent = parseBool(tokens[9], "требуется вентиляция");
            return std::make_shared<ChemicalProduct>(sku, name, category, weight, volume, tokens[8], vent);
        }
        throw InvalidDataException("Неизвестный тип товара в файле: " + type);
    }

    int findZoneIndex(const Warehouse& warehouse, const std::string& zoneName) {
        for (int i = 0; i < warehouse.getZoneCount(); ++i) {
            const auto* zone = warehouse.getZone(i);
            if (zone != nullptr && zone->getName() == zoneName) {
                return i;
            }
        }
        return -1;
    }

    bool hasProductSku(const Warehouse& warehouse, const std::string& sku) {
        for (int i = 0; i < warehouse.getZoneCount(); ++i) {
            const auto* zone = warehouse.getZone(i);
            if (zone == nullptr) {
                continue;
            }
            for (int j = 0; j < zone->getProductCount(); ++j) {
                const auto* prod = zone->getProduct(j);
                if (prod != nullptr && prod->getSku() == sku) {
                    return true;
                }
            }
        }
        return false;
    }

}

bool FileStorage::exists(std::string_view path) {
    std::error_code ec;
    return std::filesystem::exists(std::filesystem::path(path), ec);
}

void FileStorage::writeAtomic(std::string_view path, std::string_view content) {
    const std::filesystem::path targetPath(path);
    if (targetPath.has_parent_path()) {
        std::error_code ec;
        std::filesystem::create_directories(targetPath.parent_path(), ec);
    }

    const std::filesystem::path tmpPath(std::string(path) + ".tmp");
    const std::filesystem::path bakPath(std::string(path) + ".bak");

    {
        std::ofstream out(tmpPath, std::ios::trunc);
        if (!out.is_open()) {
            throw FileOpenException("Не удалось открыть временный файл для записи: " + tmpPath.string());
        }
        out << content;
        out.flush();
        if (!out) {
            out.close();
            std::error_code ec;
            std::filesystem::remove(tmpPath, ec);
            throw FileWriteException("Ошибка записи данных во временный файл: " + tmpPath.string());
        }
    }

    std::error_code ec;
    std::filesystem::remove(bakPath, ec);
    const bool hadOld = std::filesystem::exists(targetPath, ec);
    if (hadOld) {
        std::filesystem::rename(targetPath, bakPath, ec);
        if (ec) {
            std::filesystem::remove(tmpPath, ec);
            throw FileWriteException("Не удалось создать резервную копию файла: " + targetPath.string());
        }
    }

    std::filesystem::rename(tmpPath, targetPath, ec);
    if (ec) {
        if (hadOld) {
            std::error_code restoreEc;
            std::filesystem::rename(bakPath, targetPath, restoreEc);
        }
        std::filesystem::remove(tmpPath, ec);
        throw FileWriteException("Не удалось заменить целевой файл новой версией: " + targetPath.string());
    }

    if (hadOld) {
        std::filesystem::remove(bakPath, ec);
    }
}

void FileStorage::save(std::string_view path, const Warehouse& warehouse) {
    int totalRecords = warehouse.getZoneCount();
    for (int i = 0; i < warehouse.getZoneCount(); ++i) {
        const auto* zone = warehouse.getZone(i);
        if (zone != nullptr) {
            totalRecords += zone->getProductCount();
        }
    }

    std::ostringstream buf;
    buf.imbue(std::locale::classic());
    buf << MAGIC_HEADER << ";" << totalRecords << "\n";

    for (int i = 0; i < warehouse.getZoneCount(); ++i) {
        const auto* zone = warehouse.getZone(i);
        if (zone == nullptr) {
            continue;
        }
        buf << "ZONE;" << zone->getName() << ";"
            << zone->getMaxWeight() << ";" << zone->getMaxVolume() << "\n";
    }

    for (int i = 0; i < warehouse.getZoneCount(); ++i) {
        const auto* zone = warehouse.getZone(i);
        if (zone == nullptr) {
            continue;
        }
        for (int j = 0; j < zone->getProductCount(); ++j) {
            const auto* prod = zone->getProduct(j);
            if (prod != nullptr) {
                writeProduct(buf, zone->getName(), *prod);
            }
        }
    }

    buf << "END\n";
    writeAtomic(path, buf.str());
}

void FileStorage::loadAndMerge(std::string_view path, Warehouse& warehouse) {
    std::ifstream in{ std::filesystem::path(path) };
    if (!in.is_open()) {
        throw FileOpenException("Не удалось открыть файл состояния для чтения: " + std::string(path));
    }

    std::string line;
    if (!std::getline(in, line)) {
        throw FileReadException("Файл состояния пуст или поврежден.");
    }

    const auto headerTokens = splitLine(line, ';');
    if (headerTokens.size() != 2 || headerTokens[0] != MAGIC_HEADER) {
        throw InvalidDataException("Некорректный заголовок файла состояния.");
    }
    const int expectedRecords = parseInt(headerTokens[1], "количество записей");
    if (expectedRecords < 0) {
        throw InvalidDataException("Количество записей не может быть отрицательным.");
    }
    int actualRecords = 0;

    bool endReached = false;
    std::set<std::string> fileSkus;
    bool hasDuplicates = false;
    while (std::getline(in, line)) {
        if (line.empty()) {
            continue;
        }
        if (endReached) {
            throw InvalidDataException("Обнаружены лишние данные после метки END.");
        }
        const auto tokens = splitLine(line, ';');
        if (tokens[0] == "END") {
            endReached = true;
            continue;
        }
        if (tokens[0] == "ZONE") {
            if (tokens.size() != 4) {
                throw InvalidDataException("Неверный формат записи зоны.");
            }
            const std::string& zoneName = tokens[1];
            const double maxW = parseDouble(tokens[2], "максимальная масса зоны");
            const double maxV = parseDouble(tokens[3], "максимальный объем зоны");
            if (findZoneIndex(warehouse, zoneName) == -1) {
                warehouse.addZone(StorageZone(zoneName, maxW, maxV));
            }
            ++actualRecords;
        }
        else if (tokens[0] == "PRODUCT") {
            const auto product = buildProduct(tokens);
            const std::string& zoneName = tokens[2];
            const int zoneIdx = findZoneIndex(warehouse, zoneName);
            if (zoneIdx == -1) {
                throw RelationException("Зона хранения не найдена для товара: " + zoneName);
            }
            if (!fileSkus.insert(product->getSku()).second) {
                hasDuplicates = true;
                continue;
            }
            if (!hasProductSku(warehouse, product->getSku())) {
                warehouse.placeProduct(zoneIdx, *product);
            }
            ++actualRecords;
        }
        else {
            throw InvalidDataException("Неизвестный тип записи в файле: " + tokens[0]);
        }
    }

    if (in.bad()) {
        throw FileReadException("Ошибка ввода-вывода при чтении файла состояния.");
    }
    if (!endReached) {
        throw InvalidDataException("Отсутствует завершающая метка END.");
    }
    if (actualRecords != expectedRecords) {
        throw InvalidDataException("Количество записей в файле не соответствует указанному в заголовке.");
    }
    in.close();
    if (hasDuplicates) {
        save(path, warehouse);
    }
}