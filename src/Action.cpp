#include <string>
#include <vector>
#include "WareHouse.h"
#include "Volunteer.h"
#include "Order.h"
#include "Customer.h"
#include "Action.h"
#include <iostream>
#include <fstream>
#include <algorithm> 
#include <iostream>
#include <sstream>
using std::string;
using std::vector;
        BaseAction::BaseAction():errorMsg(""), status(ActionStatus()) {};
        ActionStatus BaseAction::getStatus() const{
            return status;
        }

        void BaseAction::complete(){
            status = ActionStatus::COMPLETED;
        }
        void BaseAction::error(string errorMsg){
            status = ActionStatus::ERROR;
            this->errorMsg = errorMsg;
            cout<<"Error: "+errorMsg<<endl;
        }
        string BaseAction::getErrorMsg() const{
            return errorMsg;
        }
        void BaseAction::setActionStatus(ActionStatus a){
            status = a;
        }
        const string BaseAction::getStringStatus() const {
            if (status == ActionStatus::COMPLETED) {
                return "COMPLETED ";
            } else if (status == ActionStatus::ERROR) {
                return "ERROR ";
            }

            // Default return if neither condition is true
            return "UNKNOWN_STATUS ";
        }
        BaseAction::~BaseAction(){}

        SimulateStep::SimulateStep(int numOfSteps): numOfSteps(numOfSteps){}
        void SimulateStep::act(WareHouse &wareHouse) {
            int i = numOfSteps;
            while(i>0){
                stage1(wareHouse);
                stage2(wareHouse);
                i = i-1;
            } 
            complete();
            wareHouse.addAction(this);
        }
        void SimulateStep::stage2(WareHouse &wareHouse){
            vector<Volunteer*> volunteers = wareHouse.getVolunteerVector();
            string s1 = "DriverVolunteer";
            string s2 = "LimitedDriverVolunteer";  
            string s3 = "CollectorVolunteer";
            string s4 = "LimitedCollectorVolunteer"; 
            for(Volunteer* v: volunteers){
                v->step();
                string type = v->getType();
                if(v->getActiveOrderId()==NO_ORDER&&v->getCompletedOrderId()!=NO_ORDER){
                        Order &o = wareHouse.getOrder(v->getCompletedOrderId());
                        if((type == s3 || type == s4) && o.getDriverId()==NO_VOLUNTEER&&o.getSign()==0){
                                wareHouse.removeFromProcess(&o); 
                                wareHouse.addOrder(&o);  
                                o.setSign(1); 
                                if(type==s4 ){
                                    v->setOrdersLeft();
                                }  
                        }
                        if((type == s2||type==s1) && o.getStatus()!=OrderStatus::COMPLETED){
                                o.setStatus(OrderStatus::COMPLETED);
                                wareHouse.removeFromProcess(&o);
                                wareHouse.addcompletedVector(&o);
                                o.setSign(1);
                                if(type==s2 ){
                                    v->setOrdersLeft();
                                }
                        }
                    }
                
                    if(!(v->hasOrdersLeft())){
                        cout<<"volunteer"+v->getName()+"  delete"<<endl;
                        wareHouse.removeFromVolunteers(v);
                        delete v;
                    }
        }        
    }
        

        void SimulateStep::stage1(WareHouse &wareHouse){
            vector<Order*> pendingOrders = wareHouse.getPendingOrder();
            vector<Volunteer*> volunteer = wareHouse.getVolunteerVector();
            for(Order *order:pendingOrders){
                string s1 = "CollectorVolunteer";
                string s2 = "LimitedCollectorVolunteer";
                string s3  = "DriverVolunteer";   
                string s4 = "LimitedDriverVolunteer";     
                for(Volunteer *v :  volunteer){
                    if(v->canTakeOrder(*order)){
                        if(v->getType()==s1 || v->getType()==s2){
                                order->setCollectorId(v->getId());
                                v->acceptOrder(*order);
                                OrderStatus status = OrderStatus::COLLECTING;
                                order->setStatus(status);
                                wareHouse.removeFromPending(order);
                                wareHouse.addInProssesVector(order);
                            }
                        else if(v->getType()==s3 || v->getType()==s4) {
                                order->setDriverId(v->getId());
                                v->acceptOrder(*order);
                                OrderStatus status = OrderStatus::DELIVERING;
                                order->setStatus(status);
                                wareHouse.removeFromPending(order);
                                wareHouse.addInProssesVector(order);
                            }
                            break; 
                        }   
                    }
                   
                }
            
        }
        std::string SimulateStep::toString() const {
            return "SimulateStep " + to_string(numOfSteps) +" "+ getStringStatus() ;
        };
        SimulateStep *SimulateStep::clone() const {
            return new SimulateStep(*this);
        }
        SimulateStep::~SimulateStep(){}


        AddOrder::AddOrder(int id): customerId(id){    
        };
        void AddOrder::act(WareHouse &wareHouse){
           vector<Order *> pending = wareHouse.getPendingOrder();
           Customer &c = wareHouse.getCustomer(customerId);
           if(c.getOrdersLeft() == 0 || c.getId()<0 || c.getId() > wareHouse.getCustomerCounter()){
                    error("Cannot place this order");  
                    wareHouse.addAction(this);
                    return;  
                }  
                Order * o  = new Order(wareHouse.getIdOrderCounter(),customerId,c.getCustomerDistance());
                c.addOrder(o->getId());
                o->setStatus(OrderStatus::PENDING);
                wareHouse.addOrder(o); 
                wareHouse.setIdOrderCounter(1);
                complete();
                wareHouse.addAction(this);
        }   
        string AddOrder::toString() const {
            return "Order "+ to_string(customerId)+ " "+getStringStatus() ;
        };
        AddOrder *AddOrder::clone() const {
            return new AddOrder(*this);
        }
        AddOrder::~AddOrder(){}
        
        
        AddCustomer::AddCustomer(string customerName, CustomerType customerType, int distance, int maxOrders): customerName(customerName),customerType(customerType),distance(distance),maxOrders(maxOrders){};
        void AddCustomer::act(WareHouse &wareHouse) {
            if(customerType == CustomerType::Civilian){
                CivilianCustomer* c = new CivilianCustomer(wareHouse.getCustomerCounter(),customerName,distance,maxOrders);
                wareHouse.setCustomerCounter(1);
                wareHouse.addCustomer(c);
            }
            else{
                SoldierCustomer* s = new SoldierCustomer(wareHouse.getCustomerCounter(),customerName,distance,maxOrders);
                wareHouse.setCustomerCounter(1);
                wareHouse.addCustomer(s);
                complete();
            }
            wareHouse.addAction(this);
        }
        AddCustomer *AddCustomer::clone() const {
            return new AddCustomer(*this);
        }
        string AddCustomer::toString() const {
            if (customerType == CustomerType::Civilian){
                return "Customer "+ customerName + " civilian" +" "+ to_string(distance)+" " + to_string(maxOrders)+" "+ getStringStatus() ;
            }
           else
                return "Customer "+ customerName + " solider " + to_string(distance) +" "+ to_string(maxOrders)+" "+ getStringStatus() ;
        };
        AddCustomer::~AddCustomer(){}


       PrintOrderStatus::PrintOrderStatus(int id): orderId(id){
        };
        
        void PrintOrderStatus::act(WareHouse &wareHouse){
             if(orderId >= wareHouse.getIdOrderCounter()||orderId<0){
                error("Order doesn't exist");
                wareHouse.addAction(this);
                return;
            }
            Order o = wareHouse.getOrder(orderId);
            string s  = o.toString();
            istringstream iss(s);
            string word;
            //לבדוק אם עובד
            string l = "OrderId: OrderStatus: CustomerID: Collector: Driver:";
            istringstream iss2(l);
            string word2;
            while (iss >> word) {
                // Process each word as needed
                iss2>>word2;
                cout << word2 << word << std::endl;
            }
            complete();
            wareHouse.addAction(this);
        }
        PrintOrderStatus *PrintOrderStatus::clone() const {
            PrintOrderStatus *o = new PrintOrderStatus(*this);
            return o;
        };
        string PrintOrderStatus::toString() const {
               return "orderStatus " + to_string(orderId)+" "+ getStringStatus() ;
        }       
        PrintOrderStatus::~PrintOrderStatus(){}


        PrintCustomerStatus::PrintCustomerStatus(int customerId): customerId(customerId){
        };

        void PrintCustomerStatus::act(WareHouse &wareHouse){
            Customer &c = wareHouse.getCustomer(customerId);
           if( c.getId()<0 || c.getId() > wareHouse.getCustomerCounter()){
                error( "Customer doesn’t exist");
                wareHouse.addAction(this);
                return;  
           }

            vector<int> orders = c.getOrdersIds();
            cout<<"CustomerID: "+to_string(c.getId())<<endl;
            for (int id : orders){
                 Order o = wareHouse.getOrder(id);
                 std::cout << "OrderId: " << id << std::endl;
                 if(o.getStatus() == OrderStatus::PENDING)
                    std::cout << "OrderStatus:Pending" << std::endl;
                 if(o.getStatus() == OrderStatus::COMPLETED)
                    std::cout << "OrderStatus:Completed" << std::endl;
                 if(o.getStatus() == OrderStatus::DELIVERING)
                    std::cout << "OrderStatus:Delivering" << std::endl;
                 else if(o.getStatus() == OrderStatus::COLLECTING)
                    std::cout << "OrderStatus:Collecting" << std::endl;
                   
            }
            int ordersLeft = c.getOrdersLeft();
            std::cout << "OrdersLeft: " << ordersLeft << std::endl;
            complete();
            wareHouse.addAction(this);
        };
       
       
        PrintCustomerStatus *PrintCustomerStatus::clone() const {
            PrintCustomerStatus *c = new PrintCustomerStatus(*this);
            return c;
        };
        string PrintCustomerStatus::toString() const {
            return "customerStatus "+ to_string(customerId)+" "+ getStringStatus() ;
        };
        PrintCustomerStatus::~PrintCustomerStatus(){}

        PrintVolunteerStatus::PrintVolunteerStatus(int id): volunteerId(id){
        };
        void  PrintVolunteerStatus::act(WareHouse &wareHouse) {
            Volunteer &v = wareHouse.getVolunteer(volunteerId);
            if((v.getId()>=wareHouse.getVolunteerCounter() ||v.getId()<0)){
                error("Volunteer doesn’t exist");
                wareHouse.addAction(this);
                return;
            }
            string s  = v.toString();
            istringstream iss(s);
            string word;
            string l = "VolunteerID:  isBusy:  OrderID:   TimeLeft:  OrdersLeft: ";
            istringstream iss2(l);
            string word2;
            string word3;
            while (iss >> word) {
                // Process each word as needed
                iss2>>word2;
                if(word == "No"){
                    iss>>word3;
                    cout << word2 << word<<" "<<word3 << std::endl;
                }
                else{
                    cout << word2 << word << std::endl;
                }    
            }
            complete();
            wareHouse.addAction(this);
        }
        PrintVolunteerStatus *PrintVolunteerStatus::clone() const{
            return new PrintVolunteerStatus(volunteerId);
        }
        string PrintVolunteerStatus::toString() const {
            return "volunteerStatus "+ to_string(volunteerId) +" "+ getStringStatus() ;
        };
        PrintVolunteerStatus::~PrintVolunteerStatus(){}
       
        PrintActionsLog::PrintActionsLog(){};
        void PrintActionsLog::act(WareHouse &wareHouse) {
            vector<BaseAction*> actions = wareHouse.getActionsLog();
            for (BaseAction* a : actions){
                std::cout << a->toString() << std::endl;
            }
            complete();
            wareHouse.addAction(this);
        };
    
        PrintActionsLog *PrintActionsLog::clone() const {
            PrintActionsLog * print = new PrintActionsLog(*this);
            return print;
        };
        
        string PrintActionsLog::toString() const {
            return "log " + getStringStatus();
        };   
        PrintActionsLog::~PrintActionsLog(){}


        Close::Close(){};
        void Close:: act(WareHouse &wareHouse) {
            vector<Order*> pendingOrders = wareHouse.getPendingOrder();
            vector<Order*> inProcessOrders = wareHouse.getInProcessOrders();
            vector<Order*> completedOrders = wareHouse.getCompletedOrders();
            for(Order *order:pendingOrders){
                 std::cout << "OrderId: " << order->getId() << " CustomerId: " << order->getCustomerId()
                       << " OrderStatus: " << order->orderStatusToString(order->getStatus()) << std::endl;
             }
            for(Order *order: inProcessOrders){
                  std::cout << "OrderId: " << order->getId() << " CustomerId: " << order->getCustomerId()
                       << " OrderStatus: " << order->orderStatusToString(order->getStatus()) << std::endl;
             }
            for(Order *order:completedOrders){
                  std::cout << "OrderId: " << order->getId() << " CustomerId: " << order->getCustomerId()
                       << " OrderStatus: " << order->orderStatusToString(order->getStatus()) << std::endl;
            }
            complete();
            wareHouse.addAction(this);
            wareHouse.close();
            };
        Close *Close::clone() const{
            return new Close(*this);
        }
        string Close::toString() const {
            return "Close " + getStringStatus();
        }
        Close::~Close(){}


        BackupWareHouse::BackupWareHouse(){};
        void BackupWareHouse::act(WareHouse &wareHouse){
            if(backup!=nullptr){
                delete backup;
                backup = nullptr;
            }
            WareHouse *w = new WareHouse(wareHouse);
            backup = w;
            complete();
            wareHouse.addAction(this);
        }
        BackupWareHouse *BackupWareHouse::clone() const{
            return new BackupWareHouse(*this);
        }
        string BackupWareHouse::toString() const {
            return "Backupe "+getStringStatus();
        }
        BackupWareHouse::~BackupWareHouse(){}


        RestoreWareHouse::RestoreWareHouse(){
            
        }
        void RestoreWareHouse::act(WareHouse &wareHouse){
            if(backup == nullptr){
                error("No backup available");
                wareHouse.addAction(this);
                return;
            }
            wareHouse = *backup;
            complete();
            wareHouse.addAction(this);
        }
        RestoreWareHouse *RestoreWareHouse::clone() const{
            return new RestoreWareHouse(*this);
        }
        string RestoreWareHouse::toString() const {
            return "Restore " + getStringStatus();
        }
        RestoreWareHouse::~RestoreWareHouse(){}