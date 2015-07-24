#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

inline ll sq(ll a) { return a * a; }
const double EPS = 1e-9;
bool issquare(ll a) {
	return abs(sq(floor(sqrt(a) + EPS)) - a) <= EPS;
}

ll slow(ll N) {
	ll res = 0;
	for(ll a = 1; a <= N; a++) {
		for(ll b = a; b <= N; b++) {
			for(ll c = b; c <= min(N, a + b - 1); c++) {
				if(c % 2 == 0 && issquare(2 * a * a + 2 * b * b - c * c)) {
					res++;
				}
			}
		}
	}
	return res;
}

ll F(ll N) {
	ll res = 0;
	unordered_map<ll, ll> cnt; // 2(a * a + b * b) = c + c + 4 * m * m
	for(ll c = 1; c <= N; c++) {
		// insert b = c
		for(ll a = !(c & 1) + 1; a <= c; a+=2) {
			cnt[(sq(a) + sq(c)) / 2] ++;
		}

		if(c % 2) continue;

		// remove a + b = c
		for(ll a = 1; a <= c - a; a++) {
			cnt[(sq(a) + sq(c - a)) / 2] --;
		}

		// iterate over values of m
		ll v = (c / 2) * (c / 2);
		for(ll m = 1; m <= c; m++) {
			v += 2 * m - 1;
			//cerr << c << ' ' << m << ' ' << 4 * m * m + c * c << ' ' << cnt[4 * m * m + c * c] << '\n';
			res += cnt[v];
		}

		cerr << c << '\n';
	}
	return res;
}

int main() {
	cout << F(10) << '\n';
	cout << F(50) << '\n';
	//cout << F(100000) << '\n';
	return 0;
}
