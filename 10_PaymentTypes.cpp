#pragma once
#include <iostream>
#include <iomanip>
#include <string>
#include <utility>
#include "09_Payment.cpp"

using namespace std;

// UPI Payment Strategy
class UpiPayment : public Payment {
private:
    string upiId;
    bool shouldSucceed;

public:
    explicit UpiPayment(string upiId = "user@okhdfcbank", bool shouldSucceed = true)
        : upiId(move(upiId)), shouldSucceed(shouldSucceed) {}

    bool pay(double amount) override {
        if (this->shouldSucceed) {
            cout << "[UPI] Rs." << fixed << setprecision(0) 
                 << amount << " paid successfully\n";
            return true;
        }
        cout << "[UPI] Transaction FAILED: Bank server timed out.\n";
        return false;
    }

    const char* getPaymentMethodName() const noexcept override {
        return "UPI";
    }
};

// Credit/Debit Card Payment Strategy
class CardPayment : public Payment {
private:
    string cardNumber;
    bool shouldSucceed;

public:
    explicit CardPayment(string cardNumber = "XXXX-XXXX-XXXX-1234", bool shouldSucceed = true)
        : cardNumber(move(cardNumber)), shouldSucceed(shouldSucceed) {}

    bool pay(double amount) override {
        if (this->shouldSucceed) {
            const size_t len = this->cardNumber.length();
            const string lastFour = (len > 4) ? this->cardNumber.substr(len - 4) : this->cardNumber;
            cout << "[Card] Rs." << fixed << setprecision(0) 
                 << amount << " charged to card ending in " << lastFour << " successfully.\n";
            return true;
        }
        cout << "[Card] Transaction FAILED: Insufficient balance / Card declined.\n";
        return false;
    }

    const char* getPaymentMethodName() const noexcept override {
        return "Card";
    }
};

// Cash Payment Strategy
class CashPayment : public Payment {
private:
    bool shouldSucceed;

public:
    explicit CashPayment(bool shouldSucceed = true) noexcept : shouldSucceed(shouldSucceed) {}

    bool pay(double amount) override {
        if (this->shouldSucceed) {
            cout << "[Cash] Rs." << fixed << setprecision(0) 
                 << amount << " collected at counter successfully.\n";
            return true;
        }
        cout << "[Cash] Transaction FAILED: Cash tendered is insufficient.\n";
        return false;
    }

    const char* getPaymentMethodName() const noexcept override {
        return "Cash";
    }
};
