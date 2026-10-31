#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int B; int64_t M; int Q; cin >> B >> M >> Q;

	vector<int> primes;
	for (int p = 2; B > 1; p++) {
		if (int64_t(p) * p > B) p = B;
		if (B % p == 0) {
			primes.push_back(p);
			while (B % p == 0) B /= p;
		}
	}

	vector<int64_t> vals; vals.reserve(3e6);
	priority_queue<int64_t, vector<int64_t>, greater<int64_t>> pq;
	pq.push(1);
	while (!pq.empty()) {
		int64_t cur = pq.top(); pq.pop();
		//assert(cur <= M);
		//assert(vals.empty() || cur > vals.back());
		vals.push_back(cur);

		int64_t v = cur;
		for (int p : primes) {
			if (p > M / cur) break;
			while (v % p == 0) v /= p;
			if (v == 1) {
				pq.push(cur * p);
			}
		}
	}

	for (int q = 0; q < Q; q++) {
		int64_t a; cin >> a;
		auto it = lower_bound(vals.begin(), vals.end(), a);
		if (it == vals.end()) {
			cout << -1 << '\n';
		} else {
			cout << (*it) - a << '\n';
		}
	}

	return 0;
}
