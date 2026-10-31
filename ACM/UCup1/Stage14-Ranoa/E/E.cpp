#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::vector<double> angles; angles.reserve(2*N);
	for (int i = 0; i < N; i++) {
		int64_t x, y, r; cin >> x >> y >> r;

		if (r * r >= x * x + y * y) {
			cout << "Yes" << '\n';
			exit(0);
		}

		double a_mid = std::atan2(y, x);
		if (r == 0) {
			angles.push_back(a_mid);
		} else {
			double a_diff = std::asin(double(r) / std::sqrt(x * x + y * y));
			double a_lo = a_mid - a_diff;
			if (a_lo <= -M_PI) a_lo += 2 * M_PI;
			angles.push_back(a_lo);
			double a_hi = a_mid + a_diff;
			if (a_hi > M_PI) a_hi -= 2 * M_PI;
			angles.push_back(a_hi);
		}
	}
	std::sort(angles.begin(), angles.end());
	if (angles.empty()) {
		cout << "No" << '\n';
		exit(0);
	}
	if (angles.back() - angles.front() < M_PI) {
		cout << "No" << '\n';
		exit(0);
	}
	for (int i = 0; i+1 < int(angles.size()); i++) {
		if (angles[i+1] - angles[i] > M_PI) {
			cout << "No" << '\n';
			exit(0);
		}
	}
	cout << "Yes" << '\n';
	exit(0);

	return 0;
}
