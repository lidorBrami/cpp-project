#include "Customer.h"
#include "Order.h"
#include <string>
#include <vector>
using std::string;
using std::vector;

        Customer::Customer(int _id , const string & _name, int _locationDistance, int _maxOrders): id(_id),name(_name),locationDistance(_locationDistance),maxOrders(_maxOrders),ordersId(vector<int>()),ordersLeft(maxOrders){};
        const string &Customer::getName() const{
            return name;
        }
        int Customer::getId() const{
            return id;
        }
        int Customer:: getOrdersLeft(){
            return ordersLeft;
        }
        int Customer::getCustomerDistance() const{
            return locationDistance;
        }
        int Customer::getMaxOrders() const{
            return maxOrders;
        }
        int Customer::getNumOrders() const{
            return ordersLeft;
        }
        bool Customer::canMakeOrder()const{
            return !(getNumOrders() == 0);
        }
        const vector<int> &Customer::getOrdersIds() const{
            return  ordersId;
        }
        //return OrderId if order was added successfully, -1 otherwise
        
        int Customer::addOrder(int orderId){
            if(canMakeOrder()){
                ordersId.push_back(orderId);
                ordersLeft--;
                return orderId;
            }
            return -1;        
        }
        Customer::~Customer(){}
        
        SoldierCustomer::SoldierCustomer(int id, const string & name, int locationDistance, int maxOrders): Customer::Customer(id,name,locationDistance,maxOrders),type("Soldier") {};

        SoldierCustomer *SoldierCustomer::clone() const{
             return new SoldierCustomer(*this);
        };
        SoldierCustomer::~SoldierCustomer(){}


CivilianCustomer::CivilianCustomer(int id,const string& name, int locationDistance, int maxOrders): Customer::Customer(id,name,locationDistance,maxOrders),type("Civilian"){};
       
        CivilianCustomer *CivilianCustomer::clone() const {
            return new CivilianCustomer(*this);
        }
        CivilianCustomer::~CivilianCustomer(){}