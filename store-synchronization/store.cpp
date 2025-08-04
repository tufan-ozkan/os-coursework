#include "store.h"

void Store::initStore(int cA, int cB, int cC, int mO) {
    __synchronized__;
    
    cap[AAA] = avail[AAA] = cA;
    cap[BBB] = avail[BBB] = cB;
    cap[CCC] = avail[CCC] = cC;

    maxOrder = mO;
}

void Store::buy(int aA, int aB, int aC) {
    __synchronized__;

    while (avail[AAA] < aA || avail[BBB] < aB || avail[CCC] < aC) {
        customer.wait();
    }

    avail[AAA] -= aA;
    avail[BBB] -= aB;
    avail[CCC] -= aC;

    for (int i = 0; i < 3; ++i)
        suppliers[i].notifyAll();
}

void Store::maysupply(int itemtype, int n) {
    __synchronized__;

    while (avail[itemtype] + willBeSupplied[itemtype] + n > cap[itemtype]) {
        suppliers[itemtype].wait();
    }

    willBeSupplied[itemtype] += n;
}

void Store::supply(int itemtype, int n) {
    __synchronized__;

    avail[itemtype]          += n;
    willBeSupplied[itemtype] -= n;

    customer.notifyAll();
    suppliers[itemtype].notifyAll();
}

void Store::monitorStore(int c[3], int a[3]) {
    __synchronized__;

    for (int i = 0; i < 3; ++i) {
        c[i] = cap[i];
        a[i] = avail[i];
    }
}