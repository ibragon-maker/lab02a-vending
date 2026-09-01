/*
  file: vending.cpp
  author: student
  description: vending machine program with stretch goals (restock mode, coin bank tracking,
               exact change validation, and sales reporting on exit).
*/

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <iomanip>
#include <limits>
using namespace std;

// struct for items in the vending machine
struct Item
{
    string Code;
    string name;
    int priceInCents;
    int quantity;
    int unitsSold = 0; // tracks total items sold for sales report
};

// struct to manage the machine's physical coin supply
struct CoinBank
{
    int dollars = 10;
    int quarters = 20;
    int dimes = 20;
    int nickels = 20;
};

// helper function to test if current coin bank can dispense a specific change amount
bool canMakeChange(int changeNeeded, CoinBank bank)
{
    while (changeNeeded >= 100 && bank.dollars > 0)
    {
        changeNeeded -= 100;
        bank.dollars--;
    }
    while (changeNeeded >= 25 && bank.quarters > 0)
    {
        changeNeeded -= 25;
        bank.quarters--;
    }
    while (changeNeeded >= 10 && bank.dimes > 0)
    {
        changeNeeded -= 10;
        bank.dimes--;
    }
    while (changeNeeded >= 5 && bank.nickels > 0)
    {
        changeNeeded -= 5;
        bank.nickels--;
    }
    return changeNeeded == 0;
}

