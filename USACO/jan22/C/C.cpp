#include <bits/stdc++.h>

struct pt_t {
	int x, y;
	pt_t() : x(0), y(0) {}
	pt_t(int x_, int y_) : x(x_), y(y_) {}

	pt_t& operator += (pt_t o) {
		x += o.x;
		y += o.y;
		return *this;
	}
	friend pt_t operator + (pt_t a, pt_t b) {
		return pt_t(a.x + b.x, a.y + b.y);
	}
	pt_t& operator -= (pt_t o) {
		x -= o.x;
		y -= o.y;
		return *this;
	}
	friend pt_t operator - (pt_t a, pt_t b) {
		return pt_t(a.x - b.x, a.y - b.y);
	}

	friend int64_t cross(pt_t a, pt_t b) {
		return int64_t(a.x) * int64_t(b.y) - int64_t(a.y) * int64_t(b.x);
	}
	friend int64_t cross3(pt_t a, pt_t b, pt_t c) {
		return cross(b-a, c-a);
	}

	int64_t norm() const {
		return int64_t(x) * int64_t(x) + int64_t(y) * int64_t(y);
	}
};

std::vector<pt_t> hull(std::vector<pt_t> pts) {
	if (pts.size() <= 1) return pts;
	std::sort(pts.begin(), pts.end(), [](pt_t a, pt_t b) { return std::make_pair(a.x, a.y) < std::make_pair(b.x, b.y); });
	pts.resize(std::unique(pts.begin(), pts.end(),
			[](pt_t a, pt_t b) { return std::make_pair(a.x, a.y) == std::make_pair(b.x, b.y); }) - pts.begin());
	std::vector<pt_t> lo; lo.reserve(pts.size());
	std::vector<pt_t> hi; hi.reserve(pts.size());
	for (pt_t p : pts) {
		while (int(lo.size()) >= 2 && cross3(lo.end()[-2], lo.end()[-1], p) <= 0) lo.pop_back();
		while (int(hi.size()) >= 2 && cross3(hi.end()[-2], hi.end()[-1], p) >= 0) hi.pop_back();
		lo.push_back(p);
		hi.push_back(p);
	}
	lo.insert(lo.end(), hi.rbegin()+1, hi.rend()-1);
	return lo;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	pt_t cur(0, 0);
	std::vector<pt_t> angs; angs.reserve(int(2.1e5));
	for (int z = 0; z < N; z++) {
		int G; cin >> G;
		std::vector<pt_t> poly(G);
		for (auto& p : poly) cin >> p.x >> p.y;
		poly = hull(std::move(poly));
		cur += poly[0];
		for (int i = 0; i < int(poly.size()); i++) {
			pt_t diff = poly[i+1==int(poly.size()) ? 0 : i+1] - poly[i];
			angs.push_back(diff);
		}
	}
	std::sort(angs.begin(), angs.end(), [](pt_t a, pt_t b) {
		bool a_pos = std::make_pair(a.x, a.y) > std::make_pair(0, 0);
		bool b_pos = std::make_pair(b.x, b.y) > std::make_pair(0, 0);
		if (a_pos != b_pos) {
			return a_pos > b_pos;
		}
		return cross(a, b) > 0;
	});

	int64_t ans = cur.norm();
	for (auto v : angs) {
		cur += v;
		ans = std::max(ans, cur.norm());
	}
	cout << ans << '\n';

	return 0;
}
