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
		int c1 = 0;
		for (int i = 0; i <= N; i++) {
			if (freq[i] == 0) break;
			if (freq[i] == 1 && c1) break;
			if (freq[i] == 1) {
				c1 = 1;
			}
			ans++;
		}
		cout << ans << '\n';
	}

	return 0;
}

// Alice can attain K if there's at most 1 thing of freq 1: she takes that first, then mirrors the rest
