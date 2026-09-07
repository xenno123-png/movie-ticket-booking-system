#pragma once
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include "14_SeatCategory.cpp"

using namespace std;

namespace CategoryHelper {
    inline void printSeatList(const vector<string>& seatList) {
        if (seatList.empty()) {
            cout << "None";
            return;
        }
        for (size_t i = 0; i < seatList.size(); ++i) {
            cout << seatList[i] << (i + 1 < seatList.size() ? ", " : "");
        }
    }

    inline CategoryStats computeTierStats(const Show& show, SeatType type, double price) {
        CategoryStats stats;
        stats.pricePerSeat = price;
        const auto& showSeats = show.getShowSeats();
        stats.availableSeatNumbers.reserve(showSeats.size());
        stats.reservedSeatNumbers.reserve(showSeats.size());

        for (const auto& seat : showSeats) {
            if (seat.getSeatType() != type) continue;

            stats.totalSeats++;
            if (seat.isAvailable()) {
                stats.availableSeats++;
                stats.availableSeatNumbers.push_back(seat.getSeatNumber());
            } else {
                stats.reservedSeats++;
                stats.reservedSeatNumbers.push_back(seat.getSeatNumber());
            }
        }
        return stats;
    }

    inline void renderTierOutput(const char* tierName, const CategoryStats& stats) {
        cout << "\n========================================\n";
        cout << "       " << left << setw(8) << tierName << " TIER SEAT DETAILS       \n";
        cout << "========================================\n";
        cout << "Price per Seat   : Rs." << fixed << setprecision(0) << stats.pricePerSeat << '\n';
        cout << "Total Seats      : " << stats.totalSeats << '\n';
        cout << "Reserved (Booked): " << stats.reservedSeats << " [";
        printSeatList(stats.reservedSeatNumbers);
        cout << "]\n";
        cout << "Remaining (Free) : " << stats.availableSeats << " [";
        printSeatList(stats.availableSeatNumbers);
        cout << "]\n";
        cout << "========================================\n\n";
    }
}

// Concrete derived inspectors for Silver, Gold, Platinum categories
class SilverSeatCategory : public SeatCategory {
public:
    SeatType getSeatType() const noexcept override { return SeatType::SILVER; }
    const char* getTierName() const noexcept override { return "SILVER"; }
    double getPricePerSeat() const noexcept override { return PriceCalculator::SILVER_PRICE; }

    CategoryStats calculateStats(const Show& show) const override {
        return CategoryHelper::computeTierStats(show, getSeatType(), getPricePerSeat());
    }

    void printDetailedStats(const Show& show) const override {
        CategoryHelper::renderTierOutput(getTierName(), calculateStats(show));
    }
};

class GoldSeatCategory : public SeatCategory {
public:
    SeatType getSeatType() const noexcept override { return SeatType::GOLD; }
    const char* getTierName() const noexcept override { return "GOLD"; }
    double getPricePerSeat() const noexcept override { return PriceCalculator::GOLD_PRICE; }

    CategoryStats calculateStats(const Show& show) const override {
        return CategoryHelper::computeTierStats(show, getSeatType(), getPricePerSeat());
    }

    void printDetailedStats(const Show& show) const override {
        CategoryHelper::renderTierOutput(getTierName(), calculateStats(show));
    }
};

class PlatinumSeatCategory : public SeatCategory {
public:
    SeatType getSeatType() const noexcept override { return SeatType::PLATINUM; }
    const char* getTierName() const noexcept override { return "PLATINUM"; }
    double getPricePerSeat() const noexcept override { return PriceCalculator::PLATINUM_PRICE; }

    CategoryStats calculateStats(const Show& show) const override {
        return CategoryHelper::computeTierStats(show, getSeatType(), getPricePerSeat());
    }

    void printDetailedStats(const Show& show) const override {
        CategoryHelper::renderTierOutput(getTierName(), calculateStats(show));
    }
};
