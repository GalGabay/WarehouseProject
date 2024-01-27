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


// Function to convert OrderStatus to string
string Order::orderStatusToString(OrderStatus status) const {
    switch (status) {
        case OrderStatus::PENDING:
            return "Pending";
        case OrderStatus::COLLECTING:
            return "Collecting";
        case OrderStatus::DELIVERING:
            return "Delivering";
        case OrderStatus::COMPLETED:
            return "Completed";
        default:
            return "Unknown";
    }
}

const string Order::toString() const {
   string output = "OrderId: " + std::to_string(id) + "\nOrderStatus: " + orderStatusToString(getStatus());
   return output; 
}

const int Order::getDistance() const {
    return distance;
}
