#include "message.h"
#ifndef INPUT_HEADER
#define INPUT_HEADER "nanobots.h"
#endif
#include INPUT_HEADER
#include<bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef pair<ll, ll> pll;

const ll MOD = ll(1e9) + 7;

const int NODES = NumberOfNodes();
const int ID = MyNodeId();

const ll R = GetRange();

ll search_y(ll x) {
	ll mi = 0, ma = R+1;
	while (ma - mi > 1) {
		ll md = (mi + ma) / 2;
		if (Experiment(x, md) == 'T') {
			mi = md;
		} else {
			ma = md;
		}
	}
	return mi;
}

ll search_x(ll y) {
	ll mi = 0, ma = R+1;
	while (ma - mi > 1) {
		ll md = (mi + ma) / 2;
		if (Experiment(md, y) == 'T') {
			mi = md;
		} else {
			ma = md;
		}
	}
	return mi;
}

// inclusive
pair<ll, ll> get_sum(ll a, ll b) {
	b = min(b, R);
	ll res = 0;
	ll x = a;
	int cnt = 0;
	while (x <= b) {
		ll y = search_y(x);
		if (y == 0) {
			x = b + 1;
			break;
		}
		ll nx = min(search_x(y), b);
		res += (nx - x + 1) % MOD * (y % MOD);
		res %= MOD;
		x = nx + 1;
		cnt++;
		if (cnt >= 1e5) {
			break;
		}
	}
	return pair<ll, ll>(res, x);
}

void go_master() {
	ll res = 0;
	vector<pll> ranges;
	ranges.emplace_back(1, R);
	// while someone's working or we have more ranges
	while (!ranges.empty()) {
		if (int(ranges.size()) < NODES - 1) {
			sort(ranges.begin(), ranges.end(), [] (const pll &a, const pll &b) {
					return a.second - a.first < b.second - b.first;
					});
			pll x = ranges.back();
			ll a = x.first, b = x.second;
			ranges.pop_back();
			int extra = NODES - 1 - int(ranges.size());
			for (; extra; extra--) {
				ll l = b + 1 - a;
				ll en = a + l / extra;
				if (a < en) ranges.emplace_back(a, en - 1);
				a = en;
			}
			assert(a > b);
		}
		/*
		for (pll it : ranges) {
			cerr << it.first << ' ' << it.second << '\n';
		}
		*/
		int tasks = int(ranges.size());
		assert(tasks <= NODES - 1);
		for (int i = 0; i < tasks; i++) {
			int id = i + 1;
			PutChar(id, false);
			PutLL(id, ranges[i].first);
			PutLL(id, ranges[i].second);
			Send(id);
		}
		ranges.clear();
		for (int i = 0; i < tasks; i++) {
			int id = Receive(-1);
			res += GetLL(id), res %= MOD;
			ll a = GetLL(id);
			ll b = GetLL(id);
			if (a <= b) {
				ranges.emplace_back(a, b);
			}
			//cerr << "got from " << id << '\n';
		}
	}
	cout << res << '\n';
}

void go_slave() {
	while(true) {
		Receive(0);
		bool stop = GetChar(0);
		if (stop) break;

		ll a = GetLL(0);
		ll b = GetLL(0);

		pll res = get_sum(a, b);

		PutLL(0, res.first);
		PutLL(0, res.second);
		PutLL(0, b);

		Send(0);
	}
}

int main() {
	if (ID == 0) {
		go_master();
		for (int i = 1; i < NODES; i++) {
			PutChar(i, true);
			Send(i);
		}
	} else {
		go_slave();
	}
	return 0;
}
