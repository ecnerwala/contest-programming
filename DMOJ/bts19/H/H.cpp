#include<bits/stdc++.h>
using namespace std;

using ll = long long;

int readInt() {
	int v; cin >> v;
	if (v == -400) exit(0);
	return v;
}

void go() {
	int N; cin >> N;
	const int BASE = 1e8;
	ll milo = 0, malo = 1e18;
	while (malo - milo > 1) {
		ll md = (milo + malo) / 2;
		cout << "?" << ' ' << 0 << ' ' << 1 << ' ' << 0 << ' ' << BASE << ' ' << md << '\n' << flush;
		int v = readInt();
		if (v == 1) {
			milo = md;
		} else {
			malo = md;
		}
	}
	ll mihi = 0, mahi = 1e18;
	while (mahi - mihi > 1) {
		ll md = (mihi + mahi) / 2;
		cout << "?" << ' ' << 0 << ' ' << 1 << ' ' << 0 << ' ' << BASE << ' ' << md << '\n' << flush;
		int v = readInt();
		if (v == 1 || v == 0) {
			mihi = md;
		} else {
			mahi = md;
		}
	}
	if (N == 1) {
		cout << "!" << '\n';
		cout << malo % BASE << ' ' << malo / BASE << '\n';
		cout << flush;
	} else if (N == 2) {
		cout << "!" << '\n';
		cout << milo % BASE << ' ' << milo / BASE << '\n';
		cout << mahi % BASE << ' ' << mahi / BASE << '\n';
		cout << flush;
	} else {
		cout << "!" << '\n';
		for (int i = 0; i < N; i++) {
			cout << 1 << ' ' << 1 << '\n';
		}
		cout << flush;
	}
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int T = 10;
	while (T--) {
		go();
	}

	return 0;
}
