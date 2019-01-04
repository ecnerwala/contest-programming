#include<bits/stdc++.h>
using namespace std;

template <typename T> struct Point {
	using pair = std::pair<T, T>;
	T x, y;
	Point() : x(0), y(0) {}
	Point(T x_, T y_) : x(x_), y(y_) {}

	explicit operator pair() { return pair(x, y); }
	friend std::istream& operator >> (std::istream& in, Point& p) {
		return in >> p.x >> p.y;
	}
	friend std::ostream& operator << (std::ostream& out, const Point& p) {
		return out << "(" << p.x << "," << p.y << ")";
	}

	friend bool operator == (const Point& a, const Point& b) {
		return a.x == b.x && a.y == b.y;
	}
	friend bool operator != (const Point& a, const Point& b) {
		return a.x != b.x || a.y != b.y;
	}

	Point operator - (const Point& p) { return Point(-p.x, -p.y); }

	Point& operator += (const Point& p) {
		x += p.x; y += p.y;
		return *this;
	}
	Point& operator -= (const Point& p) {
		x -= p.x; y -= p.y;
		return *this;
	}
	Point& operator *= (const T& t) {
		x *= t, y *= t;
		return *this;
	}
	Point& operator /= (const T& t) {
		x /= t, y /= t;
		return *this;
	}
	friend Point operator + (const Point& a, const Point& b) { return Point(a) += b; }
	friend Point operator - (const Point& a, const Point& b) { return Point(a) -= b; }
	friend Point operator * (const Point& a, const T& b) { return Point(a) *= b; }
	friend Point operator / (const Point& a, const T& b) { return Point(a) /= b; }
};

using ll = long long;
using P = Point<ll>;
using Seg = pair<P, P>;

struct Polygon {
	static const int MAXN = 11000;
	int N;
	P pts[MAXN + 2];
	friend std::istream& operator >> (std::istream& in, Polygon& p) {
		in >> p.N;
		assert(p.N > 0);
		for (int i = 0; i < p.N; i++) {
			in >> p.pts[i];
		}
		// this should be enough circleness
		p.pts[p.N] = p.pts[0];
		p.pts[p.N+1] = p.pts[1];
		return in;
	}
} A, B, AB;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> A >> B >> AB;

	cout << "OK" << '\n';

	return 0;
}
