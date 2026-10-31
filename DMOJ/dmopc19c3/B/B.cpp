#include<bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	using ll = long long;
	int N, K; cin >> N >> K;
	string S; cin >> S;
	vector<int> cnt(26);
	for (char c : S) {
		if (c == '*') continue;
		cnt[c - 'a']++;
	}

	vector<ll> fact(N+1);
	fact[0] = 1;
	for (int i = 1; i <= N; i++) fact[i] = fact[i-1] * i;

	auto numWays = [&](const vector<int>& cnts) -> ll {
		ll ans = fact[N];
		for (int v : cnts) ans /= fact[v];
		return ans;
	};

	ll ans = 0;
	if (K == 0) {
		ans += numWays(cnt);
	} else if (K == 1) {
		for (int i = 0; i < 26; i++) {
			cnt[i]++;
			ans += numWays(cnt);
			cnt[i]--;
		}
	} else if (K == 2) {
		for (int i = 0; i < 26; i++) {
			cnt[i]++;
			for (int j = i; j < 26; j++) {
				cnt[j]++;
				ans += numWays(cnt);
				cnt[j]--;
			}
			cnt[i]--;
		}
	} else assert(false);
	cout << ans << '\n';

	return 0;
}
