#include <bits/stdc++.h>

std::optional<std::array<std::string, 3>> solve(int64_t A, int64_t B, int64_t C) {
	int64_t G = std::gcd(std::gcd(A, B), C);
	A /= G;
	B /= G;
	C /= G;
	if (std::lcm(A, B) % C != 0 || std::lcm(B, C) % A != 0 || std::lcm(C, A) % B != 0) {
		return std::nullopt;
	}
	int64_t p = std::gcd(A, B);
	int64_t q = std::gcd(B, C);
	int64_t r = std::gcd(C, A);

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
		return __builtin_ctzll(a) & 1;
	};
	for (int g = 0; g < G; g++) {
		for (int i = 0; i < A; i++) {
			res[0][i*G+g] = "01"[tm(i % p) ^ tm(i % q) ^ tm(g)];
		}
		for (int j = 0; j < B; j++) {
			res[1][j*G+g] = "01"[tm(j % q) ^ tm(j % r) ^ tm(g)];
		}
		for (int k = 0; k < C; k++) {
			res[2][k*G+g] = "01"[tm(k % r) ^ tm(k % p) ^ tm(g)];
		}
	}
	return res;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

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
