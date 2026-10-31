#include <bits/stdc++.h>

namespace kactl {
using ll = long long;
struct Line {
	mutable ll k, m, p;
	bool operator<(const Line& o) const { return k < o.k; }
	bool operator<(ll x) const { return p < x; }
};

struct LineContainer : std::multiset<Line, std::less<>> {
	// (for doubles, use inf = 1/.0, div(a,b) = a/b)
	static const ll inf = LLONG_MAX;
	ll div(ll a, ll b) { // floored division
		return a / b - ((a ^ b) < 0 && a % b); }
	bool isect(iterator x, iterator y) {
		if (y == end()) return x->p = inf, 0;
		if (x->k == y->k) x->p = x->m > y->m ? inf : -inf;
		else x->p = div(y->m - x->m, x->k - y->k);
		return x->p >= y->p;
	}
	void add(ll k, ll m) {
		auto z = insert({k, m, 0}), y = z++, x = y;
		while (isect(y, z)) z = erase(z);
		if (x != begin() && isect(--x, y)) isect(x, y = erase(y));
		while ((y = x) != begin() && (--x)->p >= y->p)
			isect(x, erase(y));
	}
	ll query(ll x) {
		assert(!empty());
		auto l = *lower_bound(x);
		return l.k * x + l.m;
	}
};
}

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
		return data[flatten_index(idx)];
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
		strides[NDIMS-1] = 1;
		for (int i = NDIMS-1; i > 0; i--) {
			strides[i-1] = strides[i] * shape[i];
		}
		len = strides[0] * shape[0];
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

template <typename T> void setmax(T& a, T b) { if (b > a) a = b; }

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, Q; cin >> N >> Q;
	using pt_t = array<int64_t, 2>;
	vector<pair<array<pt_t, 2>, int64_t>> VIPS(N);
	vector<int64_t> Xs; Xs.reserve(2*N);
	vector<int64_t> Ys; Ys.reserve(2*N);
	for (auto& v : VIPS) {
		int64_t t, a, b, c; cin >> t >> a >> b >> c; c /= 2;
		int64_t dt = abs(a-b);
		pt_t p0{t+a, t-a};
		pt_t p1{t+dt+b, t+dt-b};
		v = {{p0, p1}, c};

		Xs.push_back(p0[0]);
		Ys.push_back(p0[1]);
		Xs.push_back(p1[0]);
		Ys.push_back(p1[1]);
	}

	sort(Xs.begin(), Xs.end());
	Xs.resize(unique(Xs.begin(), Xs.end()) - Xs.begin());
	sort(Ys.begin(), Ys.end());
	Ys.resize(unique(Ys.begin(), Ys.end()) - Ys.begin());

	int X = int(Xs.size());
	int Y = int(Ys.size());

	tensor<int64_t, 2> dx_weight({X,Y}, 0);
	tensor<int64_t, 2> dy_weight({X,Y}, 0);

	for (const auto& v : VIPS) {
		int x0 = int(lower_bound(Xs.begin(), Xs.end(), v.first[0][0]) - Xs.begin());
		int y0 = int(lower_bound(Ys.begin(), Ys.end(), v.first[0][1]) - Ys.begin());
		int x1 = int(lower_bound(Xs.begin(), Xs.end(), v.first[1][0]) - Xs.begin());
		int y1 = int(lower_bound(Ys.begin(), Ys.end(), v.first[1][1]) - Ys.begin());

		if (x0 == x1) {
			int x = x0;
			for (int y = y0+1; y <= y1; y++) {
				setmax(dy_weight[{x, y}], v.second);
			}
		} else {
			int y = y0;
			for (int x = x0+1; x <= x1; x++) {
				setmax(dx_weight[{x, y}], v.second);
			}
		}
	}

	tensor<int64_t, 2> dp({X, Y}, 0);
	for (int x = X-1; x >= 0; x--) {
		for (int y = Y-1; y >= 0; y--) {
			if (x+1 < X) {
				setmax(dp[{x,y}], dp[{x+1,y}] + dx_weight[{x+1,y}] * (Xs[x+1] - Xs[x]));
			}
			if (y+1 < Y) {
				setmax(dp[{x,y}], dp[{x,y+1}] + dy_weight[{x,y+1}] * (Ys[y+1] - Ys[y]));
			}
		}
	}

	tensor<vector<pair<int, pt_t>>, 2> queries({X,Y});
	vector<int64_t> ans(Q);
	for (int q = 0; q < Q; q++) {
		int64_t t, pos; cin >> t >> pos;
		pt_t loc{t + pos, t - pos};
		int x = int(lower_bound(Xs.begin(), Xs.end(), loc[0]) - Xs.begin());
		int y = int(lower_bound(Ys.begin(), Ys.end(), loc[1]) - Ys.begin());
		if (x == X || y == Y) {
			ans[q] = 0;
			continue;
		}
		queries[{x,y}].push_back({q, loc});
	}

	for (int x = X-1; x >= 0; x--) {
		kactl::LineContainer lc;
		for (int y = Y-1; y >= 0; y--) {
			lc.add(dx_weight[{x, y}], dp[{x,y}]);
			for (auto [q, p] : queries[{x, y}]) {
				setmax(ans[q], int64_t(lc.query(Xs[x] - p[0])));
			}
		}
	}

	for (int y = Y-1; y >= 0; y--) {
		kactl::LineContainer lc;
		for (int x = X-1; x >= 0; x--) {
			lc.add(dy_weight[{x,y}], dp[{x,y}]);
			for (auto [q, p] : queries[{x, y}]) {
				setmax(ans[q], int64_t(lc.query(Ys[y] - p[1])));
			}
		}
	}

	for (int q = 0; q < Q; q++) {
		cout << ans[q] << '\n';
	}

	return 0;
}
