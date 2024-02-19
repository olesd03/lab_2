#pragma once
#include "elevio.h"
typedef enum {
    false = 0,
    true = 1
} bool;

typedef enum {
    down,
    up,
    idle
} state;



void UpdateMaxOrder(const int orders[],int *maxOrder);
void UpdateMinOrder(const int orders[], int *minOrder);
void UpdateFloorInOrders(const int orders[], const int *floor, bool *floor_in_orders);
void DeleteOrder(int orders[], const int *floor);
void FindAndChangeState(const int orders[], const int *floor, state *state, const int *minOrder, const int *maxOrder);
void UpdateOrdersEmpty(const int orders[], bool *orders_empty);
void AddOrders(int orders[]);