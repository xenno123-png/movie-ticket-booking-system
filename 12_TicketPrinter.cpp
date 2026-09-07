#pragma once
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include "08_Booking.cpp"
#include "05_Show.cpp"
#include "11_PriceCalculator.cpp"

using namespace std;

// TicketPrinter: Formats and prints tickets, seat layouts, and payment receipts to console
class TicketPrinter {
private:
    static void printCategoryRow(const char* categoryLabel, const vector<const ShowSeat*>& seats) {
        if (seats.empty()) return;
        cout << left << setw(10) << categoryLabel << " ";
        for (const auto* seat : seats) {
            const char* marker = seat->isAvailable() ? "[ ]" : "[X]";
            cout << seat->getSeatNumber() << marker << " ";
        }
        cout << '\n';
    }

public:
    static void printTicket(const Booking& booking) {
        cout << "\n================ TICKET ================\n";
        cout << "Booking ID : " << booking.getBookingId() << '\n';
        
        const Show* show = booking.getShow();
        if (show != nullptr && show->getMovie() != nullptr && show->getScreen() != nullptr) {
            cout << "Movie      : " << show->getMovie()->getTitle() << '\n';
            cout << "Screen     : " << show->getScreen()->getScreenName() 
                 << "   " << show->getStartTime() << '\n';
        }
        
        cout << "Seats      : ";
        const auto& seats = booking.getBookedSeats();
        for (size_t i = 0; i < seats.size(); ++i) {
            if (seats[i] != nullptr) {
                cout << seats[i]->getSeatNumber() << (i + 1 < seats.size() ? ", " : "");
            }
        }
        cout << '\n';
        
        cout << "Amount     : Rs." << fixed << setprecision(0) 
             << booking.getTotalAmount()
             << "     Status: " << booking.getStatusName() << '\n';
        cout << "========================================\n\n";
    }

    static void printSeatLayout(const Show& show) {
        cout << "\n----------------------------------------\n";
        if (show.getScreen() != nullptr && show.getMovie() != nullptr) {
            cout << show.getScreen()->getScreenName() << "  " 
                 << show.getStartTime() << " |  " 
                 << show.getMovie()->getTitle() << '\n';
        }

        vector<const ShowSeat*> silverSeats, goldSeats, platinumSeats;
        const auto& allShowSeats = show.getShowSeats();
        silverSeats.reserve(allShowSeats.size());
        goldSeats.reserve(allShowSeats.size());
        platinumSeats.reserve(allShowSeats.size());

        for (const auto& showSeat : allShowSeats) {
            switch (showSeat.getSeatType()) {
                case SeatType::SILVER:   silverSeats.push_back(&showSeat); break;
                case SeatType::GOLD:     goldSeats.push_back(&showSeat); break;
                case SeatType::PLATINUM: platinumSeats.push_back(&showSeat); break;
            }
        }

        printCategoryRow("SILVER", silverSeats);
        printCategoryRow("GOLD", goldSeats);
        printCategoryRow("PLATINUM", platinumSeats);

        cout << "\n( [ ] = available  [X] = booked )\n";
        cout << "----------------------------------------\n";
    }

    static void printSeatPriceBreakdown(const vector<ShowSeat*>& selectedSeats) {
        cout << "\nSelected Seats Breakdown:\n";
        for (const auto* seat : selectedSeats) {
            if (seat != nullptr) {
                const double price = PriceCalculator::getPriceForSeatType(seat->getSeatType());
                cout << "  " << left << setw(4) << seat->getSeatNumber() 
                     << " " << left << setw(9) << seat->getTypeName() 
                     << " Rs." << fixed << setprecision(0) << price << '\n';
            }
        }
        const double total = PriceCalculator::calculateTotal(selectedSeats);
        cout << "  TOTAL     Rs." << fixed << setprecision(0) << total << "\n\n";
    }
};
