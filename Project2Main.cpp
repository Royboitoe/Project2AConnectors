#include <iostream>
#include <string>

using namespace std;

int main() {
    int option = 0;
    int price = 0;
    int bedroom = 0;
    int bathroom = 0;
    int area = 0;

    while(option == 0){
        cout << "____________________________________________" << endl;
        cout << "|           To do calculatation            |" << endl;
        cout << "|    Option 1.                 Option 2.   |"<< endl;
        cout << "|    B-Tree                  Red and Black |"<< endl;
        cout << "|                                Tree      |"<< endl;
        cout << "|                                          |" << endl;
        cout << "--------------------------------------------" << endl;
        string input;
        cin >> input;
        option = stoi(input);
        if (option != 1 && option != 2){
            cout << "not valid input" << endl;
            option = 0;
        }
    }
    while(price == 0){
        string input;
        cout << "____________________________________________" << endl;
        cout << "|                                          |"<< endl;
        cout << "|    Range 100000 - 1000000                |"<< endl;
        cout << "|    Price:...............                 |"<< endl;
        cout << "|                                          |"<< endl;
        cout << "|                                          |"<< endl;
        cout << "--------------------------------------------"<< endl;
        cin >> input;
        price = stoi(input);
        if (price < 100000 || price > 1000000) {
            cout << "not valid input" << endl;
            price = 0;
        }
    }
    while(bedroom == 0){
        string input;
        cout << "____________________________________________" << endl;
        cout << "|                                          |"<< endl;
        cout << "|    Range 1-15                            |"<< endl;
        cout << "|    Bedroom:.............                 |"<< endl;
        cout << "|                                          |"<< endl;
        cout << "--------------------------------------------"<< endl;
        cin >> input;
        bedroom = stoi(input);
        if (bedroom  < 1 ||  bedroom  > 15){
            cout << "not valid input" << endl;
            bedroom = 0;
        }
    }

    while (bathroom == 0) {
            string input;
            cout << "____________________________________________" << endl;
            cout << "|                                          |"<< endl;
            cout << "|    Range 1-15                            |"<< endl;
            cout << "|    Bathroom:.............                |"<< endl;
            cout << "|                                          |"<< endl;
            cout << "--------------------------------------------"<< endl;
            cin >> input;
            bathroom = stoi(input);
            if (bathroom < 1 ||bathroom > 15) {
                cout << "not valid input" << endl;
                bathroom = 0;
            }
    }
    while (area == 0) {
        string input;
        cout << "____________________________________________" << endl;
        cout << "|                                          |"<< endl;
        cout << "|    Range 10-10000                        |"<< endl;
        cout << "|    Area:.................                |"<< endl;
        cout << "|                                          |"<< endl;
        cout << "--------------------------------------------"<< endl;
        cin >> input;
        area  = stoi(input);
        if (area  < 10 ||area  > 10000) {
            cout << "not valid input" << endl;
            area  = 0;
        }
    }

}


//cout << "|    Bathroom:............                 |"<< endl;
//cout << "|    Range 10-10,000 (in Squar Feet)       |"<< endl;
//cout << "|    Area.................                 |"<< endl;
//cout << "|                                          |"<< endl;


// cout << "____________________________________________" << endl;
// cout << "|                                          |"<< endl;
// cout << "|    Range 1-15                            |"<< endl;
// cout << "|    Bedroom:.............                 |"<< endl;
// cout << "|    Bathroom:............                 |"<< endl;
// cout << "|    Range 10-10,000 (in Squar Feet)        |"<< endl;
// cout << "|    Area.................                 |"<< endl;
// cout << "|                                          |"<< endl;
// cout << "--------------------------------------------"<< endl;

// Suburb,
// Address,
// Rooms,
// Type,
// Price,
// Postcode,
// Bedrooms ,
// Bathrooms,
// LandSize,
// BuildingArea,
// YearBuilt,
// CouncilArea,
// Region,
//  ,
//SHOW STRUCTURE
//cout << "__________________________________________________________________________________________________________________________________________________________" << endl;
//cout << "|  Type H = House,                                                                                                                                       |" << endl;
//cout << "|                                                                                                                                                        |" << endl;
//cout << "---------------------------------------------------------------------------------------------------------------------------------------------------------|" << endl;
//cout << "| Suburb | Address | Rooms | Type | Price | Postcode | Bedrooms | Bathrooms | LandSize | BuildingArea | YearBuilt | CouncilArea | Region | PropertyCount |" << endl;
//cout << "|                                                                                                                                                        |" << endl;

// PRINT OUT
//cout << "---------------------------------------------------------------------------------------------------------------------------------------------------------|" << endl;
//cout << "| Suburb | Address | Rooms | Type | Price | Postcode | Bedrooms | Bathrooms | LandSize | BuildingArea | YearBuilt | CouncilArea | Region | PropertyCount |" << endl;
//cout << "|                                                                                                                                                        |" << endl;

//TIMER

//cout << "timer: " << endl;
#include <chrono>
#include <iostream>
#include <thread> // For std::this_thread::sleep_for

int main() {
    auto start = std::chrono::high_resolution_clock::now();

    // Your code to be timed
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    std::cout << "Time taken: " << duration.count() << " seconds" << std::endl;
    return 0;
}

// cout<< "bathroom:" get line <<endl;
// cout<< "price out of range" <<endl;

// cout<< "invalid synthx" <<endl;
