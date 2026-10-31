#include <bits/stdc++.h>

int64_t solve_prime_pow_nonzero(int64_t z, int64_t e, const std::vector<int64_t>& p_pows) {
	assert(e >= 1);
	z %= p_pows[e];
	int64_t p = p_pows[1];
	if (p == 2) {
		if (e == 1) {
			if (z == 0) return 1;
			else if (z == 1) return 2;
			else assert(false);
		} else if (e == 2) {
			if (z == 0) return 0;
			else if (z == 1) return 8;
			else if (z == 2) return 4;
			else if (z == 3) return 0;
			else assert(false);
		} else if (e >= 3) {
			// we get a factor of 16
			if (z % 8 == 1) return p_pows[e-3] * 16;
			else if (z % 8 == 2) return p_pows[e-3] * 16;
			else if (z % 8 == 5) return p_pows[e-3] * 16;
			else return 0;
		} else assert(false);
	} else {
		if (z % p == 0) {
			return (p % 4 == 1 ? 2*p-2 : 0) * p_pows[e-1];
		} else {
			return (p % 4 == 1 ? p-1 : p+1) * p_pows[e-1];
		}
	}
}

int64_t solve_prime_pow(int64_t z, const std::vector<int64_t>& p_pows) {
	int e = int(p_pows.size()) - 1;
	assert(e >= 1);
	int64_t p = p_pows[1];

	int64_t ans = 0;
	for (int x_e = 0; x_e < e; x_e++) {
		if (z % p_pows[std::min(2 * x_e, e)] != 0) {
			break;
		} else if (2 * x_e >= e) {
			// this is silly, we just pick anything that's nonzero
			ans += p_pows[e - x_e - 1] * p_pows[e - x_e - 1] * (p * p - 1);
		} else {
			// 2 * cur_e < e
			int digs = e - 2 * x_e;
			int num_extra = e - x_e - digs;

			// we care about digs things
			ans += p_pows[num_extra] * p_pows[num_extra] * solve_prime_pow_nonzero(z / p_pows[2 * x_e], digs, p_pows);
		}
	}
	if (z % p_pows[e] == 0) {
		// cur_e == e has 1 way
		ans++;
	}
	return ans;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int64_t M, CASES; cin >> M >> CASES;

	std::vector<std::vector<int64_t>> pfac;
	for (int64_t p = 2; p <= M; p++) {
		if (p * p > M) p = M;
		if (M % p == 0) {
			pfac.emplace_back(1, 1);
			int64_t q = 1;
			while (M % p == 0) {
				M /= p;
				q *= p;
				pfac.back().push_back(q);
			}
			assert(pfac.back().size() >= 2);
		}
	}

	for (int c = 0; c < CASES; c++) {
		int64_t z; cin >> z;

		int64_t ans = 1;
		for (const auto& p_pows : pfac) {
			// let's avoid any stupid overwriting shenanigans
			ans *= solve_prime_pow(z, p_pows);
		}
		cout << ans << '\n';
	}
	return 0;
}
