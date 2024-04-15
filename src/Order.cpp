#include "Order.h"
#include <string>
#include <vector>
#include <iostream>
#include <string>
using std::string;
using std::vector;

#define NO_VOLUNTEER -1

        Order::Order(int id, int customerId, int distance): id(id), customerId(customerId), distance(distance),status(OrderStatus::PENDING),collectorId(NO_VOLUNTEER),driverId(NO_VOLUNTEER),sign(0) {};

        const int Order::getId() const{
            return this->id;
        }
        Order::Order(const Order &other)
            : id(other.id),
            customerId(other.customerId),
            distance(other.distance),
            status(other.status),
            collectorId(other.collectorId) 
            ,driverId(other.driverId), sign(other.sign) {};
        
        int Order:: getSign(){
            return sign;
        }
        void Order::setSign(int i){
             sign =i;
        }
        int Order::getCustomerId() const{
            return customerId;
        }
        std::string Order::orderStatusToString(OrderStatus status) const {
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
                    // Handle unknown status, you can return an empty string or any other appropriate value.
                    return "";
        }
}   
        void Order::setStatus(OrderStatus status){
            this->status =status;
        }
        void Order::setCollectorId(int collectorId){
            this->collectorId = collectorId;
        }
        void Order::setDriverId(int driverId){
            this->driverId = driverId;
        }
        int Order::getCollectorId() const{
            return collectorId;
        }
        int Order::getDriverId() const{
            return driverId;
        }
        const OrderStatus Order::getStatus()const {
            OrderStatus s = status;
            return s;
        }
       const string Order::toString() const {
        if(collectorId == NO_VOLUNTEER){
            return std::to_string(id) + " " + orderStatusToString(status) + " " +
                std::to_string(customerId) + " " + "None" + " " + "None";
            }
        else if (driverId == NO_VOLUNTEER)
        {
            return std::to_string(id) + " " + orderStatusToString(status) + " " +
                std::to_string(customerId) + " " + std::to_string(collectorId) + " " + "None";
        }
        return std::to_string(id) + " " + orderStatusToString(status) + " " +
                std::to_string(customerId) + " " + std::to_string(collectorId)+ " " + std::to_string(driverId);
        }
            
        int Order::getDistance() const{
            return distance;
        }    