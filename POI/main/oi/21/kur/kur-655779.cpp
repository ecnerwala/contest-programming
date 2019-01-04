#include<bits/stdc++.h>
using namespace std;

#define UINT64_C(val) (val##ULL)
 
typedef unsigned long long ull;
ull x = 10; /* The state must be seeded with a nonzero value. */
 
inline ull xorshift64star(void) {
	x ^= x >> 12; // a
	x ^= x << 25; // b
	x ^= x >> 27; // c
	return x * UINT64_C(2685821657736338717);
}

inline int rnd(int l) {
	return (unsigned int)(xorshift64star()) % l;
}

const int MAXN = 1e6;
const int MAXM = 1e6;

int N, M;
int P[MAXN];

long long S[MAXN];

int Q[MAXN];
int st[MAXN];

inline int getnum(int v, int a, int b) {
	int *l = Q + st[v];
	int *r = Q + st[v + 1];
	return lower_bound(l, r, b) - lower_bound(l, r, a);
}

int query(int a, int b) {
	b++;
	int l = b - a;

	for(int i = 0; i <= 30; i++) {
		int v = P[a + rnd(l)];
		int n = getnum(v, a, b);
		//cerr << a << ' ' << b << ' ' << v << ' ' << n << '\n';
		if(n * 2 > l) return v;
	}
	return -1;
}

int main() {

	scanf("%d%d", &N, &M);
	for(int i = 0; i < N; i++) {
		scanf("%d", &P[i]);
		P[i] --;
		S[i] = ((long long)(P[i])) * N + i;
	}

	sort(S, S + N);

	for(int i = 0; i < N; i++) {
		Q[i] = S[i] % N;
		S[i] /= N;
	}

	for(int v = 0; v < N; v++) {
		int i = st[v];
		while(i < N && S[i] == v) i++;
		st[v + 1] = i;
	}

	for(int i = 0; i < M; i++) {
		int a, b; scanf("%d%d", &a, &b);
		a--, b--;
		printf("%d\n", query(a, b) + 1);
	}
	return 0;
}
