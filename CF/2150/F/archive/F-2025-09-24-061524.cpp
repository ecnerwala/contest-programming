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
		int N, M; cin >> N >> M;
		tensor<bool, 2> adj({N, N});
		for (int e = 0; e < M; e++) {
			int u, v; cin >> u >> v; u--, v--;
			adj[{u, v}] = adj[{v, u}] = true;
		}
		std::vector<std::vector<int>> tree_edges(N);
		int diam = 0;
		{
			std::vector<bool> vis(N, false);
			[&](this auto&& self, int cur) -> int {
				assert(!vis[cur]);
				vis[cur] = true;
				int d = 0;
				for (int nxt = 0; nxt < N; nxt++) {
					if (!vis[nxt] && adj[{cur, nxt}]) {
						tree_edges[cur].push_back(nxt);
						tree_edges[nxt].push_back(cur);
						int nd = self(nxt) + 1;
						diam = std::max(diam, d + nd);
						d = std::max(d, nd);
					}
				}
				return d;
			}(0);
		}
		// 2 ops
		cout << 2 << '\n';
		cout << 3 << '\n';
		{
			std::vector<std::array<int, 3>> ops;
			ops.reserve(N*N);
			for (int i = 0; i < N; i++) {
				for (int j : tree_edges[i]) {
					for (int k : tree_edges[i]) {
						if (j == k || adj[{j, k}]) continue;
						adj[{j, k}] = adj[{k, j}] = true;
						ops.push_back({j, i, k});
					}
				}
			}
			cout << ops.size() << '\n';
			for (auto o : ops) { cout << o[0]+1 << ' ' << o[1]+1 << ' ' << o[2]+1 << '\n'; }
		}

		int path_len = (diam + 1) / 2;
		cout << path_len+1 << '\n';
		int num_ops = 0;
		for (int i = 0; i < N; i++) {
			for (int j = i+1; j < N; j++) {
				if (!adj[{i, j}]) num_ops++;
			}
		}
		cout << num_ops << '\n';

		for (int i = 0; i < N; i++) {
			int furthest = [&](this auto&& self, int cur, int prv) -> std::pair<int, int> {
				std::pair<int, int> res{-1, cur};
				for (int nxt : tree_edges[cur]) {
					if (nxt == prv) continue;
					res = std::max(res, self(nxt, cur));
				}
				res.first++;
				return res;
			}(i, -1).second;
			auto get_path = [&](int st, int en) -> std::vector<int> {
				std::vector<int> res; res.reserve(N);
				[&](this auto&& self, int cur, int prv) -> bool {
					res.push_back(cur);
					if (cur == en) return true;
					for (int nxt : tree_edges[cur]) {
						if (nxt == prv) continue;
						if (self(nxt, cur)) return true;
					}
					res.pop_back();
					return false;
				}(st, -1);
				return res;
			};
			for (int j = i+1; j < N; j++) {
				if (!adj[{i, j}]) {
					// solve i->j
					auto pij = get_path(i, j);
					int dij = int(pij.size()) - 1;
					if (dij >= path_len) {
						assert(dij <= 2 * path_len);
						int skip = dij - path_len;
						int cur = 0;
						for (int z = 0; z < path_len; z++, cur++) {
							cout << pij[cur] << ' ';
							if (skip) { cur++; skip--; }
						}
						assert(skip == 0);
						assert(cur == dij);
						cout << pij[cur] << '\n';
					} else {
						// we need to go further
						auto pif = get_path(i, furthest);
						auto pjf = get_path(j, furthest);
						int dif = int(pif.size()) - 1;
						int djf = int(pjf.size()) - 1;
						int extra = (dif + djf - dij) / 2;
						{
							int need_extra = path_len - dij;
							assert(extra >= need_extra);
							while (extra > need_extra) {
								assert(pif.back() == pjf.back());
								pif.pop_back();
								pjf.pop_back();
								dif--, djf--;
								extra--;
							}
						}
						assert(extra > 0);
					}
				}
			}
		}
	}

	return 0;
}
