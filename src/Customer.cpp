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
    return numOrders;
}

bool Customer::canMakeOrder() const {
    if(maxOrders-getNumOrders() > 0) {
        return true;
    } else {
        return false;
    }
}


const vector<int>& Customer::getOrdersIds() const {
    return ordersId;
}

int Customer::addOrder(int orderId) {
    ordersId.push_back(orderId);
    numOrders++;
    return orderId; 
}
// &name ???
SoldierCustomer::SoldierCustomer(int id, string name, int locationDistance, int maxOrders) :
    Customer(id,name,locationDistance,maxOrders) {}

SoldierCustomer* SoldierCustomer::clone() const {
    return new SoldierCustomer(*this);
}

CivilianCustomer::CivilianCustomer(int id, string name, int locationDistance, int maxOrders) :
    Customer(id,name,locationDistance,maxOrders) {}

CivilianCustomer* CivilianCustomer::clone() const {
    return new CivilianCustomer(*this);
}





