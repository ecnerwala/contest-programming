#include <bits/stdc++.h>
using namespace std;

const int MAXN = 3e5;

const int S = 1 << 19;
long long val[S * 2]; // stores increment on range

// range increment: update [l, r] to m
void update(int l, int r, long long v) {
	for(int a = l + S, b = r + S; a <= b; a /= 2, b /= 2) {
		if(a % 2 == 1) {
			val[a] += v;
			a++;
		}
		if(b % 2 == 0) {
			val[b] += v;
			b--;
		}
	}
}

long long tot[MAXN];

int main() {
	ios_base::sync_with_stdio(0);
	int n, k, q;
	long long A, B;
	cin >> n >> k >> A >> B >> q;
	for(int i = 0; i < q; i++) {
		int t; cin >> t;
		if(t == 1) {
			int d; long long a; cin >> d >> a;
			d--;

			if(d >= k) update(0, d - k, -min(tot[d], A));
			update(d + 1, n + 1, -min(tot[d], B));

			tot[d] += a;

			if(d >= k) update(0, d - k, min(tot[d], A));
			update(d + 1, n + 1, min(tot[d], B));

		} else {

			int d; cin >> d;
			d--;

			long long res = 0;
			for(int x = d + S; x; x /= 2) {
				res += val[x];
			}

			cout << res << '\n';

		}
	}
	return 0;
}
