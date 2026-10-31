#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> A(N);
		for (auto& a : A) cin >> a;
		std::sort(A.begin(), A.end());
		A.resize(std::unique(A.begin(), A.end()) - A.begin());
		cout << 2 * int(A.size()) - 1 << '\n';
	}

	return 0;
}
