#pragma once
#include <string>
#include <vector>
#include "02_Seat.cpp"
#include "05_Show.cpp"
#include "11_PriceCalculator.cpp"

using namespace std;

struct CategoryStats {
    int totalSeats = 0;
    int reservedSeats = 0;
    int availableSeats = 0;
    vector<string> availableSeatNumbers;
    vector<string> reservedSeatNumbers;
    double pricePerSeat = 0.0;
};

// SeatCategory: Abstract base class for polymorphic tier inspection and statistics
class SeatCategory {
public:
    virtual ~SeatCategory() = default;

    virtual SeatType getSeatType() const noexcept = 0;
    virtual const char* getTierName() const noexcept = 0;
    virtual double getPricePerSeat() const noexcept = 0;
    virtual CategoryStats calculateStats(const Show& show) const = 0;
    virtual void printDetailedStats(const Show& show) const = 0;
};
