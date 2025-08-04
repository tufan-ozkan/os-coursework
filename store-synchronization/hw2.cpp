#include <iostream>
#include "store.h"

/* You can define any global variable
   function and class definition here.
   Programs will first call initStore() then
   all other functions
*/
Store store;

void initStore(int cA, int cB, int cC, int mO) {
    store.initStore(cA, cB, cC, mO);
}

void buy(int aA, int aB, int aC) {
    store.buy(aA, aB, aC);
}

void maysupply(int itemtype, int n) {
    store.maysupply(itemtype, n);
}

void supply(int itemtype, int n) {
    store.supply(itemtype, n);
}

void monitorStore(int c[3], int a[3]) {
    store.monitorStore(c, a);
}