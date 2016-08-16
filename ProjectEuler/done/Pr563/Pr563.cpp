#include<bits/stdc++.h>
using namespace std;

typedef long long ll;
const ll INF = 4e18;

inline void kill_factor(ll &a, const ll f) {
	while(a % f == 0) a /= f;
}

bool is_good(ll a) {
	kill_factor(a, 2);
	kill_factor(a, 3);
	kill_factor(a, 5);
	kill_factor(a, 7);
	kill_factor(a, 11);
	kill_factor(a, 13);
	kill_factor(a, 17);
	kill_factor(a, 19);
	kill_factor(a, 23);
	return a == 1;
}

ll res[200];

int main() {
	unordered_map<ll, ll> cnt;
	vector<ll> goods;
	for(ll cur = 1; cur <= ll(1e8); cur++) {
		if(is_good(cur)) {
			goods.push_back(cur);
			for(ll i = int(goods.size()) - 1; i >= 0 && goods[i] + goods[i] / 10 >= cur; i--) {
				cnt[goods[i] * cur] ++;
			}
		}
	}
	for(int i = 0; i < 200; i++) {
		res[i] = INF;
	}
	for(auto it : cnt) {
		if(it.second < 200) res[it.second] = min(res[it.second], it.first);
	}
	ll ans = 0;
	for(int i = 2; i <= 100; i++) {
		ans += res[i];
		//cerr << i << ' ' << res[i] << '\n';
	}
	cout << ans << '\n';
	return 0;
}
