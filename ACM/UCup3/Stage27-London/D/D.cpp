#include <bits/stdc++.h>
using std::cerr;

const double PI = acos(-1.);
const double TAU = 2 * PI;

template <typename T, typename AreaT=T, typename VolT=T> struct Point3D {
	using P = Point3D;

	T x, y, z;
	Point3D() : x(0), y(0), z(0) {}
	Point3D(T x_, T y_, T z_) : x(x_), y(y_), z(z_) {}

	template <typename U, typename V, typename W> explicit Point3D(const Point3D<U, V, W>& p) : x(T(p.x)), y(T(p.y)), z(T(p.z)) {}

	friend std::istream& operator >> (std::istream& i, P& p) { return i >> p.x >> p.y >> p.z; }
	friend std::ostream& operator << (std::ostream& o, const P& p) { return o << "(" << p.x << "," << p.y << "," << p.z << ")"; }

	friend bool operator == (const P& a, const P& b) { return a.x == b.x && a.y == b.y && a.z == b.z; }
	friend bool operator != (const P& a, const P& b) { return a.x != b.x || a.y != b.y || a.z != b.z; }

	P& operator += (const P& o) { x += o.x, y += o.y, z += o.z; return *this; }
	P& operator -= (const P& o) { x -= o.x, y -= o.y, z -= o.z; return *this; }
	friend P operator + (const P& a, const P& b) { return P(a) += b; }
	friend P operator - (const P& a, const P& b) { return P(a) -= b; }

	P& operator *= (const T& t) { x *= t, y *= t, z *= t; return *this; }
	P& operator /= (const T& t) { x /= t, y /= t, z /= t; return *this; }
	friend P operator * (const P& p, const T& t) { return P(p) *= t; }
	friend P operator * (const T& t, const P& p) { return P(p) *= t; }
	friend P operator / (const P& a, const T& t) { return P(a) /= t; }

	friend P operator + (const P& a) { return P(+a.x, +a.y, +a.z); }
	friend P operator - (const P& a) { return P(-a.x, -a.y, -a.z); }

	friend AreaT dot(const P& a, const P& b) { return AreaT(a.x) * AreaT(b.x) + AreaT(a.y) * AreaT(b.y) + AreaT(a.z) * AreaT(b.z); }
	friend AreaT norm(const P& a) { return dot(a,a); }
	// We're playing a little loose with this type, expliitly cast it if you need
	friend Point3D<AreaT, VolT> cross(const P& a, const P& b) { return Point3D<AreaT, VolT>(AreaT(a.y) * AreaT(b.z) - AreaT(a.z) * AreaT(b.y), AreaT(a.z) * AreaT(b.x) - AreaT(a.x) * AreaT(b.z), AreaT(a.x) * AreaT(b.y) - AreaT(a.y) * AreaT(b.x)); }

	friend T int_norm(const P& p) {
		return std::gcd(std::gcd(abs(p.x), abs(p.y)), abs(p.z));
	}
	friend P int_unit(const P& p) {
		T g = int_norm(p);
		return g ? p / g : p;
	}

	friend T abs(const P& a) { return std::sqrt(std::max(T(0), norm(a))); }
	friend P unit(const P& a) { return a / abs(a); }

	friend VolT vol(const P& a, const P& b, const P& c, const P& d) { return dot(cross(b-a, c-a), Point3D<AreaT, VolT>(d-a)); }

	friend bool lexLess(const P& a, const P& b) { return tie(a.x, a.y, a.z) < tie(b.x, b.y, b.z); }

	friend bool parallelSame(const P& a, const P& b) {
		assert(a != P());
		assert(b != P());
		return lexLess(a, P()) == lexLess(b, P());
	}
};

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

auto softplus = [](double d) { return std::log1p(std::exp(d)); };
auto deriv_softplus = [](double d) { return 1 / (1 + std::exp(-d)); };

int main() {
	std::ios_base::sync_with_stdio(false); std::cin.tie(nullptr);
	std::cerr << std::fixed << std::setprecision(10);
	std::cout << std::fixed << std::setprecision(10);
	using pt_t = Point3D<double>;
	int N; std::cin >> N;
	tensor<double, 2> D({N, N});
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			std::cin >> D[{i, j}];
		}
	}

	auto get_score = [&](std::vector<pt_t> cnd) -> double {
		double res = 0;
		for (int i = 0; i < int(cnd.size()); i++) {
			for (int j = 0; j < i; j++) {
				res += softplus(std::abs(D[{i, j}] - abs(cnd[i] - cnd[j])) / 0.1 - 1);
			}
		}
		return res;
	};
	auto gradient = [&](std::vector<pt_t> cnd, double learning_rate) -> std::vector<pt_t> {
		std::vector<pt_t> res(cnd.size());
		for (int i = 0; i < int(cnd.size()); i++) {
			pt_t cur(0, 0, 0);
			for (int j = 0; j < int(cnd.size()); j++) {
				if (j == i) continue;
				double dist = abs(cnd[i] - cnd[j]);
				double delta = (dist - D[{i, j}]);
				double s_deriv = deriv_softplus(std::abs(delta) / 0.1 - 1) / 0.1;
				if (delta > 0) {
					// we're too far away, so move closer
					cur += s_deriv * unit(cnd[j] - cnd[i]);
				} else {
					cur += s_deriv * unit(cnd[i] - cnd[j]);
				}
			}
			res[i] = cnd[i] + cur * learning_rate;
		}
		return res;
	};

	std::vector<pt_t> cur_best;

	std::mt19937 mt(48);
	for (int i = 0; i < N; i++) {
		std::vector<pt_t> nxt_best;
		double nxt_best_score;
		for (int z = 0; z < 50; z++) {
			std::vector<pt_t> cnd = cur_best;
			cnd.push_back(pt_t(
				std::uniform_real_distribution<double>(-1, 1)(mt),
				std::uniform_real_distribution<double>(-1, 1)(mt),
				std::uniform_real_distribution<double>(-1, 1)(mt)
			));
			for (int iter = 0; iter < 10; iter++) {
				cnd = gradient(cnd, 0.1);
			}
			double cnd_score = get_score(cnd);
			if (nxt_best.empty() || cnd_score < nxt_best_score) {
				nxt_best = cnd;
				nxt_best_score = cnd_score;
			}
		}
		for (int iter = 0; iter < 100; iter++) {
			nxt_best = gradient(nxt_best, 0.1);
		}
		pt_t tot(0, 0, 0);
		for (auto p : nxt_best) tot += p;
		tot /= int(nxt_best.size());
		for (auto& p : nxt_best) p -= tot;
		cur_best = nxt_best;
	}
	for (int iter = 0; iter < 1000; iter++) {
		cur_best = gradient(cur_best, 0.1);
	}
	for (int i = 0; i < N; i++) {
		std::cout << cur_best[i].x << ' ' << cur_best[i].y << ' ' << cur_best[i].z << '\n';
	}
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			cerr << std::abs(D[{i, j}] - abs(cur_best[i] - cur_best[j])) << " \n"[j+1==N];
		}
	}
}
