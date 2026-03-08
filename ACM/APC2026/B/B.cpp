#include <bits/stdc++.h>

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	int N; std::cin >> N;
	std::vector<int> P(N);
	P[0] = -1;
	for (int i = 1; i < N; i++) {
		std::cin >> P[i]; P[i]--;
	}

	std::vector<bool> is_leaf(N, true);
	for (int i = 1; i < N; i++) is_leaf[P[i]] = false;

	std::vector<int> vals(N);
	auto can_geq = [&](int v) -> bool {
		for (int i = N-1; i >= 0; i--) {
			if (is_leaf[i]) {
				vals[i] = (i >= v) ? 1 : -1;
			} else {
				vals[i] = (vals[i] > 0) - (vals[i] < 0);
			}
			if (i > 0) {
				vals[P[i]] += vals[i];
			}
		}
		std::cerr << "test " << v << '\n';
		for (auto x : vals) { std::cerr << x << ' '; } std::cerr << '\n';
		return vals[0] >= 0;
	};

	int mi = 0;
	int ma = N;
	while (ma - mi > 1) {
		int md = (mi + ma) / 2;
		if (can_geq(md)) mi = md;
		else ma = md;
	}
	std::cout << (mi+1) << '\n';

	return 0;
}
