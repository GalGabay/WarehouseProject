#include "Action.h"
#include <algorithm>
#include <vector>
#include <iostream>
extern WareHouse* backup;

// ???
BaseAction::BaseAction() {
    errorMsg = ""; //?
}

 ActionStatus BaseAction::getStatus() const {
    return status;
 }

 void BaseAction::complete() {
    status = ActionStatus::COMPLETED;
 }

 void BaseAction::error(string _errorMsg) {
    status = ActionStatus::ERROR;
    errorMsg = _errorMsg;
 }

 string BaseAction::getErrorMsg() const {
    return errorMsg;
 }

 string BaseAction::actionStatusToString(ActionStatus status) const {
    if(status == ActionStatus::COMPLETED) {
        return "COMPLETED";
    } else{
        return "ERROR";
    }
 }

// ## SIMULATE STEP ##


 SimulateStep::SimulateStep(int _numOfSteps) : numOfSteps(_numOfSteps) 
 {
 }

 void SimulateStep::act(WareHouse &wareHouse) {
    int steps = numOfSteps;
    while(steps > 0) {
        stage1(wareHouse);
        stage234(wareHouse);
        steps--;
    }
    complete();
    
 }


 string SimulateStep::toString() const {
    string output = "step " + to_string(numOfSteps) + " "  +  actionStatusToString(getStatus());
    return output;
 }

 SimulateStep* SimulateStep::clone() const  {
    return new SimulateStep(*this);
 }

// not finished!!
void SimulateStep::stage1(WareHouse &wareHouse) {
    vector<Order*> pendOrders =  wareHouse.getPendingOrders();
    for(Order* order : pendOrders) {
        if(order->getStatus() == OrderStatus::PENDING) {
            for(Volunteer* volunteer : wareHouse.getVolunteers()) {
                if(volunteer->getTypeVolunteer() == "CollectorVolunteer" || volunteer->getTypeVolunteer() == "LimitedCollectorVolunteer" ) {

                    if(volunteer->canTakeOrder(*order)) {
                        
                        cout << "volunteer " << volunteer->getName() << " entered to order: " <<order->getId() <<endl;
                        volunteer->acceptOrder(*order);
                        order->setCollectorId(volunteer->getId());
                        // ERASING:
                        wareHouse.getPendingOrders().erase(std::remove_if(wareHouse.getPendingOrders().begin(), wareHouse.getPendingOrders().end(),
                                [order](Order* o) { return o == order; }),wareHouse.getPendingOrders().end());
                    // add to its suitable vector list
                        order->setStatus(OrderStatus::COLLECTING);
                        wareHouse.getProcessOrders().push_back(order);
                        break;
                    
                    }
                }
            }
        } else if(order->getStatus() == OrderStatus::COLLECTING) {

            for(Volunteer* volunteer : wareHouse.getVolunteers()) {
                if(volunteer->getTypeVolunteer() == "DriverVolunteer" || volunteer->getTypeVolunteer() == "LimitedDriverVolunteer") {
                    if(volunteer->canTakeOrder(*order)) {
                        volunteer->acceptOrder(*order);
                        order->setDriverId(volunteer->getId());
                        // ERASING:
                        wareHouse.getPendingOrders().erase(std::remove_if(wareHouse.getPendingOrders().begin(), wareHouse.getPendingOrders().end(),
                                [order](Order* o) { return o == order; }),wareHouse.getPendingOrders().end());
                        // add to its suitable vector list
                        order->setStatus(OrderStatus::DELIVERING);
 
                        wareHouse.getProcessOrders().push_back(order);
                        break;
                                    
                    }
                }
            }
        }
    } 
  }

