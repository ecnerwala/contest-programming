#include "message.h"
#ifndef INPUT_HEADER
#define INPUT_HEADER "nanobots.h"
#endif
#include INPUT_HEADER
#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

const ll MOD = ll(1e9) + 7;

const int NODES = NumberOfNodes();
const int ID = MyNodeId();

ll split (ll a, ll i, ll n) {
	assert(a >= 0);
	assert(0 <= i && i <= n);
	return (a / n) * i + min(a % n, i);
}

const ll N = GetRange();
const ll BOTS = GetNumNanobots();
const ll K = 5;
const ll LIM = (BOTS + NODES/K - 1) / (NODES / K);

int main() {
	ll res = 0;

	ll st = split(N, ID, NODES) + 1;
	ll en = split(N, ID+1, NODES) + 1;

	for (int phase = 0; true; phase ++) {
		cerr << phase << ' ' << st << ' ' << en << '\n';
		assert(st > 0);
		assert(st <= en);
		assert(en <= N+1);

		ll hi = N+1;
		ll lo = 0;
		int num_cycs = 0;
		while (st < en) {
			while (hi - lo > 1) {
				ll v = (lo + hi) / 2;
				if (Experiment(st, v) == 'E') {
					hi = v;
				} else {
					lo = v;
				}
			}
			ll l = st;
			ll r = st+1;
			if (lo == 0) {
				r = en, l = en-1;
			} else {
				while (r < en && Experiment(r, lo) == 'T') {
					r = min(r + (r-l), en);
				}
				while (r - l > 1) {
					ll m = (l + r) / 2;
					if (Experiment(m, lo) == 'T') {
						l = m;
					} else {
						r = m;
					}
				}
			}
			assert(r > st);
			res += (r - st) % MOD * (lo % MOD);
			res %= MOD;
			st = r;
			lo = 0;
			num_cycs ++;
			if (num_cycs > LIM) break;
		}
		assert(st == en || num_cycs > LIM);

		int master = phase % NODES;
		{
			PutLL(master, st);
			PutLL(master, en);
			Send(master);
		}
		if (ID == master) {
			vector<pair<ll, ll>> todo;
			for (int i = 0; i < NODES; i++) {
				Receive(i);
				ll a = GetLL(i);
				ll b = GetLL(i);
				assert(a <= b);
				if (a == b) continue;
				todo.emplace_back(a,b);
			}
			assert(int(todo.size()) <= NODES/K);
			vector<pair<ll, ll>> jobs;
			if (todo.empty()) {
				for (int i = 0; i < NODES; i++) {
					jobs.emplace_back(-1, -1);
				}
			} else {
				ll k = NODES / int(todo.size());
				assert(k >= K);
				assert(k * int(todo.size()) <= NODES);
				for (auto it : todo) {
					ll a = it.first, b = it.second;
					for (int i = 0; i < k; i++) {
						jobs.emplace_back(split(b-a, i, k) + a, split(b-a,i+1, k) + a);
					}
				}
				assert(int(jobs.size()) <= NODES);
				while (int(jobs.size()) < NODES) {
					jobs.emplace_back(N+1,N+1);
				}
			}
			assert(int(jobs.size()) == NODES);
			for (int i = 0; i < NODES; i++) {
				PutLL(i, jobs[i].first);
				PutLL(i, jobs[i].second);
				Send(i);
			}
		}
		Receive(master);
		st = GetLL(master);
		en = GetLL(master);
		if (st == -1) {
			break;
		}
	}

	if (ID != 0) {
		PutLL(0, res);
		Send(0);
	} else {
		for (int i = 1; i < NODES; i++) {
			Receive(i);
			res += GetLL(i);
			res %= MOD;
		}
		cout << res << '\n';
	}

	return 0;
}
