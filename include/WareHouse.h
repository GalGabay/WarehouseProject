#pragma once
#include <string>
#include <vector>

//#include "Action.h"
#include "Order.h"
#include "Customer.h"
#include "Volunteer.h"

using namespace std;

class BaseAction;
class Volunteer;

// Warehouse responsible for Volunteers, Customers Actions, and Orders.


class WareHouse {

    public:
        WareHouse(const string &configFilePath);
        void start();
        void addOrder(Order* order);
        void addAction(BaseAction* action);
        Customer &getCustomer(int customerId) const;
        Volunteer &getVolunteer(int volunteerId) const;
        Order &getOrder(int orderId) const;
        const vector<BaseAction*> &getActions() const;
        void close();
        void open();

        ~WareHouse();
        WareHouse(WareHouse& other);
        void operator=(const WareHouse& other);
        template <typename T> void deleteVector(vector<T*>& toDeleteVector);
        void parseText(const string &configFilePath);
        vector<Order*>& getPendingOrders();
        vector<Order*>& getProcessOrders();
        vector<Order*>& getCompletedOrders();
        //vector<Order*>& getAllOrders();
        vector<Volunteer*>& getVolunteers();
        vector<Customer*>& getCustomers();
        void AddOrderCounter();
        int getOrderCounter();
        void addCustomerCounter();
        int getCustomerCounter();
        int getVolunteerCounter();
        void CreateCustomer(string secondWord, string thirdWord, string fourthWord, string fifthWord);

    private:
        bool isOpen;
        vector<BaseAction*> actionsLog;
        vector<Volunteer*> volunteers;
        vector<Order*> pendingOrders;
        vector<Order*> inProcessOrders;
        vector<Order*> completedOrders;
        vector<Customer*> customers;
        int customerCounter; //For assigning unique customer IDs
        int volunteerCounter; //For assigning unique volunteer IDs

        int orderCounter; // still need to use this
        //vector<Order*> allOrders;
        CollectorVolunteer* defaultVolunteer;
        CivilianCustomer* defaultCustomer;
    
};