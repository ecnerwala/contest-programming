#include <bits/stdc++.h>

// This represents a point x, y; for this problem, we'll use that to represent x + y sqrt(2)
struct Point {
	int x, y;
	Point() : x(0), y(0) {}
	Point(int x_, int y_) : x(x_), y(y_) {}
	// Hack to allow implicit cast from literal 0
	Point(std::nullptr_t) : x(0), y(0) {}

	friend std::istream& operator >> (std::istream& i, Point& a) { return i >> a.x >> a.y; }
	friend std::ostream& operator << (std::ostream& o, const Point& a) { return o << "(" << a.x << "," << a.y << ")"; }

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

// Check if t1 * V.x + t2 * V.y can be made, given oracle access to the steps
// of the path of t2/t1 in the Stern-Brocot tree. When called successively,
// `mediant_low_generator` should return true if the mediant < t2/t1 for the
// current step; e.g. it should return true on the first call iff t2/t1 > 1/1.
template <typename F> bool can_make(Point V, F mediant_low_generator) {
	// We maintain the current interval of the Stern-Brocot tree as
	//   lo.x / -lo.y < t2/t1 < hi.x / -hi.y
	Point lo(0, -1);
	Point hi(1, 0);
	Point must_reach(0, 0); // Initialize this to whatever small
	while (true) {
		assert(hi.x >= 0 && hi.y <= 0);
		assert(lo.x >= 0 && lo.y <= 0);
		assert(cross(lo, hi) == 1);
		assert(cross(hi, -lo) == 1);

		Point a = V - Point(must_reach.x, 0);
		Point b = V - Point(0, -must_reach.y);

		// Floor division by 2, because when doubling back, we can only add
		// even multiples of parts of the path; the remainder will require an
		// extra t1 or t2 at the start.
		a.x >>= 1, a.y >>= 1;
		b.x >>= 1, b.y >>= 1;

		auto ah = cross(hi, a), al = cross(a, -lo);
		if (ah >= 0 && al >= 0) return true;
		auto bh = cross(hi, b), bl = cross(b, -lo);
		if (bh >= 0 && bl >= 0) return true;

		if (ah <= 0 && al <= 0 && bh <= 0 && bl <= 0) return false;

		Point md = lo + hi;
		must_reach = (mediant_low_generator() ? lo : hi) = md;
	}
	assert(false);
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int Q; cin >> Q;
	while (Q--) {
		Point V; cin >> V;

		// The continued fraction of sqrt(2) is [1; 2, 2, 2, 2]
		auto mediant_low_generator = [i=2] () mutable -> bool {
			return (++i) & 2;
		};

		cout << (can_make(V, mediant_low_generator) ? "Yes" : "No") << '\n';
	}

	return 0;
}
