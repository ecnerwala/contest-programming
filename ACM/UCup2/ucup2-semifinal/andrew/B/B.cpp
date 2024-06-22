#include <bits/stdc++.h>

template <typename T, int NDIMS> struct tensor_view {
	static_assert(NDIMS >= 0, "NDIMS must be nonnegative");

protected:
	std::array<int, NDIMS> shape;
	std::array<int, NDIMS> strides;
	T* data;

	tensor_view(std::array<int, NDIMS> shape_, std::array<int, NDIMS> strides_, T* data_) : shape(shape_), strides(strides_), data(data_) {}

public:
	tensor_view() : shape{0}, strides{0}, data(nullptr) {}

protected:
	int flatten_index(std::array<int, NDIMS> idx) const {
		int res = 0;
		for (int i = 0; i < NDIMS; i++) { res += idx[i] * strides[i]; }
		return res;
	}
	int flatten_index_checked(std::array<int, NDIMS> idx) const {
		int res = 0;
		for (int i = 0; i < NDIMS; i++) {
			assert(0 <= idx[i] && idx[i] < shape[i]);
			res += idx[i] * strides[i];
		}
		return res;
	}

public:
	T& operator[] (std::array<int, NDIMS> idx) const {
#ifdef _GLIBCXX_DEBUG
		return data[flatten_index_checked(idx)];
#else
		return data[flatten_index(idx)];
#endif
	}
	T& at(std::array<int, NDIMS> idx) const {
		return data[flatten_index_checked(idx)];
	}

	template <int D = NDIMS>
	typename std::enable_if<(0 < D), tensor_view<T, NDIMS-1>>::type operator[] (int idx) const {
		std::array<int, NDIMS-1> nshape; std::copy(shape.begin()+1, shape.end(), nshape.begin());
		std::array<int, NDIMS-1> nstrides; std::copy(strides.begin()+1, strides.end(), nstrides.begin());
		T* ndata = data + (strides[0] * idx);
		return tensor_view<T, NDIMS-1>(nshape, nstrides, ndata);
	}
	template <int D = NDIMS>
	typename std::enable_if<(0 < D), tensor_view<T, NDIMS-1>>::type at(int idx) const {
		assert(0 <= idx && idx < shape[0]);
		return operator[](idx);
	}

	template <int D = NDIMS>
	typename std::enable_if<(0 == D), T&>::type operator * () const {
		return *data;
	}

	template <typename U, int D> friend struct tensor_view;
	template <typename U, int D> friend struct tensor;
};

template <typename T, int NDIMS> struct tensor {
	static_assert(NDIMS >= 0, "NDIMS must be nonnegative");

protected:
	std::array<int, NDIMS> shape;
	std::array<int, NDIMS> strides;
	int len;
	T* data;

public:
	tensor() : shape{0}, strides{0}, len(0), data(nullptr) {}

	explicit tensor(std::array<int, NDIMS> shape_, const T& t = T()) {
		shape = shape_;
		len = 1;
		for (int i = NDIMS-1; i >= 0; i--) {
			strides[i] = len;
			len *= shape[i];
		}
		data = new T[len];
		std::fill(data, data + len, t);
	}

	tensor(const tensor& o) : shape(o.shape), strides(o.strides), len(o.len), data(new T[len]) {
		for (int i = 0; i < len; i++) {
			data[i] = o.data[i];
		}
	}

	tensor& operator=(tensor&& o) noexcept {
		using std::swap;
		swap(shape, o.shape);
		swap(strides, o.strides);
		swap(len, o.len);
		swap(data, o.data);
		return *this;
	}
	tensor(tensor&& o) : tensor() {
		*this = std::move(o);
	}
	tensor& operator=(const tensor& o) {
		return *this = tensor(o);
	}
	~tensor() { delete[] data; }

	using view_t = tensor_view<T, NDIMS>;
	view_t view() {
		return tensor_view<T, NDIMS>(shape, strides, data);
	}
	operator view_t() {
		return view();
	}

	using const_view_t = tensor_view<const T, NDIMS>;
	const_view_t view() const {
		return tensor_view<const T, NDIMS>(shape, strides, data);
	}
	operator const_view_t() const {
		return view();
	}

	T& operator[] (std::array<int, NDIMS> idx) { return view()[idx]; }
	T& at(std::array<int, NDIMS> idx) { return view().at(idx); }
	const T& operator[] (std::array<int, NDIMS> idx) const { return view()[idx]; }
	const T& at(std::array<int, NDIMS> idx) const { return view().at(idx); }

	template <int D = NDIMS>
	typename std::enable_if<(0 < D), tensor_view<T, NDIMS-1>>::type operator[] (int idx) {
		return view()[idx];
	}
	template <int D = NDIMS>
	typename std::enable_if<(0 < D), tensor_view<T, NDIMS-1>>::type at(int idx) {
		return view().at(idx);
	}

	template <int D = NDIMS>
	typename std::enable_if<(0 < D), tensor_view<const T, NDIMS-1>>::type operator[] (int idx) const {
		return view()[idx];
	}
	template <int D = NDIMS>
	typename std::enable_if<(0 < D), tensor_view<const T, NDIMS-1>>::type at(int idx) const {
		return view().at(idx);
	}

	template <int D = NDIMS>
	typename std::enable_if<(0 == D), T&>::type operator * () {
		return *view();
	}
	template <int D = NDIMS>
	typename std::enable_if<(0 == D), const T&>::type operator * () const {
		return *view();
	}
};


