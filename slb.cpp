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

    void leaveBuy(function<void()> f) {
        unique_lock lock{mut};
        buyCnt --;

        // only notify if sellCnt == 0, if there is still a sell, we still cannot let any buy enter
        if (sellCnt == 0) {
            flag = false;
            cond.notify_all();
            f();
        }
    }

    void leaveSell(function<void()> f) {
        unique_lock lock{mut};
        sellCnt --;
        
        if (buyCnt == 0) {
            flag = false;
            cond.notify_all();
            f();
        }
    }
};

int b = 0, s = 0;
mutex mut;

void safePrint(string s) {
    lock_guard lk(mut);
    cout << s << endl;
}


void buy(int id, SLB& slb) {
    for (int i = 0; i < 2; i ++) {
        slb.enterBuy();
        cout << "buy enter" << endl;
        sleep(1);
        slb.leaveBuy([] () {cout << "wave done" << endl;});
    }
}

void sell(int id, SLB& slb) {
    for (int i = 0; i < 16; i ++) {
        slb.enterSell();
        cout << "sell enter" << endl;
        //sleep(1);
        slb.leaveSell([] () {cout << "wave done"  << endl;});
    }
}

int main() {
    SLB slb;

    thread t1{buy, 0, ref(slb)};
    thread t2{buy, 1, ref(slb)};
    thread t3{sell, 2, ref(slb)};
    thread t4{sell, 3, ref(slb)};

    t4.join(); 
    t3.join(); 
    t2.join(); 
    t1.join();

    cout << b << " " << s << endl;
}