#ifndef _ACCOUNT_UTIL_H_
#define _ACCOUNT_UTIL_H_

#include <memory>
#include <vector>
#include "Account.h"

// Utility functions for polymorphic Account pointers.

void display(const std::vector<std::unique_ptr<Account>> &accounts);
void deposit(std::vector<std::unique_ptr<Account>> &accounts, double amount);
void withdraw(std::vector<std::unique_ptr<Account>> &accounts, double amount);

#endif
