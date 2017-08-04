#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

bool is_prime(ll a) {
	if (a <= 1) return false;
	if (a % 2 == 0) return a == 2;
	for (ll i = 3; i * i <= a; i += 2) {
		if (a % i == 0) return false;
	}
	return true;
}

int main() {
	ll a, b; scanf("%lld %lld", &a, &b);
	vector<ll> vals;
	vals.push_back(2);
	vals.push_back(3);
	vals.push_back(5);
	vals.push_back(7);
	for (int i = 0; i < int(vals.size()); i++) {
		ll cur = vals[i];
		for (int j = 1; j < 10; j += 2) {
			ll nxt = cur * 10 + j;
			if (is_prime(nxt)) {
				vals.push_back(nxt);
			}
		}
	}
	ll res = 0;
	for (size_t i = 0; i < vals.size(); i++) {
		if (a <= vals[i] && vals[i] <= b) res ++;
	}
	printf("%lld\n", res);

	return 0;
}
