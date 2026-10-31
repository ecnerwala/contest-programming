#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int R, C; cin >> R >> C;
		std::vector<std::string> A(R);
		for (auto& a : A) cin >> a;
		std::vector<std::string> B(R);
		for (auto& b : B) cin >> b;
		std::vector<int> match(C, 0);
		std::vector<std::pair<int, int>> stk; stk.reserve(C+1);
		int ans = 0;
		for (int r = 0; r < R; r++) {
			stk.clear();
			auto do_push = [&](int c2, int v) {
				while (!stk.empty() && stk.back().first >= v) {
					int h = stk.back().first;
					stk.pop_back();
					int dx = c2 - 1 - (stk.empty() ? -1 : stk.back().first);
					ans = std::max(ans, h * dx);
				}
				stk.push_back({c2, v});
			};
			for (int c = 0; c < C; c++) {
				match[c] = (A[r][c] == B[r][c]) ? match[c] + 1 : 0;
				do_push(c, match[c]);
			}
			do_push(C, 0);
		}
		cout << ans << '\n';
	}

	return 0;
}
