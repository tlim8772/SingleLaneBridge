#include <bits/stdc++.h>
using namespace std;

struct Store {
    bool buyCnt = 0, sellCnt = 0, raised = 0;
};

struct SLBAtomics {
    atomic<Store> store{Store{0, 0, 0}};

    void enterBuy() {
        Store expected;
        
        start:
            while (1) {
                expected = store.load();
                if (!expected.buyCnt && !expected.raised) break;
            }

        while (1) {
            Store desired = {true, expected.sellCnt, expected.sellCnt};
            bool res = store.compare_exchange_strong(expected, desired);
            if (res) break;
            if (expected.buyCnt || expected.raised) goto start;
        }
    }

    void leaveBuy() {
        Store expected = store.load();
        while (1) {
            Store desired = {false, expected.sellCnt, expected.sellCnt};
            bool res = store.compare_exchange_strong(expected, desired);
            if (res) break;
        }
    }

    void enterSell() {
        Store expected;

        start:
            while (1) {
                expected = store.load();
                if (!expected.sellCnt && !expected.raised) break;
            }

        while (1) {
            Store desired = {expected.buyCnt, true, expected.buyCnt};
            bool res = store.compare_exchange_strong(expected, desired);
            if (res) break;
            if (expected.sellCnt || expected.raised) goto start;
        }
    }

    void leaveSell() {
        Store expected = store.load();
        while (1) {
            Store desired = {expected.buyCnt, false, expected.buyCnt};
            bool res = store.compare_exchange_strong(expected, desired);
            if (res) break;
        }
    }
};