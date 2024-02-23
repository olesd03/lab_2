#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include "driver/elevio.h"
#include "orderArray.h"

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
    goUpToClosest();
    
    printf("=== Example Program ===\n");
    printf("Press the stop button on the elevator panel to exit\n");

    // elevio_motorDirection(DIRN_UP);
    bool orders_empty;
    bool floor_in_orders;
    state elev_state=idle;
    int orders[4] = {0,0,0,0};
    int maxOrder =0;
    int minOrder =0;
    int floor = -1;
    printf("flag2\n");

    while(1){
        while (elev_state == idle) {
            goUpToClosest();
            AddOrders(orders);
            elevio_motorDirection(DIRN_STOP);
            UpdateOrdersEmpty(orders, &orders_empty);

            printf("Current floor: %d\n",floor);
            printf("State: %d\n",elev_state);
            printf("{%d,%d,%d,%d}\n",orders[0],orders[1],orders[2],orders[3]);
            printf("minOrder: %d\n",minOrder);
            printf("maxOrder: %d\n",maxOrder);
            printf("orders_empty: %d\n\n,",orders_empty);

            if (!(orders_empty)) {
                UpdateMaxOrder(orders, &maxOrder);
                UpdateMinOrder(orders, &minOrder);
                UpdateFloor(&floor);
                UpdateFloorInOrders(orders, &floor, &floor_in_orders);
                FindAndChangeState(orders, &floor, &elev_state, &minOrder, &maxOrder);

                if (floor_in_orders) {
                    elevio_motorDirection(DIRN_STOP);
                    nanosleep(&(struct timespec){3,0},NULL);
                    DeleteOrder(orders, &floor);
                }
            }
            nanosleep(&(struct timespec){0,10*1000*1000},NULL);
        }

        while (elev_state == up) {
            AddOrders(orders);
            elevio_motorDirection(DIRN_UP);
            UpdateFloor(&floor);
            UpdateFloorInOrders(orders, &floor, &floor_in_orders);
            UpdateMaxOrder(orders, &maxOrder);
            UpdateMinOrder(orders, &minOrder);

            printf("Current floor: %d\n",floor);
            printf("State: %d\n",elev_state);
            printf("{%d,%d,%d,%d}\n",orders[0],orders[1],orders[2],orders[3]);
            printf("minOrder: %d\n",minOrder);
            printf("maxOrder: %d\n",maxOrder);
            printf("orders_empty: %d\n\n,",orders_empty);

            if (floor_in_orders) {
                elevio_motorDirection(DIRN_STOP);
                nanosleep(&(struct timespec){3,0},NULL);
                UpdateOrdersEmpty(orders, &orders_empty);

                if (floor == maxOrder) {
                    if (hasThreeZeros(orders)) {
                        elev_state=idle;
                    }
                    else {
                        elev_state=down;
                    }
                }
                DeleteOrder(orders, &floor);
            }
            nanosleep(&(struct timespec){0,10*1000*1000},NULL);
        }

        while (elev_state == down) {
            AddOrders(orders);
            elevio_motorDirection(DIRN_DOWN);
            UpdateFloor(&floor);
            UpdateFloorInOrders(orders, &floor, &floor_in_orders);
            UpdateMaxOrder(orders, &maxOrder);
            UpdateMinOrder(orders, &minOrder);

            printf("Current floor: %d\n",floor);
            printf("State: %d\n",elev_state);
            printf("{%d,%d,%d,%d}\n",orders[0],orders[1],orders[2],orders[3]);
            printf("minOrder: %d\n",minOrder);
            printf("maxOrder: %d\n",maxOrder);
            printf("orders_empty: %d\n\n,",orders_empty);

            if (floor_in_orders) {
                elevio_motorDirection(DIRN_STOP);
                nanosleep(&(struct timespec){3,0},NULL);
                UpdateOrdersEmpty(orders, &orders_empty);
                UpdateMinOrder(orders, &minOrder);
                
                

                if (floor == minOrder) {
                    if (hasThreeZeros(orders)) {
                        elev_state = idle;
                    }
                    else {
                        elev_state = up;
                    }
                }
                DeleteOrder(orders, &floor);
            }
            nanosleep(&(struct timespec){0,10*1000*1000},NULL);
           
        }
        
    }
//    while(1){
//        printf("=== Ping ===\n");
        // int floor = elevio_floorSensor();

        // if(floor == 0){
        //     elevio_motorDirection(DIRN_UP);
        // }

        // if(floor == N_FLOORS-1){
        //     elevio_motorDirection(DIRN_DOWN);
        // }


        // for(int f = 0; f < N_FLOORS; f++){
        //     for(int b = 0; b < N_BUTTONS; b++){
        //         int btnPressed = elevio_callButton(f, b);
        //         elevio_buttonLamp(f, b, btnPressed);
        //     }
        // }

        // if(elevio_obstruction()){
        //     elevio_stopLamp(1);
        // } else {
        //     elevio_stopLamp(0);
        // }
        
        // if(elevio_stopButton()){
        //     elevio_motorDirection(DIRN_STOP);
        //     break;
        // }
        
//        nanosleep(&(struct timespec){0, 20*1000*1000}, NULL);
//    }

    return 0;
}
