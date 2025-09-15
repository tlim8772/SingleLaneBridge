#include <bits/stdc++.h>
#include "slb.hpp"

// test when 1 buy takes very long, I cannot let sells just come in

void t1f1(SLB& slb) {
    slb.enterBuy();
    this_thread::sleep_for(1000ms);
    slb.leaveBuy();
}

void t1f2(SLB& slb, int& test) {
    for (int i = 0; i < 1000000; i++) {
        slb.enterSell(); // remove enterSell, enterBuy to see the difference
        test++;
        slb.leaveSell();
    }
}

void test1() {
    SLB slb;
    int test = 0;
    {
        jthread t1(t1f1, ref(slb));
        jthread t2(t1f2, ref(slb), ref(test)), t3(t1f2, ref(slb), ref(test));
    }

    cout << test << endl;
}

// test whether only 1 buy and 1 sell in the 'critical section'

void t2f1(SLB& slb, int& i) {
    for (int r = 0; r < 1000000; r++) {
        slb.enterBuy();
        i++;
        slb.leaveBuy();
    }
}

void t2f2(SLB& slb, int& i) {
    for (int r = 0; r < 1000000; r++) {
        slb.enterSell();
        i++;
        slb.leaveSell();
    }
}

void test2() {
    SLB slb;
    int b = 0, s = 0;
    {
        jthread t1(t2f1, ref(slb), ref(b)), t2(t2f1, ref(slb), ref(b));
        jthread t3(t2f2, ref(slb), ref(s));
    }
    cout << b << " " << s << endl;
}

int main() {
    //test1();
    test2();
}