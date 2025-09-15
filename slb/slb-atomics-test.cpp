#include <bits/stdc++.h>
using namespace std;

#include "slb-atomics.hpp"

void t1f1(SLBAtomics& slb) {
    for (int r = 0; r < 2; r++) {
        slb.enterBuy();
        cout << "enter buy" << endl;
        this_thread::sleep_for(1000ms);
        cout << "leave buy" << endl;
        slb.leaveBuy();
    }
    
}

void t1f2(SLBAtomics& slb, int& test) {
    for (int r = 0; r < 1; r++) {
        slb.enterSell();
        cout << "enter sell" << endl;;
        test++;
        cout << "leave sell" << endl;
        slb.leaveSell();
    }
}

void test1() {
    SLBAtomics slb;
    int test = 0;
    {
        jthread t1(t1f1, ref(slb));
        vector<jthread> ths;
        for (int i = 0; i < 2; i++) {
            ths.push_back(std::move(jthread(t1f2, ref(slb), ref(test))));
        }
    }
    cout << test << endl;
}

void t2f1(SLBAtomics& slb, int& i) {
    for (int r = 0; r < 1000000; r++) {
        slb.enterBuy();
        i++;
        slb.leaveBuy();
    }
}

void t2f2(SLBAtomics& slb, int& i) {
    for (int r = 0; r < 1000000; r++) {
        slb.enterSell();
        i++;
        slb.leaveSell();
    }
}

void test2() {
    SLBAtomics slb;
    int b = 0, s = 0;
    {
        jthread t1(t2f1, ref(slb), ref(b)), t2(t2f1, ref(slb), ref(b));
        jthread t3(t2f2, ref(slb), ref(s)), t4(t2f2, ref(slb), ref(s));
    }
    cout << b << " " << s << endl;
}

int main() {
    test1();
    //test2();
}
