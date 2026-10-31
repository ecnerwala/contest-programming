#include <bits/stdc++.h>
using namespace std;

int main() {
	int N; cin >> N;
	vector<int64_t> A(N); for (auto& a : A) cin >> a;
	int64_t tot = std::accumulate(A.begin(), A.end(), int64_t(0));
	for (auto& a : A) { a *= N, a -= tot; }
	map<__int128, int> mp;
	__int128 cur = 0;
	for (auto a : A) {
		cur += a;
		mp[cur]++;
	}
	int ans = N;
	for (auto it : mp) {
		ans = min(ans, N - it.second);
	}
	cout << ans << '\n';
}
