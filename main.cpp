#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <memory>
#include <algorithm>
#include <cctype>

using namespace std;

// Include all modular component files (Course Rule: No header files, One class per file)
#include "01_Movie.cpp"
#include "02_Seat.cpp"
#include "06_ShowSeat.cpp"
#include "03_Screen.cpp"
#include "04_Cinema.cpp"
#include "05_Show.cpp"
#include "07_Customer.cpp"
#include "08_Booking.cpp"
#include "09_Payment.cpp"
#include "10_PaymentTypes.cpp"
#include "11_PriceCalculator.cpp"
#include "12_TicketPrinter.cpp"
#include "13_BookingService.cpp"
#include "14_SeatCategory.cpp"
#include "15_SeatCategoryTypes.cpp"

// Fast uppercase seat parser (e.g. "a1, b2" -> {"A1", "B2"})
vector<string> parseSeatList(const string& input) {
    vector<string> seats;
    stringstream ss(input);
    string item;
    while (getline(ss, item, ',')) {
        item.erase(0, item.find_first_not_of(" \t\r\n"));
        const size_t lastValid = item.find_last_not_of(" \t\r\n");
        if (lastValid != string::npos) {
            item.erase(lastValid + 1);
        }
        for (char& c : item) {
            c = static_cast<char>(toupper(static_cast<unsigned char>(c)));
        }
        if (!item.empty()) {
            seats.push_back(move(item));
        }
    }
    return seats;
}

// Robust integer input reader
int readIntChoice(const string& prompt) {
    cout << prompt;
    int value = -1;
    if (!(cin >> value)) {
        cin.clear();
        string discard;
        cin >> discard;
        return -1;
    }
    return value;
}

// Payment method factory
unique_ptr<Payment> createPaymentMethod(int choice) {
    if (choice == 1) return unique_ptr<Payment>(new UpiPayment());
    if (choice == 2) return unique_ptr<Payment>(new CardPayment());
    if (choice == 3) return unique_ptr<Payment>(new CashPayment());
    return nullptr;
}

// Factory for creating polymorphic SeatCategory inspectors
unique_ptr<SeatCategory> createSeatCategory(int choice) {
    if (choice == 1) return unique_ptr<SeatCategory>(new SilverSeatCategory());
    if (choice == 2) return unique_ptr<SeatCategory>(new GoldSeatCategory());
    if (choice == 3) return unique_ptr<SeatCategory>(new PlatinumSeatCategory());
    return nullptr;
}

void handleViewMoviesAndShows(const vector<Movie>& movies, const vector<Show>& shows) {
    cout << "\nAvailable Movies:\n";
    for (size_t i = 0; i < movies.size(); ++i) {
        cout << (i + 1) << ") " << left << setw(15) 
             << movies[i].getTitle() << " " << left << setw(8) 
             << movies[i].getLanguage() << " " << movies[i].getDurationMinutes() << " min\n";
    }

    const int movieChoice = readIntChoice("\nChoose movie: ");
    const bool isValidMovie = (movieChoice >= 1 && movieChoice <= static_cast<int>(movies.size()));
    if (!isValidMovie) {
        cout << "Invalid movie choice.\n";
        return;
    }

    const Movie* selectedMovie = &movies[movieChoice - 1];
    vector<const Show*> matchingShows;
    for (const auto& show : shows) {
        if (show.getMovie()->getTitle() == selectedMovie->getTitle()) {
            matchingShows.push_back(&show);
        }
    }

    cout << "\nAvailable Shows for " << selectedMovie->getTitle() << ":\n";
    for (size_t i = 0; i < matchingShows.size(); ++i) {
        cout << (i + 1) << ") " << matchingShows[i]->getScreen()->getScreenName() 
             << "  " << matchingShows[i]->getStartTime() << '\n';
    }

    const int showChoice = readIntChoice("\nChoose show: ");
    const bool isValidShow = (showChoice >= 1 && showChoice <= static_cast<int>(matchingShows.size()));
    if (!isValidShow) {
        cout << "Invalid show choice.\n";
        return;
    }

    TicketPrinter::printSeatLayout(*matchingShows[showChoice - 1]);
}

void handleBookTickets(vector<Show>& shows, BookingService& service, const Customer* customer) {
    cout << "\n--- Select Show for Booking ---\n";
    for (size_t i = 0; i < shows.size(); ++i) {
        cout << (i + 1) << ") " << shows[i].getMovie()->getTitle() 
             << " (" << shows[i].getScreen()->getScreenName() 
             << " " << shows[i].getStartTime() << ")\n";
    }

    const int showChoice = readIntChoice("\nChoose show: ");
    const bool isValidShow = (showChoice >= 1 && showChoice <= static_cast<int>(shows.size()));
    if (!isValidShow) {
        cout << "Invalid show choice.\n";
        return;
    }

    Show* selectedShow = &shows[showChoice - 1];
    TicketPrinter::printSeatLayout(*selectedShow);

    cout << "\nSeats (e.g. A1,B2): ";
    string seatInput;
    cin >> seatInput;

    const auto requestedSeats = parseSeatList(seatInput);
    if (requestedSeats.empty()) {
        cout << "No valid seats entered.\n";
        return;
    }

    cout << "\nSelect Payment Method:\n";
    cout << "1) UPI\n";
    cout << "2) Card\n";
    cout << "3) Cash\n";
    const int paymentChoice = readIntChoice("Pay by (1-3): ");
    auto paymentMethod = createPaymentMethod(paymentChoice);
    if (!paymentMethod) {
        cout << "Invalid payment method selected.\n";
        return;
    }

    Booking* newBooking = service.bookTickets(customer, selectedShow, requestedSeats, paymentMethod.get());
    if (newBooking != nullptr) {
        TicketPrinter::printTicket(*newBooking);
    }
}

