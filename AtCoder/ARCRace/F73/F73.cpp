#include<bits/stdc++.h>
using namespace std;

using ll = long long;
const ll INF = 1e18;

const int MAXN = 2.1e5;
int N;
int Q;
int X[MAXN];

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N >> Q;
	int A, B; cin >> A >> B;

	for (int q = 0; q < Q; q++) { cin >> X[q]; }

	ll cval = 0;
	map<int, ll> mp;
	mp[A] = 0;
	int prevVal = B;

	auto queryPt = [&](int x) -> ll {
		ll ans = INF;
		auto it = mp.lower_bound(x);
		if (it != mp.end()) {
			ans = min(ans, (it->first - x) + it->second);
		}
		if (it != mp.begin()) {
			--it;
			ans = min(ans, (x - it->first) + it->second);
		}
		return ans;
	};

	auto insertPt = [&](int x, ll v) {
		if (v >= queryPt(x)) return;
		auto it = mp.lower_bound(x);
		while (it != mp.end()) {
			if ((it->first - x) + v <= it->second) {
				it = mp.erase(it);
			} else {
				break;
			}
		}
		while (it != mp.begin()) {
			--it;
			if ((x - it->first) + v <= it->second) {
				it = mp.erase(it);
			} else {
				break;
			}
		}
		mp[x] = v;
	};

	for (int q = 0; q < Q; q++) {
		int curVal = X[q];
		insertPt(prevVal, queryPt(curVal) - abs(prevVal - curVal));
		cval += abs(prevVal - curVal);

		prevVal = X[q];
	}
	ll ans = INF;
	for (auto it : mp) ans = min(ans, it.second);
	cout << ans + cval << '\n';


	return 0;
}
