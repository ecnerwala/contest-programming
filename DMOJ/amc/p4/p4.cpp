#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;

	auto get_area = [](double v) -> double {
		assert(-1 <= v && v <= 1);
		return std::acos(-v) + v * std::sqrt(1 - v * v);
	};

	for (int i = 1; i < N; i++) {
		double tgt = double(i) / double(N) * M_PI;
		double mi = -1;
		double ma = 1;
		for (int z = 0; z < 50; z++) {
			double md = mi + std::pow(0.5, z);
			if (get_area(md) < tgt) mi = md;
			else ma = md;
		}
		cout << fixed << setprecision(20) << ma << '\n';
	}

	return 0;
}
