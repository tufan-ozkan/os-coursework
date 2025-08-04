# Homework 2 - Concurrent Store (Threads & Synchronization)

This project simulates a multi-threaded store system where customers buy items and suppliers restock them. All access to shared state is synchronized using mutexes and condition variables.

## 🧠 Key Concepts
- Thread synchronization (mutex, cond vars)
- Monitor-based design
- Deadlock prevention and fairness
- Bulk transactions and conditional blocking

## 🛒 Functional Overview
- Supports unlimited customer/supplier threads
- `buy()` blocks if items are insufficient
- `maysupply()` blocks if capacity is exceeded
- `supply()` commits items and notifies buyers

