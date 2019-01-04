#include "carclib.h"

#include <iostream>
#include <list>
#include <deque>

using namespace std;

const int MAXK = 2e6;
int K;

typedef list<int>::iterator lit;

list<int> P;
deque<lit> Q;

int main() {
	//cin >> K;
	K = inicjuj();
    while(true) {
		//int a; cin >> a;
		int a = wczytaj();
        if(a == 0) break;
		
		if(P.size() > 0) {
			if(a > P.back()) {
				Q.push_back(--P.end());
			}
		}
		P.push_back(a);

		if(P.size() > K) {
			if(Q.empty()) {
				P.pop_back();
			} else {
				lit i = Q.front(); Q.pop_front();
				//cerr << *i << ' ';
				i = P.erase(i);
				//cerr << *i << '\n';
				if(i != P.begin()) {
					int v = *i;
					i--;
					if(v > (*i)) Q.push_front(i);
				}
			}
		}
		//for(lit it = P.begin(); it != P.end(); it++) {
			//cerr << *it << ' ';
		//}
		//cerr << '\n';
    }
    for(lit it = P.begin(); it != P.end(); it++) {
		//cout << *it << ' ';
		wypisz(*it);
	}
    return 0;
}
