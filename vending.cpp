/*
  file: vending.cpp
  author: student
  description: vending machine lab program that handles inventory display,
               inserting coins, purchasing items, and giving back change.
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
    int priceInCents; // keeping this in cents so floating point math doesn't ruin totals
    int quantity;
};

// main function to run the vending machine loop
int main()
{
    // setting up the initial inventory with prices and stock levels
    Item items[6] = {
        {"A1", "Water",     100, 3},
        {"A2", "Coke",      150, 4},
        {"B1", "Chips",     200, 1},
        {"B2", "Chocolate", 150, 2},
        {"C1", "Granola",   100, 0},
        {"C2", "Candy",     150, 5},
    };

    int choice = 0;
    int currentCredit = 0; // keeps track of money added in cents

    do
    {
        cout << "\n=== VENDING MACHINE ===" << endl;
        cout << "1. Inventory" << endl;
        cout << "2. Insert coins" << endl;
        cout << "3. Buy Item" << endl;
        cout << "4. Refund" << endl;
        cout << "5. Exit" << endl;
        // dividing by 100.0 to display total money as formatted dollars
        cout << "Current credit: $" << fixed << setprecision(2) << (currentCredit / 100.0) << endl;
        cout << "Enter choice: ";

        cin >> choice;

        if (choice < 1 || choice > 5)
        {
            cout << "Error, please select a number between 1-5" << endl;
        }
        else if (choice == 1)
        {
            // print inventory table with clean formatting
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
                cout << " " << endl;
                cout << "Pick an option (1-4) or 0 to stop: ";
                cin >> choiceCoin;

                if (choiceCoin == 0)
                {
                    cout << "Returning to main menu..." << endl;
                }
                else if (choiceCoin == 1) 
                {
                    currentCredit += 5; 
                }
                else if (choiceCoin == 2)
                {
                    currentCredit += 10;
                }
                else if (choiceCoin == 3)
                {
                    currentCredit += 25;
                }
                else if (choiceCoin == 4)
                {
                    currentCredit += 100;
                }
                else
                {
                    cout << "Error, please pick an option between 1-4 (or 0 to cancel)" << endl;
                }
            } while(choiceCoin != 0);
        }
        else if (choice == 3) // buying an item
        {
            // keeping starting credit saved so receipt can show tendered amount
            int duplicateCredit = currentCredit;
            string slotChoice;
            cout << "Select a slot number (e.g., A1, B2): ";
            cout << " " << endl;
            cin >> slotChoice;

            bool found = false;

            for (int i = 0; i < 6; i++)
            {
                if (items[i].Code == slotChoice)
                {
                    found = true;

                    // make sure item is in stock and user added enough coins
                    if (items[i].quantity <= 0)
                    {
                        cout << "Sorry, " << items[i].name << " is out of stock!" << endl;
                    }
                    else if (currentCredit < items[i].priceInCents)
                    {
                        int missing = items[i].priceInCents - currentCredit;
                        cout << "Insufficient credit! You need $" 
                             << fixed << setprecision(2) << (missing / 100.0) 
                             << " more." << endl;
                    }
                    else
                    {
                        // subtract price and take one from stock
                        currentCredit -= items[i].priceInCents;
                        items[i].quantity--;

                        cout << "\n------- Receipt -------" << endl;
                        cout << "Item: " << items[i].name << endl;
                        cout << "Price: $" << fixed << setprecision(2) << (items[i].priceInCents / 100.0) << endl;
                        cout << "Tendered: $" << fixed << setprecision(2) << (duplicateCredit / 100.0) << endl;
                        cout << "Change: $" << fixed << setprecision(2) << ((duplicateCredit - items[i].priceInCents) / 100.0) << endl;
                        cout << "-----------------------" << endl;
                    }

                    break; 
                }
            }

            if (!found)
            {
                cout << "Invalid code! Item not found." << endl;
            }
        }
        else if (choice == 4) // giving back remaining change
        {
            if (currentCredit == 0)
            {
                cout << "No credit to refund." << endl;
            }
            else
            {
                int refundAmount = currentCredit;
                int dollars = 0;
                int quarters = 0;
                int dimes = 0;
                int nickels = 0;

                // give largest coin coins back first to use as few coins as possible
                while (currentCredit >= 100)
                {
                    currentCredit -= 100;
                    dollars++;
                }

                while (currentCredit >= 25)
                {
                    currentCredit -= 25;
                    quarters++;
                }

                while (currentCredit >= 10)
                {
                    currentCredit -= 10;
                    dimes++;
                }

                while (currentCredit >= 5)
                {
                    currentCredit -= 5;
                    nickels++;
                }

                cout << "\n------- Refund Dispensed -------" << endl;
                cout << "Total Refunded: $" << fixed << setprecision(2) << (refundAmount / 100.0) << endl;
                cout << "Dollar Coins ($1.00): " << dollars << endl;
                cout << "Quarters     ($0.25): " << quarters << endl;
                cout << "Dimes        ($0.10): " << dimes << endl;
                cout << "Nickels      ($0.05): " << nickels << endl;
                cout << "--------------------------------" << endl;
            }
        }

    } while (choice != 5); 

    cout << " " << endl;
    cout << "Thanks for your business come back soon!";

    return 0;
}
