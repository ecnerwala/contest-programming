#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	vector<int> H(N);
	for (int i = 0; i < N; i++) {
		cin >> H[i];
	}

	int64_t ans = 0;
	map<int, int> mp;
	for (int i = 0; i < N; i++) {
		// H[i] + H[j] = i - j
		ans += mp[H[i] - i];
	}

	cout << ans << '\n';
	return 0;
}
