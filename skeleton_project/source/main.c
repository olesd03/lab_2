#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include "driver/elevio.h"
#include "elevio.h"
#include "orderArray.h"

/*
typedef enum { 
    DIRN_DOWN   = -1,
    DIRN_STOP   = 0,
    DIRN_UP     = 1
} MotorDirection;
*/
typedef enum {
    up,
    down,
    idle
} state;



void goUpToClosest(void) {
    while (elevio_floorSensor() == -1) {
        elevio_motorDirection(DIRN_UP); 
    }
}

void UpdateFloor(int *floor) {
    if (elevio_floorSensor() != -1) {
        *floor = elevio_floorSensor() +1;
    }
}


int main(){
    elevio_init();

    //Setting up the order system
    void goUpToClosest();
    bool orders_empty;
    bool floor_in_orders;
    state state=idle;
    int orders[4];
    int maxOrder;
    int minOrder;
    int floor;

    while(1){

        while (state==idle) {
            goUpToClosest();
            AddOrders(orders);
            elevio_motorDirection(DIRN_STOP);
            UpdateOrdersEmpty(orders, &orders_empty);
            
            if (!(orders_empty)) {
                UpdateMaxOrder(orders, &maxOrder);
                UpdateMinOrder(orders, &maxOrder);
                UpdateFloor(&floor);
                UpdateFloorInOrders(orders, &floor, &floor_in_orders);
                FindAndChangeState(orders, &floor, &state, &minOrder, &maxOrder);
                if (floor_in_orders) {
                    elevio_motorDirection(DIRN_STOP);
                    //nanosleep(3s);
                    DeleteOrder(orders, &floor);
                }
            }
            //nanosleep(2ms)
        }

        while (state==up) {
            AddOrders(orders);
            elevio_motorDirection(DIRN_UP);
            UpdateFloor(&floor);
            UpdateFloorInOrders(orders, &floor, &floor_in_orders);
            UpdateMaxOrder(orders, &maxOrder);
            UpdateMinOrder(orders, &minOrder);

            if (floor_in_orders) {
                elevio_motorDirection(DIRN_STOP);
                //nanoslepp(3 s)
                UpdateOrdersEmpty(orders, &orders_empty);
                if (floor == maxOrder) {
                    if (orders_empty) {
                        state=idle;
                    }
                    else {
                        state=down;
                    }
                }
                DeleteOrder(orders, &floor);
            }
            //nanosleep(2 ms)
        }

        while (state==down) {
            AddOrders(orders);
            elevio_motorDirection(DIRN_DOWN);
            UpdateFloor(&floor);
            UpdateFloorInOrders(orders, &floor, &floor_in_orders);
            UpdateMaxOrder(orders, &maxOrder);
            UpdateMinOrder(orders, &minOrder);

            if (floor_in_orders) {
                elevio_motorDirection(DIRN_STOP);
                //nanosleep(3s)
                UpdateOrdersEmpty(orders, &orders_empty);
                if (floor == minOrder) {
                    if (orders_empty) {
                        state = idle;
                    }
                    else {
                        state = up;
                    }
                }
                DeleteOrder(orders, &floor);
            }
            //nanosleep(2ms)
           
        }
        
    }

    return 0;
}
