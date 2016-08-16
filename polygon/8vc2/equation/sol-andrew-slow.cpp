#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
int main() {
	ll s, x; cin >> s >> x;
	ll res = 0;
	for(ll a = 1, b = s - 1; b > 0; a++, b--) {
		res += ((a ^ b) == x);
	}
	cout << res << '\n';
	return 0;
}
