#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int64_t> A(N);
		for (auto& a : A) cin >> a;
		for (int b = 0; true; b++) {
			if (b >= 30) {
				cout << "NO" << '\n';
				break;
			}

			int cnt = 0;
			for (auto a : A) {
				if ((a >> b) & 1) cnt++;
			}
			if (cnt >= 2) {
				std::vector<int> splits; splits.reserve(N+1);
				splits.push_back(0);
				for (int i = 0; i < N; i++) {
					if ((A[i] >> b) & 1) splits.push_back(i+1);
				}
				assert(splits.size() >= 3);
				splits.back() = N;
				cout << "YES" << '\n';
				cout << splits.size()-1 << '\n';
				for (int i = 0; i+1 < int(splits.size()); i++) {
					cout << splits[i]+1 << ' ' << splits[i+1] << '\n';
				}
				break;
			}
		}
	}
	return 0;
}
