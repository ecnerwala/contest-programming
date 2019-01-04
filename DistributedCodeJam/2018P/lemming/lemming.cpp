#include "message.h"
#ifndef INPUT_HEADER
#define INPUT_HEADER "lemming.h"
#endif
#include INPUT_HEADER
#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

const int NODES = NumberOfNodes();
const int ID = MyNodeId();

ll split(ll a, ll i) {
	return (a / NODES) * i + min(a % NODES, i);
}

const ll C = GetColumns();
const ll R = GetRows();

ll GetNext(ll p) {
	ll c = p / R, r = p % R;
	char dir = GetDirection(r,c);
	switch (dir) {
		case '^': r--; break;
		case 'v': r++; break;
		case '<': c--; break;
		case '>': c++; break;
		default: assert(false);
	}
	if (0 <= c && c < C && 0 <= r && r < R) {
		return c * R + r;
	} else {
		// loop back to self
		return p;
	}
}

const ll MAXC = 4e4;
const ll MAXR = 4e4;
const ll MAXN = MAXC * MAXR / 100;
ll st, en;
ll nxt[MAXN];
ll dp[MAXN];

ll res = 0;

const ll NEW = -1;
const ll STACK = -2;
const ll LOOP = -3;

ll dfs(ll cur) {
	if (!(st <= cur && cur < en)) {
		return cur;
	}
	if (dp[cur-st] == STACK) {
		res++;
		return LOOP;
	} else if (dp[cur-st] != NEW) {
		return dp[cur-st];
	} else if (dp[cur-st] == NEW) {
		dp[cur-st] = STACK;
		dp[cur-st] = dfs(nxt[cur-st]);
		return dp[cur-st];
	} else assert(false);
	return LOOP;
}

void go() {
	memset(dp, -1, sizeof(dp));
	for (ll i = st; i < en; i++) {
		dfs(i);
	}
}

ll convert_dest(ll p) {
	if (p == LOOP) return p;

	assert(0 <= p && p < R * C);

	if (st - R <= p && p < st) {
		return p + 2 * ID * R - st;
	} else if (en <= p && p < en + R) {
		return p - en + (2 * ID + 2) * R;
	} else assert(false);
}

void phase1() {
	st = split(C, ID) * R, en = split(C, ID+1) * R;
	for (ll i = st; i < en; i++) {
		nxt[i-st] = GetNext(i);
	}
	go();
	if (st < en) {
		for (ll i = 0; i < R; i++) {
			PutLL(0, convert_dest(dp[(st + i) - st]));
		}
		for (ll i = 0; i < R; i++) {
			PutLL(0, convert_dest(dp[(en - R + i) - st]));
		}
	} else {
		for (ll i = 0; i < R; i++) {
			PutLL(0, (2 * ID + 2) * R + i);
		}
		for (ll i = 0; i < R; i++) {
			PutLL(0, (2 * ID - 1) * R + i);
		}
	}
	PutLL(0, res);
	Send(0);
}

void phase2() {
	if (ID) return;
	res = 0;
	st = 0, en = NODES * 2 * R;
	for (ll z = 0; z < NODES; z++) {
		int n = Receive(-1);
		for (ll i = 0; i < R; i++) {
			nxt[2 * n * R + i] = GetLL(n);
		}
		for (ll i = 0; i < R; i++) {
			nxt[2 * n * R + R + i] = GetLL(n);
		}
		res += GetLL(n);
	}
	go();
	cout << res << '\n';
}

int main() {
	phase1();
	phase2();
	return 0;
}
