# Movie Ticket Booking System

A simple console-based movie ticket booking project written in C++ for college System Design (TCS-504) assignment.

**Name:** Dhruv Bhatt  
**Branch:** B.Tech CSE (5th Semester)  
**Date:** 07 September 2026  

---

## What Does This Project Do?

This project simulates a cinema hall booking system (like PVR or INOX) directly in the console:
* **Browse Movies & Shows:** See movies playing, timings, and screen numbers.
* **Live Seat Layout:** Shows available seats [ ] and booked seats [X].
* **Seat Types & Pricing:** 
  * Silver = Rs. 150
  * Gold = Rs. 250
  * Platinum = Rs. 400
* **Safe Booking & Rollback:** If a seat is already booked or payment fails, seats are automatically released.
* **Payment Options:** Pay using UPI, Card, or Cash.
* **Cancel Ticket:** Cancel a booking using Booking ID to make seats free again.

---

## Project Structure

As per assignment instructions, all classes are written in separate files without extra header files and included into main.cpp:

* 01_Movie.cpp - Movie details like name, language, runtime
* 02_Seat.cpp - Physical seat number and category
* 03_Screen.cpp - Screen auditorium containing seats
* 04_Cinema.cpp - Cinema hall containing screens
* 05_Show.cpp - Show timings and movie details
* 06_ShowSeat.cpp - Tracks whether a seat is free or booked for that show
* 07_Customer.cpp - User details like name and phone
* 08_Booking.cpp - Booking record and ticket ID generator
* 09_Payment.cpp - Base payment interface
* 10_PaymentTypes.cpp - UPI, Card, and Cash payment logic
* 11_PriceCalculator.cpp - Calculates total ticket price
* 12_TicketPrinter.cpp - Displays ticket and seat layout
* 13_BookingService.cpp - Handles booking, seat locking, and cancel logic
* 14_SeatCategory.cpp - Base class for seat categories
* 15_SeatCategoryTypes.cpp - Silver, Gold, and Platinum categories
* main.cpp - Main menu and test runner

---

## How to Run

Open command prompt or terminal in this folder and type:

1. Compile the project:
g++ main.cpp -o cinema_booking

2. Run the program:
cinema_booking

3. Run automated tests:
cinema_booking --test

---

## Assignment Report

Detailed system design, relationship tables, and UML diagrams are available in:  
TCS-504_System_Design_Assignment_1.pdf