#include "shortcut.h"
#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

const ll INF = 1e18;

struct shortcut {
	static const int MAXN = 1e6 + 200;
	int N;
	ll X[MAXN], Y[MAXN];
	ll C;
	ll max_minus[MAXN];
	ll max_plus[MAXN];

	ll V;
	ll misum, masum, midif, madif;

	// i is first unreachable from j, or -1 if none
	inline void update(int i, int j) {
		i++;
		misum = max(misum, (C-V) + max_plus[i] + (Y[j] + X[j]));
		midif = max(midif, (C-V) + max_minus[i] + (Y[j] + X[j]));
		masum = min(masum, (V-C) - max_minus[i] - (Y[j] - X[j]));
		madif = min(madif, (V-C) - max_plus[i] - (Y[j] - X[j]));
	}
	
	inline bool do_sweep() {
		if(!(misum <= masum)) return false;
		if(!(midif <= madif)) return false;
		for(int i = 0, l = N; i < N; i++) {
			ll minv = max(misum - X[i], midif + X[i]);
			ll maxv = min(masum - X[i], madif + X[i]);
			while(l > 0 && X[l-1] >= minv) l--;
			while(l < N && X[l] < minv) l++;
			if(l < N && X[l] <= maxv) return true;
		}
		return false;
	}

	pair<ll, int> st[MAXN];

	bool is_good(ll v) {
		V = v;
		//cerr << V << '\n';
		for(int i = 0; i < N; i++) {
			if(Y[i] > V) return false;
		}
		misum = 0, masum = INF, midif = 0, madif = INF;
		int L = 0;
		// st is reverse sorted
		st[0] = pair<ll, int>(INF, -1);
		for(int j = 0; j < N; j++) {
			ll val = V  - (Y[j] + X[j]);
			// find the smallest thing in st > val
			int mi = 0, ma = L+1;
			while(ma - mi > 1) {
				int md = (mi + ma) / 2;
				if(st[md].first > val) mi = md;
				else ma = md;
			}
			int i;
			/*
			for(i = j - 1; i >= 0 && (Y[j] + X[j]) + (Y[i] - X[i]) <= V; i--) {}
			assert(i == st[mi].second);
			*/
			i = st[mi].second;
			//cerr << i << ' ' << j << '\n';
			update(i, j);
			val = (Y[j] - X[j]);
			while(st[L].first <= val) {
				L--;
			}
			st[++L] = pair<ll, int>(val, j);
		}
		return do_sweep();
	}

	void precomp() {
		max_minus[0] = max_plus[0] = -INF;
		for(int i = 0; i < N; i++) {
			max_minus[i+1] = max(max_minus[i], Y[i] - X[i]);
			max_plus[i+1] = max(max_plus[i], Y[i] + X[i]);
		}
	}

	ll go() {
		precomp();
		ll mi = 0, ma = INF;
		while(ma - mi > 1) {
			ll md = (mi + ma) / 2;
			if(is_good(md)) {
				ma = md;
			} else {
				mi = md;
			}
		}
		return ma;
	}

	shortcut(int n, vector<int> l, vector<int> d, int c)
		: N(n), C(c) {
		X[0] = 0;
		for(int i = 1; i < N; i++) {
			X[i] = X[i-1] + l[i-1];
		}
		for(int i = 0; i < N; i++) {
			Y[i] = d[i];
		}
	}
};

ll find_shortcut(int n, vector<int> l, vector<int> d, int c) {
	return shortcut(n, l, d, c).go();
}
