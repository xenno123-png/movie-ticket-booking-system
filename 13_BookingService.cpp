#pragma once
#include <vector>
#include <string>
#include <iostream>
#include "08_Booking.cpp"
#include "09_Payment.cpp"
#include "11_PriceCalculator.cpp"
#include "12_TicketPrinter.cpp"

using namespace std;

// BookingService: Orchestrates booking workflows, validation, temporary locking, and rollback
class BookingService {
private:
    vector<Booking> bookings;

    bool validateSeats(Show* show, const vector<string>& seatNumbers, vector<ShowSeat*>& outSeats) {
        outSeats.clear();
        outSeats.reserve(seatNumbers.size());
        for (const auto& seatNo : seatNumbers) {
            ShowSeat* seat = show->findShowSeat(seatNo);
            if (seat == nullptr) {
                cout << "[Booking Rejected] Seat " << seatNo << " does not exist.\n";
                return false;
            }
            if (!seat->isAvailable()) {
                cout << "[Booking Rejected] Seat " << seatNo << " is already BOOKED. No seats reserved.\n";
                return false;
            }
            outSeats.push_back(seat);
        }
        return true;
    }

    void lockSeats(const vector<ShowSeat*>& seats) noexcept {
        for (auto* seat : seats) {
            if (seat != nullptr) {
                seat->bookSeat();
            }
        }
    }

    void rollbackSeats(const vector<ShowSeat*>& seats) noexcept {
        for (auto* seat : seats) {
            if (seat != nullptr) {
                seat->releaseSeat();
            }
        }
    }

public:
    BookingService() {}

    Booking* bookTickets(
        const Customer* customer, 
        Show* show, 
        const vector<string>& seatNumbers, 
        Payment* payment
    ) {
        const bool hasInvalidArgs = (customer == nullptr || show == nullptr || payment == nullptr || seatNumbers.empty());
        if (hasInvalidArgs) {
            cout << "[Error] Invalid booking request parameters.\n";
            return nullptr;
        }

        vector<ShowSeat*> selectedSeats;
        const bool isValid = validateSeats(show, seatNumbers, selectedSeats);
        if (!isValid) {
            return nullptr;
        }

        const double totalAmount = PriceCalculator::calculateTotal(selectedSeats);
        TicketPrinter::printSeatPriceBreakdown(selectedSeats);

        // Atomic lock step
        lockSeats(selectedSeats);

        // Dynamic payment dispatch
        const bool isPaymentSuccess = payment->pay(totalAmount);

        if (isPaymentSuccess) {
            this->bookings.emplace_back(customer, show, selectedSeats, totalAmount);
            return &this->bookings.back();
        }

        // Atomic rollback on payment failure
        rollbackSeats(selectedSeats);
        cout << "[Booking Failed] Payment was not successful. Seats have been released.\n";
        return nullptr;
    }

    bool cancelBooking(const string& bookingId) {
        for (auto& booking : this->bookings) {
            if (booking.getBookingId() != bookingId) continue;

            if (booking.getStatus() == BookingStatus::CANCELLED) {
                cout << "[Cancel Error] Booking " << bookingId << " is already CANCELLED.\n";
                return false;
            }

            rollbackSeats(booking.getBookedSeats());
            booking.markCancelled();
            cout << "[Success] Booking " << bookingId << " has been cancelled. Seats are now AVAILABLE.\n";
            return true;
        }

        cout << "[Cancel Error] Booking ID " << bookingId << " not found.\n";
        return false;
    }

    const Booking* findBooking(const string& bookingId) const noexcept {
        for (const auto& booking : this->bookings) {
            if (booking.getBookingId() == bookingId) {
                return &booking;
            }
        }
        return nullptr;
    }

    const vector<Booking>& getAllBookings() const noexcept {
        return this->bookings;
    }
};
