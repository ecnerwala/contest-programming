#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

ll to_ll(string s) {
	bool neg = false;
	ll res = 0;

	ll dec = -1;
	for(char c : s) {
		if(c == '-') neg = true;
		else if(c == '.') {
			dec = 0;
		} else {
			res *= 10;
			res += c - '0';
			if(dec >= 0) dec++;
		}
	}

	assert(dec <= 2);
	if(dec <= 1) res *= 10;
	if(dec <= 0) res *= 10;

	return neg ? -res : res;
}

int main() {
	ios_base::sync_with_stdio(0);
	ll N;
	cin >> N;
	vector<ll> res;
	vector<pair<ll, ll>> neg;
	ll maxval = -1000000000, maxind = -1;
	for(ll i = 0; i < N; i++) {
		string s; cin >> s;
		ll v = to_ll(s);
		if(v > maxval) maxval = v, maxind = i;
		if(v > 100) {
			res.push_back(i);
		} else if(v < 0) {
			neg.push_back(make_pair(-v, i));
		} else {
			// v >= 0, v <= 1,pretty useless
		}
	}
	assert(maxind >= 0);
	sort(neg.begin(), neg.end());
	reverse(neg.begin(), neg.end());

	for(ll i = 0; i + 1 < ll(neg.size()); i+=2) {
		ll a = neg[i].first, b = neg[i + 1].first;
		assert(a > 0 && b > 0);
		if(a * b > 10000) {
			res.push_back(neg[i].second);
			res.push_back(neg[i + 1].second);
		}
	}

	if(res.empty()) {
		assert(maxval <= 100);
		if(ll(neg.size()) >= 2) {
			ll a = neg[0].first, b = neg[1].first;
			assert(a * b <= 10000);
			if(a * b > maxval * 100) {
				res.push_back(neg[0].second);
				res.push_back(neg[1].second);
			} else {
				res.push_back(maxind);
			}
		} else {
			res.push_back(maxind);
		}
	}
	sort(res.begin(), res.end());
	cout << res.size() << '\n';
	cout << res[0] + 1;
	for(ll i = 1;  i < ll(res.size()); i++) {
		cout << ' ' << res[i] + 1;
	}
	cout << '\n';

}

// N >= 2 => answer is always nonnegative
// => even number of negatives
