#include<bits/stdc++.h>

int main() {
	using std::cerr;
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	int N; std::cin >> N;
	std::vector<int64_t> A(N); for (auto& a : A) std::cin >> a;
	std::vector<int64_t> B(N); for (auto& b : B) std::cin >> b;

	{
		int z = int(std::max_element(A.begin(), A.end()) - A.begin());
		std::rotate(A.begin(), A.begin()+z, A.end());
		std::rotate(B.begin(), B.begin()+z, B.end());
	}

	A.push_back(A[0]);

	std::vector<int64_t> C(N+1);
	for (int i = 1; i <= N-1; i++) {
		C[i+1] = 2 * (C[i] + B[i]) - C[i-1];
	}

	double ans = 0;
	std::vector<std::pair<int, int64_t>> hull;
	for (int i = 0; i <= N; i++) {
		int64_t v = A[i] - C[i];
		while (hull.size() >= 2 &&
				(hull[hull.size()-1].second - hull[hull.size()-2].second) * (i - hull[hull.size()-2].first)
				<= (v - hull[hull.size()-2].second) * (hull[hull.size()-1].first - hull[hull.size()-2].first)) {
			hull.pop_back();
		}
		hull.emplace_back(i, v);
	}
	for (int z = 1; z < int(hull.size()); z++) {
		auto [i, v0] = hull[z-1];
		auto [j, v1] = hull[z];
		for (int k = i+1; k <= j; k++) {
			ans += double((v0 + C[k]) * (j - k) + (v1 + C[k]) * (k - i)) / (j - i);
		}
	}
	ans /= N;
	std::cout << std::fixed << std::setprecision(20) << ans << '\n';

	return 0;
}
