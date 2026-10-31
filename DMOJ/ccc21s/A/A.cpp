#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	vector<int> H(N+1); for (auto& h : H) cin >> h;
	vector<int> W(N); for (auto& w : W) cin >> w;
	int ans = 0;
	for (int i = 0; i < N; i++) {
		ans += (H[i] + H[i+1]) * W[i];
	}
	cout << ans/2 << (ans&1 ? ".5" : "") << '\n';

	return 0;
}
