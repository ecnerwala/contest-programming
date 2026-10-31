#include<bits/stdc++.h>
using namespace std;

using ll = long long;

// we'll write it as sum/diff of 10^n / 9

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	map<ll, ll> cnt;
	for (int z = 0; z < 2; z++) {
		int len; cin >> len;
		vector<pair<ll, ll>> digs(len);
		for (int i = 0; i < len; i++) {
			cin >> digs[i].first >> digs[i].second;
		}
		reverse(digs.begin(), digs.end());
		ll loc = 0;
		for (int i = 0; i < len; i++) {
			cnt[loc] -= digs[i].second;
			loc += digs[i].first;
			cnt[loc] += digs[i].second;
		}
	}

	vector<pair<ll, ll>> res;
	ll loc = 0;
	while (true) {
		ll v = cnt[loc];
		cnt.erase(loc);
		// v must be between -9 and -0
		cnt[loc+1] += v/10;
		v %= 10;
		if (v > 0) v -= 10, cnt[loc+1] ++;
		assert(-9 <= v && v <= -0);
		while (!cnt.empty() && cnt.begin()->second % 10 == 0) {
			if (cnt.begin()->second) {
				cnt[cnt.begin()->first+1] += cnt.begin()->second / 10;
			}
			cnt.erase(cnt.begin());
		}
		if (cnt.empty()) {
			assert(v == 0);
			break;
		}
		res.emplace_back(cnt.begin()->first - loc, -v);
		cnt.begin()->second += v;
		loc = cnt.begin()->first;
	}

	reverse(res.begin(), res.end());
	cout << res.size() << '\n';
	for (auto it : res) {
		cout << it.first << ' ' << it.second << '\n';
	}

	return 0;
}
