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
            SimulateStep* step = new SimulateStep(stoi(secondWord)); 
            step->act(*this); // ?? is that working?
            addAction(step);
        } else if(firstWord == "order") { // WORKING!!
            iterateWord >> secondWord;
            AddOrder* addOrder = new AddOrder(stoi(secondWord));
            addOrder->act(*this);
            addAction(addOrder);
        } else if(firstWord == "customer") { // WORKING!!
            iterateWord >> secondWord >> thirdWord >> fourthWord >> fifthWord;
            AddCustomer* addCustomer = new AddCustomer(secondWord,thirdWord,stoi(fourthWord),stoi(fifthWord));
            addCustomer->act(*this);
            addAction(addCustomer);
        } else if(firstWord == "orderStatus") {
            iterateWord >> secondWord;
            PrintOrderStatus* printOrderS = new PrintOrderStatus(stoi(secondWord)); 
            printOrderS->act(*this);
            addAction(printOrderS);
        } else if(firstWord == "customerStatus") {
            iterateWord >> secondWord;
            PrintCustomerStatus* printCustomerS = new PrintCustomerStatus(stoi(secondWord));
            printCustomerS->act(*this);
            addAction(printCustomerS);
        } else if(firstWord == "volunteerStatus") {
            iterateWord >> secondWord;
            PrintVolunteerStatus* printVolunteerS = new PrintVolunteerStatus(stoi(secondWord));
            printVolunteerS->act(*this);
            addAction(printVolunteerS);
        } else if(firstWord == "log") {
            PrintActionsLog* printActionsL = new PrintActionsLog;
            printActionsL->act(*this);
            addAction(printActionsL);
        } else if(firstWord == "close") {
            Close* close = new Close;
            close->act(*this);
            addAction(close);
        } else if(firstWord == "backup") {
            BackupWareHouse* backup = new BackupWareHouse;
            backup->act(*this);
            addAction(backup);
        } else if(firstWord == "restore") {
            RestoreWareHouse* restore = new RestoreWareHouse;
            restore->act(*this);
            addAction(restore);
        } 
    }

}

void WareHouse::addOrder(Order* order) {
    //if(order->getStatus() == OrderStatus::PENDING) 
    pendingOrders.push_back(order);
        // else if(order->getStatus() == OrderStatus::COLLECTING) 
        //     inProcessOrders.push_back(order);
        // else if(order->getStatus() == OrderStatus::DELIVERING)
        //     inProcessOrders.push_back(order);
        // else if(order->getStatus() == OrderStatus::COMPLETED)
        //     completedOrders.push_back(order);
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
    Order* order;
    for(Order* order : pendingOrders) {
        if(order->getId() == orderId) {
            return *order;
        }
    }
    for(Order* order : inProcessOrders) {
        if(order->getId() == orderId) {
            return *order;
        }
    }
    for(Order* order : completedOrders) {
        if(order->getId() == orderId) {
            return *order;
        }
    }
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
vector<Order*>& WareHouse::getCompletedOrders() {
    return completedOrders;
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

int WareHouse::getVolunteerCounter() {
    return volunteerCounter;
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

template <typename T> void WareHouse::deleteVector(vector<T*>& toDeleteVector) {
    for (T* element : toDeleteVector) {
         delete element;
         element = nullptr;
    }
    toDeleteVector.clear();  
}

 WareHouse::WareHouse(WareHouse& other) : customerCounter(other.customerCounter), volunteerCounter(other.volunteerCounter),
    orderCounter(other.orderCounter), isOpen(other.isOpen)
  {
    defaultVolunteer = other.defaultVolunteer->clone();
    defaultCustomer = other.defaultCustomer->clone();
    for(Volunteer* volunteer : other.volunteers) {
        volunteers.push_back(volunteer->clone());
    }
    for(Customer* customer : other.customers) {
        customers.push_back(customer->clone());
    }
     for(BaseAction* action : other.actionsLog) {
        addAction(action->clone());
    }
    for(Order* order : other.pendingOrders) {
        pendingOrders.push_back(order->clone());

    }
    for(Order* order : other.inProcessOrders) {
        inProcessOrders.push_back(order->clone());
    }    
    for(Order* order : other.completedOrders) {
        completedOrders.push_back(order->clone());
    }   
    // for(Order* order : other.allOrders) {
    //    allOrders.push_back(order->clone());
    // }
 }

WareHouse::~WareHouse() {
    deleteVector(actionsLog);
    deleteVector(volunteers);
    deleteVector(pendingOrders);
    deleteVector(inProcessOrders);
    deleteVector(completedOrders);
    allOrders.clear();
    deleteVector(customers);
    delete defaultCustomer;
    delete defaultVolunteer;
    defaultCustomer = nullptr;
    defaultVolunteer = nullptr;
}

void WareHouse::operator=(const WareHouse& other) {
    if(&other != this) {
        isOpen = other.isOpen;
        customerCounter = other.customerCounter;
        volunteerCounter = other.volunteerCounter;
        orderCounter = other.orderCounter;
        // defaultVolunteer = other.defaultVolunteer;
        // defaultCustomer = other.defaultCustomer;
        // actionsLog = other.actionsLog;
        // volunteers = other.volunteers;
        // pendingOrders = other.pendingOrders;
        // inProcessOrders = other.inProcessOrders;
        // completedOrders = other.completedOrders;
        // customers = other.customers;
        
        // allOrders = other.allOrders;



         //check::
        delete defaultCustomer;
        delete defaultVolunteer;
        defaultCustomer = nullptr;
        defaultVolunteer = nullptr;
        defaultVolunteer = new CollectorVolunteer(*other.defaultVolunteer);
        defaultCustomer = new CivilianCustomer(*other.defaultCustomer);
       
        //deleteVector(actionsLog);
        deleteVector(volunteers);
        deleteVector(pendingOrders);
        deleteVector(inProcessOrders);
        deleteVector(completedOrders);
        allOrders.clear();
        deleteVector(customers);



        // for(Action* action : other.actionsLog) {
        //     actionsLog.push_back(action);
        // }
        for(Volunteer* action : other.volunteers) {
            volunteers.push_back(action->clone());
        }
        for(Order* action : other.pendingOrders) {
            pendingOrders.push_back(action->clone());
        }
        for(Order* action : other.inProcessOrders) {
            inProcessOrders.push_back(action->clone());
        }
        for(Order* action : other.completedOrders) {
            completedOrders.push_back(action->clone());
        }
        for(Customer* action : other.customers) {
            customers.push_back(action->clone());
        }
        for(Order* action : other.allOrders) {
            allOrders.push_back(action->clone());
        }
        
        
    }
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