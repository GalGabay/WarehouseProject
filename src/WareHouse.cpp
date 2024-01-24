#include "WareHouse.h"
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>


WareHouse::WareHouse(const string &configFilePath) {
    customerCounter = 0;
    volunteerCounter = 0;
    orderCounter = 0;
    isOpen = false;

    parseText(configFilePath);

}

// not finished
void WareHouse::start() {
    isOpen = true;
    cout << "WareHouse is Open!" << endl;

    while(isOpen) {
        string input;
        cin >> input;
        

    }
}

void WareHouse::addOrder(Order* order) {
    if(order->getStatus() == OrderStatus::PENDING) 
        pendingOrders.push_back(order);
    else if(order->getStatus() == OrderStatus::COLLECTING)
        inProcessOrders.push_back(order);
    else if(order->getStatus() == OrderStatus::DELIVERING)
        inProcessOrders.push_back(order);
    else if(order->getStatus() == OrderStatus::COMPLETED)
        completedOrders.push_back(order);
    allOrders.push_back(order);
}

void WareHouse::addAction(BaseAction* action) {
    actionsLog.push_back(action);
}

// ?? do we need * ???
Customer& WareHouse::getCustomer(int customerId) const {
    Customer* costumer = customers[customerId];
    return *costumer;
}

// ?? ?? do we need * ???
Volunteer& WareHouse::getVolunteer(int volunteerId) const {
    Volunteer* volunteer = volunteers[volunteerId];
    return *volunteer;
}

// ?? ?? do we need * ???
Order& WareHouse::getOrder(int orderId) const {
    Order* order = allOrders[orderId];
    return *order;
}

const vector<BaseAction*>& WareHouse::getActions() const {
    return actionsLog;
}

void WareHouse::close() {
    isOpen = false;
}

void WareHouse::open() {
    isOpen = true;
}


const vector<Order*>& WareHouse::getPendingOrders() const {
        return pendingOrders;
    }
const vector<Order*>& WareHouse::getProcessOrders() const {
    return inProcessOrders;
}
const vector<Volunteer*>& WareHouse::getVolunteers() const {
        return volunteers;
    }







void WareHouse::parseText(const string &configFilePath) {
    std::ifstream inputFile(configFilePath);
    
    if (!inputFile.is_open()) {
        cerr << "Error opening the file." << endl;
    }

    std::string line;
    while (std::getline(inputFile, line)) {
        
        std::istringstream iterateWord(line);
        std::string word;
        std::string firstWord;
        std::string secondWord;
        std::string thirdWord;
        std::string fourthWord;
        std::string fifthWord;
        std::string sixthWord;

        iterateWord >> firstWord;
        
        if(firstWord=="customer") {
            iterateWord >> secondWord;
            iterateWord >> thirdWord;
            iterateWord >> fourthWord;
            iterateWord >> fifthWord;
            if(thirdWord=="soldier") {
                SoldierCustomer newSoldierCustomer(customerCounter,secondWord,stoi(fourthWord),stoi(fifthWord));
                customers.push_back(&newSoldierCustomer);
            } else {
                CivilianCustomer newCivilianCustomer(customerCounter,secondWord,stoi(fourthWord),stoi(fifthWord));
                customers.push_back(&newCivilianCustomer);
            }
            customerCounter++;
        } else if(firstWord=="volunteer") {
            iterateWord >> secondWord;
            iterateWord >> thirdWord;
            
            if(thirdWord == "collector") {
                iterateWord >> fourthWord;
                CollectorVolunteer newCollectorVolunteer(volunteerCounter,secondWord,stoi(fourthWord));
                volunteers.push_back(&newCollectorVolunteer);
            }
            else if(thirdWord=="limited_collector") {
                iterateWord >> fourthWord;
                iterateWord >> fifthWord;
                LimitedCollectorVolunteer newLimitedCollectorVolunteer(volunteerCounter,secondWord,stoi(fourthWord),stoi(fifthWord));
                volunteers.push_back(&newLimitedCollectorVolunteer);
            } else if(thirdWord=="driver") {
                iterateWord >> fourthWord;
                iterateWord >> fifthWord;
                DriverVolunteer newDriverVolunteer(volunteerCounter,secondWord,stoi(fourthWord),stoi(fifthWord));
                volunteers.push_back(&newDriverVolunteer);
            } else {
                iterateWord >> fourthWord;
                iterateWord >> fifthWord;
                iterateWord >> sixthWord;
                LimitedDriverVolunteer newLimitedDriverVolunteer(volunteerCounter,secondWord,stoi(fourthWord),stoi(fifthWord),stoi(sixthWord));
                volunteers.push_back(&newLimitedDriverVolunteer);
            }
            volunteerCounter++;
        } 

}