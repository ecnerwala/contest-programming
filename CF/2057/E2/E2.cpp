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

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, M, Q; cin >> N >> M >> Q;
		std::vector<std::pair<int, std::array<int, 2>>> edges(M);
		for (auto& [w, vs] : edges) {
			auto& [u, v] = vs;
			cin >> u >> v >> w; u--, v--;
		}
		std::sort(edges.begin(), edges.end());

		tensor<std::vector<std::pair<int, int>>, 2> queries({N, N});
		for (int q = 0; q < Q; q++) {
			int a, b, k; cin >> a >> b >> k; a--, b--;
			assert(k <= N-1);
			queries[{a, b}].push_back({k, q});
		}

		// distance and minimum edge
		auto merge = [&](std::pair<int, int> a, std::pair<int, int> b) -> std::pair<int, int> {
			return {a.first + b.first, std::min(a.second, b.second)};
		};
		tensor<std::pair<int, int>, 2> dist({N, N}, {N+1, M});
		for (int i = 0; i < N; i++) dist[{i, i}] = {0, M};
		for (int e = 0; e < M; e++) {
			auto [u, v] = edges[e].second;
			dist[{u, v}] = dist[{v, u}] = {1, e};
			cerr << "edge " << e << ' ' << u << '-' << v << ' ' << edges[e].first << '\n';
		}
		for (int k = 0; k < N; k++) {
			for (int i = 0; i < N; i++) {
				for (int j = 0; j < N; j++) {
					dist[{i, j}] = std::min(dist[{i, j}], merge(dist[{i, k}], dist[{k, j}]));
				}
			}
		}

		std::vector<std::vector<std::array<int, 2>>> upds(M+1);

		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				upds[dist[{i, j}].second].push_back({i, j});
				std::sort(queries[{i, j}].begin(), queries[{i, j}].end());
				if (!queries[{i, j}].empty()) {
					assert((queries[{i, j}].back().first <= dist[{i, j}].first));
				}
			}
		}

		std::vector<int> ans(Q, -1);

		for (int e = 0; e < M; e++) {
			int w = edges[e].first;
			auto [u, v] = edges[e].second;
			for (auto [i, j] : upds[e]) {
				cerr << e << ' ' << i << ' ' << j << '\n';
				std::pair<int, int> cnd = std::min(merge(dist[{i, u}], dist[{v, j}]), merge(dist[{i, v}], dist[{u, j}]));
				assert((cnd.first == dist[{i, j}].first - 1));
				dist[{i, j}] = cnd;
				upds[cnd.second].push_back({i, j});
				while (!queries[{i, j}].empty() && queries[{i, j}].back().first > cnd.first) {
					ans[queries[{i, j}].back().second] = w;
					queries[{i, j}].pop_back();
				}
			}
		}

		for (int q = 0; q < Q; q++) {
			cout << ans[q] << " \n"[q+1==Q];
		}
	}

	return 0;
}
