#pragma once

#include <memory>

class OrdersRepository;

class DatabaseComponent {
public:

  DatabaseComponent() { ; }

  DatabaseComponent(const DatabaseComponent&) = delete;

  DatabaseComponent& operator=(const DatabaseComponent&) = delete;

  DatabaseComponent(DatabaseComponent&&) = delete;

  DatabaseComponent& operator=(DatabaseComponent&&) = delete;

  virtual std::shared_ptr<OrdersRepository> getOrdersRepository() = 0;

  virtual ~DatabaseComponent() { ; }
};