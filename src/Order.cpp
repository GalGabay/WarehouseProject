#include "Order.h";
#include <iostream>
using namespace std;

Order::Order(int _id, int _customerId, int _distance) : 
    id(_id), customerId(_customerId), distance(_distance), 
    driverId(NO_VOLUNTEER), collectorId(NO_VOLUNTEER),
    status(OrderStatus::PENDING)
    {}


int Order::getId() const {
    return id;
}

int Order::getCustomerId() const {
    return customerId;
}


void Order::setStatus(OrderStatus _status) {
    status = _status;
}
void Order::setCollectorId(int _collectorId) {
    collectorId = _collectorId;
}
void Order::setDriverId(int _driverId) {
    driverId = _driverId;
    
}
int Order::getCollectorId() const {
    return collectorId;
}
int Order::getDriverId() const {
    return driverId;
}
OrderStatus Order::getStatus() const {
    return status;
}

 //not finished
//const string Order::toString() const {
 //   cout << "Order: " << id << "is: " << status << endl
//}

const int Order::getDistance() const {
    return distance;
}