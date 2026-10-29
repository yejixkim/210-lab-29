// COMSC 210 | Lab 29 | Yeji Kim
// Cafe Rush Hour Simulation

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <array>
#include <list>

using namespace std;

// Each station has three lists:
// 0 = Customer names
// 1 = Order types
// 2 = Order statuses

using ShopData = map<string, array<list<string>, 3>>;

// Function prototypes
void displayShop(const ShopData& shop);
void moveCustomer(ShopData& shop, const string& from,
                  const string& to, const string& newStatus);
void simulateMinute(ShopData& shop, int minute);

int main() {
    // Create the map for the coffee shop.
    ShopData shop;

    // Open the external data file.
    ifstream fin("data.txt");

    if (!fin) {
        cout << "Error opening data.txt\n";
        return 1;
    }

    // Read customer records from the file.
    string line;

    while (getline(fin, line)) {
        stringstream ss(line);

        string station, customer, order, status;

        getline(ss, station, ',');
        getline(ss, customer, ',');
        getline(ss, order, ',');
        getline(ss, status);

        // Store the information in the station's three lists.
        shop[station][0].push_back(customer);
        shop[station][1].push_back(order);
        shop[station][2].push_back(status);
    }

    fin.close();

    // Display the initial state.
    cout << "INITIAL COFFEE SHOP STATE\n";
    displayShop(shop);

    // Simulate 25 one-minute time periods.
    for (int minute = 1; minute <= 25; minute++) {
        cout << "\nMinute " << minute << endl;

        // Simulate changes at each station.
        simulateMinute(shop, minute);

        // Display the updated state.
        displayShop(shop);
    }

    return 0;
}

void displayShop(const ShopData& shop) {
    // Display each station and its three lists.
    for (const auto& station : shop) {
        cout << "\nStation: " << station.first << endl;

        for (int category = 0; category < 3; category++) {
            if (category == 0)
                cout << "Customers: ";
            else if (category == 1)
                cout << "Orders: ";
            else
                cout << "Statuses: ";

            for (const string& item : station.second[category]) {
                cout << item << ", ";
            }

            cout << endl;
        }
    }
}

void moveCustomer(ShopData& shop, const string& from,
                  const string& to, const string& newStatus) {
    // Check whether the source station has a customer.
    if (shop[from][0].empty()) {
        return;
    }

    // Get the first customer's information.
    string customer = shop[from][0].front();
    string order = shop[from][1].front();

    // Remove the customer from the original station.
    shop[from][0].pop_front();
    shop[from][1].pop_front();
    shop[from][2].pop_front();

    // Add the customer to the next station.
    shop[to][0].push_back(customer);
    shop[to][1].push_back(order);
    shop[to][2].push_back(newStatus);

    cout << customer << "'s order moved from "
         << from << " to " << to << ".\n";
}

void simulateMinute(ShopData& shop, int minute) {
    // Move a customer to drink preparation at minute 1.
    if (minute == 1) {
        moveCustomer(shop, "Order Line",
                     "Drink Preparation", "Preparing");
    }

    // Move the prepared drink to pickup at minute 2.
    if (minute == 2) {
        moveCustomer(shop, "Drink Preparation",
                     "Pickup Counter", "Ready for pickup");
    }
}