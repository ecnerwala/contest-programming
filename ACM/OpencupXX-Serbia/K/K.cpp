#include<bits/stdc++.h>

template <typename T> void setmax(T& a, const T& b) { if (b > a) a = b; }

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, MX, MY; cin >> N >> MX >> MY;

	struct data {
		int x, y, h;
		int t;
		int yi;
	};

	vector<data> pts(N);
	for (int i = 0; i < N; i++) {
		cin >> pts[i].x >> pts[i].y >> pts[i].h;
		pts[i].t = i;
	}

	vector<int> ys(N);
	for (int i = 0; i < N; i++) {
		ys[i] = pts[i].y;
	}
	sort(ys.begin(), ys.end());
	unique(ys.begin(), ys.end());
	for (int i = 0; i < N; i++) {
		pts[i].yi = int(lower_bound(ys.begin(), ys.end(), pts[i].y) - ys.begin());
		assert(ys[pts[i].yi] == pts[i].y);
	}

	sort(pts.begin(), pts.end(), [](const data& a, const data& b) { return a.x < b.x; });

	auto is_good = [&](int T) -> bool {
		const int INF = 2.11e9;
		vector<pair<int, int>> plusBit(N, {-INF, -INF});
		vector<pair<int, int>> minusBit(N, {-INF, -INF});
		int parity = 0;
		for (const auto& p : pts) {
			if (p.t >= T) continue;
			parity |= 1 << ((p.x ^ p.y ^ p.h) & 1);
			if (parity == 3) return false;

			pair<int, int> plusVal(p.x + p.y + p.h, p.x + p.y - p.h);
			for (int a = p.yi+1; a > 0; a -= (a & -a)) {
				if (plusVal.first < plusBit[a-1].first || plusVal.second < plusBit[a-1].second) {
					return false;
				}
			}
			for (int a = p.yi+1; a <= N; a += (a & -a)) {
				setmax(plusBit[a-1].first, plusVal.first);
				setmax(plusBit[a-1].second, plusVal.second);
			}
			pair<int, int> minusVal(p.x - p.y + p.h, p.x - p.y - p.h);
			for (int a = p.yi+1; a <= N; a += (a & -a)) {
				if (minusVal.first < minusBit[a-1].first || minusVal.second < minusBit[a-1].second) {
					return false;
				}
			}
			for (int a = p.yi+1; a > 0; a -= (a & -a)) {
				setmax(minusBit[a-1].first, minusVal.first);
				setmax(minusBit[a-1].second, minusVal.second);
			}
		}

		return true;
	};

	int mi = 0, ma = N+1;
	while (ma - mi > 1) {
		int md = (mi + ma) / 2;
		if (is_good(md)) mi = md;
		else ma = md;
	}

	if (mi == N) {
		cout << "bravo komisijo" << '\n';
	} else {
		cout << mi+1 << '\n';
	}

	return 0;
}
