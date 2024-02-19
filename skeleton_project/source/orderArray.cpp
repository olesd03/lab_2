#include "orderArray.h"



void UpdateMaxOrder(const int orders[],int *maxOrder) {
    int temp_max=0;
    for (int i=0; i<4; i++) {
        if (orders[i] > temp_max) {
            temp_max = orders[i];
        }
    }
    *maxOrder = temp_max;
}
void UpdateMinOrder(const int orders[], int *minOrder) {
    int temp_min=4;
    for (int i=0; i<4; i++) {
        if (orders[i]<temp_min) {
            temp_min = orders[i];
        }
    }
    *minOrder = temp_min;
}

void UpdateFloorInOrders(const int orders[], const int *floor, bool *floor_in_orders) {
    *floor_in_orders = false;
    for (int i=0; i<4; i++) {
        if ((orders[i]) == (*floor)) {
            *floor_in_orders = true;
            break;
        }
    }
}

void DeleteOrder(int orders[], const int *floor) {;
    for (int i=0; i<4; i++) {
        if ((orders[i]) == (*floor)) {
            orders[i] = 0;
            break;
        }
    }
}

void FindAndChangeState(const int orders[], const int *floor, state *state, const int *minOrder, const int *maxOrder) {
    if (     (*minOrder)<(*floor) && (*maxOrder!=*floor) && (*minOrder != 0)) {
        *state = down;
    }
    else if ((*maxOrder)>(*floor) && (*minOrder != *floor)) {
        *state = up;
    }
}

void UpdateOrdersEmpty(const int orders[], bool *orders_empty) {
    *orders_empty = true;
    for (int i=0; i<4; i++) {
        if (orders[i] != 0) {
            *orders_empty = false;
            break;
        }
    }
}

void AddOrders(int orders[]) {
    for (int i=0; i<4; i++) {
        for (int j=0; j<3; j++) {
            if (elevio_callButton(i,j)) {
                orders[i] = i+1;
            }
        }
    }
}
