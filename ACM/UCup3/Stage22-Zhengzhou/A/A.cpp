#include <bits/stdc++.h>

using std::cerr;

std::optional<std::array<std::string, 3>> solve(int64_t A, int64_t B, int64_t C) {
	std::array<std::string, 3> res;
	res[0] = "0";
	res[1] = "0";
	res[2] = "0";

	for (int p = 2; p <= std::max({A, B, C}); p++) {
		if (A % p == 0 || B % p == 0 || C % p == 0) {
			int eA = 0; while (A % p == 0) { A /= p, eA++; }
			int eB = 0; while (B % p == 0) { B /= p, eB++; }
			int eC = 0; while (C % p == 0) { C /= p, eC++; }
			auto pow_p = [&](int e) -> int {
				int v = 1;
				for (int z = 0; z < e; z++) v *= p;
				return v;
			};

			std::array<std::pair<int, int>, 3> vals{std::pair<int, int>{eA, 0}, {eB, 1}, {eC, 2}};
			std::sort(vals.begin(), vals.end());
			if (vals[1].first != vals[2].first) return std::nullopt;
			int z_mi = vals[0].second;
			int z_ma_x = vals[1].second;
			int z_ma_y = vals[2].second;

			int e_mi = vals[0].first;
			int e_ma = vals[1].first;

			int p_mi = pow_p(e_mi);
			int p_ma = pow_p(e_ma);

			if (p == 2 && e_mi == 1 && e_ma == 1) return std::nullopt;

			std::array<std::string, 3> pres;
			pres[0].assign(pow_p(eA), '0');
			pres[1].assign(pow_p(eB), '0');
			pres[2].assign(pow_p(eC), '0');

			if (p_ma == 2) {
				return std::nullopt;
			}
			pres[z_mi][1] = '1';
			pres[z_ma_x][0] = '1';
			pres[z_ma_y][0] = '1';
			pres[z_ma_y][1] = '1';

			for (int z = 0; z < 3; z++) {
				std::string x; x.resize(res[z].size() * pres[z].size());
				for (int i = 0; i < int(x.size()); i++) {
					x[i] = char(res[z][i % int(res[z].size())] ^ pres[z][i % int(pres[z].size())] ^ '0');
				}
				res[z] = std::move(x);
			}
		}
	}
	return res;
	/*
	int64_t G = std::gcd(std::gcd(A, B), C);
	A /= G;
	B /= G;
	C /= G;
	if (std::lcm(A, B) % C != 0 || std::lcm(B, C) % A != 0 || std::lcm(C, A) % B != 0) {
		return std::nullopt;
	}

	int64_t p = std::gcd(C, A);
	int64_t q = std::gcd(A, B);
	int64_t r = std::gcd(B, C);

	assert(std::gcd(p, q) == 1);
	assert(std::gcd(q, r) == 1);
	assert(std::gcd(r, p) == 1);
	assert(A == p * q);
	assert(B == q * r);
	assert(C == r * p);

	std::array<std::string, 3> res;
	res[0].resize(A*G);
	res[1].resize(B*G);
	res[2].resize(C*G);

	auto tm = [&](int64_t a) -> int {
		return __builtin_popcountll(a) & 1;
	};
	for (int g = 0; g < G; g++) {
		for (int i = 0; i < A; i++) {
			res[0][i*G+g] = "01"[tm(i % p) ^ tm(i % q) ^ tm(g % 3 != 0)];
		}
		for (int j = 0; j < B; j++) {
			res[1][j*G+g] = "01"[tm(j % q) ^ tm(j % r) ^ tm(g % 3 != 1)];
		}
		for (int k = 0; k < C; k++) {
			res[2][k*G+g] = "01"[tm(k % r) ^ tm(k % p) ^ tm(g % 3 != 2)];
		}
	}
	return res;
	*/
}

bool has_period(std::string S) {
	int N = int(S.size());
	for (int i = 1; i < N; i++) {
		if (N % i == 0) {
			bool good = true;
			for (int j = 0; j + i < N; j++) {
				if (S[j] != S[j + i]) { good = false; break; }
			}
			if (good) return true;
		}
	}
	return false;
}

void test() {
	for (int A = 1; A <= 100; A++) {
		for (int B = 1; B <= A; B++) {
			for (int C = 1; C <= B; C++) {
				auto res = solve(A, B, C);
				if (res) {
					const auto& v = *res;
					if (has_period(v[0]) || has_period(v[1]) || has_period(v[2])) {
						cerr << "bad" << '\n';
						cerr << A << ' ' << B << ' ' << C << '\n';
						cerr << v[0] << '\n';
						cerr << v[1] << '\n';
						cerr << v[2] << '\n';
						assert(false);
					}
				}
			}
		}
		cerr << "tested " << A << '\n';
	}
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	//test();

	int T; cin >> T;
	while (T--) {
		int64_t A, B, C; cin >> A >> B >> C;
		auto res = solve(A, B, C);
		if (res) {
			cout << "YES" << '\n';
			for (const auto& s : *res) cout << s << '\n';
		} else {
			cout << "NO" << '\n';
		}
	}

	return 0;
}
