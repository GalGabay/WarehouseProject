#include "WareHouse.h"
#include "Action.h"
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

WareHouse::WareHouse(const string &configFilePath) {
    customerCounter = 0;
    volunteerCounter = 0;
    orderCounter = 0;
    isOpen = false;
    defaultCustomer = new CivilianCustomer(-1, "", 0,0);
    defaultVolunteer = new CollectorVolunteer(-1, "", 0);

    parseText(configFilePath);

}

// not finished
void WareHouse::start() {
    isOpen = true;
    cout << "WareHouse is Open!" << endl;

    while(isOpen) {
        string input;
        getline(cin, input); // WORKING !
        istringstream iterateWord(input);
        string firstWord; 
        string secondWord;  
        string thirdWord;
        string fourthWord;
        string fifthWord;
        iterateWord >> firstWord;
        if(firstWord == "step") {
            iterateWord >> secondWord;
            SimulateStep step(stoi(secondWord)); 
            addAction(&step);
            step.act(*this); // ?? is that working?
        } else if(firstWord == "order") { // WORKING!!
            iterateWord >> secondWord;
            Customer* customer = customers[stoi(secondWord)];
            AddOrder addOrder(stoi(secondWord));
            if(stoi(secondWord) > customerCounter) {
                //ERROR
                cerr << "Cannot place this order" << endl;
            } else if(customer->getMaxOrders() == customer->getNumOrders()) {
                //ERROR
                cerr << "Cannot place this order" << endl;
            } else {
                
                addOrder.act(*this);
                addAction(&addOrder);
            }
        } else if(firstWord == "customer") { // WORKING!!
            iterateWord >> secondWord >> thirdWord >> fourthWord >> fifthWord;
            AddCustomer addCustomer(secondWord,thirdWord,stoi(fourthWord),stoi(fifthWord));
            addCustomer.act(*this);
            addAction(&addCustomer);
        } else if(firstWord == "orderStatus") {
            // don't forget to check the input
            iterateWord >> secondWord;
            if(stoi(secondWord) <= orderCounter) {
                PrintOrderStatus printOrderS(stoi(secondWord)); // ??
                printOrderS.act(*this);
                addAction(&printOrderS);
            } else {
                cerr << "Order doesn't exist" << endl;
            }
        } else if(firstWord == "customerStatus") {
            iterateWord >> secondWord;
            if(stoi(secondWord) <= customerCounter) {
                PrintCustomerStatus printCustomerS(stoi(secondWord));
                printCustomerS.act(*this);
                addAction(&printCustomerS);
            } else {
                cerr << "Customer doesn't exist" << endl;
            }
        } else if(firstWord == "volunteerStatus") {
            iterateWord >> secondWord;
            if(stoi(secondWord) <= volunteerCounter) {
                PrintVolunteerStatus printVolunteerS(stoi(secondWord));
                printVolunteerS.act(*this);
                addAction(&printVolunteerS);
            } else {
                cerr << "Volunteer doesn't exist" << endl;
            }
        } else if(firstWord == "log") {
            PrintActionsLog printActionsL;
            printActionsL.act(*this);
            addAction(&printActionsL);
        } else if(firstWord == "close") {
            Close close;
            close.act(*this);
            addAction(&close);
        } else if(firstWord == "backup") {
            BackupWareHouse backup;
            backup.act(*this);
            addAction(&backup);
        } else if(firstWord == "restore") {
            // NEED TO ADD ERROR
            RestoreWareHouse restore;
            restore.act(*this);
            addAction(&restore);
        } 


        

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
    orderCounter++;
}

void WareHouse::addAction(BaseAction* action) {
    actionsLog.push_back(action);
}

// ?? do we need * ???
Customer& WareHouse::getCustomer(int customerId) const {
    if(customerCounter < customerId)
        return *defaultCustomer;
    else {
        Customer* costumer = customers[customerId];
        return *costumer;
    }
}

// ?? ?? do we need * 
Volunteer& WareHouse::getVolunteer(int volunteerId) const {
    bool isExist = false;
    for(Volunteer* volunteer : volunteers) {
        if(volunteer->getId() == volunteerId) {
            isExist = true;
        }
    }
    if(isExist) {
        Volunteer* volunteer = volunteers[volunteerId];
        return *volunteer;
    } else {
        return *defaultVolunteer;
    }
    
}

// ?? ?? do we need * ??? NEED TO CHANGE
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


vector<Order*>& WareHouse::getPendingOrders() {
        return pendingOrders;
    }
vector<Order*>& WareHouse::getProcessOrders() {
    return inProcessOrders;
}
vector<Order*>& WareHouse::getAllOrders() {
    return allOrders;
}

vector<Volunteer*>& WareHouse::getVolunteers() {
        return volunteers;
    }
vector<Customer*>& WareHouse::getCustomers() {
        return customers;
    }

void WareHouse::AddOrderCounter() {
    orderCounter++;
}
int WareHouse::getOrderCounter() {
    return orderCounter;
}

void WareHouse::addCustomerCounter(){
    customerCounter++;
}
int WareHouse::getCustomerCounter() {
    return customerCounter;
}





void WareHouse::CreateCustomer(string secondWord, string thirdWord, string fourthWord, string fifthWord) {
    if(thirdWord=="soldier") {
        SoldierCustomer* newSoldierCustomer = new SoldierCustomer(customerCounter,secondWord,stoi(fourthWord),stoi(fifthWord));
        customers.push_back(newSoldierCustomer);
    } else {
        CivilianCustomer* newCivilianCustomer = new CivilianCustomer(customerCounter,secondWord,stoi(fourthWord),stoi(fifthWord));
        customers.push_back(newCivilianCustomer);
    }
    customerCounter++;
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
            iterateWord >> secondWord >> thirdWord >> fourthWord >> fifthWord;
            CreateCustomer(secondWord,thirdWord,fourthWord,fifthWord);
            // if(thirdWord=="soldier") {
            //     SoldierCustomer* newSoldierCustomer = new SoldierCustomer(customerCounter,secondWord,stoi(fourthWord),stoi(fifthWord));
            //     customers.push_back(newSoldierCustomer);
            // } else {
            //     CivilianCustomer* newCivilianCustomer = new CivilianCustomer(customerCounter,secondWord,stoi(fourthWord),stoi(fifthWord));
            //     customers.push_back(newCivilianCustomer);
            // }
            // customerCounter++;
        } else if(firstWord=="volunteer") {
            
            iterateWord >> secondWord;
            iterateWord >> thirdWord;
     
            
            if(thirdWord == "collector") {
            
                iterateWord >> fourthWord;
                CollectorVolunteer* newCollectorVolunteer = new CollectorVolunteer(volunteerCounter,secondWord,stoi(fourthWord));
                volunteers.push_back(newCollectorVolunteer);
     
            }
            else if(thirdWord=="limited_collector") {
                iterateWord >> fourthWord;
                iterateWord >> fifthWord;
                LimitedCollectorVolunteer* newLimitedCollectorVolunteer = new LimitedCollectorVolunteer(volunteerCounter,secondWord,stoi(fourthWord),stoi(fifthWord));
                volunteers.push_back(newLimitedCollectorVolunteer);
   
            } else if(thirdWord=="driver") {
                iterateWord >> fourthWord;
                iterateWord >> fifthWord;
                DriverVolunteer* newDriverVolunteer = new DriverVolunteer(volunteerCounter,secondWord,stoi(fourthWord),stoi(fifthWord));
                volunteers.push_back(newDriverVolunteer);
    
            } else {
                iterateWord >> fourthWord;
                iterateWord >> fifthWord;
                iterateWord >> sixthWord;
                LimitedDriverVolunteer* newLimitedDriverVolunteer = new LimitedDriverVolunteer(volunteerCounter,secondWord,stoi(fourthWord),stoi(fifthWord),stoi(sixthWord));
                volunteers.push_back(newLimitedDriverVolunteer);
            }


            volunteerCounter++;
        } 

}
}