void SimulateStep::stage234(WareHouse &wareHouse) { 
    vector<Order*> processOrders =  wareHouse.getProcessOrders();
    for(Order* order : processOrders) {
        if(order->getStatus() ==OrderStatus::COLLECTING) {

            // why &warehouse and not warehouse
            CollectorVolunteer* collector = dynamic_cast<CollectorVolunteer*>(&wareHouse.getVolunteer(order->getCollectorId()));
            (*collector).setTimeLeft((*collector).getTimeLeft() - 1);
            //stage 3
            if(collector->getTimeLeft() == 0) {
                collector->setActiveOrderId(NO_ORDER);
                collector->setCompletedOrderId(order->getId());
                //ERASING:
                wareHouse.getProcessOrders().erase(std::remove_if(wareHouse.getProcessOrders().begin(), wareHouse.getProcessOrders().end(),
                                [order](Order* o) { return o == order; }),wareHouse.getProcessOrders().end());
                wareHouse.getPendingOrders().push_back(order); 

                //stage 4
                if(collector->getTypeVolunteer() == "LimitedCollectorVolunteer") {
                    LimitedCollectorVolunteer* limitedcollector = dynamic_cast<LimitedCollectorVolunteer*>(collector);
                    if(limitedcollector->getNumOrdersLeft() == 0) {
                        // ERASING:
                        wareHouse.getVolunteers().erase(std::remove_if(wareHouse.getVolunteers().begin(), wareHouse.getVolunteers().end(),
                                [limitedcollector](Volunteer* v) { return v == limitedcollector; }),wareHouse.getVolunteers().end());

                        delete limitedcollector;
                        limitedcollector = nullptr;
                    }
                }
            }
        } else if(order->getStatus() ==OrderStatus::DELIVERING) {

            // why &warehouse and not warehouse
            DriverVolunteer* driver = dynamic_cast<DriverVolunteer*>(&wareHouse.getVolunteer(order->getDriverId()));
            (*driver).decreaseDistanceLeft();
            //stage 3
            if(driver->getDistanceLeft() == 0) {
                driver->setActiveOrderId(NO_ORDER);
                driver->setCompletedOrderId(order->getId());
                //ERASING:
                wareHouse.getProcessOrders().erase(std::remove_if(wareHouse.getProcessOrders().begin(), wareHouse.getProcessOrders().end(),
                                [order](Order* o) { return o == order; }),wareHouse.getProcessOrders().end());
                wareHouse.getCompletedOrders().push_back(order);
                order->setStatus(OrderStatus::COMPLETED);
                //stage 4
                if(driver->getTypeVolunteer() == "LimitedDriverVolunteer") {
                    LimitedDriverVolunteer* limiteddriver = dynamic_cast<LimitedDriverVolunteer*>(driver);
                    if(limiteddriver->getMaxOrders() == 0) {
                        //ERASING:
                        wareHouse.getVolunteers().erase(std::remove_if(wareHouse.getVolunteers().begin(), wareHouse.getVolunteers().end(),
                                [limiteddriver](Volunteer* v) { return v == limiteddriver; } ),wareHouse.getVolunteers().end());

                        delete limiteddriver;
                        limiteddriver = nullptr;

                    }
                }
            }
        }
    }
  }

AddOrder::AddOrder(int id) : customerId(id) 
{}

void AddOrder::act(WareHouse &wareHouse) {

    if(customerId > wareHouse.getCustomerCounter()) {
        //ERROR
        error("Cannot place this order");
        cerr << getErrorMsg() << endl;
    } 
    else { 
        Customer* theCustomer = wareHouse.getCustomers()[customerId]; 
        //cout << "maxorders: " << theCustomer->getMaxOrders() << endl;
        //cout << "numOrders: " << theCustomer->getNumOrders() << endl;
       
        if(theCustomer->getMaxOrders() == theCustomer->getNumOrders()) {
            
            //ERROR
            error("Cannot place this order");
            cerr << getErrorMsg() << endl;
            
        } 
        else {

            //need to push to ordersId of customer
            //theCustomer->getOrdersIds().push_back(wareHouse.getOrderCounter());
            Order* newOrder = new Order(wareHouse.getOrderCounter(), customerId, theCustomer->getCustomerDistance() );
            theCustomer->addOrder(wareHouse.getOrderCounter());
            //wareHouse.AddOrderCounter();
            wareHouse.addOrder(newOrder);
            

            complete();
        } 
    }
}


string AddOrder::toString() const {
    string output = "order " + to_string(customerId) + " "  +  actionStatusToString(getStatus());
    return output;
}

AddOrder* AddOrder::clone() const {
    return new AddOrder(*this);
}

AddCustomer::AddCustomer(string _customerName, string _customerType, int _distance, int _maxOrders) :
    customerName(_customerName), customerType(_customerType == "soldier" ? CustomerType::Soldier : CustomerType::Civilian), distance(_distance), maxOrders(_maxOrders)
 {}

void AddCustomer::act(WareHouse &wareHouse) {
    wareHouse.addCustomerCounter();
    if(customerType == CustomerType::Soldier) {
        SoldierCustomer* newSoldierCustomer = new SoldierCustomer(wareHouse.getCustomerCounter(), customerName, distance, maxOrders);
        wareHouse.getCustomers().push_back(newSoldierCustomer);
    } else {
        CivilianCustomer* newCivilianCustomer = new CivilianCustomer(wareHouse.getCustomerCounter(), customerName, distance, maxOrders);
        wareHouse.getCustomers().push_back(newCivilianCustomer);
    }
    complete();
}

