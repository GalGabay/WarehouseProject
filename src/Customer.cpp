#include "Customer.h"
#include <string>
#include <vector>
using std::string;
using std::vector;


Customer::Customer(int _id, const string &_name, int _locationDistance, int _maxOrders) : 
id(_id), name(_name), locationDistance(_locationDistance), maxOrders(_maxOrders)
{}

const string& Customer::getName() const {
    return name;
}

int Customer::getId() const {
    return id;
}

int Customer::getCustomerDistance() const {
    return locationDistance;
}

int Customer::getMaxOrders() const {
    return maxOrders;
}

int Customer::getNumOrders() const {

}

bool Customer::canMakeOrder() const {
    if(maxOrders-getNumOrders() > 0) {
        return true;
    } else {
        return false;
    }
}

//??
const vector<int>& Customer::getOrdersIds() const {

}

// ??
int Customer::addOrder(int orderId) {
    ordersId.push_back(orderId);
    return orderId; 
}






