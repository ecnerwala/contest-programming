#include<bits/stdc++.h>
using namespace std;

using ll = long long;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	ll S; cin >> S;
	ll BASE = ll(1e9);
	cout << 0 << ' ' << 0 << ' ' << (S + BASE - 1) / BASE << ' ' << ((S + BASE - 1) / BASE * BASE - S) << ' ' << 1 << ' ' << BASE << '\n';

	return 0;
}
