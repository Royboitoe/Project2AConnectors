#include <iostream>
#include <string>

int main(){
    int option = 0;
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
        option = stri(input); 
        if (option != 1 && option != 2){
            cout << "not valid input" << endl;
            option = 0;
        }  
    }
}



// cout << "____________________________________________" << endl;
// cout << "|                                          |"<< endl;
// cout << "|    Range 100,000 - 1,000,000             |"<< endl;
// cout << "|    Price:...............                 |"<< endl;
// cout << "|                                          |"<< endl;
// cout << "|                                          |"<< endl;
// cout << "--------------------------------------------"<< endl;


// cout << "____________________________________________" << endl;
// cout << "|                                          |"<< endl;
// cout << "|    Range 1-15                            |"<< endl;
// cout << "|    Bathroom:............                 |"<< endl;
// cout << "|    Bedroom:.............                 |"<< endl;
// cout << "|    Range 10-10,000 (in Squar Feet        |"<< endl;
// cout << "|    Area.................                 |"<< endl;
// cout << "|                                          |"<< endl;
// cout << "--------------------------------------------"<< endl;

// "Suburb,
// Address,
// Rooms,
// Type,
// Price,
// Postcode,
// Bedrooms,
// Bathrooms,
// LandSize,
// BuildingArea,
// YearBuilt,
// CouncilArea,
// Region,
// PropertyCount"
// cout<< "bathroom:" get line <<endl;
// cout<< "price out of range" <<endl;

// cout<< "invalid synthx" <<endl;
