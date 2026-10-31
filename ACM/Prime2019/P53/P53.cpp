#include<bits/stdc++.h>
using namespace std;

template <typename T> int signum (T a) { return (a > 0) - (a < 0); }

template <typename T> struct Point2D {
	using P = Point2D;

	T x, y;

	Point2D() : x(0), y(0) {}
	Point2D(T x_, T y_) : x(x_), y(y_) {}

	friend std::ostream& operator << (std::ostream& o, const P& p) { return o << p.x << ' ' << p.y; }
	friend std::istream& operator >> (std::istream& i, P& p) { return i >> p.x >> p.y; }

	friend bool operator == (const P& a, const P& b) { return a.x == b.x && a.y == b.y; }
	friend bool operator != (const P& a, const P& b) { return a.x != b.x || a.y != b.y; }

	P& operator += (const P& o) { x += o.x; y += o.y; return *this; }
	P& operator -= (const P& o) { x -= o.x; y -= o.y; return *this;}
	P& operator *= (const T& v) { x *= v; y *= v; return *this; }
	P& operator /= (const T& v) { x /= v; y /= v; return *this; }
	friend P operator + (const P& a, const P& b) { return P(a) += b; }
	friend P operator - (const P& a, const P& b) { return P(a) -= b; }
	friend P operator * (const P& a, const T& v) { return P(a) *= v; }
	friend P operator / (const P& a, const T& v) { return P(a) /= v; }

	friend T norm(const P& a) { return a.x * a.x + a.y * a.y; }
	friend T dot(const P& a, const P& b) { return a.x * b.x + a.y * b.y; }
	friend T cross(const P& a, const P& b) { return a.x * b.y - a.y * b.x; }
	friend T area(const P& a, const P& b, const P& c) { return cross(b-a, c-a); }
};

template <typename T> vector<Point2D<T>> convexHull(vector<Point2D<T>>& pts) {
	sort(pts.begin(), pts.end(), [](const Point2D<T>& a, const Point2D<T>& b) { return tie(a.x, a.y) < tie(b.x, b.y); });
	pts.resize(unique(pts.begin(), pts.end()) - pts.begin());
	if (pts.size() <= 1) return pts;

	vector<Point2D<T>> upper, lower;
	for (auto pt : pts) {
		while (upper.size() >= 2 && area(upper[int(upper.size())-2], upper[int(upper.size())-1], pt) >= 0) upper.pop_back();
		upper.push_back(pt);
		while (lower.size() >= 2 && area(lower[int(lower.size())-2], lower[int(lower.size())-1], pt) <= 0) lower.pop_back();
		lower.push_back(pt);
	}
	lower.pop_back();
	lower.insert(lower.end(), upper.rbegin(), upper.rend());
	lower.pop_back();
	return lower;
}

using P = Point2D<long long>;

const int MAXN = 5.1e4;
const int MAXK = 55;
int N, K;
P pts[MAXN];
vector<P> hulls[MAXK];

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int T; cin >> T;
	while (T--) {
		cin >> N >> K;
		for (int i = 0; i < N; i++) {
			cin >> pts[i];
		}

		for (int i = 0; i <= K; i++) {
			hulls[i] = {};
		}
	}

	return 0;
}
