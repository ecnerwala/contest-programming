#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, K; cin >> N >> K;

	std::vector<int64_t> A(N);
	for (auto& a : A) cin >> a;

	using ld = long double;
	ld sum = 0;
	for (auto a : A) {
		sum += ld(a) / ld(1 + a);
	}
	ld coeff = K / sum;
	cout << fixed << setprecision(20);
	for (auto a : A) {
		cout << ld(a) / ld(1 + a) * coeff << '\n';
	}

	return 0;
}

// p/(1-p) proportional to a_i