template <typename T> struct Point {
public:
	T x, y;
	Point() : x(0), y(0) {}
	Point(T x_, T y_) : x(x_), y(y_) {}
	template <typename U> explicit Point(const Point<U>& p) : x(p.x), y(p.y) {}
	Point(const std::pair<T, T>& p) : x(p.first), y(p.second) {}
	Point(const std::complex<T>& p) : x(real(p)), y(imag(p)) {}
	explicit operator std::pair<T, T> () const { return std::pair<T, T>(x, y); }
	explicit operator std::complex<T> () const { return std::complex<T>(x, y); }

	friend std::ostream& operator << (std::ostream& o, const Point& p) { return o << '(' << p.x << ',' << p.y << ')'; }
	friend std::istream& operator >> (std::istream& i, Point& p) { return i >> p.x >> p.y; }
	friend bool operator == (const Point& a, const Point& b) { return a.x == b.x && a.y == b.y; }
	friend bool operator != (const Point& a, const Point& b) { return !(a==b); }

	Point operator + () const { return Point(+x, +y); }
	Point operator - () const { return Point(-x, -y); }

	Point& operator += (const Point& p) { x += p.x, y += p.y; return *this; }
	Point& operator -= (const Point& p) { x -= p.x, y -= p.y; return *this; }
	Point& operator *= (const T& t) { x *= t, y *= t; return *this; }
	Point& operator /= (const T& t) { x /= t, y /= t; return *this; }

	friend Point operator + (const Point& a, const Point& b) { return Point(a.x+b.x, a.y+b.y); }
	friend Point operator - (const Point& a, const Point& b) { return Point(a.x-b.x, a.y-b.y); }
	friend Point operator * (const Point& a, const T& t) { return Point(a.x*t, a.y*t); }
	friend Point operator * (const T& t ,const Point& a) { return Point(t*a.x, t*a.y); }
	friend Point operator / (const Point& a, const T& t) { return Point(a.x/t, a.y/t); }

	T dist2() const { return x * x + y * y; }
	auto dist() const { return std::sqrt(dist2()); }
	Point unit() const { return *this / this->dist(); }
	auto angle() const { return std::atan2(y, x); }

	T int_norm() const { return __gcd(x,y); }
	Point int_unit() const { if (!x && !y) return *this; return *this / this->int_norm(); }

	// Convenient free-functions, mostly for generic interop
	friend auto norm(const Point& a) { return a.dist2(); }
	friend auto abs(const Point& a) { return a.dist(); }
	friend auto unit(const Point& a) { return a.unit(); }
	friend auto arg(const Point& a) { return a.angle(); }
	friend auto int_norm(const Point& a) { return a.int_norm(); }
	friend auto int_unit(const Point& a) { return a.int_unit(); }

	Point perp_cw() const { return Point(y, -x); }
	Point perp_ccw() const { return Point(-y, x); }

	friend T dot(const Point& a, const Point& b) { return a.x * b.x + a.y * b.y; }
	friend T cross(const Point& a, const Point& b) { return a.x * b.y - a.y * b.x; }
	friend T cross3(const Point& a, const Point& b, const Point& c) { return cross(b-a, c-a); }

	// Complex numbers and rotation
	friend Point conj(const Point& a) { return Point(a.x, -a.y); }

	// Returns conj(a) * b
	friend Point dot_cross(const Point& a, const Point& b) { return Point(dot(a, b), cross(a, b)); }
	friend Point cmul(const Point& a, const Point& b) { return dot_cross(conj(a), b); }
	friend Point cdiv(const Point& a, const Point& b) { return dot_cross(b, a) / b.norm(); }

	// Must be a unit vector; otherwise multiplies the result by abs(u)
	Point rotate(const Point& u) const { return dot_cross(conj(u), *this); }
	Point unrotate(const Point& u) const { return dot_cross(u, *this); }

	friend bool lex_less(const Point& a, const Point& b) {
		return std::tie(a.x, a.y) < std::tie(b.x, b.y);
	}

	friend bool same_dir(const Point& a, const Point& b) { return cross(a,b) == 0 && dot(a,b) > 0; }

	// check if 180 <= s..t < 360
	friend bool is_reflex(const Point& a, const Point& b) { auto c = cross(a,b); return c ? (c < 0) : (dot(a, b) < 0); }

	// operator < (s,t) for angles in [base,base+2pi)
	friend bool angle_less(const Point& base, const Point& s, const Point& t) {
		int r = is_reflex(base, s) - is_reflex(base, t);
		return r ? (r < 0) : (0 < cross(s, t));
	}

	friend auto angle_cmp(const Point& base) {
		return [base](const Point& s, const Point& t) { return angle_less(base, s, t); };
	}
	friend auto angle_cmp_center(const Point& center, const Point& dir) {
		return [center, dir](const Point& s, const Point& t) -> bool { return angle_less(dir, s-center, t-center); };
	}

	// is p in [s,t] taken ccw? 1/0/-1 for in/border/out
	friend int angle_between(const Point& s, const Point& t, const Point& p) {
		if (same_dir(p, s) || same_dir(p, t)) return 0;
		return angle_less(s, p, t) ? 1 : -1;
	}
};

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	using pt_t = Point<int>;
	int N_tot; cin >> N_tot;
	std::vector<pt_t> orig_pts(N_tot);
	for (auto& p : orig_pts) cin >> p;

	std::vector<pt_t> hull_pts; hull_pts.reserve(N_tot);
	std::vector<pt_t> interior; interior.reserve(N_tot);

	{
		std::sort(orig_pts.begin(), orig_pts.end(), [&](auto a, auto b) {
			return std::tie(a.x, a.y) < std::tie(b.x, b.y);
		});
		std::vector<pt_t> upper; upper.reserve(N_tot);
		std::vector<pt_t> lower; lower.reserve(N_tot);
		for (int i = 0; i < N_tot; i++) {
			pt_t cur = orig_pts[i];
			while (upper.size() >= 2 && cross3(upper.end()[-2], upper.end()[-1], cur) > 0) upper.pop_back();
			while (lower.size() >= 2 && cross3(lower.end()[-2], lower.end()[-1], cur) < 0) lower.pop_back();
			upper.push_back(cur);
			lower.push_back(cur);
		}
		std::vector<bool> is_hull(N_tot);
		for (int i = 0, j = 0; i < N_tot; i++) {
			if (j < int(upper.size()) && orig_pts[i] == upper[j]) {
				is_hull[i] = true;
				j++;
			}
		}
		for (int i = 0, j = 0; i < N_tot; i++) {
			if (j < int(lower.size()) && orig_pts[i] == lower[j]) {
				is_hull[i] = true;
				j++;
			}
		}
		hull_pts.insert(hull_pts.end(), lower.begin(), lower.end()-1);
		hull_pts.insert(hull_pts.end(), upper.rbegin(), upper.rend()-1);
		for (int i = 0; i < N_tot; i++) {
			if (!is_hull[i]) interior.push_back(orig_pts[i]);
		}
	}
	int H = int(hull_pts.size());
	int P = int(interior.size());
	assert(H + P == N_tot);

	auto hull = [&](int a) -> pt_t {
		a %= H;
		if (a < 0) a += H;
		return hull_pts[a];
	};

	cerr << "hull " << H << '\n';
	for (auto p : hull_pts) { cerr << p << '\n'; }
	cerr << "interior " << P << '\n';
	for (auto p : interior) { cerr << p << '\n'; }

	tensor<int, 2> inter_pt({P, P}, -1);
	// index i always refers to hull(i) -> hull(i+1) on the hull
	tensor<bool, 2> bad_tri({P, H});
	int64_t ans_tri_pair = 0;
	int64_t tot_good_tris = 0;
	for (int i = 0; i < P; i++) {
		for (int j = 0; j < P; j++) {
			if (i == j) continue;
			int v = -1;
			for (int h = 0; h < H; h++) {
				assert(cross3(interior[i], hull(h), hull(h+1)) > 0);
				assert(cross3(interior[j], hull(h), hull(h+1)) > 0);
				if (cross3(interior[i], hull(h), interior[j]) > 0 && cross3(interior[i], interior[j], hull(h+1)) > 0) {
					assert(v == -1);
					v = h;
					bad_tri[{i, h}] = true;
				}
			}
			assert(v != -1);
			inter_pt[{i, j}] = v;
		}
		int own_good_tris = 0;
		for (int h = 0; h < H; h++) {
			own_good_tris += !bad_tri[{i, h}];
		}
		ans_tri_pair += int64_t(tot_good_tris) * int64_t(own_good_tris);
		tot_good_tris += own_good_tris;
	}
	cerr << "inter_pt " << '\n';
	for (int i = 0; i < P; i++) {
		for (int j = 0; j < P; j++) {
			cerr << inter_pt[{i, j}] << ' ';
		}
		cerr << '\n';
	}
	cerr << "bad_tri" << '\n';
	for (int i = 0; i < P; i++) {
		for (int j = 0; j < H; j++) {
			cerr << bad_tri[{i, j}] << ' ';
		}
		cerr << '\n';
	}


	int64_t ans_quad = 0;

	for (int i = 0; i < P; i++) {
		for (int j = 0; j < P; j++) {
			if (i == j) continue;
			auto pi = interior[i];
			auto pj = interior[j];
			int lo = inter_pt[{i, j}];
			int hi = inter_pt[{j, i}];
			assert(cross3(pi, pj, hull(lo+1)) > 0);
			assert(cross3(pi, pj, hull(lo)) < 0);
			assert(cross3(pi, pj, hull(hi)) > 0);
			assert(cross3(pi, pj, hull(hi+1)) < 0);
			assert(lo != hi);
			if (hi < lo) hi += H;
			assert(lo < hi);

			cerr << "running " << i << '-' << j << ' ' << lo << '-' << hi << '\n';

			// First, count the bad triangles
			{
				int num_bad = 0;
				int cnt = 0;
				for (int h = lo + 1; h < hi; h++) {
					cerr << "h " << h << ' ' << !bad_tri[{i, h%H}] << ' ' << !bad_tri[{j, h%H}] << '\n';
					if (!bad_tri[{i, h%H}]) {
						cnt++;
					}
					if (!bad_tri[{j, h%H}]) {
						cerr << "adding " << cnt << '\n';
						num_bad += cnt;
					}
				}
				cerr << i << '-' << j << ' ' << num_bad << '\n';
				ans_tri_pair -= num_bad;
			}

			std::vector<int> pref(hi-lo+2, 0);
			// Now, let's count
			auto mark_bad = [&](int first, int last) -> void {
				// inclusive range
				assert(lo <= first && first <= last && last <= hi);
				assert(first <= last);
				pref[first-lo]++;
				pref[last-lo+1]--;
			};
			for (int k = 0; k < P; k++) {
				if (cross3(pi, pj, interior[k]) > 0) {
					// K is in the upper half plane, so let's deal with it
					int a = inter_pt[{i, k}];
					if (a < lo) a += H;
					assert(lo <= a && a <= hi);
					int b = inter_pt[{j, k}];
					if (b < lo) b += H;
					assert(lo <= b && b <= hi);
					assert(a <= b);
					mark_bad(a, b);
				}
			}
			// take care of bottom half plane stuff
			if (bad_tri[{j, lo%H}]) {
				// lo is bad
				mark_bad(lo, lo);
			}
			if (bad_tri[{i, hi%H}]) {
				// hi is bad
				mark_bad(hi, hi);
			}
			int num_good = 0;
			for (int k = 0; k+1 < int(pref.size()); k++) {
				if (pref[k] == 0) num_good++;
				pref[k+1] += pref[k];
			}
			assert(pref.back() == 0);
			cerr << i << "--" << j << ' ' << num_good << '\n';
			ans_quad += num_good;
		}
	}

	int64_t ans = 1;
	ans += tot_good_tris;
	ans += ans_tri_pair;
	ans += ans_quad;
	cout << ans << '\n';

	return 0;
}

