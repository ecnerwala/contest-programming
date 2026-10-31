#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	const int V = 1e6;
	vector<int> primes; primes.reserve(V);
	vector<int> pfac(V+1);
	for (int i = 2; i <= V; i++) {
		if (!pfac[i]) {
			primes.push_back(i);
			pfac[i] = i;
		}
		for (int p : primes) {
			if (p > V / i) break;
			pfac[i*p] = p;
			if (p == pfac[i]) break;
		}
	}

	auto is_prime = [&](int a) -> bool {
		if (a <= V) return pfac[a] == a;
		for (int p : primes) {
			if (p * p > a) break;
			if (a % p == 0) return false;
		}
		return true;
	};

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {

		int64_t Z; cin >> Z;
		// one of the primes is <= sqrt(Z)
		// so it's either prev_prime(sqrt(Z)) .. next_prime(sqrt(Z))
		// or it's prev_prime_2(sqrt(Z)) .. prev_prime(sqrt(Z))
		int RT;
		{
			int mi = 0;
			int ma = int(1e9)+1;
			while (ma - mi > 1) {
				int md = (mi + ma) / 2;
				if (int64_t(md) * int64_t(md) <= Z) mi = md;
				else ma = md;
			}
			RT = mi;
		}
		int p1 = RT;
		while (!is_prime(p1)) p1--;
		int p2 = RT+1;
		while (!is_prime(p2)) p2++;

		cout << "Case #" << case_num << ": ";
		if (int64_t(p1) * int64_t(p2) <= Z) {
			cout << int64_t(p1) * int64_t(p2) << '\n';
		} else {
			int p0 = p1-1;
			while (!is_prime(p0)) p0--;
			cout << int64_t(p1) * int64_t(p0) << '\n';
		}
	}

	return 0;
}