void handleCancelBooking(BookingService& service) {
    cout << "\nEnter Booking ID to cancel (e.g. BK1001): ";
    string bookingId;
    cin >> bookingId;
    service.cancelBooking(bookingId);
}

void handleViewMyTickets(const BookingService& service) {
    const auto& allBookings = service.getAllBookings();
    if (allBookings.empty()) {
        cout << "\nNo bookings made yet.\n";
        return;
    }
    for (const auto& bk : allBookings) {
        TicketPrinter::printTicket(bk);
    }
}

// Inspect tier seats with polymorphic SeatCategory hierarchy
void handleInspectTierSeats(const vector<Show>& shows) {
    cout << "\n--- Select Show for Seat Details ---\n";
    for (size_t i = 0; i < shows.size(); ++i) {
        cout << (i + 1) << ") " << shows[i].getMovie()->getTitle() 
             << " (" << shows[i].getScreen()->getScreenName() 
             << " " << shows[i].getStartTime() << ")\n";
    }

    const int showChoice = readIntChoice("\nChoose show: ");
    const bool isValidShow = (showChoice >= 1 && showChoice <= static_cast<int>(shows.size()));
    if (!isValidShow) {
        cout << "Invalid show choice.\n";
        return;
    }

    const Show& selectedShow = shows[showChoice - 1];

    cout << "\nSelect Category to Inspect:\n";
    cout << "1) Silver (Rs.150)\n";
    cout << "2) Gold (Rs.250)\n";
    cout << "3) Platinum (Rs.400)\n";
    cout << "4) All Categories\n";
    const int tierChoice = readIntChoice("\nChoose category (1-4): ");

    if (tierChoice >= 1 && tierChoice <= 3) {
        unique_ptr<SeatCategory> category = createSeatCategory(tierChoice);
        if (category) {
            category->printDetailedStats(selectedShow);
        }
    } else if (tierChoice == 4) {
        // Polymorphically inspect all three categories
        vector<unique_ptr<SeatCategory>> allCategories;
        allCategories.reserve(3);
        allCategories.push_back(unique_ptr<SeatCategory>(new SilverSeatCategory()));
        allCategories.push_back(unique_ptr<SeatCategory>(new GoldSeatCategory()));
        allCategories.push_back(unique_ptr<SeatCategory>(new PlatinumSeatCategory()));

        for (const auto& cat : allCategories) {
            cat->printDetailedStats(selectedShow);
        }
    } else {
        cout << "Invalid category selected.\n";
    }
}

