#include<bits/stdc++.h>

using namespace std;

typedef long long ll;
const ll INF = 1e18;

ll ceil(ll a, ll b) { return (a + b - 1) / b; }
ll floor(ll a, ll b) { return a / b; }

ll HD, AD, HK, AK, B, D;

ll go() {
	ll NA = ceil(HK, AD);
	for (ll b = 1; true; b++) {
		ll n = ceil(HK, AD + B * b) + b;
		if (n < NA) {
			NA = n;
		} else if (n > NA) {
			break;
		}
	}
	cerr << "need " << NA << " attacks" << '\n';

	assert(NA >= 1);
	if (NA == 1) return 1;

	assert(NA >= 2);
	ll res = INF;
	for (ll a = AK, h = HD, t = 0; t + NA < res;) {
		assert(h > 0);
		assert(a >= 0);
		{ // let's just attack now
			if (a == 0) {
				res = min(res, t + NA);
				break;
			}
			if (NA <= ceil(h, a)) {
				res = min(res, t + NA);
				break;
			}
			ll V = ceil(HD, a) - 1;
			if (V >= 2) {
				ll n = t + NA + ceil(NA - (ceil(h,a) - 1) - 1, V-1);
				if (n < res) {
					res = n;
				} else if (n > res) {
					//break;
				}
			}
		}
		{ // let's do an extra debuff
			ll a2 = max(a - D, 0ll);
			if (a2 == a) break;
			if (h - a2 <= 0) {
				t ++;
				h = HD;
				h -= a;
			}
			if (h - a2 <= 0) {
				break; // no way to escape, he'll always just kill me
			}
			t++;
			a = a2;
			h -= a;
		}
	}
	return res;
}

int main() {
	ios_base::sync_with_stdio(0);
	int T; cin >> T;

	for(int case_num = 1; case_num <= T; case_num ++) {
		cin >> HD >> AD >> HK >> AK >> B >> D;
		
		ll res = go();
		
		cout << "Case #" << case_num << ": ";
		if (res == INF) cout << "IMPOSSIBLE\n";
		else cout << res << '\n';
	}

	return 0;
}
