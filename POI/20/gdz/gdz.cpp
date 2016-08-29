// gdz.cpp - Przykladowe uĹźycie biblioteki do zadania GDZ
#include "cgdzlib.h"
#include<algorithm>
#include<cassert>
using namespace std;

typedef pair<int, int> pii;
#define A first
#define B second

const int MAXN = 6e5;
int N;
int V[MAXN];

pii getbounds(int l, int r, int v = 2) {
	assert(r - l >= 2);
	if(r - l == 2) {
		return pii(V[l], V[l + 1]);
	} else {
		int m = l + 1;
		for(int i = l + 1; i < r; i++) {
			if(f(V[l], V[i], v)) {
				if(m != i) swap(V[m], V[i]);
				m++;
			}
		}
		if(m - l > r - m) {
			return getbounds(l, m, v * 2);
		} else if(r - m > m - l) {
			return getbounds(m, r, v * 2);
		} else {
			pii a = getbounds(l, m, v * 2);
			pii b = getbounds(m, r, v * 2);
			int d = r - l - 1;
			if(f(a.A, b.A, d)) return pii(a.A, b.A);
			if(f(a.A, b.B, d)) return pii(a.A, b.B);
			if(f(a.B, b.A, d)) return pii(a.B, b.A);
			if(f(a.B, b.B, d)) return pii(a.B, b.B);
			assert(false);
		}
	}
}

int main() {
    N = inicjuj();
	if(N == 1) {
		odpowiedz(1);
	}
	for(int i = 0; i < N; i++) V[i] = i+1;
	pii res = getbounds(0, N);
	if(g(res.A, res.B)) {
		//p[res.A] > p[res.B]
		odpowiedz(res.B);
	} else {
		odpowiedz(res.A);
	}
    return 0;
}
