#include <bits/stdc++.h>

struct Zrt2 {
	int x, y;
	Zrt2() : x(0), y(0) {}
	Zrt2(int x_, int y_) : x(x_), y(y_) {}
	// Hack to allow implicit cast from literal 0
	Zrt2(std::nullptr_t) : x(0), y(0) {}

	friend std::istream& operator >> (std::istream& i, Zrt2& a) { return i >> a.x >> a.y; }
	friend std::ostream& operator << (std::ostream& o, const Zrt2& a) { return o << a.x << "+" << a.y << "rt2"; }

	friend bool operator == (const Zrt2& a, const Zrt2& b) { return a.x == b.x && a.y == b.y; }
	friend bool operator != (const Zrt2& a, const Zrt2& b) { return a.x != b.x || a.y != b.y; }

	// not reduced signum, use at your own risk
	int64_t signum() const {
		auto sq_2 = [](int a) { return int64_t(a) * int64_t(a) * (a < 0 ? -1 : 1); };
		return sq_2(x) + sq_2(y) * 2;
	}

	Zrt2 operator - () const { return Zrt2(-x, -y); }

	Zrt2& operator += (const Zrt2& o) {
		x += o.x; y += o.y;
		return *this;
	}
	Zrt2& operator -= (const Zrt2& o) {
		x -= o.x; y -= o.y;
		return *this;
	}

	friend Zrt2 operator + (const Zrt2& a, const Zrt2& b) { return Zrt2(a.x + b.x, a.y + b.y); }
	friend Zrt2 operator - (const Zrt2& a, const Zrt2& b) { return Zrt2(a.x - b.x, a.y - b.y); }

	friend bool operator < (const Zrt2& a, const Zrt2& b) { return (a-b).signum() < 0; }
	friend bool operator > (const Zrt2& a, const Zrt2& b) { return (a-b).signum() > 0; }
	friend bool operator <= (const Zrt2& a, const Zrt2& b) { return (a-b).signum() <= 0; }
	friend bool operator >= (const Zrt2& a, const Zrt2& b) { return (a-b).signum() >= 0; }

	friend int64_t cross(const Zrt2& a, const Zrt2& b) {
		return int64_t(a.x) * int64_t(b.y) - int64_t(a.y) * int64_t(b.x);
	}
};

bool can_make(Zrt2 V) {
	assert(V >= 0);
	Zrt2 hi(1, 0);
	Zrt2 lo(0, -1);
	while (true) {
		assert(hi.x >= 0 && hi.y <= 0);
		assert(lo.x >= 0 && lo.y <= 0);
		assert(hi > 0 && -lo > 0);
		assert(cross(lo, hi) == 1);
		assert(cross(hi, -lo) == 1);

		auto test_val = [&](Zrt2 curV) -> bool {
			// nonnegative linear combination of hi and -lo
			curV.x >>= 1, curV.y >>= 1; // Floor division by 2
			return cross(hi, curV) >= 0 && cross(curV, -lo) >= 0;
		};

		Zrt2 closer = (hi.x > lo.x) ? hi : lo;
		Zrt2 a = V - Zrt2(closer.x, 0);
		Zrt2 b = V - Zrt2(0, -closer.y);
		if (a < 0 && b < 0) return false;
		if (test_val(a) || test_val(b)) return true;

		Zrt2 md = lo + hi;
		if (md < 0) lo = md;
		else hi = md;
	}
	assert(false);
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int Q; cin >> Q;
	while (Q--) {
		Zrt2 V; cin >> V;
		cout << (can_make(V) ? "Yes" : "No") << '\n';
	}

	return 0;
}
