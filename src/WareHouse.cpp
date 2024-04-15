#include <iostream>
#include <fstream> 
#include <string>
#include <vector>
#include "../include/Order.h"
#include "Customer.h"
#include <sstream>
#include "WareHouse.h"  // Include your header files as needed
#include "Volunteer.h"
#include "Action.h"
#include <algorithm> 
#include "../include/Parser.h"
using namespace std;
        
        int WareHouse::getCustomerCounter()const{
            return customerCounter;
        }
        void WareHouse::setCustomerCounter(int customerCounter){
            this->customerCounter = this->customerCounter + customerCounter;
        }

       WareHouse::WareHouse(const string &configFilePath): isOpen(false),actionsLog(vector<BaseAction*>()),volunteers(vector<Volunteer*>()), pendingOrders(vector<Order*>()),inProcessOrders(vector<Order*>()),completedOrders(vector<Order*>()),customers(vector<Customer*>()), customerCounter(0),volunteerCounter(0),idOrderCounter(0) {
            Parser::parseConfigurationFile(configFilePath,*this);
        };
        void WareHouse::start(){
            open();
            string s;
            while(isOpen){
                std::cout<<"Enter an action"<<endl;
                getline(cin,s);
                if(!s.empty()){
                    Action(s);
                }    
            }
        }
        WareHouse::WareHouse(const WareHouse &other): isOpen(other.isOpen),actionsLog(vector<BaseAction*>()),volunteers(vector<Volunteer*>()), pendingOrders(vector<Order*>()),inProcessOrders(vector<Order*>()),completedOrders(vector<Order*>()),customers(vector<Customer*>()), customerCounter(other.customerCounter),volunteerCounter(other.volunteerCounter), idOrderCounter(other.idOrderCounter) {
           for(BaseAction* b:other.actionsLog){
                actionsLog.push_back(b->clone());
           }
           for(Volunteer* v : other.volunteers){
                volunteers.push_back(v->clone());
           }
           for(Order* o: other.pendingOrders){
                pendingOrders.push_back(new Order(*o));
           }
           for(Order* o: other.inProcessOrders){
                inProcessOrders.push_back(new Order(*o));
           }
           for(Order* o: other.completedOrders){
                completedOrders.push_back(new Order(*o));
           }
           for(Customer* c: other.customers){
                customers.push_back(c->clone());
           }
        }

        WareHouse& WareHouse::operator=(const WareHouse & other){
            if(this != &other){
                isOpen = other.isOpen;
                customerCounter = other.customerCounter;
                volunteerCounter = other.volunteerCounter;
                idOrderCounter  = other.idOrderCounter;
                for(BaseAction* log: actionsLog){
                    delete log;
                }
                for(Volunteer* vol: volunteers){
                    delete vol;
                }
                for(Order* pen: pendingOrders){
                    delete pen;
                }
                for(Order* process: inProcessOrders){
                    delete process;
                }
                for(Order* complete: completedOrders){
                    delete complete;
                }
                for(Customer* c: customers){
                    delete c;
                }
                actionsLog.clear();
                volunteers.clear();
                pendingOrders.clear();
                inProcessOrders.clear();
                completedOrders.clear();
                customers.clear();
                for(BaseAction* b:other.actionsLog){
                    actionsLog.push_back(b->clone());
                }
                for(Volunteer* v : other.volunteers){
                        volunteers.push_back(v->clone());
                }
                for(Order* o: other.pendingOrders){
                        pendingOrders.push_back(new Order(*o));
                }
                for(Order* o: other.inProcessOrders){
                        inProcessOrders.push_back(new Order(*o));
                }
                for(Order* o: other.completedOrders){
                        completedOrders.push_back(new Order(*o));
                }
                for(Customer* c: other.customers){
                        customers.push_back(c->clone());
                }
            }
            return *this;
        }
        WareHouse::~WareHouse(){
            for(BaseAction* log: actionsLog){
                delete log;
            }
            for(Volunteer* vol: volunteers){
                delete vol;
            }
            for(Order* pen: pendingOrders){
                delete pen;
            }
            for(Order* process: inProcessOrders){
                delete process;
            }
            for(Order* complete: completedOrders){
                delete complete;
            }
            for(Customer* c: customers){
                delete c;
            }
        }
        WareHouse::WareHouse(WareHouse && other): isOpen(other.isOpen),actionsLog(vector<BaseAction*>()),volunteers(vector<Volunteer*>()), pendingOrders(vector<Order*>()),inProcessOrders(vector<Order*>()),completedOrders(vector<Order*>()),customers(vector<Customer*>()), customerCounter(other.customerCounter), volunteerCounter(other.volunteerCounter), idOrderCounter(other.idOrderCounter){
                for(BaseAction* b:other.actionsLog){
                    actionsLog.push_back(b);
                    b=nullptr;
                }
                for(Volunteer* v : other.volunteers){
                        volunteers.push_back(v);
                        v = nullptr;

                }
                for(Order* o: other.pendingOrders){
                        pendingOrders.push_back(o);
                        o = nullptr;
                }
                for(Order* o: other.inProcessOrders){
                        inProcessOrders.push_back(o);
                        o = nullptr;
                }
                for(Order* o: other.completedOrders){
                        completedOrders.push_back(o);
                        o = nullptr;
                }
                for(Customer* c: other.customers){
                        customers.push_back(c);
                        c = nullptr;
                }
        }
        WareHouse & WareHouse:: operator = (WareHouse&& other){
            if(this != &other){
                isOpen = other.isOpen;
                customerCounter = other.customerCounter;
                volunteerCounter = other.volunteerCounter;
                idOrderCounter  = other.idOrderCounter;
                for(BaseAction* log: actionsLog){
                    delete log;
                }
                for(Volunteer* vol: volunteers){
                    delete vol;
                }
                for(Order* pen: pendingOrders){
                    delete pen;
                }
                for(Order* process: inProcessOrders){
                    delete process;
                }
                for(Order* complete: completedOrders){
                    delete complete;
                }
                for(Customer* c: customers){
                    delete c;
                }
                actionsLog.clear();
                volunteers.clear();
                pendingOrders.clear();
                inProcessOrders.clear();
                completedOrders.clear();
                customers.clear();
                for(BaseAction* b:other.actionsLog){
                    actionsLog.push_back(b);
                    b=nullptr;
                }
                for(Volunteer* v : other.volunteers){
                        volunteers.push_back(v);
                        v=nullptr;
                }
                for(Order* o: other.pendingOrders){
                        pendingOrders.push_back(o);
                         o = nullptr;

                }
                for(Order* o: other.inProcessOrders){
                        inProcessOrders.push_back(o);
                        o = nullptr;
                }
                for(Order* o: other.completedOrders){
                        completedOrders.push_back(o);
                        o = nullptr;
                }
                for(Customer* c: other.customers){
                        customers.push_back(c);
                        c = nullptr;
                }
            }
            return *this;
        }
    
        void WareHouse::Action(string s){
            istringstream iss(s);
            string entityType;
            iss >> entityType;
            if(entityType == "step"){
                int numOfSteps;
                iss>>numOfSteps;
                SimulateStep *step = new SimulateStep(numOfSteps);
                step->act(*this);
            }
            else if (entityType == "order")
            {
               int customerId;
               iss>>customerId;
               AddOrder *a = new AddOrder(customerId);
               a->act(*this);
            }
            else if(entityType == "customer"){
                string name;
                iss>>name;
                CustomerType type;
                string t;
                iss>>t;
                if(t=="Soldier"){
                    type = CustomerType::Soldier;
                }
                else{
                    type = CustomerType::Civilian;
                }
                int distance;
                iss>>distance;
                int maxOrders;
                iss>>maxOrders;
                AddCustomer *c = new AddCustomer(name,type,distance,maxOrders);
                c->act(*this);
            }
            else if(entityType == "orderStatus"){
                int id;
                iss>>id;
                PrintOrderStatus *p = new PrintOrderStatus(id);
                p->act(*this);     
            }
            else if(entityType == "customerStatus"){
                int id;
                iss>>id;
                PrintCustomerStatus *p = new PrintCustomerStatus(id);
                p->act(*this);
            }
            else if(entityType == "volunteerStatus"){
                int id;
                iss>>id;
                PrintVolunteerStatus *print = new PrintVolunteerStatus(id);
                print->act(*this);
            }
            else if(entityType == "log"){
                PrintActionsLog *log = new PrintActionsLog();
                log->act(*this);
            }
            else if (entityType == "close"){
                Close *close = new Close();
                close->act(*this);
            }
            else if(entityType == "backup"){
                BackupWareHouse *b = new BackupWareHouse();
                b->act(*this);
            }
            else if(entityType == "restore"){
                RestoreWareHouse *r = new RestoreWareHouse();
                r->act(*this);
            }
        }
        void WareHouse::addVolunteer(Volunteer * vol){
            volunteers.push_back(vol);
            volunteerCounter++;
        }
        vector<BaseAction*> & WareHouse::getActionsLog() {
            return actionsLog;
        }
        vector<Order*> &WareHouse::getPendingOrder() {
            return pendingOrders;
        }
        vector<Volunteer*> &WareHouse::getVolunteerVector() {
            return volunteers;
        }
        vector<Order*> &WareHouse::getInProcessOrders() {
            return inProcessOrders;
        }

        vector<Order*> &WareHouse::getCompletedOrders() {
            return completedOrders;
        }
        vector<Customer*> &WareHouse::getCustomers(){
            return customers;
        }
        void WareHouse::setIdOrderCounter(int orderCounter){
            this->idOrderCounter = this->idOrderCounter + orderCounter;
        }
        int WareHouse::getIdOrderCounter()const{
            return idOrderCounter;
        }
        int WareHouse::getVolunteerCounter()const{
            return volunteerCounter;
        }

        const vector<BaseAction*> &WareHouse::getActions() const{
            return actionsLog;
        }
        void WareHouse::addOrder(Order* order){
            pendingOrders.push_back(order);
        }
        void WareHouse::addInProssesVector(Order *o){
            inProcessOrders.push_back(o);
        }
        void WareHouse::addcompletedVector(Order *o){
            completedOrders.push_back(o);
        }
        void WareHouse::addAction(BaseAction* action){
            actionsLog.push_back(action);
        }
        Customer &WareHouse::getCustomer(int customerId) const{
            for(Customer *c : customers){
                if (c->getId() == customerId){
                    return *c;
                }
            }
            static SoldierCustomer c(-1,"error",-1,-1);
            return  c;
        }
        Volunteer &WareHouse::getVolunteer(int volunteerId) const{
            for(Volunteer* v : volunteers){
                if (v->getId() == volunteerId){
                    return *v;
                }
            }
            static CollectorVolunteer v(-1,"error",-1);
            return v;
        }
        Order &WareHouse::getOrder(int orderId) const{
            Order *order;
            for(Order* o : pendingOrders){
                if (o->getId() == orderId){
                    order = o;
                }
            }
            for(Order* o : inProcessOrders){
                if (o->getId() == orderId){
                    order = o;
                }
            }
            for(Order* o : completedOrders){
                if (o->getId() == orderId){
                    order = o;
                }
            }
            return *order;
        }
        void WareHouse::addCustomer(Customer *c){
            customers.push_back(c);
            customerCounter++;
        }
        void WareHouse::removeFromPending(Order * order)
        {
            auto it = std::find(pendingOrders.begin(), pendingOrders.end(), order);
            if (it != pendingOrders.end())
            {
                pendingOrders.erase(it);
                }

        }  
        
                         

        void WareHouse::removeFromProcess(Order *o) {
            auto it = std::find(inProcessOrders.begin(), inProcessOrders.end(), o);
            if (it != inProcessOrders.end())
                {
                    inProcessOrders.erase(it);
                }
        } 
        void WareHouse::removeFromVolunteers(Volunteer * v){
            auto it = std::find(volunteers.begin(),volunteers.end(), v);
            if (it != volunteers.end())
                {
                    volunteers.erase(it);
                }
        }
        
        void WareHouse::close(){
            isOpen = false ;
        }
        void WareHouse::open(){
            isOpen = true;
            std::cout<<"Warehouse is open!"<<endl;
        } 