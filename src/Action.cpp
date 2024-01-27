#include "Action.h"
#include <algorithm>
#include <vector>
#include <iostream>

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
    
 }

// ???
 string SimulateStep::toString() const {

 }

 SimulateStep* SimulateStep::clone() const  {
    return new SimulateStep(*this);
 }

// not finished!!
void SimulateStep::stage1(WareHouse &wareHouse) {
    for(Order* order : wareHouse.getPendingOrders()) {
        if(order->getStatus() == OrderStatus::PENDING) {

            for(Volunteer* volunteer : wareHouse.getVolunteers()) {
                if(volunteer->getTypeVolunteer() == "CollectorVolunteer" || volunteer->getTypeVolunteer() == "LimitedCollectorVolunteer" ) {
                    if(volunteer->canTakeOrder(*order)) {
                        volunteer->acceptOrder(*order);
                        // ERASING:
                        wareHouse.getPendingOrders().erase(std::remove_if(wareHouse.getPendingOrders().begin(), wareHouse.getPendingOrders().end(),
                                [order](Order* o) { return o == order; }),wareHouse.getPendingOrders().end());
                    // add to its suitable vector list
                        order->setStatus(OrderStatus::COLLECTING);
                        wareHouse.addOrder(order); // to InProcessOrder

                    
                    }
                }
            }
        } else if(order->getStatus() == OrderStatus::COLLECTING) {

            for(Volunteer* volunteer : wareHouse.getVolunteers()) {
                if(volunteer->getTypeVolunteer() == "DriverVolunteer" || volunteer->getTypeVolunteer() == "LimitedDriverVolunteer") {
                    if(volunteer->canTakeOrder(*order)) {
                        volunteer->acceptOrder(*order);
                        // ERASING:
                        wareHouse.getPendingOrders().erase(std::remove_if(wareHouse.getPendingOrders().begin(), wareHouse.getPendingOrders().end(),
                                [order](Order* o) { return o == order; }),wareHouse.getPendingOrders().end());
                        // add to its suitable vector list
                        order->setStatus(OrderStatus::DELIVERING);
                        wareHouse.addOrder(order);
                    
                    }
                }
            }
        }
    } 
  }

void SimulateStep::stage234(WareHouse &wareHouse) { 
    for(Order* order : wareHouse.getProcessOrders()) {
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
                wareHouse.addOrder(order); // NOTWORKING add the order to pendingOrders vector

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
            (*driver).setDistanceLeft((*driver).getDistanceLeft() - (*driver).getDistancePerStep());
            //stage 3
            if(driver->getDistanceLeft() == 0) {
                driver->setActiveOrderId(NO_ORDER);
                driver->setCompletedOrderId(order->getId());
                //ERASING:
                wareHouse.getProcessOrders().erase(std::remove_if(wareHouse.getProcessOrders().begin(), wareHouse.getProcessOrders().end(),
                                [order](Order* o) { return o == order; }),wareHouse.getProcessOrders().end());
                wareHouse.addOrder(order); // NOTWORKING add the order to completedOrders vector

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

    
    Customer* theCustomer = wareHouse.getCustomers()[customerId];

    //need to push to ordersId of customer
    //theCustomer->getOrdersIds().push_back(wareHouse.getOrderCounter());
    Order* newOrder = new Order(wareHouse.getOrderCounter(), customerId, theCustomer->getCustomerDistance() );
    wareHouse.AddOrderCounter();
    wareHouse.getPendingOrders().push_back(newOrder);
    wareHouse.getAllOrders().push_back(newOrder);
}

// ???
string AddOrder::toString() const {

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
}

AddCustomer* AddCustomer::clone() const {
    return new AddCustomer(*this);
}

/// ???
string AddCustomer::toString() const {

}


PrintOrderStatus::PrintOrderStatus(int id) : orderId(id) 
{}

void PrintOrderStatus::act(WareHouse &wareHouse) {
    Order* theOrder = wareHouse.getAllOrders()[orderId];
    cout << theOrder->toString() << endl;
    cout << "CustomerId: " << theOrder->getCustomerId() << endl;
    if(theOrder->getCollectorId() != NO_VOLUNTEER) {
        cout << "Collector: " <<theOrder->getCollectorId() << endl;
    } else if(theOrder->getDriverId() != NO_VOLUNTEER) {
        cout << "Driver: " <<theOrder->getDriverId() << endl;
    }
}

PrintOrderStatus* PrintOrderStatus::clone() const {
    return new PrintOrderStatus(*this);
}



/// Stopped here::



// WHY ???
string PrintOrderStatus::toString() const {

}

PrintCustomerStatus::PrintCustomerStatus(int _customerId) : customerId(_customerId) 
{}

void PrintCustomerStatus::act(WareHouse &wareHouse) {
    Customer* customer = wareHouse.getCustomers()[customerId];
    cout << "CustomerID: " << customerId << endl;
    for(int orderId : customer->getOrdersIds()) {
        Order* order = wareHouse.getAllOrders()[orderId];
        cout << "OrderID: " << order->getId() << endl;
        cout << "OrderStatus: " << order->orderStatusToString(order->getStatus()) << endl;
    }
}

PrintCustomerStatus* PrintCustomerStatus::clone() const {
    return new PrintCustomerStatus(*this);
}

// WHY ???
string PrintCustomerStatus::toString() const {

}

PrintVolunteerStatus::PrintVolunteerStatus(int _id) : VolunteerId(_id) 
{}

void PrintVolunteerStatus::act(WareHouse &wareHouse) {
    Volunteer* volunteer = wareHouse.getVolunteers()[VolunteerId];
    cout << "VolunteerID: " << VolunteerId << endl;
    cout << "isBusy: " << volunteer->isBusy();
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
            cout << "TimeLeft: " << limitedCollectorVolunteer->getTimeLeft();
        else cout << "TimeLeft: None" << endl;
        cout << "OrdersLeft: " << limitedCollectorVolunteer->getNumOrdersLeft();
    }else if (volunteerType == "DriverVolunteer"){
        DriverVolunteer* driverVolunteer = dynamic_cast<DriverVolunteer*>(volunteer);
        if (volunteer->isBusy())
            cout << "TimeLeft: " << driverVolunteer->getDistanceLeft();
        else cout << "TimeLeft: None" << endl;
        cout << "OrdersLeft: No Limit" << endl;
    }else if (volunteerType == "LimitedDriverVolunteer"){
        LimitedDriverVolunteer* limitedDriverVolunteer = dynamic_cast<LimitedDriverVolunteer*>(volunteer);
        if (volunteer->isBusy())
            cout << "TimeLeft: " << limitedDriverVolunteer->getDistanceLeft();
        cout << "TimeLeft: None" << endl;
        cout << "OrdersLeft: " << limitedDriverVolunteer->getNumOrdersLeft();
    }
}

PrintVolunteerStatus* PrintVolunteerStatus::clone() const {
    return new PrintVolunteerStatus(*this);
}
// ???
string PrintVolunteerStatus::toString() const {

}

PrintActionsLog::PrintActionsLog() {
}

void PrintActionsLog::act(WareHouse &wareHouse) {
    for (BaseAction* action : wareHouse.getActions()){
        action->toString();
    }
}
PrintActionsLog* PrintActionsLog::clone() const {
    return new PrintActionsLog(*this);
}
string PrintActionsLog::toString() const {
    
}


