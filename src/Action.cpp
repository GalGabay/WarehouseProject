#include "Action.h"

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
    stage1(wareHouse);
    stage2(wareHouse);
    
 }

// not finished!!
void SimulateStep::stage1(WareHouse &wareHouse) {
    for(Order* order : wareHouse.getPendingOrders()) {
        if(order->getStatus() == OrderStatus::PENDING) {

            // how do i know which type each volunteer is - NEED TO BE CHANGED
            for(Volunteer* volunteer : wareHouse.getVolunteers()) {
                if(volunteer->canTakeOrder(*order)) {
                    volunteer->acceptOrder(*order);
                    // ???? how to erase order from pendingOrders ??

                    // add to its suitable vector list
                    order->setStatus(OrderStatus::COLLECTING);
                    wareHouse.addOrder(order);
                    
                }
            }
        } else if(order->getStatus() == OrderStatus::COLLECTING) {

            // how do i know which type each volunteer is - NEED TO BE CHANGED
            for(Volunteer* volunteer : wareHouse.getVolunteers()) {
                if(volunteer->canTakeOrder(*order)) {
                    volunteer->acceptOrder(*order);
                    // ???? how to erase order from pendingOrders ??

                    // add to its suitable vector list
                    order->setStatus(OrderStatus::DELIVERING);
                    wareHouse.addOrder(order);
                    
                }
            }
        }
    } 
  }

void SimulateStep::stage2(WareHouse &wareHouse) { 
    for(Order* order : wareHouse.getProcessOrders()) {
        if(order->getStatus() ==OrderStatus::COLLECTING) {

            // why &warehouse and not warehouse
            CollectorVolunteer* collector = dynamic_cast<CollectorVolunteer*>(&wareHouse.getVolunteer(order->getCollectorId()));
            (*collector).setTimeLeft((*collector).getTimeLeft() - 1);
        } else if(order->getStatus() ==OrderStatus::DELIVERING) {

            // why &warehouse and not warehouse
            DriverVolunteer* driver = dynamic_cast<DriverVolunteer*>(&wareHouse.getVolunteer(order->getDriverId()));
            (*driver).setDistanceLeft((*driver).getDistanceLeft() - (*driver).getDistancePerStep());
        }
    }
  }
