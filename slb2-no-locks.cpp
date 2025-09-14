#include <bits/stdc++.h>
using namespace std;

struct Store {
    int buyCnt = 0, sellCnt = 0;
};  

struct SLB2NoLocks {
    atomic<Store> store{Store{0, 0}};

    void enterBuy(function<void()> f = [](){}) {
        Store expected;
        start:
            while (1) {
                expected = store.load();
                if (expected.sellCnt == 0) break;
            }

        // invariant: when we reach here sellCnt must be 0 so the compare_exchange works correctly
        // previously I did not do this, so there were bugs
        while (1) {
            Store desired = {expected.buyCnt + 1, expected.sellCnt};
            bool res = store.compare_exchange_strong(expected, desired);
            
            if (res) break;
            if (expected.sellCnt > 0) goto start; // repeat the entire process again
        }

        f();
    }

    // if we enter, we can leave with no problems;
    void leaveBuy(function<void()> f = [](){}) {
        f();
        Store expected = store.load();
        while (!store.compare_exchange_strong(expected, {expected.buyCnt - 1, expected.sellCnt}));
    }

    void enterSell(function<void()> f = [](){}) {
        Store expected;
        start:
            while (1) {
                expected = store.load();
                if (expected.buyCnt == 0) break;
            }

        while (1) {
            Store desired = {expected.buyCnt, expected.sellCnt + 1};
            bool res = store.compare_exchange_strong(expected, desired);

            if (res) break;
            if (expected.buyCnt > 0) goto start;
        }

        f();
    }

    void leaveSell(function<void()> f = [](){}) {
        f();
        Store expected = store.load();
        while(!store.compare_exchange_strong(expected, {expected.buyCnt, expected.sellCnt - 1}));
    }


};

SLB2NoLocks slb2;
int test = 0;

void printEnterBuy(int tid) {
    cout << tid << " enter buy" << endl;
}

void printLeaveBuy(int tid) {
    cout << tid << " leave buy" << endl;
}

void printEnterSell(int tid) {
    cout << tid << " enter sell" << endl;
}

void printLeaveSell(int tid) {
    cout << tid << " leave sell" << endl;
}

void f1(int tid) {
    for (int i = 0; i < 1000000; i++) {
        slb2.enterBuy();
        test++;
        slb2.leaveBuy();
    }
}

void f2(int tid) {
    for (int i = 0; i < 1000000; i++) {
        slb2.enterSell();
        test--;
        slb2.leaveSell();
    }
}

int main() {
    {
        jthread t1(f1, 1);
        jthread t2(f2, 2);
    }
    
    //cout << "Final result " << test << endl;
}