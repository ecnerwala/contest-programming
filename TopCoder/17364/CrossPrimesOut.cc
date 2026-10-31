#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

// https://github.com/kth-competitive-programming/kactl/blob/main/content/number-theory/MillerRabin.h
namespace kactl {
using ll = int64_t;
using ull = uint64_t;

ull modmul(ull a, ull b, ull M) {
	ll ret = a * b - M * ull(1.L / M * a * b);
	return ret + M * (ret < 0) - M * (ret >= (ll)M);
}
ull modpow(ull b, ull e, ull mod) {
	ull ans = 1;
	for (; e; b = modmul(b, b, mod), e /= 2)
		if (e & 1) ans = modmul(ans, b, mod);
	return ans;
}

bool isPrime(ull n) {
	if (n < 2 || n % 6 % 4 != 1) return (n | 1) == 3;
	ull A[] = {2, 325, 9375, 28178, 450775, 9780504, 1795265022},
	    s = __builtin_ctzll(n-1), d = n >> s;
	for (ull a : A) {   // ^ count trailing zeroes
		ull p = modpow(a%n, d, n), i = s;
		while (p != 1 && p != n - 1 && a % n && i--)
			p = modmul(p, p, n);
		if (p != n-1 && i != s) return 0;
	}
	return 1;
}
}

namespace ecnerwala {

class CrossPrimesOut {

public:

	int findTerm(int X, int N) {
		auto to_vec = [&](int64_t a) -> std::vector<int> {
			std::vector<int> res;
			for (int64_t v = a; v > 0; v /= 10) res.push_back(v % 10);
			std::reverse(res.begin(), res.end());
			return res;
		};
		std::vector<int> digs;
		std::vector<int> residual;
		{
			int R = 1;
			while (R * R <= X) R++;
			R--;
			assert(R * R != X);
			assert(R >= 1);

			residual = to_vec(X-R*R);
			digs = to_vec(R);
		}

		auto append_dig = [&]() {
			digs.push_back(0);
			residual.push_back(0);
			residual.push_back(0);
			while (true) {
				bool is_leq;
				assert(digs[0] > 0);
				int double_dig_len = int(digs.size()) + (digs.front() >= 5);
				if (double_dig_len != int(residual.size())) {
					is_leq = double_dig_len < int(residual.size());
				} else {
					is_leq = true;
					int c = 1;
					int i = 0;
					for (; i < int(digs.size()) || c > 0; i++) {
						int v = (i >= int(digs.size()) ? 0 : digs[int(digs.size())-1-i]) * 2 + c;
						c = v / 10;
						v %= 10;
						int rv = residual[int(residual.size())-1-i];
						if (v < rv) is_leq = true;
						else if (v > rv) is_leq = false;
					}
					assert(i == double_dig_len);
					assert(c == 0);
				}

				if (!is_leq) break;

				for (int i = 0, c = 1; i < double_dig_len; i++) {
					int v = (i >= int(digs.size()) ? 0 : digs[int(digs.size())-1-i]) * 2 + c;
					c = v / 10;
					v %= 10;
					assert(i < int(residual.size()));
					residual[int(residual.size())-1-i] -= v;
				}
				for (int i = int(residual.size())-1; i >= 0; i--) {
					if (residual[i] < 0) {
						residual[i] += 10;
						assert(i > 0);
						residual[i-1]--;
					}
				}
				while (residual.front() == 0) residual.erase(residual.begin());
				assert(!residual.empty());

				digs.back()++;
				assert(digs.back() <= 9);
			}
		};

		for (int z = 0; z < 1000; z++) {
			append_dig();
		}
		for (int d : digs) { cerr << d; } cerr << '\n';
		std::set<std::vector<int>> used_primes;
		int num_tokens = 0;
		int dig_idx = 0;
		int64_t p = 2;
		while (true) {
			while (!kactl::isPrime(p)) p++;
			std::vector<int> pdigs = to_vec(p);
			if (used_primes.count(pdigs)) {
				p++;
				continue;
			}
			cerr << "Looking for " << p << '\n';
			// now look for p
			int st = dig_idx;
			int en = st;
			while (true) {
				while (int(digs.size()) < en + int(pdigs.size())) append_dig();
				bool good = true;
				for (int z = 0; z < int(pdigs.size()); z++) {
					if (digs[en+z] != pdigs[z]) {
						good = false;
						break;
					}
				}
				if (good) break;
				en++;
			}
			dig_idx = en + int(pdigs.size());
			used_primes.insert(pdigs);

			std::vector<int> cur(digs.begin()+st, digs.begin()+en);
			for (int d : cur) { cerr << d; } cerr << '\n';
			for (int l = 1; l <= int(cur.size()); l++) {
				std::vector<std::pair<std::vector<int>, int>> cnds;
				for (int i = 0; i+l <= int(cur.size()); i++) {
					if (cur[i] == 0) continue;
					{
						bool good = true;
						for (int j = 0; j < l; j++) {
							if (cur[i+j] == -1) good = false;
						}
						if (!good) continue;
					}
					cnds.emplace_back(std::vector<int>(cur.begin()+i, cur.begin()+i+l), i);
				}
				std::sort(cnds.begin(), cnds.end());
				for (const auto& it : cnds) {
					int i = it.second;
					{
						bool good = true;
						for (int j = 0; j < l; j++) {
							if (cur[i+j] == -1) good = false;
						}
						if (!good) continue;
					}
					std::vector<int> cur_p(cur.begin()+i, cur.begin()+i+l);
					if (used_primes.count(cur_p)) continue;
					int64_t v = 0;
					for (int j = 0; j < l; j++) {
						v *= 10;
						v += cur[i+j];
					}
					if (!kactl::isPrime(v)) continue;
					used_primes.insert(cur_p);
					for (int j = 0; j < l; j++) {
						cur[i+j] = -1;
					}
				}
			}
			for (int i = 0; i < int(cur.size()); i++) {
				if (cur[i] == -1) cerr << ' ';
				else cerr << cur[i];
			}
			cerr << '\n';
			for (int i = 0; i < int(cur.size()); i++) {
				if (cur[i] == -1) continue;
				int64_t v = 0;
				for (; i < int(cur.size()) && cur[i] != -1; i++) {
					cerr << cur[i];
					v *= 10;
					v += cur[i];
					v %= int(1e9)+7;
				}
				cerr << '\n';
				num_tokens++;
				if (num_tokens == N+1) {
					return int(v);
				}
			}
		}
		assert(false);
	}
};

void my_tests() {
	// FILL IN TESTS
	for (int i = 1, j = 1; i <= 10000; i++) {
		if (i == j * j) {
			j++;
			continue;
		}
		cerr << "Running " << i << ' ' << 30 << '\n';
		CrossPrimesOut().findTerm(i, 30);
	}
}

}  // namespace ecnerwala

using ecnerwala::CrossPrimesOut;

// vim:ft=cpp
