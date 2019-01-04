#include "carclib.h"

#include <iostream>

using namespace std;

const int MAXK = 1e6 + 20;
int K;

int pool[MAXK];
int psize;

int P[MAXK + 1];
int prv[MAXK + 1];
int nxt[MAXK + 1];

int Q[MAXK];
int st = 0;
int en = 0;

int main() {
	K = inicjuj();
	int num = 0;

	for(int i = 1; i <= K + 1; i++) pool[psize++] = i;

	while(true) {
		int a = wczytaj();
		if(a == 0) break;

		if(prv[0]) {
			if(a > P[prv[0]]) {
				Q[en++] = prv[0];
				if(en == K + 1) en = 0;
			}
		}
		int ind = pool[--psize];
		P[ind] = a;
		prv[ind] = prv[0];
		nxt[ind] = 0;
		nxt[prv[ind]] = ind;
		//prv[nxt[ind]] = ind;
		prv[0] = ind;

		num++;

		//cerr << num << ' ' << 0 << ' ' << prv[0] << ' ' << nxt[0] << '\n';
		//for(int i = nxt[0]; i; i = nxt[i]) {
			//cerr << P[i] << ' ' << i << ' ' << prv[i] << ' ' << nxt[i] << '\n';
		//}
		//cerr << st << ' ' << en << '\n';
		//for(int i = st; i != en; i++, i %= K + 1) { cerr << Q[i] << ' '; } cerr << '\n';


		if(num > K) {
			if(st == en) {
				//cerr << "remove back " << prv[0] << '\n';
				pool[psize++] = prv[0];
				nxt[prv[prv[0]]] = 0;
				prv[0] = prv[prv[0]];
			} else {
				int i = Q[st];
				//cerr << "remove " << i << '\n';
				prv[nxt[i]] = prv[i];
				nxt[prv[i]] = nxt[i];

				pool[psize++] = i;

				if(prv[i] != 0 && P[nxt[i]] > P[prv[i]]) {
					Q[st] = prv[i];
				} else {
					st++;
					if(st == K + 1) st = 0;
				}
			}
		}
		//cerr << "after\n";
		//cerr << num << ' ' << 0 << ' ' << prv[0] << ' ' << nxt[0] << '\n';
		//for(int i = nxt[0]; i; i = nxt[i]) {
			//cerr << P[i] << ' ' << i << ' ' << prv[i] << ' ' << nxt[i] << '\n';
		//}
		//cerr << st << ' ' << en << '\n';
		//for(int i = st; i != en; i++, i %= K + 1) { cerr << Q[i] << ' '; } cerr << '\n';
	}
	for(int i = nxt[0]; i; i = nxt[i]) {
		wypisz(P[i]);
	}
	return 0;
}
