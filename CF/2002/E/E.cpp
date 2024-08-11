#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<std::pair<int64_t, int>> stk; stk.reserve(N);
		for (int i = 0; i < N; i++) {
			int64_t a; int b; cin >> a >> b;
			int64_t t = 0;
			while (true) {
				if (!stk.empty() && stk.back().second == b) {
					a += stk.back().first - t;
					t = stk.back().first;
					stk.pop_back();
				} else if (!stk.empty() && a >= stk.back().first) {
					t = stk.back().first;
					stk.pop_back();
				} else {
					t = a;
					stk.push_back({a, b});
					break;
				}
			}
			cout << stk[0].first << " \n"[i+1==N];
		}
	}

	return 0;
}
