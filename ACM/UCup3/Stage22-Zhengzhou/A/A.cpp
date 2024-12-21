#include <bits/stdc++.h>

using std::cerr;

std::optional<std::array<std::string, 3>> solve(int64_t A, int64_t B, int64_t C) {
	if (A == 2 && B == 2 && C == 2) return std::nullopt;
	if (A == 1 && B == 1 && C == 1) return std::array<std::string, 3>{"0", "0", "0"};
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

	int vp;
	int vq;
	int vr;
	if (p*G >= 3) {
		vp = 2;
		if (q*G >= 2) vq = 1, vr = -1;
		else vr = 1, vq = -1;
	} else if (q*G >= 3) {
		vq = 2;
		if (p*G >= 2) vp = 1, vr = -1;
		else vp = 1, vr = -1;
	} else if (r*G >= 3) {
		vr = 2;
		if (p*G >= 2) vp = 1, vq = -1;
		else vp = 1, vq = -1;
	} else {
		if (p*G == 2) vp = 1, vq = -1, vr = -1;
		else if (q*G == 2) vq = 1, vp = -1, vr = -1;
		else if (r*G == 2) vr = 1, vp = -1, vq = -1;
		else vp = -1, vq = -1, vr = -1;
	}
	//cerr << vp << ' ' << vq << ' ' << vr << '\n';

	auto fp = [&](int64_t a) -> int {
		return a == 0;
	};
	auto fq = [&](int64_t a) -> int {
		return a == 0;
	};
	auto fr = [&](int64_t a) -> int {
		return a == 0;
	};
	for (int i = 0; i < A*G; i++) {
		res[0][i] = "01"[fp(i % (p*G)) ^ fq(i % (q*G))];
	}
	for (int j = 0; j < B*G; j++) {
		res[1][j] = "01"[fq(j % (q*G)) ^ fr(j % (r*G))];
	}
	for (int k = 0; k < C*G; k++) {
		res[2][k] = "01"[fr(k % (r*G)) ^ fp(k % (p*G))];
	}
	return res;
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
	test();

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
