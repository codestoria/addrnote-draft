#include <iostream>
#include "Account_Util.h"

// Displays the Account objects in the vector.
void display(const std::vector<Account> &accounts) {
    std::cout << "\n=== Accounts===========================================" << std::endl;
    for (const auto &acc : accounts)
        std::cout << acc << std::endl;
}

// Attempts to deposit the supplied amount into each Account object.
void deposit(std::vector<Account> &accounts, double amount) {
    std::cout << "\n=== Depositing to Accounts =================================" << std::endl;
    for (auto &acc : accounts) {
        if (acc.deposit(amount))
            std::cout << "Deposited " << amount << " to " << acc << std::endl;
        else
            std::cout << "Failed Deposit of " << amount << " to " << acc << std::endl;
    }
}

// Attempts to withdraw the supplied amount from each Account object.
void withdraw(std::vector<Account> &accounts, double amount) {
    std::cout << "\n=== Withdrawing from Accounts ==============================" << std::endl;
    for (auto &acc : accounts) {
        if (acc.withdraw(amount))
            std::cout << "Withdrew " << amount << " from " << acc << std::endl;
        else
            std::cout << "Failed Withdrawal of " << amount << " from " << acc << std::endl;
    }
}

// Utility functions for Savings Account objects.

// Displays the Savings Account objects in the vector.
void display(const std::vector<Savings_Account> &accounts) {
    std::cout << "\n=== Savings Accounts=====================================" << std::endl;
    for (const auto &acc : accounts)
        std::cout << acc << std::endl;
}

// Attempts to deposit the supplied amount into each Savings Account object.
void deposit(std::vector<Savings_Account> &accounts, double amount) {
    std::cout << "\n=== Depositing to Savings Accounts===========================" << std::endl;
    for (auto &acc : accounts) {
        if (acc.deposit(amount))
            std::cout << "Deposited " << amount << " to " << acc << std::endl;
        else
            std::cout << "Failed Deposit of " << amount << " to " << acc << std::endl;
    }
}

// Attempts to withdraw the supplied amount from each Savings Account object.
void withdraw(std::vector<Savings_Account> &accounts, double amount) {
    std::cout << "\n=== Withdrawing from Savings Accounts=======================" << std::endl;
    for (auto &acc : accounts) {
        if (acc.withdraw(amount))
            std::cout << "Withdrew " << amount << " from " << acc << std::endl;
        else
            std::cout << "Failed Withdrawal of " << amount << " from " << acc << std::endl;
    }
}

// Utility functions for Checking Account objects.

// Displays the Checking Account objects in the vector.
void display(const std::vector<Checking_Account> &accounts) {
    std::cout << "\n=== Checking Accounts=====================================" << std::endl;
    for (const auto &acc : accounts)
        std::cout << acc << std::endl;
}

// Attempts to deposit the supplied amount into each Checking Account object.
void deposit(std::vector<Checking_Account> &accounts, double amount) {
    std::cout << "\n=== Depositing to Checking Accounts===========================" << std::endl;
    for (auto &acc : accounts) {
        if (acc.deposit(amount))
            std::cout << "Deposited " << amount << " to " << acc << std::endl;
        else
            std::cout << "Failed Deposit of " << amount << " to " << acc << std::endl;
    }
}

// Attempts to withdraw the supplied amount from each Checking Account object.
void withdraw(std::vector<Checking_Account> &accounts, double amount) {
    std::cout << "\n=== Withdrawing from Checking Accounts=======================" << std::endl;
    for (auto &acc : accounts) {
        if (acc.withdraw(amount))
            std::cout << "Withdrew " << amount << " from " << acc << std::endl;
        else
            std::cout << "Failed Withdrawal of " << amount << " from " << acc << std::endl;
    }
}

// Utility functions for Trust Account objects.

// Displays the Trust Account objects in the vector.
void display(const std::vector<Trust_Account> &accounts) {
    std::cout << "\n=== Trust Accounts=====================================" << std::endl;
    for (const auto &acc : accounts)
        std::cout << acc << std::endl;
}

// Attempts to deposit the supplied amount into each Trust Account object.
void deposit(std::vector<Trust_Account> &accounts, double amount) {
    std::cout << "\n=== Depositing to Trust Accounts===========================" << std::endl;
    for (auto &acc : accounts) {
        if (acc.deposit(amount))
            std::cout << "Deposited " << amount << " to " << acc << std::endl;
        else
            std::cout << "Failed Deposit of " << amount << " to " << acc << std::endl;
    }
}

// Attempts to withdraw the supplied amount from each Trust Account object.
void withdraw(std::vector<Trust_Account> &accounts, double amount) {
    std::cout << "\n=== Withdrawing from Trust Accounts=======================" << std::endl;
    for (auto &acc : accounts) {
        if (acc.withdraw(amount))
            std::cout << "Withdrew " << amount << " from " << acc << std::endl;
        else
            std::cout << "Failed Withdrawal of " << amount << " from " << acc << std::endl;
    }
}
