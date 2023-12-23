#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> pts; pts.reserve(2*N);
		for (int i = 0; i < N; i++) {
			int l; cin >> l;
			pts.push_back(2*l+1);
		}
		for (int i = 0; i < N; i++) {
			int r; cin >> r;
			pts.push_back(2*r+0);
		}
		std::sort(pts.begin(), pts.end());
		std::vector<int> lens; lens.reserve(N);
		std::vector<int> stk; stk.reserve(N);
		for (int v : pts) {
			if (v & 1) {
				stk.push_back(v >> 1);
			} else {
				assert(!stk.empty());
				lens.push_back((v >> 1) - stk.back());
				stk.pop_back();
			}
		}
		std::sort(lens.begin(), lens.end());
		std::vector<int> C(N);
		for (auto& c : C) cin >> c;
		std::sort(C.begin(), C.end(), std::greater<>());
		int64_t ans = 0;
		for (int i = 0; i < N; i++) {
			ans += int64_t(C[i]) * lens[i];
		}
		cout << ans << '\n';
	}

	return 0;
}
