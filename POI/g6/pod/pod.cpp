#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

int main() {
	ios_base::sync_with_stdio(0);
	ll xi = 0, xa = 0, x = 0, yi = 0, ya = 0, y = 0;
	int N;
	cin >> N;

	while(N--) {
		int K;
		string s;
		cin >> K >> s;
		ll tx = 0, ty = 0;
		for(int i = 0; i < int(s.size()); i++) {
			char c = s[i];
			if(c == 'E') {
				tx++;
			} else if(c == 'W') {
				tx--;
			} else if(c == 'N') {
				ty++;
			} else if(c == 'S') {
				ty--;
			} else assert(false);
		}
		tx *= K - 1;
		ty *= K - 1;
		for(int i = 0; i < int(s.size()); i++) {
			char c = s[i];
			if(c == 'E') {
				xa = max(xa, ++x + max(ll(0), tx));
			} else if(c == 'W') {
				xi = min(xi, --x + min(ll(0), tx));
			} else if(c == 'N') {
				ya = max(ya, ++y + max(ll(0), ty));
			} else if(c == 'S') {
				yi = min(yi, --y + min(ll(0), ty));
			} else assert(false);
		}
		x += tx, y += ty;
	}

	cout << ya - yi << ' ' << xa - xi << '\n';

	return 0;
}
