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

template <typename T> void setmax(T& a, T b) { if (b > a) a = b; }

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M, T; cin >> N >> M >> T;

	vector<int> D(N);
	for (int i = 0; i < N; i++) {
		cin >> D[i];
	}

	struct edge_t {
		array<int, 2> v;
		int r;
	};
	vector<edge_t> edges(M);
	vector<vector<int>> adj(N);
	for (int e = 0; e < M; e++) {
		int u, v, r; cin >> u >> v >> r; u--, v--;
		assert(r >= 5);
		edges[e].v = {u, v};
		edges[e].r = r;

		adj[u].push_back(e);
		adj[v].push_back(e);
	}

	// it's the last edge you took and the time you're there
	const int INF = 1e9;
	tensor<int, 2> best_val({T+1, N+4*M}, -INF);
	best_val[{0, 4*M+0}] = 0;

	int ans = -INF;

	for (int t = 0; t <= T; t++) {
		for (int x = 0; x < 4*M+N; x++) {
			int cur_val = best_val[{t, x}];
			if (cur_val == -INF) continue;
			int v;
			if (x < 4*M) {
				v = edges[x / 4].v[x / 2 % 2];
			} else {
				v = x - (4*M);
			}

			//cerr << t << ' ' << v << ' ' << x << ' ' << best_val[{t, x}] << '\n';

			if (v == N-1) setmax(ans, cur_val);

			if (t + 15 <= T) {
				setmax(best_val[{t + 15, 4 * M + v}], cur_val + D[v]);
			}

			for (int e : adj[v]) {
				int u_dir = edges[e].v[1] != v;
				int nx = e * 4 + u_dir * 2;
				int u = edges[e].v[u_dir];
				assert(u != v);
				assert(v == edges[e].v[!u_dir]);
				int nt = t + edges[e].r;

				if (nt > T) continue;

				if (x < 4*M && e == x / 4) {
					// then tracing it back isn't worth bounty
					setmax(best_val[{nt, nx + 1}], cur_val);

					// we can also wait
					int nt2 = nt + max(0, 15 - 2 * edges[e].r);
					if (nt2 <= T) {
						setmax(best_val[{nt2, 4 * M + u}], cur_val + D[u]);
					}
				} else {
					if (x < 4*M && x % 2 == 1) {
						nx = 4 * M + u;
					}
					setmax(best_val[{nt, nx}], cur_val + D[u]);
				}

			}
		}
	}

	assert(ans != -INF);

	cout << ans << '\n';

	return 0;
}
