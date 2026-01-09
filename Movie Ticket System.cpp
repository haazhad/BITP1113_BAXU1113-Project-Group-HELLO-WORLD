#include <iostream>
#include <string>
#include <iomanip>
#include <vector> 
using namespace std;

// Constants for system configuration
const int MAX_ROWS = 5;
const int MAX_COLS = 5;
const double BASE_PRICE = 12.50;
const double WEEKDAY_DISCOUNT = 0.85;

// Struct to store individual sales data
struct SaleRecord {
    int row;
    int col;
    double price;
    char dayType; // 'D' for Weekday, 'E' for Weekend
};

// Function prototypes
void displayMenu();
void initializeSeats(char seats[][MAX_COLS]);
void displaySeats(char seats[][MAX_COLS]);
bool bookSeat(char seats[][MAX_COLS], int row, int col);
double calculatePrice(int row, char day, int age);
void displayTicket(int row, int col, double price, string movieName);
int countAvailableSeats(char seats[][MAX_COLS]);
void displayPricing();
int getValidInt(string prompt, int min, int max);

int main() {
    // Variable declarations
    char seats[MAX_ROWS][MAX_COLS];
    int choice = 0, row, col;
    char day;
    string movieName = "AVATAR 3";
    double ticketPrice;
    bool continueBooking = true;

    // Vector to store the sales history (The "Database")
    vector<SaleRecord> weeklySales;

    // Initialize all seats as available
    initializeSeats(seats);

    cout << "========================================" << endl;
    cout << "   MOVIE TICKET BOOKING SYSTEM" << endl;
    cout << "========================================" << endl;
    cout << "Movie: " << movieName << endl;
    cout << "========================================" << endl;

    // Main program loop
    while (continueBooking) {
        displayMenu();
        choice = getValidInt("Enter your choice: ", 1, 4);

        // Process user choice
        switch (choice) {
        case 1:
            // Display current seat availability
            displaySeats(seats);
            cout << "\nAvailable seats: " << countAvailableSeats(seats)
                << "/" << (MAX_ROWS * MAX_COLS) << endl;
            break;

        case 2:
            // Book a ticket
            displaySeats(seats);
            row = getValidInt("\nEnter row number (1-" + to_string(MAX_ROWS) + "): ", 1, MAX_ROWS);
            col = getValidInt("Enter seat number (1-" + to_string(MAX_COLS) + "): ", 1, MAX_COLS);

            // Double check validation (redundant safety but good)
            if (row < 1 || row > MAX_ROWS || col < 1 || col > MAX_COLS) {
                cout << "\nInvalid seat selection!" << endl;
                break;
            }

            // Check if seat is available and book it
            if (bookSeat(seats, row - 1, col - 1)) {
                cout << "\nIs this a Weekday or Weekend? (Enter 'D' for Weekday, 'E' for Weekend): ";
                cin >> day;

                // Ask for Age
                int age = getValidInt("Enter the customer's age (0-120): ", 0, 120);

                // Pass age to the calculator
                ticketPrice = calculatePrice(row, day, age);
                // Clear buffer in case user typed multiple chars
                cin.ignore(10000, '\n');

                // Calculate dynamic price
                ticketPrice = calculatePrice(row, day, age);

                // Save the sale to vector history
                weeklySales.push_back({ row, col, ticketPrice, (char)toupper(day) });

                // Display ticket confirmation
                displayTicket(row, col, ticketPrice, movieName);
            }
            else {
                cout << "\nSeat already booked! Please choose another." << endl;
            }
            break;

        case 3:
            // Display pricing information
            displayPricing();
            break;

        case 4:
            // Exit system AND Print Weekly Report
            cout << "\n========================================" << endl;
            cout << "       CLOSING SYSTEM & GENERATING REPORT" << endl;
            cout << "========================================" << endl;

            if (weeklySales.empty()) {
                cout << "No sales were recorded this session." << endl;
            }
            else {
                cout << "\n--- WEEKLY SALES SUMMARY ---" << endl;
                cout << left << setw(10) << "Day Type"
                    << setw(10) << "Seat"
                    << right << setw(10) << "Price (RM)" << endl;
                cout << "----------------------------------------" << endl;

                double totalRevenue = 0.0;

                for (const auto& record : weeklySales) {
                    string seatLabel = "R" + to_string(record.row) + ":S" + to_string(record.col);
                    string dayLabel = (record.dayType == 'D') ? "Weekday" : "Weekend";

                    cout << left << setw(10) << dayLabel
                        << setw(10) << seatLabel
                        << right << setw(10) << fixed << setprecision(2) << record.price << endl;

                    totalRevenue += record.price;
                }

                cout << "----------------------------------------" << endl;
                cout << left << setw(20) << "TOTAL REVENUE:"
                    << right << setw(10) << totalRevenue << endl;
            }

            cout << "\nThank you for using our booking system!" << endl;
            continueBooking = false;
            break;

        default:
            cout << "\nInvalid choice! Please try again." << endl;
        }

        if (continueBooking) {
            cout << "\nPress Enter to continue...";
            cin.get();
        }
    }

    return 0;
}

