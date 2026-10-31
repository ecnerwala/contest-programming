#include<bits/stdc++.h>

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
	std::enable_if_t<(0 < D), tensor_view<T, NDIMS-1>> operator[] (int idx) const {
		std::array<int, NDIMS-1> nshape; std::copy(shape.begin()+1, shape.end(), nshape.begin());
		std::array<int, NDIMS-1> nstrides; std::copy(strides.begin()+1, strides.end(), nstrides.begin());
		T* ndata = data + (strides[0] * idx);
		return tensor_view<T, NDIMS-1>(nshape, nstrides, ndata);
	}
	template <int D = NDIMS>
	std::enable_if_t<(0 < D), tensor_view<T, NDIMS-1>> at(int idx) const {
		assert(0 <= idx && idx < shape[0]);
		return operator[](idx);
	}

	template <int D = NDIMS>
	std::enable_if_t<(0 == D), T&> operator * () const {
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
	std::enable_if_t<(0 < D), tensor_view<T, NDIMS-1>> operator[] (int idx) {
		return view()[idx];
	}
	template <int D = NDIMS>
	std::enable_if_t<(0 < D), tensor_view<T, NDIMS-1>> at(int idx) {
		return view().at(idx);
	}

	template <int D = NDIMS>
	std::enable_if_t<(0 < D), tensor_view<const T, NDIMS-1>> operator[] (int idx) const {
		return view()[idx];
	}
	template <int D = NDIMS>
	std::enable_if_t<(0 < D), tensor_view<const T, NDIMS-1>> at(int idx) const {
		return view().at(idx);
	}

	template <int D = NDIMS>
	std::enable_if_t<(0 == D), T&> operator * () {
		return *view();
	}
	template <int D = NDIMS>
	std::enable_if_t<(0 == D), const T&> operator * () const {
		return *view();
	}
};

template <typename T> void setmin(T& a, T b) { if (b < a) a = b; }

namespace kactl {
using namespace std;

#define rep(i, a, b) for(int i = a; i < (b); ++i)
#define all(x) begin(x), end(x)
#define sz(x) (int)(x).size()
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;

struct TwoSat {
	int N;
	vector<vi> gr;
	vi values; // 0 = false, 1 = true

	TwoSat(int n = 0) : N(n), gr(2*n) {}

	int addVar() { // (optional)
		gr.emplace_back();
		gr.emplace_back();
		return N++;
	}

	void either(int f, int j) {
		f = max(2*f, -1-2*f);
		j = max(2*j, -1-2*j);
		gr[f].push_back(j^1);
		gr[j].push_back(f^1);
	}
	void setValue(int x) { either(x, x); }

	void atMostOne(const vi& li) { // (optional)
		if (sz(li) <= 1) return;
		int cur = ~li[0];
		rep(i,2,sz(li)) {
			int next = addVar();
			either(cur, ~li[i]);
			either(cur, next);
			either(~li[i], next);
			cur = ~next;
		}
		either(cur, ~li[1]);
	}

	vi val, comp, z; int time = 0;
	int dfs(int i) {
		int low = val[i] = ++time, x; z.push_back(i);
		for(int e : gr[i]) if (!comp[e])
			low = min(low, val[e] ?: dfs(e));
		if (low == val[i]) do {
			x = z.back(); z.pop_back();
			comp[x] = low;
			if (values[x>>1] == -1)
				values[x>>1] = x&1;
		} while (x != i);
		return val[i] = low;
	}

	bool solve() {
		values.assign(N, -1);
		val.assign(2*N, 0); comp = val;
		rep(i,0,2*N) if (!comp[i]) dfs(i);
		rep(i,0,N) if (comp[2*i] == comp[2*i+1]) return 0;
		return 1;
	}
};

#undef rep
#undef all
#undef sz
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int S, T; cin >> S >> T;

	auto dist = [](array<int, 2> a, array<int, 2> b) -> double {
		return hypot(a[0] - b[0], a[1] - b[1]);
	};

	vector<array<array<int, 2>, 2>> stores(S);
	for (auto& s : stores) {
		cin >> s[0][0] >> s[0][1] >> s[1][0] >> s[1][1];
	}
	vector<array<int, 2>> printers(T);
	for (auto& t : printers) {
		cin >> t[0] >> t[1];
	}

	const double INF = 1e99;
	tensor<double, 2> printer_dist({T, T}, INF);
	for (int i = 0; i < T; i++) {
		for (int j = 0; j < T; j++) {
			printer_dist[{i, j}] = dist(printers[i], printers[j]);
		}
	}

	tensor<double, 3> dp({T, 1 << T, T}, INF);
	for (int st = 0; st < T; st++) {
		dp[{st, 1 << st, st}] = 0;
		for (int m = 0; m < (1 << T); m++) {
			for (int vi = m; vi; vi &= vi-1) {
				int i = __builtin_ctz(vi);
				assert(m & (1 << i));
				for (int vj = (1 << T) - 1 - m; vj; vj &= vj-1) {
					int j = __builtin_ctz(vj);
					assert(!(m & (1 << j)));

					setmin(dp[{st, m|(1<<j), j}], dp[{st, m, i}] + printer_dist[{i, j}]);
				}
			}
		}
	}

	for (int i = 0; i < T; i++) {
		for (int j = 0; j < T; j++) {
			printer_dist[{i, j}] = dp[{i, (1 << T)-1, j}];
		}
	}

	auto path_dist = [&](array<int, 2> a, array<int, 2> b) -> double {
		double ans = INF;
		for (int i = 0; i < T; i++) {
			for (int j = 0; j < T; j++) {
				setmin(ans, dist(a, printers[i]) + printer_dist[{i, j}] + dist(printers[j], b));
			}
		}
		return ans;
	};

	tensor<double, 4> shop_dist({S, S, 2, 2});
	for (int i = 0; i < S; i++) {
		for (int j = 0; j < S; j++) {
			for (int x = 0; x < 2; x++) {
				for (int y = 0; y < 2; y++) {
					shop_dist[{i, j, x, y}] = path_dist(stores[i][x], stores[j][y]);
				}
			}
		}
	}

	auto is_good = [&](double cutoff) -> bool {
		kactl::TwoSat sat(S);
		for (int i = 0; i < S; i++) {
			for (int j = 0; j < S; j++) {
				if (i == j) continue;
				for (int x = 0; x < 2; x++) {
					for (int y = 0; y < 2; y++) {
						if (shop_dist[{i, j, x, y}] > cutoff) {
							sat.either(~(x ? i : ~i), ~(y ? j : ~j));
						}
					}
				}
			}
		}
		return sat.solve();
	};

	double mi = 0;
	double ma = INF;
	for (int i = 0; i < 64; i++) {
		uint64_t mi_v; memcpy(&mi_v, &mi, sizeof(mi));
		uint64_t ma_v; memcpy(&ma_v, &ma, sizeof(mi));
		uint64_t md_v = mi_v + (ma_v - mi_v) / 2;
		double md; memcpy(&md, &md_v, sizeof(md_v));
		if (is_good(md)) ma = md;
		else mi = md;
	}

	cout << fixed << setprecision(10) << ma << '\n';

	return 0;
}
