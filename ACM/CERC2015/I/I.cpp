#include<bits/stdc++.h>
using namespace std;

template <typename T> T sq(T a) { return a * a; }

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int N; cin >> N;
	const int D = 500;
	vector<vector<double>> igloos(D+2, vector<double>(D+2, -1));
	for (int i = 0; i < N; i++) {
		int x, y; double r; cin >> x >> y >> r;
		assert(igloos[x][y] < 0);
		igloos[x][y] = r;
	}


	int Q; cin >> Q;
	while (Q--) {
		int x1, y1, x2, y2; cin >> x1 >> y1 >> x2 >> y2;
		if (x2 < x1) swap(x1, x2), swap(y1, y2);
		assert(x1 <= x2);

		if (x1 == x2) {
			if (y1 > y2) swap(y1, y2);
		}

		int dx = x2-x1, dy = y2-y1;

		int dot1 = x1 * dx + y1 * dy;
		int dot2 = x2 * dx + y2 * dy;
		assert(dot1 < dot2);

		double len = hypot(x2-x1, y2-y1);

		auto ptLnDist = [&](int x, int y) -> double {
			int dot = x * dx + y * dy;
			if (dot <= dot1) {
				return hypot(x-x1, y-y1);
			} else if (dot >= dot2) {
				return hypot(x2-x, y2-y);
			} else {
				return abs((x-x1) * dy - (y-y1) * dx) / len;
			}
		};

		int ans = 0;
		if (x1 == x2) {
			for (int y = y1; y <= y2; y++) {
				if (ptLnDist(x1, y) <= igloos[x1][y]) {
					ans++;
				}
			}
		} else {
			for (int x = x1; x <= x2; x++) {
				int midY = (y1 * (x2-x) + y2 * (x-x1)) / (x2-x1);
				//cerr << "start " << x << ' ' << midY << '\n';
				for (int y = midY; true; y--) {
					double d = ptLnDist(x, y);
					if (d > 1) break;
					//cerr << x << ' ' << y << ' ' << d << '\n';
					if (d <= igloos[x][y]) {
						ans++;
					}
				}
				for (int y = midY+1; true; y++) {
					double d = ptLnDist(x, y);
					if (d > 1) break;
					//cerr << x << ' ' << y << ' ' << d << '\n';
					if (d <= igloos[x][y]) {
						ans++;
					}
				}
			}
		}

		//cerr << ans << '\n';
		cout << ans << '\n';
	}

	return 0;
}
