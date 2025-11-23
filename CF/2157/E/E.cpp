#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, K; cin >> N >> K;
		std::vector<int> freq(2*N);
		for (int i = 0; i < N; i++) {
			int a; cin >> a; a--; freq[a]++;
		}
		std::vector<int> stk; stk.reserve(3*N);
		for (int i = 3*N-1; i >= 2*N; i--) stk.push_back(i);
		int ans = 0;
		for (int v = 2*N-1; v >= 0; v--) {
			stk.push_back(v);
			while (freq[v] > 0) {
				if (freq[v] == K) ans = std::max(ans, stk.back() - v);
				stk.pop_back();
				freq[v]--;
			}
		}
		cout << ans << '\n';
	}

	return 0;
}
