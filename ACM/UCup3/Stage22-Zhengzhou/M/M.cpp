#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, K; cin >> N >> K;

	std::vector<int64_t> A(N);
	for (auto& a : A) cin >> a;

	using ld = long double;
	auto eval = [&](ld c) -> ld {
		ld sum = 0;
		for (auto a : A) {
			sum += ld(c * a) / ld(1 + c * a);
		}
		return sum;
	};
	ld mi = 0;
	ld ma = N;
	for (int z = 0; z <= 60; z++) {
		ld md = mi + (ma - mi) / 2;
		if (eval(md) >= K) ma = md;
		else mi = md;
	}

	cout << fixed << setprecision(20);
	ld c = mi;
	for (auto a : A) {
		cout << ld(c * a) / ld(1 + c * a) << '\n';
	}

	return 0;
}

// p/(1-p) proportional to a_i
