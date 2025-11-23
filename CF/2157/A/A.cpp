#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> freq(N+1);
		for (int i = 0; i < N; i++) { int a; cin >> a; freq[a]++; }
		int ans = 0;
		for (int i = 0; i <= N; i++) {
			if (freq[i] < i) ans += freq[i];
			else ans += freq[i] - i;
		}
		cout << ans << '\n';
	}

	return 0;
}
