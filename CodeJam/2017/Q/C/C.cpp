#include<bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef pair<ll, ll> pll;
#define L first
#define R second

ll N, K;

ll go() {
	priority_queue<ll> pq;
	unordered_map<ll, ll> mp;
	pq.push(N);
	mp[N] = 1;
	assert(K <= N);
	while (K > 0) {
		ll S = pq.top(); pq.pop();
		assert(S);
		ll cnt = mp[S];
		if (cnt >= K) {
			return S;
		}
		K -= cnt;
		ll L = (S-1)/2;
		ll R = (S-1) - L;
		if (!mp.count(L)) { pq.push(L); }
		mp[L] += cnt;
		if (!mp.count(R)) { pq.push(R); }
		mp[R] += cnt;
	}
	assert(false);
	return -1;
}

int main() {
	ios_base::sync_with_stdio(0);
	int T; cin >> T;

	for(int case_num = 1; case_num <= T; case_num ++) {
		cin >> N >> K;
		ll res = go();
		cout << "Case #" << case_num << ": " << (res) / 2 << " " << (res - 1) / 2 << '\n';
	}

	return 0;
}
