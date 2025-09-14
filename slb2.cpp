#include <bits/stdc++.h>
#include <thread>
using namespace std;

// a single lane bridge that only allows all buys or all sells inside
struct SLB2 {
    mutex mut;
    condition_variable cond;
    int buyCnt = 0, sellCnt = 0;

    void enterBuy(function<void()> f = [](){}) {
        unique_lock lock{mut};
        if (sellCnt > 0) cond.wait(lock, [this] () {return this->sellCnt == 0;});
        buyCnt++;
        f();
    }

    void leaveBuy(function<void()> f = [](){}) {
        unique_lock lock{mut};
        buyCnt--;
        if (buyCnt == 0) cond.notify_all();
        f();
    }

    void enterSell(function<void()> f = [](){}) {
        unique_lock lock{mut};
        if (buyCnt > 0) cond.wait(lock, [this] () {return this->buyCnt == 0;});
        sellCnt++;
        f();
    }

    void leaveSell(function<void()> f = [](){}) {
        unique_lock lock{mut};
        sellCnt--;
        if (sellCnt == 0) cond.notify_all();
        f();
    }
};

SLB2 slb2;
int test = 0;

void t1(int tid) {
    for (int i = 0; i < 1000000; i++) {
        slb2.enterBuy([tid](){});
        test++;
        slb2.leaveBuy([tid](){});
    }
}

void t2(int tid) {
    for (int i = 0; i < 1000000; i++) {
        slb2.enterSell([tid](){});
        test--;
        slb2.leaveSell([tid](){});
    }
}

int main() {
    {
        jthread j1(t1, 1);
        jthread j2(t2, 2);
    }
    cout << "Final result " << test << endl;
}