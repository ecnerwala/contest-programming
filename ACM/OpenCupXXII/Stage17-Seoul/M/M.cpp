#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::vector<int> P(N);
	for (auto& p : P) cin >> p;
	std::vector<int> Q(N);
	for (auto& q : Q) cin >> q;
	std::vector<int> X(N);
	std::vector<int> Y(N);
	for (int i = 0; i < N; i++) {
		X[i] = P[i] + Q[i];
		Y[i] = P[i] - Q[i];
	}

	auto get_val = [&](std::vector<int>& v) -> int64_t {
		std::sort(v.begin(), v.end());
		int64_t ans = 0;
		for (int i = 0; i < N; i++) {
			ans += int64_t(v[i]) * int64_t(2 * i - (N-1));
		}
		return ans * 2;
	};

	cout << get_val(P) + get_val(Q) - (get_val(X) + get_val(Y))/2 << '\n';

	return 0;
}
