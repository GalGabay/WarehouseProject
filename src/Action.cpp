#include "Action.h"
#include <algorithm>
#include <vector>
#include <iostream>

// ???
BaseAction::BaseAction() {
    
}

 ActionStatus BaseAction::getStatus() const {
    return status;
 }

 void BaseAction::complete() {
    status = ActionStatus::COMPLETED;
 }

 void BaseAction::error(string errorMsg) {
    status = ActionStatus::ERROR;
    cout << "Error: " << errorMsg << endl;
 }

 string BaseAction::getErrorMsg() const {
    return errorMsg;
 }

// ## SIMULATE STEP ##


 SimulateStep::SimulateStep(int _numOfSteps) : numOfSteps(_numOfSteps) 
 {}

 void SimulateStep::act(WareHouse &wareHouse) {
    int steps = numOfSteps;
    while(steps > 0) {
        stage1(wareHouse);
        stage23(wareHouse);
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
                    // ???? how to erase order from pendingOrders ??
                        vector<Order*> pendingOrd = wareHouse.getPendingOrders();

                        // need to check if it's working
                        pendingOrd.erase(std::remove_if(pendingOrd.begin(), pendingOrd.end(),
                                [order](Order* o) { return o == order; }),pendingOrd.end());
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
                        // ???? how to erase order from pendingOrders ??
                        vector<Order*> pendingOrd = wareHouse.getPendingOrders();

                        // need to check if it's working
                        pendingOrd.erase(std::remove_if(pendingOrd.begin(), pendingOrd.end(),
                                [order](Order* o) { return o == order; }),pendingOrd.end());
                        // add to its suitable vector list
                        order->setStatus(OrderStatus::DELIVERING);
                        wareHouse.addOrder(order);
                    
                    }
                }
            }
        }
    } 
  }

void SimulateStep::stage23(WareHouse &wareHouse) { 
    for(Order* order : wareHouse.getProcessOrders()) {
        if(order->getStatus() ==OrderStatus::COLLECTING) {

            // why &warehouse and not warehouse
            CollectorVolunteer* collector = dynamic_cast<CollectorVolunteer*>(&wareHouse.getVolunteer(order->getCollectorId()));
            (*collector).setTimeLeft((*collector).getTimeLeft() - 1);
            //stage 3
            if(collector->getTimeLeft() == 0) {
                collector->setActiveOrderId(NO_ORDER);
                collector->setCompletedOrderId(order->getId());
                // ???? how to erase order from inProcessOrders ??
                vector<Order*> processOrd = wareHouse.getProcessOrders();
                        // need to check if it's working
                processOrd.erase(std::remove_if(processOrd.begin(), processOrd.end(),
                                [order](Order* o) { return o == order; }),processOrd.end());
                wareHouse.addOrder(order); // NOTWORKING add the order to pendingOrders vector

                //stage 4
                if(collector->getTypeVolunteer() == "LimitedCollectorVolunteer") {
                    LimitedCollectorVolunteer* limitedcollector = dynamic_cast<LimitedCollectorVolunteer*>(collector);
                    if(limitedcollector->getNumOrdersLeft() == 0) {
                        vector<Volunteer*> volunteers = wareHouse.getVolunteers();
                        // need to check if it's working
                        //erasing from volunteers ?
                        volunteers.erase(std::remove_if(volunteers.begin(), volunteers.end(),
                                [limitedcollector](Volunteer* v) { return v == limitedcollector; }),volunteers.end());

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
                // ???? how to erase order from inProcessOrders ??
                vector<Order*> processOrd = wareHouse.getProcessOrders();
                        // need to check if it's working
                processOrd.erase(std::remove_if(processOrd.begin(), processOrd.end(),
                                [order](Order* o) { return o == order; }),processOrd.end());
                wareHouse.addOrder(order); // NOTWORKING add the order to completedOrders vector

                //stage 4
                if(driver->getTypeVolunteer() == "LimitedDriverVolunteer") {
                    LimitedDriverVolunteer* limiteddriver = dynamic_cast<LimitedDriverVolunteer*>(driver);
                    if(limiteddriver->getMaxOrders() == 0) {
                        vector<Volunteer*> volunteers = wareHouse.getVolunteers();
                        // need to check if it's working
                        //erasing from volunteers ?
                        volunteers.erase(std::remove_if(volunteers.begin(), volunteers.end(),
                                [limiteddriver](Volunteer* v) { return v == limiteddriver; } ),volunteers.end());

                        delete limiteddriver;
                        limiteddriver = nullptr;

                    }
                }
            }
        }
    }
  }

AddOrder::AddOrder(int id) : customerId(id) {

}

void AddOrder::act(WareHouse &wareHouse) {
    
}