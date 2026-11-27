#include "Checking_Account.h"

Checking_Account::Checking_Account(std::string name, double balance)
    : Account{name, balance} {}

bool Checking_Account::withdraw(double amount) {
    if (amount < 0)
        return false;
    amount += per_check_fee;
    return Account::withdraw(amount);
}

bool Checking_Account::deposit(double amount) { return Account::deposit(amount); }

void Checking_Account::print(std::ostream &os) const {
    os.precision(2);
    os << std::fixed;
    os << "[Checking_Account: " << name << ": " << balance << "]";
}
