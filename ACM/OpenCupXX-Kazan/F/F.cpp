#include<bits/stdc++.h>
using namespace std;

void wssert(bool b) {
	if (!b) {
		cout << "NO SAD\n";
		exit(0);
	}
}

using ll = long long;

const int MAXN = 3.1e5;
int N;
ll W[MAXN];
set<pair<ll, int>> evts[MAXN];

int par[MAXN];
int getPar(int a) {
	return (par[a] == -1) ? a : (par[a] = getPar(par[a]));
}

const int MAXM = 3.1e5;
int M;
int A[MAXM][2];
ll S[MAXM];
ll evtSum[MAXM];

set<int> ready;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N >> M;
	for (int i = 0; i < N; i++) {
		cin >> W[i];
	}

	for (int e = 0; e < M; e++) {
		cin >> A[e][0] >> A[e][1] >> S[e];
		A[e][0] --, A[e][1] --;
	}

	memset(par, -1, sizeof(par));

	auto checkEdge = [&](int e) {
		int a = getPar(A[e][0]);
		int b = getPar(A[e][1]);
		assert(a != b);
		ll rem = S[e] - (W[a] + W[b]);
		if (rem <= 0) {
			ready.insert(e);
		} else {
			ll va = W[a] + (rem+1) / 2;
			ll vb = W[b] + (rem+1) / 2;
			evts[a].emplace(va, e);
			evts[b].emplace(vb, e);
			evtSum[e] = va + vb;
		}
	};

	for (int e = 0; e < M; e++) {
		checkEdge(e);
	}

	auto otherEvt = [&](pair<ll, int> evt) {
		return pair<ll, int>(evtSum[evt.second] - evt.first, evt.second);
	};
	auto checkEvts = [&]() {
		for (int i = 0; i < N; i++) {
			for (auto evt : evts[i]) {
				int evtE = evt.second;
				assert(i == getPar(A[evtE][0]) || i == getPar(A[evtE][1]));
				assert(getPar(A[evtE][0]) != getPar(A[evtE][1]));
				int o = getPar(A[evtE][0]) ^ getPar(A[evtE][1]) ^ i;
				assert(i != o);
				auto nevt = otherEvt(evt);
				wssert(evts[o].count(nevt));
			}
		}
	};

	vector<int> ans;
	while (!ready.empty()) {
		int e = *ready.begin();
		ready.erase(ready.begin());

		int a = getPar(A[e][0]);
		int b = getPar(A[e][1]);
		if (a == b) {
			continue;
		}
		ans.push_back(e);

		if (evts[a].size() < evts[b].size()) {
			swap(a,b);
		}
		assert(evts[a].size() >= evts[b].size());

		par[b] = a;
		W[a] += W[b];
		for (auto evt : evts[b]) {
			auto nevt = otherEvt(evt);
			if (evts[a].count(nevt)) {
				// they're merged, so it's obsolete
				evts[a].erase(nevt);
			} else {
				evts[a].insert(evt);
			}
		}
		evts[b] = {};

		while (!evts[a].empty() && W[a] >= evts[a].begin()->first) {
			auto evt = *evts[a].begin();
			int evtE = evt.second;
			assert(a == getPar(A[evtE][0]) || a == getPar(A[evtE][1]));
			assert(getPar(A[evtE][0]) != getPar(A[evtE][1]));
			int o = getPar(A[evtE][0]) ^ getPar(A[evtE][1]) ^ a;
			assert(a != o);
			auto nevt = otherEvt(evt);
			assert(evts[a].count(evt));
			evts[a].erase(evt);
			assert(evts[o].count(nevt));
			evts[o].erase(nevt);

			checkEdge(evtE);
		}
	}

	cout << ans.size() << '\n';
	for (int i = 0; i < int(ans.size()); i++) {
		cout << ans[i]+1 << " \n"[i+1==int(ans.size())];
	}

	return 0;
}
