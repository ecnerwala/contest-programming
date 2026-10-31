#include "job.h"
#include <bits/stdc++.h>
using namespace std;

using ll = long long;

ll scheduling_cost(vector<int> P, vector<int> U_, vector<int> D_) {
	vector<ll> U(U_.begin(), U_.end());
	vector<ll> D(D_.begin(), D_.end());
	int N = int(P.size());
	ll ans = 0;
	for (int i = 0; i < N; i++) {
		ans += U[i] * D[i];
	}

	// build things earlier with high U[i] / D[i]
	auto cmp = [](pair<pair<ll, ll>, int> i, pair<pair<ll, ll>, int> j) {
		// returns true if j comes first
		return j.first.second * i.first.first < i.first.second * j.first.first;
	};

	priority_queue<pair<pair<ll, ll>, int>, vector<pair<pair<ll, ll>, int>>, decltype(cmp)> pq(cmp);
	vector<char> dead(N);

	for (int i = 1; i < N; i++) {
		if (U[i] == 0 && D[i] == 0) {
			dead[i] = true;
		} else {
			pq.emplace(pair<ll, ll>(U[i], D[i]), i);
		}
	}

	int cnt = 0;
	while (!pq.empty()) {
		auto it = pq.top(); pq.pop();
		int cur = it.second;
		if (dead[cur] || it.first != pair<ll, ll>(U[cur], D[cur])) continue;

		int prv = P[cur];
		while (dead[prv]) {
			assert(0 <= P[prv] && P[prv] < prv);
			prv = P[prv];
		}

		for (int v = P[cur]; v != prv; ) {
			int nxt = P[v];
			P[v] = prv;
			v = nxt;
		}

		ans += D[prv] * U[cur];
		U[prv] += U[cur];
		D[prv] += D[cur];
		dead[cur] = true;
		if (prv != 0) {
			pq.emplace(pair<ll, ll>(U[prv], D[prv]), prv);
		}
		cnt++;
	}
	return ans;
}
