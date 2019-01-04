#define NDEBUG
#include<bits/stdc++.h>

using namespace std;

int N; // number of connections, 
int V; // number of seats
int Q; // number of queries

const int INF = 1e9;

const int MAXN = 1e5;

const int S = 1 << 17;

int lazy[S * 2];
int val[S * 2];

void propogate(int i) {
	if(i < S) {
		int l = 2 * i, r = 2 * i + 1;
		lazy[l] += lazy[i];
		val[l] += lazy[i];
		lazy[r] += lazy[i];
		val[r] += lazy[i];
	}
	lazy[i] = 0;
	if(i < S) {
		assert(val[i] == lazy[i] + max(val[2 * i], val[2 * i + 1]));
	}
}

// l and r are inclusive/exclusive
int get_max(int a, int b, int i = 1, int l = 0, int r = N) {
	assert(l <= a && a < b && b <= r);
	if(b - a == r - l) return val[i];
	assert(r - l > 1);

	propogate(i);
	assert(!lazy[i]);

	int m = (l + r) / 2;
	assert(l < m && m < r);
	int res = -INF;
	if(a < m) {
		res = max(res, get_max(a, min(b, m), 2 * i, l, m));
	}
	if(b > m) {
		res = max(res, get_max(max(a, m), b, 2 * i + 1, m, r));
	}
	//res += lazy[i]; // lazy[i] == 0
	return res;
}

void add(int a, int b, int v, int i = 1, int l = 0, int r = N) {
	assert(l <= a && a < b && b <= r);
	if(b - a == r - l) {
		lazy[i] += v;
		val[i] += v;
		return;
	}
	assert(r - l > 1);

	propogate(i);
	assert(!lazy[i]);

	int m = (l + r) / 2;
	assert(l < m && m < r);
	if(a < m) {
		add(a, min(b, m), v, 2 * i, l, m);
	}
	if(b > m) {
		add(max(a, m), b, v, 2 * i + 1, m, r);
	}

	assert(i < S);
	val[i] = max(val[2 * i], val[2 * i + 1]);
	val[i] += lazy[i];
}

int main() {
	scanf("%d %d %d", &N, &V, &Q);
	for(int i = 0; i < Q; i++) {
		int l, r, v;
		scanf("%d %d %d", &l, &r, &v);
		l--, r--;
		if(v + get_max(l, r) > V) {
			printf("N\n");
		} else {
			printf("T\n");
			add(l, r, v);
		}
	}
}