int main()
{
    // initial inventory
    Item items[6] = {
        {"A1", "Water",     100, 3, 0},
        {"A2", "Coke",      150, 4, 0},
        {"B1", "Chips",     200, 1, 0},
        {"B2", "Chocolate", 150, 2, 0},
        {"C1", "Granola",   100, 0, 0},
        {"C2", "Candy",     150, 5, 0},
    };

    CoinBank bank;
    int choice = 0;
    int currentCredit = 0;
    int totalRevenue = 0;
    const int MAX_CAPACITY = 10;

    do
    {
        cout << "\n=== VENDING MACHINE ===" << endl;
        cout << "1. Inventory" << endl;
        cout << "2. Insert coins" << endl;
        cout << "3. Buy Item" << endl;
        cout << "4. Refund" << endl;
        cout << "5. Exit" << endl;
        cout << "Current credit: $" << fixed << setprecision(2) << (currentCredit / 100.0) << endl;
        cout << "Enter choice: ";

        cin >> choice;

        if (choice == 99) // stretch goal 1: hidden operator restock mode
        {
            cout << "\n*** OPERATOR RESTOCK MODE ***" << endl;
            string slot;
            cout << "Enter item code to restock (or 0 to cancel): ";
            cin >> slot;

            if (slot != "0")
            {
                bool found = false;
                for (int i = 0; i < 6; i++)
                {
                    if (items[i].Code == slot)
                    {
                        found = true;
                        items[i].quantity = MAX_CAPACITY;
                        cout << items[i].name << " restocked to max capacity (" << MAX_CAPACITY << ")." << endl;
                        break;
                    }
                }
                if (!found) cout << "Invalid code!" << endl;
            }
        }
        else if (choice < 1 || choice > 5)
        {
            cout << "Error, please select a number between 1-5" << endl;
        }
        else if (choice == 1)
        {
            cout << "\n--------------------------------------------------" << endl;
            cout << left  << setw(6)  << "Code" 
                 << left  << setw(18) << "Name" 
                 << right << setw(10) << "Price" 
                 << right << setw(12) << "Quantity" << endl;
            cout << "--------------------------------------------------" << endl;

            for (int i = 0; i < 6; i++) {
                cout << left  << setw(6)  << items[i].Code
                     << left  << setw(18) << items[i].name
                     << right << setw(4)  << "$" 
                     << right << setw(6)  << fixed << setprecision(2) << (items[i].priceInCents / 100.0)
                     << right << setw(12) << items[i].quantity 
                     << endl;
            }
            cout << "--------------------------------------------------" << endl;
        }
        else if (choice == 2)
        {
            int choiceCoin;
            do {
                cout << "\n1. Nickel\n2. Dime\n3. Quarter\n4. Dollar Coin ($1.00)\n0. Cancel" << endl;
                cout << "Pick an option (1-4) or 0 to stop: ";
                cin >> choiceCoin;

                // stretch goal 2: coin bank grows as payments are inserted
                if (choiceCoin == 1) { currentCredit += 5; bank.nickels++; }
                else if (choiceCoin == 2) { currentCredit += 10; bank.dimes++; }
                else if (choiceCoin == 3) { currentCredit += 25; bank.quarters++; }
                else if (choiceCoin == 4) { currentCredit += 100; bank.dollars++; }
                else if (choiceCoin != 0) { cout << "Invalid selection." << endl; }

            } while(choiceCoin != 0);
        }
        else if (choice == 3) // buying an item
        {
            int duplicateCredit = currentCredit;
            string slotChoice;
            cout << "Select a slot number (e.g., A1, B2): ";
            cin >> slotChoice;

            bool found = false;

            for (int i = 0; i < 6; i++)
            {
                if (items[i].Code == slotChoice)
                {
                    found = true;
                    int changeNeeded = currentCredit - items[i].priceInCents;

                    if (items[i].quantity <= 0)
                    {
                        cout << "Sorry, " << items[i].name << " is out of stock!" << endl;
                    }
                    else if (currentCredit < items[i].priceInCents)
                    {
                        int missing = items[i].priceInCents - currentCredit;
                        cout << "Insufficient credit! You need $" 
                             << fixed << setprecision(2) << (missing / 100.0) << " more." << endl;
                    }
                    // stretch goal 3: check exact change capability
                    else if (!canMakeChange(changeNeeded, bank))
                    {
                        cout << "\n[EXACT CHANGE ONLY] Machine cannot provide change for this purchase!" << endl;
                        cout << "Transaction cancelled. Please insert exact change or select a different item." << endl;
                    }
                    else
                    {
                        currentCredit -= items[i].priceInCents;
                        items[i].quantity--;
                        items[i].unitsSold++;
                        totalRevenue += items[i].priceInCents;

                        cout << "\n------- Receipt -------" << endl;
                        cout << "Item: " << items[i].name << endl;
                        cout << "Price: $" << fixed << setprecision(2) << (items[i].priceInCents / 100.0) << endl;
                        cout << "Tendered: $" << fixed << setprecision(2) << (duplicateCredit / 100.0) << endl;
                        cout << "Change: $" << fixed << setprecision(2) << (changeNeeded / 100.0) << endl;
                        cout << "-----------------------" << endl;
                    }
                    break; 
                }
            }

            if (!found) cout << "Invalid code! Item not found." << endl;
        }
        else if (choice == 4) // refund change using actual coin bank
        {
            if (currentCredit == 0)
            {
                cout << "No credit to refund." << endl;
            }
            else
            {
                int refundAmount = currentCredit;
                int d = 0, q = 0, dm = 0, n = 0;

                while (currentCredit >= 100 && bank.dollars > 0)
                { currentCredit -= 100; bank.dollars--; d++; }
                while (currentCredit >= 25 && bank.quarters > 0)
                { currentCredit -= 25; bank.quarters--; q++; }
                while (currentCredit >= 10 && bank.dimes > 0)
                { currentCredit -= 10; bank.dimes--; dm++; }
                while (currentCredit >= 5 && bank.nickels > 0)
                { currentCredit -= 5; bank.nickels--; n++; }

                cout << "\n------- Refund Dispensed -------" << endl;
                cout << "Total Refunded: $" << fixed << setprecision(2) << (refundAmount / 100.0) << endl;
                cout << "Dollar Coins ($1.00): " << d << endl;
                cout << "Quarters     ($0.25): " << q << endl;
                cout << "Dimes        ($0.10): " << dm << endl;
                cout << "Nickels      ($0.05): " << n << endl;
                cout << "--------------------------------" << endl;
            }
        }

    } while (choice != 5); 

    // stretch goal 4: sales report on exit
    cout << "\n==========================================" << endl;
    cout << "             FINAL SALES REPORT           " << endl;
    cout << "==========================================" << endl;
    cout << "Total Revenue: $" << fixed << setprecision(2) << (totalRevenue / 100.0) << endl;
    cout << "------------------------------------------" << endl;
    cout << "Units Sold Per Slot:" << endl;

    int maxSold = -1;
    string bestSeller = "None";

    for (int i = 0; i < 6; i++)
    {
        cout << items[i].Code << " (" << items[i].name << "): " << items[i].unitsSold << " sold" << endl;
        if (items[i].unitsSold > maxSold && items[i].unitsSold > 0)
        {
            maxSold = items[i].unitsSold;
            bestSeller = items[i].name;
        }
    }

    cout << "------------------------------------------" << endl;
    cout << "Best-Selling Item: " << bestSeller << endl;
    cout << "==========================================" << endl;
    cout << "\nThanks for your business come back soon!" << endl;

    return 0;
}