// Function to display main menu
void displayMenu() {
    cout << "\n========================================" << endl;
    cout << "             MAIN MENU" << endl;
    cout << "========================================" << endl;
    cout << "1. View Available Seats" << endl;
    cout << "2. Book a Ticket" << endl;
    cout << "3. View Pricing Information" << endl;
    cout << "4. Finish & Print Weekly Report" << endl;
    cout << "========================================" << endl;
}

// Function to initialize all seats as available
void initializeSeats(char seats[][MAX_COLS]) {
    for (int i = 0; i < MAX_ROWS; i++) {
        for (int j = 0; j < MAX_COLS; j++) {
            seats[i][j] = 'O';  // 'O' represents available seat
        }
    }
}

// Function to display seat layout
void displaySeats(char seats[][MAX_COLS]) {
    cout << "\n========================================" << endl;
    cout << "             SEAT LAYOUT" << endl;
    cout << "========================================" << endl;
    cout << "          SCREEN" << endl;
    cout << "========================================" << endl;

    // Display column numbers
    cout << "    ";
    for (int j = 1; j <= MAX_COLS; j++) {
        cout << setw(3) << j;
    }
    cout << endl;

    // Display rows with seat status
    for (int i = 0; i < MAX_ROWS; i++) {
        cout << setw(2) << (i + 1) << "  ";

        for (int j = 0; j < MAX_COLS; j++) {
            cout << "[" << seats[i][j] << "]";
        }
        cout << endl;
    }

    cout << "\nLegend: O = Available, X = Booked" << endl;
}

// Function to book a seat
bool bookSeat(char seats[][MAX_COLS], int row, int col) {
    // Check if seat is available
    if (seats[row][col] == 'O') {
        seats[row][col] = 'X';  // Mark seat as booked
        return true;
    }
    return false;
}

// Function to calculate dynamic ticket price
double calculatePrice(int row, char day, int age) {
    double finalPrice = BASE_PRICE;

    // Apply Age-Based Discounts FIRST
    if (age <= 12) {
        finalPrice = finalPrice * 0.50; // 50% discount for kids
    }
    else if (age >= 60) {
        finalPrice = finalPrice * 0.70; // 30% discount for seniors
    }

    // Apply Day-Based Discounts (Stackable)
    if (toupper(day) == 'D') {
        finalPrice = finalPrice * WEEKDAY_DISCOUNT;
    }

    return finalPrice;
}

// Function to display ticket details
void displayTicket(int row, int col, double price, string movieName) {
    cout << "\n========================================" << endl;
    cout << "          TICKET CONFIRMATION" << endl;
    cout << "========================================" << endl;
    cout << "Movie: " << movieName << endl;
    cout << "Row: " << row << endl;
    cout << "Seat: " << col << endl;
    cout << fixed << setprecision(2);
    cout << "Price: RM " << price << endl;
    cout << "========================================" << endl;
    cout << "   Booking Successful! Enjoy the show!" << endl;
    cout << "========================================" << endl;
}

// Function to count available seats
int countAvailableSeats(char seats[][MAX_COLS]) {
    int count = 0;
    for (int i = 0; i < MAX_ROWS; i++) {
        for (int j = 0; j < MAX_COLS; j++) {
            if (seats[i][j] == 'O') {
                count++;
            }
        }
    }
    return count;
}

// Function to display pricing information
void displayPricing() {
    cout << "\n========================================" << endl;
    cout << "         PRICING INFORMATION" << endl;
    cout << "========================================" << endl;
    cout << fixed << setprecision(2);
    cout << "Base Price: RM " << BASE_PRICE << endl;
    cout << "- Weekend: Standard Price" << endl;
    cout << "\nAge Discounts:" << endl;
    cout << "- Children (12 and below): 50% off" << endl;
    cout << "- Seniors (60 and above): 30% off" << endl;
    cout << "\nDay Discounts:" << endl;
    cout << "- Weekday: " << ((1 - WEEKDAY_DISCOUNT) * 100) << "% off (Stackable!)" << endl;
    cout << "\n--- Price Examples ---" << endl;
    cout << "Child on Weekday: RM " << (BASE_PRICE * 0.50 * WEEKDAY_DISCOUNT) << endl;
    cout << "Senior on Weekend: RM " << (BASE_PRICE * 0.70) << endl;
    cout << "========================================" << endl;
}
// Function to get valid integer input within a range
int getValidInt(string prompt, int min, int max) {
    int input;
    while (true) {
        cout << prompt;
        cin >> input;

        // Check if input is a number AND within range
        if (!cin.fail() && input >= min && input <= max) {
            return input;
        }

        // Clear error state and ignore bad input
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Invalid input! Please enter a number between " << min << " and " << max << ".\n";
    }
}
