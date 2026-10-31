#include<bits/stdc++.h>
using namespace std;

using ll = long long;

struct pt {
	double x, y;
	friend bool operator < (pt a, pt b) {
		return tie(a.x, a.y) < tie(b.x, b.y);
	}
	friend bool operator == (pt a, pt b) {
		return tie(a.x, a.y) == tie(b.x, b.y);
	}
};

struct cone {
	double h, r;
	double area() const {
		return h * r * r;
	}
	friend bool operator < (cone a, cone b) {
		return a.area() < b.area();
	}
};

int main() {
	if (fopen("dome.in", "r")) {
		freopen("dome.in", "r", stdin);
		freopen("dome.out", "w", stdout);
	}
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int N; cin >> N;
	vector<pt> pts;
	for (int i = 0; i < N; i++) {
		double x, y, z; cin >> x >> y >> z;
		pts.push_back({sqrt(x * x + y * y), z});
	}
	sort(pts.begin(), pts.end());
	vector<pt> hull;
	for (pt nxt : pts) {
		while (!hull.empty() && nxt.y >= hull.back().y) {
			hull.pop_back();
		}
		while (hull.size() >= 2) {
			pt cur = hull.back(); hull.pop_back();
			pt prv = hull.back();
			if ((cur.x - prv.x) * (nxt.y - prv.y) - (cur.y - prv.y) * (nxt.x - prv.x) < 0) {
				hull.push_back(cur);
				break;
			} else {
				continue;
			}
		}
		hull.push_back(nxt);
	}
	cone best{1e9, 1e9};
	vector<cone> cones;
	for (int i = 0; i+1 < int(hull.size()); i++) {
		pt a = hull[i], b = hull[i+1];
		double h = a.y + a.x * (a.y - b.y) / (b.x - a.x);
		double r = b.x + b.y * (b.x - a.x) / (a.y - b.y);
		cone cnd{h, r};
		best = min(best, cnd);
		cones.push_back(cnd);
	}
	for (int i = 0; i < int(hull.size()); i++) {
		pt a = hull[i];
		if (a.x < 1e-9) continue;
		cone cnd{3 * a.y, 3 * a.x / 2};
		bool isGood = true;
		if (i > 0) {
			if (cnd.h < cones[i-1].h) {
				isGood = false;
			}
		}
		if (i + 1 < int(hull.size())) {
			if (cnd.h > cones[i].h) {
				isGood = false;
			}
		}
		if (isGood) {
			best = min(best, cnd);
		}
	}
	cout << fixed << setprecision(10) << best.h << ' ' << best.r << '\n';

	return 0;
}

// x / r + y / h = 1
// x / r = p, y / h = 1-p
// r = x/p, h = y / (1-p)
// min(r^2 h)
// min(x^2 y / (p^2 * (1-p)))
// max(p^2 * (1-p))
// p = 2/3
// r = 3x/2
// h = 3y
