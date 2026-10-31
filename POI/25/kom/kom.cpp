#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T; T *= 2;
	vector<int> A(T);
	for (int i = 0; i < T; i += 2) {
		cin >> A[i] >> A[i+1];
		A[i]--;
	}

	for (auto& a : A) {
		if (a == int(1e1) || a == int(1e2) || a == int(1e3) || a == int(1e4) || a == int(1e5) || a == int(1e6) || a == int(1e7) || a == int(1e8) || a == int(1e9)) a--;
	}

	int max_A = *max_element(A.begin(), A.end());
	int V = 1;
	while (V < int(1e8) && V <= max_A) V *= 10;

	V--;

	V = min(V, int(1e8) / 6);

	vector<int> primes; primes.reserve(V+1);
	vector<int> pfac(V+1);
	vector<int> cnt(int(1e8)+1);
	cnt[1] = 1;
	for (int i = 2; i <= V; i++) {
		if (!pfac[i]) {
			primes.push_back(i);
			int p = i;
			for (int x = 1; x <= V / p; x *= p) {
				pfac[x * p] = p;
				cnt[x * p] = cnt[x] + 1;
			}
		}
		for (int p : primes) {
			if (p > V / i || p == pfac[i]) break;
			for (int x = i; x <= V / p; x *= p) {
				pfac[x * p] = p;
				cnt[x * p] = cnt[x] + cnt[i];
			}
		}
	}

	// Length 9: squares of semiprimes and 11^8 and 13^8
	vector<int> v9;
	for (int i = int(1e4); i * i <= max_A; i++) {
		if (cnt[i] == 4 && pfac[i / pfac[i]] == i / pfac[i]) {
			v9.push_back(i * i);
		}
	}
	{
		int p11 = 11 * 11; p11 *= p11; p11 *= p11;
		v9.insert(lower_bound(v9.begin(), v9.end(), p11), p11);
		int p13 = 13 * 13; p13 *= p13; p13 *= p13;
		v9.insert(lower_bound(v9.begin(), v9.end(), p13), p13);
	}

	cnt[0] = 0;
	for (int p10 = 1, l = 1; p10 < V+1 && p10 < int(1e7); p10 *= 10, l++) {
		for (int i = p10; i < 10*p10; i++) {
			cnt[i] = cnt[i] == l;
			cnt[i] += cnt[i-1];
		}
	}

	const int P7 = int(1e7)-1;
	const int P8 = int(1e8)-1;
	if (max_A > P7) {
		for (int i = P7+1; i <= P8; i++) {
			cnt[i] = 0;
		}

		// Case 1: pqr
		for (int i = 0; i < int(primes.size()); i++) {
			for (int j = i+1; j+1 < int(primes.size()) && primes[j+1] <= P8 / primes[i] / primes[j]; j++) {
				for (auto it = upper_bound(primes.begin(), primes.end(), max(primes[j], P7 / primes[i] / primes[j])); it != primes.end() && *it <= P8 / primes[i] / primes[j]; ++it) {
					cnt[primes[i]*primes[j]**it] = 1;
				}
			}
		}
		// Case 2: p^3 q
		for (int i = 0; i < int(primes.size()); i++) {
			int p = primes[i];
			if (p * p * p > P8) break;
			for (auto it = upper_bound(primes.begin(), primes.end(), P7 / p / p / p); it != primes.end() && *it <= P8 / p / p / p; ++it) {
				if (*it == p) continue;
				cnt[p*p*p**it] = 1;
			}
		}
		// Case 3: p^7
		cnt[11 * 11 * 11 * 11 * 11 * 11 * 11] = 1;
		cnt[13 * 13 * 13 * 13 * 13 * 13 * 13] = 1;

		for (int i = P7+1; i <= P8; i++) {
			cnt[i] += cnt[i-1];
		}
	}

	vector<int> ans(T);
	for (int i = 0; i < T; i++) {
		if (A[i] <= P8) {
			ans[i] = cnt[A[i]];
		} else {
			ans[i] = cnt[P8] + int(upper_bound(v9.begin(), v9.end(), A[i]) - v9.begin());
		}
	}

	for (int i = 0; i < T; i += 2) {
		cout << ans[i+1] - ans[i] << '\n';
	}

	return 0;
}
