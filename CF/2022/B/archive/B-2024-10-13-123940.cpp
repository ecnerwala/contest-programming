#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, X; cin >> N >> X;
		std::vector<int> A(N);
		for (auto& a : A) cin >> a;
		int64_t tot = std::accumulate(A.begin(), A.end(), int64_t(0));
		cout << std::max<int64_t>((tot + X - 1) / X, *std::max_element(A.begin(), A.end())) << '\n';
	}

	return 0;
}