AddCustomer* AddCustomer::clone() const {
    return new AddCustomer(*this);
}


string AddCustomer::toString() const {
    string typeCustomer = (customerType == CustomerType::Soldier ? "Soldier" : "Civilian");
    string output = "customer " + customerName + " "  + typeCustomer + " " + to_string(distance) + " " + to_string(maxOrders) + " " + actionStatusToString(getStatus());
    return output;
}


PrintOrderStatus::PrintOrderStatus(int id) : orderId(id) 
{}

void PrintOrderStatus::act(WareHouse &wareHouse) {
    if(orderId < wareHouse.getOrderCounter()) {
        cout << "entered" << endl;
        Order* theOrder = wareHouse.getAllOrders()[orderId];
        cout << theOrder->toString() << endl;
        cout << "CustomerId: " << theOrder->getCustomerId() << endl;
        if(theOrder->getCollectorId() != NO_VOLUNTEER) {
            cout << "Collector: " <<theOrder->getCollectorId() << endl;
        } else {
            cout << "Collector: None" << endl;
        }
        if(theOrder->getDriverId() != NO_VOLUNTEER) {
            cout << "Driver: " <<theOrder->getDriverId() << endl;
        } else {
            cout << "Driver: None" << endl;
        }
        complete();
    }
    else {
        error("Order doesn't exist");
        cerr << getErrorMsg() << endl;
    }
}
PrintOrderStatus* PrintOrderStatus::clone() const {
    return new PrintOrderStatus(*this);
}




string PrintOrderStatus::toString() const {
    string output = "orderStatus " + to_string(orderId) + " "  +  actionStatusToString(getStatus());
    return output;
}

PrintCustomerStatus::PrintCustomerStatus(int _customerId) : customerId(_customerId) 
{}

void PrintCustomerStatus::act(WareHouse &wareHouse) {
    if(customerId < wareHouse.getCustomerCounter()) {
        Customer* customer = wareHouse.getCustomers()[customerId];
        cout << "CustomerID: " << customerId << endl;
        for(int orderId : customer->getOrdersIds()) {
            Order* order = wareHouse.getAllOrders()[orderId];
            cout << "OrderID: " << order->getId() << endl;
            cout << "OrderStatus: " << order->orderStatusToString(order->getStatus()) << endl;
        }
        cout << "numOrdersLeft: " << customer->getMaxOrders()-customer->getNumOrders() << endl;
        complete();
    } else {
        error("Customer doesn't exist");
        cerr << getErrorMsg() << endl;
    }
    
}

PrintCustomerStatus* PrintCustomerStatus::clone() const {
    return new PrintCustomerStatus(*this);
}


string PrintCustomerStatus::toString() const {
    string output = "customerStatus " + to_string(customerId) + " "  +  actionStatusToString(getStatus());
    return output;
}

PrintVolunteerStatus::PrintVolunteerStatus(int _id) : VolunteerId(_id) 
{}

void PrintVolunteerStatus::act(WareHouse &wareHouse) {

    Volunteer* volunteer = nullptr;
    for(Volunteer* vol : wareHouse.getVolunteers()) {
        if(vol->getId() == VolunteerId) {
            volunteer = vol;
            break;
        }
    }
    if(volunteer != nullptr) {
        cout << "VolunteerID: " << VolunteerId << endl;
        string isBusy = volunteer->isBusy() ? "true" : "false";
        cout << "isBusy: " << isBusy << endl;
        if (volunteer->isBusy()){
            cout << "OrderID: " << volunteer->getActiveOrderId() << endl; 
        }
        else cout << "OrderID: None" << endl;
        string volunteerType = volunteer->getTypeVolunteer();
        if(volunteerType == "CollectorVolunteer") {
            CollectorVolunteer* collectorVolunteer = dynamic_cast<CollectorVolunteer*>(volunteer);
            if (volunteer->isBusy())
                cout << "TimeLeft: " << collectorVolunteer->getTimeLeft() << endl;
            else cout << "TimeLeft: None" << endl;
            cout << "OrdersLeft: No Limit" << endl;
        }else if (volunteerType == "LimitedCollectorVolunteer"){
            LimitedCollectorVolunteer* limitedCollectorVolunteer = dynamic_cast<LimitedCollectorVolunteer*>(volunteer);
            if (volunteer->isBusy())
                cout << "TimeLeft: " << limitedCollectorVolunteer->getTimeLeft() << endl;
            else cout << "TimeLeft: None" << endl;
            cout << "OrdersLeft: " << limitedCollectorVolunteer->getNumOrdersLeft() << endl;
        }else if (volunteerType == "DriverVolunteer"){
            DriverVolunteer* driverVolunteer = dynamic_cast<DriverVolunteer*>(volunteer);
            if (volunteer->isBusy())
                cout << "TimeLeft: " << driverVolunteer->getDistanceLeft() << endl;
            else cout << "TimeLeft: None" << endl;
            cout << "OrdersLeft: No Limit" << endl;
        }else if (volunteerType == "LimitedDriverVolunteer"){
            LimitedDriverVolunteer* limitedDriverVolunteer = dynamic_cast<LimitedDriverVolunteer*>(volunteer);
            if (volunteer->isBusy())
                cout << "TimeLeft: " << limitedDriverVolunteer->getDistanceLeft() << endl;
            cout << "TimeLeft: None" << endl;
            cout << "OrdersLeft: " << limitedDriverVolunteer->getNumOrdersLeft() << endl;
        }
        complete();
    }
    else {
        error("Volunteer doesn't exist");
        cerr << getErrorMsg() << endl;
    }
}