/*
icpc@ucup00:~/andrew/B$ make test
mkdir -p archive
cp B.cpp archive/B-$(date +%Y-%m-%d-%H%M%S).cpp
g++ -std=c++20 -O2 -g -Wall -Wextra -pedantic -Wshadow -Wformat=2 -Wfloat-equal -Wconversion -Wlogical-op -Wshift-overflow=2 -Wduplicated-cond -Wcast-qual -Wcast-align -Wno-unused-result -Wno-sign-conversion -fsanitize=address -fsanitize=undefined -fsanitize=float-divide-by-zero -fsanitize=float-cast-overflow -fno-sanitize-recover=all -fstack-protector-all -D_FORTIFY_SOURCE=2 -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC    -isystem /home/icpc/.precompiled_headers B.cpp   -o B
\time ./B < s1.in > s1.res
hull 5
(0,2)
(1,0)
(3,0)
(4,2)
(2,5)
interior 2
(1,3)
(3,3)
inter_pt 
-1 3 
4 -1 
bad_tri
0 0 0 1 0 
0 0 0 0 1 
0-1 0
0--1 2
1-0 3
1--0 5

  real  0.00s
  user  0.00s
  sys   0.00s
  mem   8080KB
Built with DEBUG flags enabled, code may be slower than normal
diff s1.res s1.out
1c1
< 29
---
> 26
make: *** [Makefile:114: __test_s1] Error 1
icpc@ucup00:~/andrew/B$ make runs
\time ./B < s2.in > s2.res
hull 3
(0,0)
(4,0)
(3,3)
interior 2
(2,1)
(3,1)
inter_pt 
-1 1 
2 -1 
bad_tri
0 1 0 
0 0 1 
0-1 0
0--1 2
1-0 1
1--0 3

  real  0.00s
  user  0.00s
  sys   0.00s
  mem   7972KB
Built with DEBUG flags enabled, code may be slower than normal
\time ./B < s3.in > s3.res
hull 3
(0,0)
(3,0)
(0,3)
interior 0
inter_pt 
bad_tri

  real  0.00s
  user  0.00s
  sys   0.00s
  mem   8088KB
Built with DEBUG flags enabled, code may be slower than normal
icpc@ucup00:~/andrew/B$ make test
diff s1.res s1.out
1c1
< 29
---
> 26
make: [Makefile:114: __test_s1] Error 1 (ignored)
diff s2.res s2.out
diff s3.res s3.out
*/
