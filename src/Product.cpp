#include "Product.h"

#include <utility>

Product::Product(int id, std::string name, std::string category, double price, int quantity)
    : id_(id), name_(std::move(name)), category_(std::move(category)), price_(price), quantity_(quantity) {}

int Product::getId() const {
    return id_;
}

const std::string& Product::getName() const {
    return name_;
}

const std::string& Product::getCategory() const {
    return category_;
}

double Product::getPrice() const {
    return price_;
}

int Product::getQuantity() const {
    return quantity_;
}

void Product::setName(const std::string& name) {
    name_ = name;
}

void Product::setCategory(const std::string& category) {
    category_ = category;
}

void Product::setPrice(double price) {
    price_ = price;
}

void Product::setQuantity(int quantity) {
    quantity_ = quantity;
}

double Product::totalCost() const {
    return price_ * quantity_;
}
