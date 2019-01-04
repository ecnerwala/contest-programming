#include "cliclib.h"

#include<algorithm>
#include<map>
#include<cassert>
#include<vector>
#include<iostream>
using namespace std;

typedef pair<int, int> pii;
#define A first
#define B second

// pile, limit

pii dpot(pii s) {
	return pii(s.A * 2, s.B);
}

pii tpot(pii s) {
	return pii(s.A * 3, s.B);
}

pii pass(pii s) {
	return pii(1, s.B - s.A);
}

pii makemove(pii s, int a) {
	switch(a) {
		case 1: return pass(s);
		case 2: return dpot(s);
		case 3: return tpot(s);
		default: assert(false);
	}
}

const int MAXN = 5e4;
int N;

const int MAXP = 200;
int P;
int piles[MAXP];
int pnum[3 * MAXN];

int move[MAXN][MAXP];

int exp(int a, int b) {
	int r = 1;
	for(int i = 0; i < b; i++) r *= a;
	return r;
}

int main() {
	N = inicjuj();
	for(int i = 0; exp(2, i) < N; i++) {
		for(int j = 0; exp(2, i) * exp(3, j) < N; j++) {
			piles[P++] = exp(2, i) * exp(3, j);
		}
	}
	//cerr << P << '\n';
	sort(piles, piles + P);
	for(int i = 0; i < P; i++) {
		//cerr << i << ' ' << piles[i] << '\n';
		pnum[piles[i]] = i;
	}

	pii st (1, N);
	for(int s = 1; s <= N; s++) {
		int i = 0;
		while(i < P && piles[i] < s) i++;
		for(i--; i >= 0; i--) {
			int p = piles[i];
			assert(p < s);
			int m = 0;
			if(p * 3 >= s) {
				m = 3;
			} else if(move[s][pnum[p * 3]] == 0){
				m = 3;
			} else if(move[s][pnum[p * 2]] == 0) {
				m = 2;
			} else if(move[s - p][0] == 0) {
				m = 1;
			} 
			move[s][i] = m;
			//cerr << s << ' ' << p << ' ' << m << '\n';
		}
	}
	while (st.B > 0) {
		int m = move[st.B][pnum[st.A]];
		assert(m);
		alojzy(m);
		st = makemove(st, m);
		int x = bajtazar();
		st = makemove(st, x);
	}
	return 0;
}