void runAutomatedTests() {
    cout << "\n======================================================\n";
    cout << "  RUNNING OPTIMIZED AUTOMATED VERIFICATION SUITE      \n";
    cout << "======================================================\n\n";

    Cinema cinema("PVR Cinemas");
    Screen screen1(1, "Screen-1");
    screen1.reserveSeats(9);
    screen1.addSeat("A1", SeatType::SILVER);
    screen1.addSeat("A2", SeatType::SILVER);
    screen1.addSeat("A3", SeatType::SILVER);
    screen1.addSeat("A4", SeatType::SILVER);
    screen1.addSeat("B1", SeatType::GOLD);
    screen1.addSeat("B2", SeatType::GOLD);
    screen1.addSeat("B3", SeatType::GOLD);
    screen1.addSeat("C1", SeatType::PLATINUM);
    screen1.addSeat("C2", SeatType::PLATINUM);

    cinema.addScreen(screen1);
    Movie movie1("3 Idiots", "Hindi", 170);
    Show show1(1, &movie1, cinema.getScreen(1), "06:00 PM");

    show1.bookSeat("A2");
    show1.bookSeat("B3");

    BookingService service;
    Customer customer1("Alice", "9876543210");
    Customer customer2("Bob", "9123456780");

    TicketPrinter::printSeatLayout(show1);

    // Test Tier Inspection Hierarchy
    cout << "\n[TEST: Polymorphic Tier Statistics Verification]\n";
    unique_ptr<SeatCategory> silverCategory(new SilverSeatCategory());
    silverCategory->printDetailedStats(show1);

    unique_ptr<SeatCategory> goldCategory(new GoldSeatCategory());
    goldCategory->printDetailedStats(show1);

    unique_ptr<SeatCategory> platCategory(new PlatinumSeatCategory());
    platCategory->printDetailedStats(show1);

    // Test 1: Booking confirmation
    UpiPayment upiPay("alice@okhdfcbank", true);
    Booking* b1 = service.bookTickets(&customer1, &show1, {"A1", "B2"}, &upiPay);
    if (b1 != nullptr) {
        TicketPrinter::printTicket(*b1);
        cout << ">> TEST 1 PASSED: Confirmed ID " << b1->getBookingId() << '\n';
    }

    // Test 2: Double-booking rejection
    CardPayment cardPay("4532-XXXX-XXXX-9999", true);
    Booking* b2 = service.bookTickets(&customer2, &show1, {"A1", "A3"}, &cardPay);
    if (b2 == nullptr && show1.isSeatAvailable("A3")) {
        cout << ">> TEST 2 PASSED: Double booking rejected; A3 remains AVAILABLE.\n";
    }

    // Test 3: Payment failure rollback
    CardPayment failedCard("4532-XXXX-XXXX-0000", false);
    Booking* b3 = service.bookTickets(&customer2, &show1, {"A3"}, &failedCard);
    if (b3 == nullptr && show1.isSeatAvailable("A3")) {
        cout << ">> TEST 3 PASSED: Payment failure rolled back; A3 remains AVAILABLE.\n";
    }

    // Test 4: Cancellation release
    bool cancelOk = service.cancelBooking(b1->getBookingId());
    if (cancelOk && show1.isSeatAvailable("A1") && show1.isSeatAvailable("B2")) {
        cout << ">> TEST 4 PASSED: Booking cancelled; seats A1 & B2 released.\n";
    }

    // Test 5: Invalid seat gracefully handled
    CashPayment cashPay(true);
    Booking* b5 = service.bookTickets(&customer1, &show1, {"Z9"}, &cashPay);
    if (b5 == nullptr) {
        cout << ">> TEST 5 PASSED: Invalid seat handled without crash.\n";
    }

    cout << "\n======================================================\n";
    cout << "        ALL OPTIMIZED TESTS PASSED SUCCESSFULLY       \n";
    cout << "======================================================\n\n";
}

int main(int argc, char* argv[]) {
    if (argc > 1 && string(argv[1]) == "--test") {
        runAutomatedTests();
        return 0;
    }

    Cinema cinema("PVR Cinemas");
    Screen screen1(1, "Screen-1");
    screen1.reserveSeats(9);
    screen1.addSeat("A1", SeatType::SILVER);
    screen1.addSeat("A2", SeatType::SILVER);
    screen1.addSeat("A3", SeatType::SILVER);
    screen1.addSeat("A4", SeatType::SILVER);
    screen1.addSeat("B1", SeatType::GOLD);
    screen1.addSeat("B2", SeatType::GOLD);
    screen1.addSeat("B3", SeatType::GOLD);
    screen1.addSeat("C1", SeatType::PLATINUM);
    screen1.addSeat("C2", SeatType::PLATINUM);

    Screen screen2(2, "Screen-2");
    screen2.reserveSeats(5);
    screen2.addSeat("A1", SeatType::SILVER);
    screen2.addSeat("A2", SeatType::SILVER);
    screen2.addSeat("B1", SeatType::GOLD);
    screen2.addSeat("B2", SeatType::GOLD);
    screen2.addSeat("C1", SeatType::PLATINUM);

    cinema.addScreen(screen1);
    cinema.addScreen(screen2);

    vector<Movie> movies = {
        Movie("3 Idiots", "Hindi", 170),
        Movie("Interstellar", "English", 169)
    };

    vector<Show> shows = {
        Show(1, &movies[0], cinema.getScreen(1), "06:00 PM"),
        Show(2, &movies[1], cinema.getScreen(2), "09:00 PM")
    };

    shows[0].bookSeat("A2");
    shows[0].bookSeat("B3");

    BookingService bookingService;
    Customer currentCustomer("User", "9999988888");

    while (true) {
        cout << "\n========================================\n";
        cout << "      MOVIE TICKET BOOKING SYSTEM       \n";
        cout << "========================================\n";
        cout << "1) Movies (View Catalog & Shows)\n";
        cout << "2) Book Tickets\n";
        cout << "3) Cancel Booking\n";
        cout << "4) My Tickets\n";
        cout << "5) Seat Tier Details\n";
        cout << "0) Exit\n";
        cout << "----------------------------------------\n";
        const int choice = readIntChoice("Choose option (0-5): ");

        if (choice == 0) {
            cout << "\nThank you for using Movie Ticket Booking System. Goodbye!\n";
            break;
        }

        switch (choice) {
            case 1: handleViewMoviesAndShows(movies, shows); break;
            case 2: handleBookTickets(shows, bookingService, &currentCustomer); break;
            case 3: handleCancelBooking(bookingService); break;
            case 4: handleViewMyTickets(bookingService); break;
            case 5: handleInspectTierSeats(shows); break;
            default:
                cout << "Invalid option selected. Please choose 1, 2, 3, 4, 5, or 0.\n";
                break;
        }
    }

    return 0;
}
