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

namespace std {

template<class Fun>
class y_combinator_result {
	Fun fun_;
public:
	template<class T>
	explicit y_combinator_result(T &&fun): fun_(std::forward<T>(fun)) {}

	template<class ...Args>
	decltype(auto) operator()(Args &&...args) {
		return fun_(std::ref(*this), std::forward<Args>(args)...);
	}
};

template<class Fun>
decltype(auto) y_combinator(Fun &&fun) {
	return y_combinator_result<std::decay_t<Fun>>(std::forward<Fun>(fun));
}

} // namespace std

template <typename T> void setmax(T& a, T b) { if (b > a) a = b; }

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {

		int N, M, K; cin >> N >> M >> K;
		cerr << "Case #" << case_num << ": " << N << ' ' << M << ' ' << K << '\n';
		std::vector<std::vector<int>> adj(N);
		for (int e = 0; e < M; e++) {
			int u, v; cin >> u >> v; u--, v--;
			adj[u].push_back(v);
			adj[v].push_back(u);
		}
		std::vector<int> C(N);
		for (auto& c : C) cin >> c;

		std::vector<std::vector<int>> cycs(N);
		std::vector<int> depth(N);
		// 0 means no parent, 1 means parent
		const int64_t INF = 1e18;
		tensor<int64_t, 3> dp({N, K+1, 2}, 0);
		tensor<int64_t, 2> dp2({K+1, 2});
		tensor<int64_t, 2> ndp2({K+1, 2});
		std::y_combinator([&](auto self, int cur, int prv) -> int {
			depth[cur] = (prv == -1 ? 0 : depth[prv] + 1);
			int lv = -1;
			for (int k = 0; k <= K; k++) {
				dp[{cur, k, 1}] += C[k];
			}
			for (int nxt : adj[cur]) {
				if (nxt == prv) continue;
				if (depth[nxt] == -1) {
					int nlv = self(nxt, cur);
					if (nlv == -1 || nlv == cur) {
						// bridge edge or top of the cycle
						for (int root_val = 0; root_val <= K; root_val++) {
							auto root_v = dp[cur][root_val];
							std::array<int64_t, 2> n_root_v{INF, INF};
							for (int leaf_val = std::max(0, root_val - 1); leaf_val <= std::min(K, root_val + 1); leaf_val++) {
								for (int k = 0; k <= K; k++) {
									dp2[{k, 0}] = dp2[{k, 1}] = INF;
								}
								dp2[{root_val, 1}] = dp2[{root_val, 0}] = 0;
								for (int z = 0; z < int(cycs[cur].size()); z++) {
									for (int k = 0; k <= K; k++) {
										auto v = dp[cycs[cur][z]][k];
										std::array<int64_t, 2> nv{INF, INF};
										for (int ch_val = std::max(0, k-1); ch_val <= std::min(K, k+1); ch_val++) {
											for (int a = 0; a < 2; a++) {
												for (int b = (k < ch_val) ? 0 : 1; b < 2; b++) {
													setmax(nv[a || (ch_val < k)], (*v[a]) + dp2[{ch_val, b}]);
												}
											}
										}
										if (z == 0 && k != leaf_val) {
											nv = {INF, INF};
										}
										ndp2[{k, 0}] = nv[0];
										ndp2[{k, 1}] = nv[1];
									}
									for (int k = 0; k <= K; k++) {
										dp2[{k, 0}] = ndp2[{k, 0}];
										dp2[{k, 1}] = ndp2[{k, 1}];
									}
								}
								for (int ch_val = std::max(0, root_val-1); ch_val <= std::min(K, root_val+1); ch_val++) {
									for (int a = 0; a < 2; a++) {
										for (int b = (root_val < ch_val) ? 0 : 1; b < 2; b++) {
											setmax(n_root_v[a || (ch_val < root_val) || (leaf_val < root_val)], (*root_v[a]) + dp2[{ch_val, b}]);
										}
									}
								}
							}
							*root_v[0] = n_root_v[0];
							*root_v[1] = n_root_v[1];
						}
						for (int k = 0; k <= K; k++) {
						}
						cycs[cur].clear();
					} else {
						assert(lv == -1);
						lv = nlv;
					}
				} else if (depth[nxt] < depth[cur]) {
					assert(lv == -1);
					lv = nxt;
				}
			}
			if (lv != -1) {
				cycs[lv].push_back(cur);
			} else if (prv != -1) {
				cycs[prv].push_back(cur);
			}
			return lv;
		})(0, -1);

		int64_t ans;

		cout << "Case #" << case_num << ": " << ans << '\n' << flush;
	}

	return 0;
}