PrintVolunteerStatus* PrintVolunteerStatus::clone() const {
    return new PrintVolunteerStatus(*this);
}
// ???
string PrintVolunteerStatus::toString() const {
    string output = "volunteerStatus " + to_string(VolunteerId) + " "  +  actionStatusToString(BaseAction::getStatus());
    return output;
}

PrintActionsLog::PrintActionsLog() {
}

void PrintActionsLog::act(WareHouse &wareHouse) {
    for (BaseAction* action : wareHouse.getActions()){
        cout << action->toString() << endl;
    }
    complete();
}
PrintActionsLog* PrintActionsLog::clone() const {
    return new PrintActionsLog(*this);
}
string PrintActionsLog::toString() const {
    string output = "log " +  actionStatusToString(getStatus());
    return output;
}

// NEED TO DO

Close::Close() {
}
void Close::act(WareHouse &wareHouse) {
    for(Order* order : wareHouse.getAllOrders()) {
        cout << "OrderID: " << order->getId() << " , CustomerID: " << order->getCustomerId() << " , OrderStatus: " << order->orderStatusToString(order->getStatus()) << endl;
    }
    //delete &wareHouse;
    wareHouse.close();
}

Close* Close::clone() const {
    return new Close(*this);
}
string Close::toString() const {
    return "close " + actionStatusToString(getStatus());
}

BackupWareHouse::BackupWareHouse() {
}
void BackupWareHouse::act(WareHouse &wareHouse) {
    if(backup != nullptr) {
        backup = nullptr;
    }
    backup = new WareHouse(wareHouse);
    complete();
    // // what about the fields
    // for(Volunteer* volunteer : wareHouse.getVolunteers()) {
    //     backup->getVolunteers().push_back(volunteer->clone());
    // }
    // for(Customer* customer : wareHouse.getCustomers()) {
    //     backup->getCustomers().push_back(customer->clone());
        
    // }
    //  for(BaseAction* action : wareHouse.getActions()) {
    //     backup->addAction(action->clone());
    // }
    // for(Order* order : wareHouse.getPendingOrders()) {
    //     backup->getPendingOrders().push_back(new Order(*order));
    // }
    // for(Order* order : wareHouse.getProcessOrders()) {
    //     backup->getProcessOrders().push_back(new Order(*order));
    // }    
    // for(Order* order : wareHouse.getCompletedOrders()) {
    //     backup->getCompletedOrders().push_back(new Order(*order));
    // }   
    // for(Order* order : wareHouse.getAllOrders()) {
    //     backup->getAllOrders().push_back(new Order(*order));
    // }

}
BackupWareHouse* BackupWareHouse::clone() const {
    return new BackupWareHouse(*this);
}
string BackupWareHouse::toString() const {
    return "backup " + actionStatusToString(getStatus());
}

RestoreWareHouse::RestoreWareHouse() {

}
void RestoreWareHouse::act(WareHouse &wareHouse) {
    if(backup!=nullptr) {
        //delete &wareHouse;
        wareHouse = *backup;
        complete();
    } else {
        error("No backup available");
        cerr << getErrorMsg() << endl;
    }
}
RestoreWareHouse* RestoreWareHouse::clone() const {
    return new RestoreWareHouse(*this);
}
string RestoreWareHouse::toString() const {
    return "restore " +  actionStatusToString(getStatus());
}