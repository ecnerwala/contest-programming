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

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M; cin >> N >> M;

	tensor<bool, 2> G({N, N}, false);
	std::vector<int> G_deg(N);
	for (int e = 0; e < M; e++) {
		int u, v; cin >> u >> v; u--, v--;
		assert(u != v);
		assert((G[{u, v}] == 0));
		G[{u, v}] = G[{v, u}] = true;
		G_deg[u] ++;
		G_deg[v] ++;
	}
	tensor<bool, 2> H({N, N}, false);
	std::vector<int> H_deg(N);
	for (int e = 0; e < M; e++) {
		int u, v; cin >> u >> v; u--, v--;
		assert(u != v);
		assert((H[{u, v}] == 0));
		H[{u, v}] = H[{v, u}] = true;
		H_deg[u] ++;
		H_deg[v] ++;
	}

	if (G_deg != H_deg) {
		cout << -1 << '\n';
		exit(0);
	}

	tensor<bool, 2> used({N, N}, false);

	std::set<std::pair<int, int>> edges;
	std::vector<std::set<int>> G_adj(N);
	std::vector<std::set<int>> H_adj(N);

	for (int i = 0; i+1 < N; i++) {
		for (int j = i+1; j < N; j++) {
			if (G[{i, j}] != H[{i, j}]) edges.insert({i, j});
		}
	}
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (G[{i,j}] && !H[{i, j}]) G_adj[i].insert(j);
			if (H[{i, j}] && !G[{i,j}]) H_adj[i].insert(j);
		}
	}

	std::vector<std::array<int, 4>> forward_ops;
	forward_ops.reserve(4*N*N);
	std::vector<std::array<int, 4>> rev_ops;
	rev_ops.reserve(4*N*N);

	for (int v0 = 0; v0 < N; v0++) {
		while (!G_adj.empty()) {
			int v1 = *G_adj[v0].begin();
			assert(v0 != v1);
			assert(!H_adj[v1].empty());
			int v2 = *H_adj[v1].begin();
			assert(v1 != v2);

			assert(v0 != v2);

			assert(!G_adj[v2].empty());
			int v3 = *G_adj[v2].begin();
			assert(v3 != v2);
			assert(v3 != v1);

			if (v3 != v0) {
				assert((G[{v0, v1}] && H[{v1, v2}] && G[{v2, v3}]));
				if (!G[{v3, v0}]) {
				} else if (H[{v3, v0}]) {
				}
			}
		}
	}

	std::vector<int> cycle; cycle.reserve(N*N + 1);
	for (int st = 0; st < N; st++) {
		cycle.clear();
		cycle.push_back(st);
		int cur = st;
		do {
			while (true) {
				assert(!G_adj[cur].empty());
				int nxt = G_adj[cur].back(); G_adj[cur].pop_back();
				if (used[{cur, nxt}]) continue;
				used[{cur, nxt}] = used[{nxt, cur}] = true;
				break;
			}
			cycle.push_back(cur);
			while (true) {
				assert(!H_adj[cur].empty());
				int nxt = H_adj[cur].back(); H_adj[cur].pop_back();
				if (used[{cur, nxt}]) continue;
				used[{cur, nxt}] = used[{nxt, cur}] = true;
				break;
			}
			cycle.push_back(cur);
		} while (cur != st);

		while (cycle.size() >= 4) {
		}
	}

	return 0;
}
