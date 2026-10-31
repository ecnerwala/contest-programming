#include<bits/stdc++.h>
using namespace std;

const int INF = 1e9;

template <typename T> void setmax(T& a, T b) { if (b > a) a = b; }

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int N; cin >> N;
	int xsub = -INF;
	int xadd = -INF;
	int ysub = -INF;
	int yadd = -INF;

	for (int i = 0; i < N; i++) {
		int x, y, h; cin >> x >> y >> h;
		setmax(xsub, h-x);
		setmax(xadd, h+x);
		setmax(ysub, h-y);
		setmax(yadd, h+y);
	}

	if ((xsub + xadd) & 1) {
		xsub++;
	}
	if ((ysub + yadd) & 1) {
		ysub++;
	}
	int x = (xadd-xsub)/2;
	int y = (yadd-ysub)/2;
	int h = max(xsub+xadd, ysub+yadd) / 2;

	cout << x << ' ' << y << ' ' << h << '\n';
	return 0;
}
