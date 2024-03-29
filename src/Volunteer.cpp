#include "../include/Volunteer.h"
#include <iostream>


// ## VOLUNTEER ##

Volunteer::Volunteer(int _id, const string &_name) :
    id(_id), name(_name) {
    activeOrderId = NO_ORDER;
    completedOrderId = NO_ORDER;
}

int Volunteer::getId() const {
    return id;
}

const string& Volunteer::getName() const {
    return name;
}

 int Volunteer::getActiveOrderId() const {
    return activeOrderId;
 }

 int Volunteer::getCompletedOrderId() const {
    return completedOrderId;
 }

 bool Volunteer::isBusy() const {
    return activeOrderId != NO_ORDER;
 }

 void Volunteer::setCompletedOrderId(int completeOrder) {
    completedOrderId = completeOrder;
 }
  void Volunteer::setActiveOrderId(int activeOrder) {
    activeOrderId = activeOrder;
 }


// ## COLLECTOR VOLUNTEER ##
 
 CollectorVolunteer::CollectorVolunteer(int _id, string _name, int _coolDown) : 
  Volunteer::Volunteer(_id,_name), coolDown(_coolDown)
   {
    timeLeft = NO_ORDER; 
    typeVolunteer = "CollectorVolunteer";
   }

CollectorVolunteer* CollectorVolunteer::clone() const {
    return new CollectorVolunteer(*this);
}


void CollectorVolunteer::step() {
    if(activeOrderId != NO_ORDER) {
        if(decreaseCoolDown()) {
            completedOrderId = activeOrderId;
            activeOrderId = NO_ORDER; 
        }
    } 
}

int CollectorVolunteer::getCoolDown() const {
    return coolDown;
}

int CollectorVolunteer::getTimeLeft() const {
    return timeLeft;
}

bool CollectorVolunteer::decreaseCoolDown() {
    timeLeft--;
    return timeLeft == 0;
}

bool CollectorVolunteer::hasOrdersLeft() const {
    return true;
}

bool CollectorVolunteer::canTakeOrder(const Order &order) const {
    return !isBusy() && order.getStatus() == OrderStatus::PENDING;
}


void CollectorVolunteer::acceptOrder(const Order &order) {
    activeOrderId = order.getId();
    timeLeft = coolDown;
}


string CollectorVolunteer::toString() const {
    string output = "Collector: " + this->getId();
    return output;
}

void CollectorVolunteer::setTimeLeft(int _timeLeft) {
    timeLeft = _timeLeft;
}

string CollectorVolunteer::getTypeVolunteer() {
    return typeVolunteer;
}

// ## LIMITED COLLECTOR VOLUNTEER ##

LimitedCollectorVolunteer::LimitedCollectorVolunteer(int _id, string _name, int _coolDown ,int _maxOrders) :
CollectorVolunteer(_id,_name,_coolDown), maxOrders(_maxOrders), ordersLeft(maxOrders)
{
    typeVolunteer = "LimitedCollectorVolunteer";
}

LimitedCollectorVolunteer* LimitedCollectorVolunteer::clone() const {
    return new LimitedCollectorVolunteer(*this);
}

bool LimitedCollectorVolunteer::hasOrdersLeft() const {
    return ordersLeft > 0;
}

bool LimitedCollectorVolunteer::canTakeOrder(const Order &order) const {
    return !isBusy() && hasOrdersLeft() && order.getStatus() == OrderStatus::PENDING;
}


void LimitedCollectorVolunteer::acceptOrder(const Order &order) {
    activeOrderId = order.getId();
    setTimeLeft(getCoolDown());
    ordersLeft--;
}

int LimitedCollectorVolunteer::getMaxOrders() const {
    return maxOrders;
}

int LimitedCollectorVolunteer::getNumOrdersLeft() const {
    return ordersLeft;
}


string LimitedCollectorVolunteer::toString() const {
    string output = "LimitedCollector: " + this->getId();
    return output;
}

string LimitedCollectorVolunteer::getTypeVolunteer() {
    return typeVolunteer;
}

// ## DRIVER VOLUNTEER ##

DriverVolunteer::DriverVolunteer(int _id, string _name, int _maxDistance, int _distancePerStep) :
Volunteer(_id,_name), maxDistance(_maxDistance), distancePerStep(_distancePerStep)
{
    distanceLeft = 0;
    typeVolunteer = "DriverVolunteer";
}

DriverVolunteer* DriverVolunteer::clone() const  {
    return new DriverVolunteer(*this);
}

int DriverVolunteer::getDistanceLeft() const {
    return distanceLeft;
}
 
 int DriverVolunteer::getMaxDistance() const {
    return maxDistance;
 }
 int DriverVolunteer::getDistancePerStep() const {
    return distancePerStep;
 }  

 bool DriverVolunteer::decreaseDistanceLeft() {
    distanceLeft -= distancePerStep;
    if(distanceLeft < 0) 
        distanceLeft = 0;
    return distanceLeft == 0;
 }

 bool DriverVolunteer::hasOrdersLeft() const {
    return true;
 }

 bool DriverVolunteer::canTakeOrder(const Order &order) const {
    int distance = order.getDistance();
    return (maxDistance >= distance && !isBusy());
 }

 void DriverVolunteer::acceptOrder(const Order &order) {
    activeOrderId = order.getId();
    distanceLeft = order.getDistance();
 }

 void DriverVolunteer::step() {
    decreaseDistanceLeft();
 }


 string DriverVolunteer::toString() const {
    string output = "Driver: " + this->getId();
    return output;
 }

  void DriverVolunteer::setDistanceLeft(int newDistance) {
    distanceLeft = newDistance;
  }

string DriverVolunteer::getTypeVolunteer() {
    return typeVolunteer;
}

 // ## LIMITED DRIVER VOLUNTEER ##

LimitedDriverVolunteer::LimitedDriverVolunteer(int _id, const string &_name, int _maxDistance, int _distancePerStep,int _maxOrders) :
DriverVolunteer(_id,_name,_maxDistance,_distancePerStep), maxOrders(_maxOrders), ordersLeft(_maxOrders)
{
    typeVolunteer = "LimitedDriverVolunteer";
}

LimitedDriverVolunteer* LimitedDriverVolunteer::clone() const {
    return new LimitedDriverVolunteer(*this);
}

int LimitedDriverVolunteer::getMaxOrders() const {
    return maxOrders;
}

 int LimitedDriverVolunteer::getNumOrdersLeft() const {
    return ordersLeft;
 }

bool LimitedDriverVolunteer::hasOrdersLeft() const {
    return ordersLeft > 0;
}

bool LimitedDriverVolunteer::canTakeOrder(const Order &order) const {
    int distance = order.getDistance();
    return (getMaxDistance() >= distance && !isBusy() && hasOrdersLeft());
}


 void LimitedDriverVolunteer::acceptOrder(const Order &order) {
    activeOrderId = order.getId();
    setDistanceLeft(order.getDistance());
    ordersLeft--;
 }

 
 string LimitedDriverVolunteer::toString() const {
    string output = "LimitedDriver: " + this->getId();
    return output;
 }

string LimitedDriverVolunteer::getTypeVolunteer() {
    return typeVolunteer;
}
