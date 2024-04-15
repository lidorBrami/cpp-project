#pragma once
#include <string>
#include <vector>
using namespace std;

#include "Order.h"
#include "Customer.h"

class BaseAction;
class Volunteer;

// Warehouse responsible for Volunteers, Customers Actions, and Orders.


class WareHouse {

    public:
        WareHouse(const string &configFilePath);
        void start();
        vector<BaseAction*> &getActionsLog() ;
        void addOrder(Order* order);
        void addAction(BaseAction* action);
        Customer &getCustomer(int customerId) const;
        Volunteer &getVolunteer(int volunteerId) const;
        Order &getOrder(int orderId) const;
        void close();
        void open();
        vector<Order*> &getPendingOrder();
        vector<Order*> &getInProcessOrders();
        vector<Volunteer*> &getVolunteerVector();
        vector<Order*> &getCompletedOrders();
        vector<Customer*> &getCustomers();
        int getIdOrderCounter()const;
        void setIdOrderCounter(int orderCounter);
        int getCustomerCounter()const;
        void setCustomerCounter(int customerCounter);
        int getVolunteerCounter()const;
        WareHouse(const WareHouse & other);
        ~WareHouse();
        void addCustomer(Customer *c);
        void removeFromPending(Order * order);
        void removeFromProcess(Order *o);
        void addcompletedVector(Order *o);
        void addInProssesVector(Order *o);
        void removeFromVolunteers(Volunteer * v);
        WareHouse& operator=(const WareHouse & other);
        WareHouse(WareHouse && other);
        WareHouse & operator = (WareHouse&& other);
        void Action(string s);
        const vector<BaseAction*> &getActions() const;
        void addVolunteer(Volunteer * vol);

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
        int idOrderCounter;
};