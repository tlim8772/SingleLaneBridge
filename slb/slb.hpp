#include <bits/stdc++.h>
#include <unistd.h>
using namespace std;

// SLB for 1 buy and 1 sell only
// if 1 buy and 1 sell inside, and the sell leaves, no sell can enter until the buy also leaves

struct SLB {
    int buyCnt = 0, sellCnt = 0;
    bool flag = false; // to indicate when there are 1 buy and 1 sell, it is to prevent a sell from repeatedly coming in
                       // when a buy is taking quite long
    mutex mut;
    condition_variable cond;

    void enterBuy() {
        unique_lock lock{mut};
        while (buyCnt != 0 || flag) cond.wait(lock);
        buyCnt ++;
        if (sellCnt == 1) flag = true;
    }

    void enterSell() {
        unique_lock lock{mut};
        while (sellCnt != 0 || flag) cond.wait(lock);
        sellCnt ++;
        if (buyCnt == 1) flag = true;
    }

    void leaveBuy(function<void()> f = [](){}) {
        unique_lock lock{mut};
        buyCnt --;

        // only notify if sellCnt == 0, if there is still a sell, we still cannot let any buy enter
        if (sellCnt == 0) {
            flag = false;
            cond.notify_all();
            f();
        }
    }

    void leaveSell(function<void()> f = [](){}) {
        unique_lock lock{mut};
        sellCnt --;
        
        if (buyCnt == 0) {
            flag = false;
            cond.notify_all();
            f();
        }
    }
};
