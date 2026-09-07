#pragma once
#include <vector>
#include "02_Seat.cpp"
#include "06_ShowSeat.cpp"

using namespace std;

// PriceCalculator: Calculates pricing totals based on seat categories
class PriceCalculator {
public:
    static constexpr double SILVER_PRICE = 150.0;
    static constexpr double GOLD_PRICE = 250.0;
    static constexpr double PLATINUM_PRICE = 400.0;

    static constexpr double getPriceForSeatType(SeatType type) noexcept {
        switch (type) {
            case SeatType::SILVER:   return SILVER_PRICE;
            case SeatType::GOLD:     return GOLD_PRICE;
            case SeatType::PLATINUM: return PLATINUM_PRICE;
            default:                 return 0.0;
        }
    }

    // Overload 1: Calculate total for a list of ShowSeat pointers
    static double calculateTotal(const vector<ShowSeat*>& seats) noexcept {
        double total = 0.0;
        for (const auto* showSeat : seats) {
            if (showSeat != nullptr) {
                total += getPriceForSeatType(showSeat->getSeatType());
            }
        }
        return total;
    }

    // Overload 2: Calculate total for a given seat type and quantity
    static constexpr double calculateTotal(SeatType type, int quantity) noexcept {
        return getPriceForSeatType(type) * quantity;
    }
};
