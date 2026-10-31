#include <bits/stdc++.h>

void inicjuj(int m);
void dodaj(int k);
void koryguj(int i, int k);
long long suma(int i);

namespace ecnerwala {
int M;
int64_t tot_sum;
int nxt_idx;
std::vector<int64_t> bit;

int64_t query_pref(int i) {
	assert(0 <= i && i <= M);
	int64_t r = 0;
	for (int a = i; a > 0; a &= a-1) {
		r += bit[a-1];
	}
	return r;
}
void update_pos(int i, int64_t x) {
	assert(0 <= i && i < M);
	tot_sum += x;
	for (int a = i; a < M; a |= a+1) {
		bit[a] += x;
	}
}
}

void inicjuj(int m) {
	using namespace ecnerwala;
	M = m;
	tot_sum = 0;
	nxt_idx = 0;
	bit.assign(M, 0);
}

void dodaj(int k) {
	using namespace ecnerwala;
	update_pos(nxt_idx, k - (query_pref(nxt_idx + 1) - query_pref(nxt_idx)));
	nxt_idx++; if (nxt_idx == M) nxt_idx = 0;
}

void koryguj(int i, int k) {
	using namespace ecnerwala;
	int idx = nxt_idx - i;
	if (idx < 0) idx += M;
	update_pos(idx, k);
}

long long suma(int i) {
	using namespace ecnerwala;
	int lo = nxt_idx - i, hi = nxt_idx;
	int64_t ans = query_pref(hi);
	if (lo < 0) {
		ans += tot_sum - query_pref(lo + M);
	} else {
		ans -= query_pref(lo);
	}
	return ans;
}
