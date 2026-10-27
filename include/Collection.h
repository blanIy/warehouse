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

    void add(const T& item);
    void add(T&& item);
    bool remove(const T& item);
    bool removeAt(size_t index);
    T& get(size_t index);
    const T& get(size_t index) const;
    T& operator[](size_t index);
    const T& operator[](size_t index) const;
    int find(const T& item) const;
    size_t getSize() const;
    bool isEmpty() const;
    void clear();
    void print() const;
};

template<typename T, typename Predicate>
Collection<T> filterCollection(const Collection<T>& collection, Predicate pred);

template<typename T, typename Predicate>
const T* findFirstMatch(const Collection<T>& collection, Predicate pred);

#include "Collection.tpp"