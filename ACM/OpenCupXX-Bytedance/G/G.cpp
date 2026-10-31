#include<bits/stdc++.h>

template <typename T, int NDIMS> struct tensor_view {
	static_assert(NDIMS >= 0);

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

template <typename T> T& setmin(T& a, const T& b) { if (b < a) a = b; return a; }

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int NUM_CASES; cin >> NUM_CASES;
	while (NUM_CASES--) {
		int N, M, S, T; cin >> N >> M >> S >> T; S--, T--;
		vector<int> P(N);
		for (int i = 0; i < N; i++) cin >> P[i];

		const int INF = accumulate(P.begin(), P.end(), 1);
		tensor<int, 2> dist({N, N}, INF);
		for (int i = 0; i < N; i++) dist[{i,i}] = 0;

		for (int e = 0; e < M; e++) {
			int u, v; cin >> u >> v; u--, v--;
			dist[{u,v}] = 0;
		}

		for (int k = 0; k < N; k++) {
			for (int i = 0; i < N; i++) {
				for (int j = 0; j < N; j++) {
					setmin(dist[{i,j}], dist[{i,k}] + P[k] + dist[{k,j}]);
				}
			}
		}

		tensor<int, 2> dp({N, N}, INF);
		tensor<int, 2> dp2({N, N}, INF);
		vector<vector<array<int, 2>>> q(INF);
		vector<vector<array<int, 2>>> q2(INF);
		dp[{S,S}] = P[S];
		q[P[S]].push_back({S,S});

		for (int d = 0; d < INF; d++) {
			for (auto [c0, c1] : q2[d]) {
				for (int c2 = 0; c2 < N; c2++) {
					if (c2 == c0) continue;
					int cnd = d + dist[{c2, c0}] + (c2 != c1 ? dist[{c1, c2}] + P[c2] : 0);
					if (cnd < dp[{c1, c2}]) {
						dp[{c1, c2}] = cnd;
						q[cnd].push_back({c1, c2});
					}
				}
			}
			for (auto [c0, c1] : q[d]) {
				for (int c2 = 0; c2 < N; c2++) {
					int cnd = d + dist[{c1,c2}] + P[c2];
					if (cnd < dp2[{c0, c2}]) {
						dp2[{c0, c2}] = cnd;
						q2[cnd].push_back({c0, c2});
					}
				}
			}
		}

		int ans = dp[{T, T}];
		if (ans == INF) cout << -1 << '\n';
		else cout << ans << '\n';
	}

	return 0;
}
