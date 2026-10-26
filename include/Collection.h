#pragma once
#include <vector>
#include <iostream>
#include <utility>
#include <cstddef>

template<typename T>
class Collection {
private:
    std::vector<T> items;

public:
    Collection() = default;
    Collection(const Collection&) = default;
    Collection& operator=(const Collection&) = default;
    Collection(Collection&&) noexcept = default;
    Collection& operator=(Collection&&) noexcept = default;
    ~Collection() = default;

    void add(const T& item) {
        items.push_back(item);
    }

    void add(T&& item) {
        items.push_back(std::move(item));
    }

    bool remove(const T& item) {
        for (auto it = items.begin(); it != items.end(); ++it) {
            if (*it == item) {
                items.erase(it);
                return true;
            }
        }
        return false;
    }

    bool removeAt(size_t index) {
        if (index < items.size()) {
            items.erase(items.begin() + index);
            return true;
        }
        return false;
    }

    T& get(size_t index) {
        return items.at(index);
    }

    const T& get(size_t index) const {
        return items.at(index);
    }

    T& operator[](size_t index) {
        return items[index];
    }

    const T& operator[](size_t index) const {
        return items[index];
    }

    int find(const T& item) const {
        for (size_t i = 0; i < items.size(); ++i) {
            if (items[i] == item) {
                return static_cast<int>(i);
            }
        }
        return -1;
    }

    size_t getSize() const {
        return items.size();
    }

    bool isEmpty() const {
        return items.empty();
    }

    void clear() {
        items.clear();
    }

    void print() const {
        if (items.empty()) {
            std::cout << "Коллекция пуста.\n";
            return;
        }
        for (size_t i = 0; i < items.size(); ++i) {
            std::cout << (i + 1) << ". " << items[i] << "\n";
        }
    }
};

template<typename T, typename Predicate>
Collection<T> filterCollection(const Collection<T>& collection, Predicate pred) {
    Collection<T> result;
    for (size_t i = 0; i < collection.getSize(); ++i) {
        if (pred(collection[i])) {
            result.add(collection[i]);
        }
    }
    return result;
}

template<typename T, typename Predicate>
const T* findFirstMatch(const Collection<T>& collection, Predicate pred) {
    for (size_t i = 0; i < collection.getSize(); ++i) {
        if (pred(collection[i])) {
            return &collection[i];
        }
    }
    return nullptr;
}