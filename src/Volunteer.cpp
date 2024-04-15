#include <string>
#include <vector>
#include "Order.h"
#include "Volunteer.h"
using std::string;
using std::vector;
#define NO_ORDER -1

        Volunteer::Volunteer(int id, const string& name): completedOrderId(NO_ORDER),activeOrderId(NO_ORDER), id(id),name(name) {};

        int Volunteer::getId() const{
            return id;
        }
        const string &Volunteer::getName() const{
            return name;
        }
        int Volunteer::getActiveOrderId() const{
            return activeOrderId;
        }
        int Volunteer::getCompletedOrderId() const{
            return completedOrderId;
        }
        // Signal whether the volunteer is currently processing an order
        bool Volunteer::isBusy() const{
            return !(activeOrderId==-1);
        }    
        void Volunteer::setOrdersLeft(){
            return;
        }
        Volunteer::~Volunteer(){}

        CollectorVolunteer::CollectorVolunteer(int _id, const string &_name, int _coolDown): Volunteer(_id,_name), coolDown(_coolDown),timeLeft(0),type("CollectorVolunteer") {};

        int CollectorVolunteer::getCoolDown()const{
            return coolDown;
        }
        int CollectorVolunteer::getTimeLeft() const{
            return timeLeft;
        }
        //Decrease timeLeft by 1,return true if timeLeft=0,false otherwise
        bool CollectorVolunteer::decreaseCoolDown(){
            if(isBusy()){
                timeLeft--;
                if(timeLeft==0){
                    this->completedOrderId = this->activeOrderId;
                    this->activeOrderId = NO_ORDER;
                    return false;
                }
                return true;
            }
            return false;
        }
        bool CollectorVolunteer::hasOrdersLeft() const {
            return true;
        }
        bool CollectorVolunteer::canTakeOrder(const Order &order) const {
            return (!isBusy() && order.getStatus() == OrderStatus::PENDING);
        }
        void CollectorVolunteer::acceptOrder(const Order &order) {
            activeOrderId = order.getId();
            timeLeft = coolDown;
        }
        void CollectorVolunteer::setOrdersLeft(){
            return;
        }
        void CollectorVolunteer::step(){
            decreaseCoolDown();
        }
        bool CollectorVolunteer::finishedOrder() const{
            return timeLeft==0 ;
        }
        string CollectorVolunteer::toString() const {

            if(isBusy()){
                return std::to_string(getId()) +" "+ "True" + " " + std::to_string(getActiveOrderId()) + " "+ std::to_string(timeLeft)  +" "+ "No Limit";
            }
            else{
                return std::to_string(getId())+ " " +"False" + " " + "None" + " " + "None" + " " +" "+ "No Limit";
            }
        }
        const string CollectorVolunteer::getType()const{
            return type;
        }
        void CollectorVolunteer::setTimeLeft(){
            timeLeft=coolDown;
        }

        CollectorVolunteer *CollectorVolunteer::clone()const{
            return new CollectorVolunteer(*this);
        }

        CollectorVolunteer::~CollectorVolunteer(){}

        LimitedCollectorVolunteer::LimitedCollectorVolunteer(int id, const string &name, int coolDown ,int _maxOrders): CollectorVolunteer(id,name,coolDown), maxOrders(_maxOrders), ordersLeft(_maxOrders),type("LimitedCollectorVolunteer") {}
        LimitedCollectorVolunteer *LimitedCollectorVolunteer::clone() const{
            return new LimitedCollectorVolunteer(getId(),getName(),getCoolDown(),maxOrders);
        }
        bool LimitedCollectorVolunteer::hasOrdersLeft() const {
            return !(ordersLeft==0);
        }
        bool LimitedCollectorVolunteer::canTakeOrder(const Order &order) const {
            return (CollectorVolunteer::canTakeOrder(order) && hasOrdersLeft());
        }
        void LimitedCollectorVolunteer::acceptOrder(const Order &order) {
            activeOrderId = order.getId();
            setTimeLeft();
        }

        int LimitedCollectorVolunteer::getMaxOrders() const{
            return maxOrders;
        }
        int LimitedCollectorVolunteer::getNumOrdersLeft() const{
            return ordersLeft;
        }
        string LimitedCollectorVolunteer::toString() const {
            if(isBusy()){
                return std::to_string(getId()) + " "+  "True" + " " + std::to_string(getActiveOrderId()) + " "  + std::to_string(getTimeLeft()) + " " + std::to_string(ordersLeft-1);
            }
            else{
                return std::to_string(getId()) + " " +"False" + " "  + "None" + " "  + "None" + " "  + std::to_string(ordersLeft-1);
            }
        }
        const string LimitedCollectorVolunteer::getType()const{
            return type;
        }
        void LimitedCollectorVolunteer::setOrdersLeft(){
            ordersLeft = ordersLeft - 1;
        }

        LimitedCollectorVolunteer::~LimitedCollectorVolunteer(){}
        
        DriverVolunteer::DriverVolunteer(int id, const string &name, int maxDistance, int distancePerStep): Volunteer(id, name), maxDistance(maxDistance),distancePerStep(distancePerStep),distanceLeft(0),type("DriverVolunteer"){};
        DriverVolunteer *DriverVolunteer::clone() const {
            return new DriverVolunteer(getId(),getName(),maxDistance,distancePerStep);
        }
        int DriverVolunteer::getDistanceLeft() const{
            return distanceLeft;
        }
        int DriverVolunteer::getMaxDistance() const{
            return maxDistance;
        }
        void DriverVolunteer::setOrdersLeft(){
            return;
        }
        int DriverVolunteer::getDistancePerStep() const{
            return distancePerStep;
        }
        bool DriverVolunteer::decreaseDistanceLeft(){
            distanceLeft = distanceLeft - distancePerStep;
            if(distanceLeft<0){
                distanceLeft=0;
            }
            return distanceLeft == 0;
        } //Decrease distanceLeft by distancePerStep,return true if distanceLeft<=0,false otherwise
        bool DriverVolunteer::finishedOrder() const{
            return distanceLeft == 0 ;
        }
        bool DriverVolunteer::hasOrdersLeft() const {
            return true;
        }
        bool DriverVolunteer::canTakeOrder(const Order &order) const {
            return (!isBusy() && (order.getDistance() <= getMaxDistance()) && (order.getStatus() == OrderStatus::COLLECTING));
        }
        void DriverVolunteer::acceptOrder(const Order &order) {
            activeOrderId = order.getId();
            distanceLeft = order.getDistance();
        } // Reset distanceLeft to maxDistance
        void DriverVolunteer::step() {
            if(decreaseDistanceLeft()){
                completedOrderId =  activeOrderId;
                activeOrderId = NO_ORDER;
            }
        }
        void DriverVolunteer::setDistanceLeft(int distance){
            distanceLeft = distance;
        }
        string DriverVolunteer::toString() const {
            if(isBusy()){
                return std::to_string(getId()) + " " + "True" + " "  + std::to_string(getActiveOrderId()) + " " + std::to_string(distanceLeft) + " " + "No Limit";
            }
            else{
                return std::to_string(getId()) + " " +"False" + " "  + "None" + " " + "None" + " "  + "No Limit";
            }
        }
        const string DriverVolunteer::getType()const{
            return type;
        }

        DriverVolunteer::~DriverVolunteer(){}


        LimitedDriverVolunteer::LimitedDriverVolunteer(int id, const string &name, int maxDistance, int distancePerStep,int maxOrders): DriverVolunteer::DriverVolunteer(id,name,maxDistance,distancePerStep),maxOrders(maxOrders),ordersLeft(maxOrders),type("LimitedDriverVolunteer"){};

        LimitedDriverVolunteer * LimitedDriverVolunteer::clone() const{
            return new LimitedDriverVolunteer(*this);
        }
        int  LimitedDriverVolunteer::getMaxOrders() const{
            return maxOrders;
        }
        int  LimitedDriverVolunteer::getNumOrdersLeft() const{
            return ordersLeft;
        }
        bool  LimitedDriverVolunteer::hasOrdersLeft() const {
            return !(ordersLeft==0);
        }
        bool  LimitedDriverVolunteer::canTakeOrder(const Order &order) const {
            return (DriverVolunteer::canTakeOrder(order) && hasOrdersLeft());
        }
        void  LimitedDriverVolunteer::acceptOrder(const Order &order){
            activeOrderId = order.getId();
            setDistanceLeft(order.getDistance());
            ordersLeft = ordersLeft - 1;
        } 
        string  LimitedDriverVolunteer::toString() const {
            if(isBusy()){
                return std::to_string(getId()) + " " +  "True"+ " "  + std::to_string(getActiveOrderId()) + " " + std::to_string(getDistanceLeft()) + " "+ std::to_string(ordersLeft-1);
            }
            else{
                return std::to_string(getId())+ " " +"False" + " " + "None" + " " + "None" + " " + std::to_string(ordersLeft-1);
            }
        }
        const string  LimitedDriverVolunteer::getType()const{
            return type;
        }
        void  LimitedDriverVolunteer::setOrdersLeft(){
            ordersLeft = ordersLeft - 1;
        }
        LimitedDriverVolunteer::~LimitedDriverVolunteer(){}