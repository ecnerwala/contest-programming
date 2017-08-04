#include<bits/stdc++.h>
using namespace std;

const int MAXK = 1e5 + 200;
const int MAXP = 1e5 + 200;
int K, P;

template <int N> struct ds {
	int par[N];
	int getpar(int a) {
		return par[a] == -1 ? a : (par[a] = getpar(par[a]));
	}
	void reset() {
		memset(par, -1, sizeof(par));
	}
	bool merge(int a, int b ) {
		a = getpar(a), b = getpar(b);
		if (a == b) return false;
		par[a] = b;
		return true;
	}
};

ds<MAXK * 2> vals;

bool go() {
	vals.reset();
	cin >> K >> P;
	for (int i = 0; i < P; i++) {
		int a, b; char c;
		cin >> a >> b >> c;
		a--, b--;
		if (c == 'A') {
			vals.merge(2 * a, 2 * b);
			vals.merge(2 * a + 1, 2 * b + 1);
		} else {
			vals.merge(2 * a + 1, 2 * b);
			vals.merge(2 * a, 2 * b + 1);
		}
	}
	for (int i = 0; i < K; i++) {
		if (vals.getpar(2 * i) == vals.getpar(2 * i + 1)) {
			return false;
		}
	}
	return true;
}
int main() {
	ios_base::sync_with_stdio(0);
	int T; cin >> T;
	for (int t = 0; t < T; t++) {
		cout << (go() ? "TAK" : "NIE") << '\n';
	}
	return 0;
}
