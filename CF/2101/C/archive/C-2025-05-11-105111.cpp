#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> A(N); for (auto& a : A) { cin >> a; }

		int64_t ans = 0;
		std::vector<int> locs; locs.reserve(N);
		for (int z = 0; z < 2; z++, std::reverse(A.begin(), A.end()), std::reverse(locs.begin(), locs.end())) {
			std::set<int> cnds;
			for (int i = 0; i < N; i++) cnds.insert(i);
			for (int i = 0; i < N; i++) {
				auto it = cnds.lower_bound(A[i]);
				if (it != cnds.begin()) {
					cnds.erase(std::prev(it));
					if (z) {
						if (locs.empty()) break;
						if (N - 1 - i > locs.back()) {
							ans += (N - 1 - i) - locs.back();
							locs.pop_back();
						} else {
							break;
						}
					} else {
						locs.push_back(i);
					}
				}
			}
		}
		cout << ans << '\n';
	}

	return 0;
}
