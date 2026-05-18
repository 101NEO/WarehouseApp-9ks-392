#pragma once

#include <string>

class Product {
public:
    Product() = default;
    Product(int id, std::string name, std::string category, double price, int quantity);

    int getId() const;
    const std::string& getName() const;
    const std::string& getCategory() const;
    double getPrice() const;
    int getQuantity() const;

    void setName(const std::string& name);
    void setCategory(const std::string& category);
    void setPrice(double price);
    void setQuantity(int quantity);

    double totalCost() const;

private:
    int id_ = 0;
    std::string name_;
    std::string category_;
    double price_ = 0.0;
    int quantity_ = 0;
};
