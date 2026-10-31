#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);


	int64_t W, H, N; cin >> W >> H >> N;
	std::vector<std::array<int, 2>> C(N);
	for (auto &c : C) {
		cin >> c[0] >> c[1];
	}



	int A; cin >> A;
	std::vector<int> X(A);
	for (auto& x : X) cin >> x;


	int B; cin >> B;
	std::vector<int> Y(B);
	for (auto& y : Y) cin >> y;



	std::map<std::array<int, 2>, int> mp;
	for (auto [x,  y] : C) {
		int i = std::lower_bound(X.begin(), X.end(), x) - X.begin();

		int j = std::lower_bound(Y.begin(), Y.end(), y) - Y.begin();

		mp[{i, j}] ++;

	}


	int mi = N+1, ma = 0;
	if (mp.size() < int64_t(A+1) * (B+1)) mi = 0;

	for (auto [_, c] : mp) {
		mi = std::min(mi, c);
		ma = std::max(ma, c);
	}

	return 0;


}
