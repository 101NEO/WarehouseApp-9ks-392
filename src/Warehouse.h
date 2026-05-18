#pragma once

#include "Product.h"

#include <string>
#include <vector>

enum class SortField {
    Price,
    Quantity,
    Name
};

class Warehouse {
public:
    bool addProduct(const Product& product, std::string& error);
    bool updateProduct(int id, const std::string& name, const std::string& category, double price, int quantity, std::string& error);
    bool removeById(int id);
    int removeByName(const std::string& name);

    Product* findById(int id);
    const Product* findById(int id) const;

    std::vector<Product> searchByName(const std::string& query) const;
    std::vector<Product> filterByCategory(const std::string& category) const;
    std::vector<Product> filterByPriceRange(double minPrice, double maxPrice) const;
    std::vector<Product> sorted(SortField field, bool ascending) const;

    double totalInventoryValue() const;
    std::vector<Product> lowStock(int limit = 5) const;
    std::vector<Product> topExpensive(int count = 3) const;

    const std::vector<Product>& all() const;
    void replaceAll(const std::vector<Product>& products);

private:
    std::vector<Product> products_;

    bool isIdUnique(int id) const;
    static std::string lowerCopy(const std::string& text);
    static bool equalsIgnoreCase(const std::string& left, const std::string& right);
    static bool isBlank(const std::string& text);
};
