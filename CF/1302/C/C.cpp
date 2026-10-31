#include<bits/stdc++.h>
using namespace std;

using ll = long long;

const int S = 1 << 17;
ll seg[2*S];

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);

	int T; cin >> T;
	while (T--) {
		int N, Q; cin >> N >> Q;
		for (int q = 0; q < Q; q++) {
			int t; cin >> t;
			if (t == 1) {
				int i; ll v; cin >> i >> v; i--;
				ll d = v - seg[S+i];
				for (int a = S+i; a; a /= 2) {
					seg[a] += d;
				}
			} else if (t == 2) {
				int l, r; cin >> l >> r; l--, r--;
				ll ans = 0;
				for (int a = S+l, b = S+r+1; a < b; a /= 2, b /= 2) {
					if (a&1) ans += seg[a++];
					if (b&1) ans += seg[--b];
				}
				cout << ans << '\n';
			} else assert(false);
		}
	}

	return 0;
}
