#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	const int MAXN = int(1e9) + 20;
	std::vector<int> pfac(MAXN+1);
	std::vector<int> primes; primes.reserve(MAXN / 2);
	for (int i = 2; i < MAXN; i++) {
		if (!pfac[i]) {
			primes.push_back(i);
			pfac[i] = i;
		}
		for (int p : primes) {
			if (int64_t(i) * int64_t(p) > MAXN) break;
			pfac[i*p] = p;
			if (p == pfac[i]) break;
		}
	}
	cerr << "Sieve done" << '\n';
	auto get_exp = [&](int n, int p) -> int {
		int r = 0;
		while (n >= p) {
			n /= p;
			r += n;
		}
		return r;
	};

	auto get_m8 = [&](int n) -> int {
		int r = 1;
		for (; n; n /= 2) {
			for (int z = 1; z <= (n % 8); z += 2) {
				r *= z;
				r %= 8;
			}
		}
		return r;
	};

	auto solve = [&](int R, int L) -> int {
		assert(0 <= L && L <= R);
		// R! / L!
		auto is_1 = [&]() -> bool {
			for (int p : primes) {
				if (p > R) break;
				if ((get_exp(R, p) - get_exp(L, p)) & 1) {
					return false;
				}
			}
			return true;
		};
		if (is_1()) return 1;

		auto is_2 = [&]() -> bool {
			for (int p : primes) {
				if (p > R) break;
				if (p % 4 != 3) continue;
				if ((get_exp(R, p) - get_exp(L, p)) & 1) {
					return false;
				}
			}
			return true;
		};
		if (is_2()) return 2;

		auto is_3 = [&]() -> bool {
			if ((get_exp(R, 2) - get_exp(L, 2)) & 1) {
				return true;
			}
			// check the value mod 8
			if ((get_m8(R) + get_m8(L)) % 8 == 0) {
				return false;
			}
			return true;
		};
		if (is_3()) return 3;

		return 4;
	};

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {
		int N, K; cin >> N >> K;
		cerr << "Case #" << case_num << ": " << N << ' ' << K << '\n';

		if (N == K) {
			cout << "Case #" << case_num << ": " << 1 << '\n';
			continue;
		}

		// (N-1)! / K!

		int ans = solve(N-1, K);
		cerr << "Case #" << case_num << ": " << ans << '\n';
		cout << "Case #" << case_num << ": " << ans << '\n';
	}

	return 0;
}
