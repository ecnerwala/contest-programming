#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> ans; ans.reserve(N);
		for (int i = 1; i <= N; i += 2) {
			for (int v = i; v <= N; v *= 2) {
				ans.push_back(v);
			}
		}
		for (int i = 0; i < N; i++) {
			cout << ans[i] << " \n"[i+1==N];
		}
	}

	return 0;
}
