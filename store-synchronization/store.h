#ifndef __STORE_H
#define __STORE_H
#include "monitor.h"

enum ItemType { AAA, BBB, CCC };

class Store : public Monitor {
    int cap[3], avail[3], willBeSupplied[3];
    int maxOrder;
    Condition customer, suppliers[3];

public:
    Store() : customer(this), suppliers{ Condition(this), Condition(this), Condition(this) } {}

    void initStore(int cA, int cB, int cC, int mO);
    void buy(int aA, int aB, int aC);
    void maysupply(int itemtype, int n);
    void supply(int itemtype, int n);
    void monitorStore(int c[3], int a[3]);
};

#endif