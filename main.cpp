// COMSC 210 | Lab 31 | Yeji Kim
// Cafe Rush Hour Simulation

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <array>
#include <list>
#include <algorithm>
#include <cctype>
#include <random>
#include <vector>

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

bool containsOnlySpaces(const string& text) {
    return !text.empty() &&
           all_of(text.begin(), text.end(),
                  [](unsigned char c) { return isspace(c); });
}

// Create structure to store customer info.
struct Customer {
    string name;
    string order;
    string status;
    string station;
    int arrivalTime;
};

// Generate random customer arrival times.
void assignArrivalTimes(vector<Customer>& customers) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> arrivalDist(0, 24);

    for (Customer& customer : customers) {
        customer.arrivalTime = arrivalDist(gen);
    }
}

// Add arriving customers to their assigned starting stations.
void addArrivingCustomers(vector<Customer>& customers,
                          ShopData& shop, int currentMinute) {
    for (Customer& customer : customers) {
        if (customer.arrivalTime == currentMinute) {
            shop[customer.station][0].push_back(customer.name);
            shop[customer.station][1].push_back(customer.order);
            shop[customer.station][2].push_back(customer.status);

            cout << customer.name << " arrived at the coffee shop."
                 << endl;
        }
    }
}

int main() {
    // Create the map for the coffee shop.
    ShopData shop;
shop["Order Line"];
shop["Drink Preparation"];
shop["Pickup Counter"];

    // Open the external data file.
    ifstream fin("data.txt");

    if (!fin) {
        cout << "Error opening data.txt\n";
        return 1;
    }

    // Read customer records from the file.
    string line;

    vector<Customer> customers;

    while (getline(fin, line)) {
        stringstream ss(line);

        string station, customer, order, status;

        getline(ss, station, ',');
getline(ss, customer, ',');
getline(ss, order, ',');
getline(ss, status, ',');

string extra;
bool hasExtraField = static_cast<bool>(getline(ss, extra, ',')) ||
                     (!line.empty() && line.back() == ',');

if ((station != "Order Line" &&
     station != "Drink Preparation" &&
     station != "Pickup Counter") ||
    station.empty() || containsOnlySpaces(station) ||
    customer.empty() || containsOnlySpaces(customer) ||
    order.empty() || containsOnlySpaces(order) ||
    status.empty() || containsOnlySpaces(status) ||
    hasExtraField) {
    cout << "Skipping invalid record: " << line << endl;
    continue;
}

// Store valid records in the station's three lists.
Customer newCustomer;
newCustomer.name = customer;
newCustomer.order = order;
newCustomer.status = status;
newCustomer.station = station;
newCustomer.arrivalTime = 0;

customers.push_back(newCustomer);
    }

    fin.close();

    assignArrivalTimes(customers);

    // Display the initial state.
cout << "INITIAL COFFEE SHOP STATE\n";
displayShop(shop);

// Simulate 25 one-minute time periods.
for (int minute = 0; minute <= 24; minute++) {
    cout << "\nMinute " << minute << endl;

    // Add customers scheduled to arrive.
    addArrivingCustomers(customers, shop, minute);

    // Simulate changes at each station.
    if (minute > 0) {
        simulateMinute(shop, minute);
    }

    // Display the updated state.
    displayShop(shop);
}

    return 0;
}


void displayShop(const ShopData& shop) {
    cout << "------------------------------\n";

    // Display stations in a consistent order
    const string stations[] = {
        "Order Line",
        "Drink Preparation",
        "Pickup Counter"
    };

    for (const string& name : stations) {
        auto it = shop.find(name);

        if (it != shop.end()) {
            size_t count = it->second[0].size();

            cout << name << ": " << count
                 << (count == 1 ? " customer" : " customers")
                 << "\n";
        }
    }

    cout << "------------------------------\n";
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
    for (int i = 0; i < 2; i++) {
        moveCustomer(shop, "Order Line", "Drink Preparation",
                     "Preparing");
    }

    for (int i = 0; i < 2; i++) {
        moveCustomer(shop, "Drink Preparation", "Pickup Counter",
                     "Ready for Pickup");
    }
}