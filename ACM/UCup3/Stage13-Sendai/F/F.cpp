#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> A(N);
		for (auto& a : A) cin >> a;
		int mi = *std::min_element(A.begin(), A.end());
		if (N & 1) {
			assert(mi > 0);
			cout << "First" << '\n';
		} else {
			int cnt = 0;
			for (auto a : A) cnt += a > mi;
			if (cnt & 1) cout << "First" << '\n';
			else cout << "Second" << '\n';
		}
	}

	return 0;
}
