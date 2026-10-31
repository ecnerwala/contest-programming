#include <bits/stdc++.h>

// This represents a point x, y; for this problem, we'll use that to represent x + y sqrt(2)
struct Point {
	int x, y;
	Point() : x(0), y(0) {}
	Point(int x_, int y_) : x(x_), y(y_) {}
	// Hack to allow implicit cast from literal 0
	Point(std::nullptr_t) : x(0), y(0) {}

	friend std::istream& operator >> (std::istream& i, Point& a) { return i >> a.x >> a.y; }
	friend std::ostream& operator << (std::ostream& o, const Point& a) { return o << a.x << "+" << a.y << "rt2"; }

	friend bool operator == (const Point& a, const Point& b) { return a.x == b.x && a.y == b.y; }
	friend bool operator != (const Point& a, const Point& b) { return a.x != b.x || a.y != b.y; }

	Point operator - () const { return Point(-x, -y); }

	Point& operator += (const Point& o) {
		x += o.x; y += o.y;
		return *this;
	}
	Point& operator -= (const Point& o) {
		x -= o.x; y -= o.y;
		return *this;
	}

	friend Point operator + (const Point& a, const Point& b) { return Point(a.x + b.x, a.y + b.y); }
	friend Point operator - (const Point& a, const Point& b) { return Point(a.x - b.x, a.y - b.y); }

	friend int64_t cross(const Point& a, const Point& b) {
		return int64_t(a.x) * int64_t(b.y) - int64_t(a.y) * int64_t(b.x);
	}
};

// Check if a * V.x + b * V.y can be made, given oracle access to
// signum(Point v) = signum(a * v.x + b * v.y)
template <typename F> bool can_make(Point V, F signum) {
	Point hi(1, 0);
	Point lo(0, -1);
	Point must_reach(0, 0); // Initialize this to whatever small
	while (true) {
		assert(hi.x >= 0 && hi.y <= 0);
		assert(lo.x >= 0 && lo.y <= 0);
		assert(cross(lo, hi) == 1);
		assert(cross(hi, -lo) == 1);

		auto test_val = [&](Point curV) -> bool {
			// nonnegative linear combination of hi and -lo
			curV.x >>= 1, curV.y >>= 1; // Floor division by 2
			return cross(hi, curV) >= 0 && cross(curV, -lo) >= 0;
		};

		Point a = V - Point(must_reach.x, 0);
		Point b = V - Point(0, -must_reach.y);
		if (signum(a) < 0 && signum(b) < 0) return false;
		if (test_val(a) || test_val(b)) return true;

		Point md = lo + hi;
		must_reach = (signum(md) < 0 ? lo : hi) = md;
	}
	assert(false);
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int Q; cin >> Q;
	while (Q--) {
		Point V; cin >> V;

		// Returns signum(x + y * sqrt(2))
		auto signum = [](const Point& p) {
			auto sq_2 = [](int a) { return int64_t(a) * int64_t(a) * (a < 0 ? -1 : 1); };
			return sq_2(p.x) + sq_2(p.y) * 2;
		};

		cout << (can_make(V, signum) ? "Yes" : "No") << '\n';
	}

	return 0;
}
