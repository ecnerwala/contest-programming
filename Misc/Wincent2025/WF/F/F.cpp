#include <bits/stdc++.h>

template <typename T> void setmin(T& a, T b) { if (b < a) a = b; }

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

	std::array<int, NDIMS> shape;
	std::array<int, NDIMS> strides;
protected:
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

	const int64_t INF = int64_t(2e18);
	const int MAXC = 7;
	std::vector<std::vector<int>> c_masks(MAXC+1);
	using mat = tensor<int64_t, 2>;
	std::vector<std::vector<tensor<int64_t, 2>>> c_mats(MAXC+1);
	std::vector<tensor<bool, 2>> c_edge_valid(MAXC+1);
	std::vector<std::vector<bool>> c_line_valid(MAXC+1);
	auto mul = [&](const mat& a, const mat& b) -> mat {
		int I = a.shape[0];
		int J = a.shape[1];
		assert(a.shape[1] == b.shape[0]);
		int K = b.shape[1];
		mat res({I, K}, INF);
		for (int i = 0; i < I; i++) {
			for (int j = 0; j < J; j++) {
				if (a[{i, j}] == INF) continue;
				for (int k = 0; k < K; k++) {
					if (b[{j, k}] == INF) continue;
					assert((a[{i, j}] + b[{j, k}] < INF));
					setmin(res[{i, k}], a[{i, j}] + b[{j, k}]);
				}
			}
		}
		return res;
	};
	for (int C = 1; C <= 7; C++) {
		std::vector<int> msks; msks.reserve(40);
		for (int m = 0; m < (1 << C); m++) {
			bool good = true;
			for (int i = 0; i+1 < C; i++) {
				if (((m >> i) & 3) == 3) {
					good = false;
					break;
				}
			}
			if (!good) continue;
			msks.push_back(m);
		}
		c_masks[C] = msks;

		int M = int(msks.size());

		{
			std::vector<bool> line_valid(M);
			for (int i = 0; i < M; i++) {
				for (int j = 0; j < M; j++) {
					int mi = msks[i];
					bool is_valid = [&]() -> bool {
						for (int c = 0; c < C; c++) {
							std::array<bool, 2> has{false, false};
							if (c-1 >= 0) has[(mi >> (c-1)) & 1] = true;
							if (c+1 < C) has[(mi >> (c+1)) & 1] = true;
							if ((mi >> c) & 1) {
								if (has[0] && has[1]) return false;
							} else {
								if (!(has[0] && has[1])) return false;
							}
						}
						return true;
					}();
					line_valid[i] = is_valid;
				}
			}
			c_line_valid[C] = line_valid;
		}
		{
			tensor<bool, 2> edge_valid({M, M}, false);
			for (int i = 0; i < M; i++) {
				for (int j = 0; j < M; j++) {
					int mi = msks[i];
					int mj = msks[j];
					bool is_valid = [&]() -> bool {
						for (int c = 0; c < C; c++) {
							std::array<bool, 2> has{false, false};
							has[(mj >> c) & 1] = true;
							if (c-1 >= 0) has[(mi >> (c-1)) & 1] = true;
							if (c+1 < C) has[(mi >> (c+1)) & 1] = true;
							if ((mi >> c) & 1) {
								if (has[0] && has[1]) return false;
							} else {
								if (!(has[0] && has[1])) return false;
							}
						}
						return true;
					}();
					edge_valid[{i, j}] = is_valid;
				}
			}
			c_edge_valid[C] = edge_valid;
		}

		tensor<int64_t, 2> m0({M*M, M*M}, INF);
		for (int i = 0; i < M; i++) {
			for (int j = 0; j < M; j++) {
				for (int k = 0; k < M; k++) {
					// we need to check all entries of j
					int mi = msks[i];
					int mj = msks[j];
					int mk = msks[k];
					bool is_valid = [&]() -> bool {
						for (int c = 0; c < C; c++) {
							std::array<bool, 2> has{false, false};
							has[(mi >> c) & 1] = true;
							has[(mk >> c) & 1] = true;
							if (c-1 >= 0) has[(mj >> (c-1)) & 1] = true;
							if (c+1 < C) has[(mj >> (c+1)) & 1] = true;
							if ((mj >> c) & 1) {
								if (has[0] && has[1]) return false;
							} else {
								if (!(has[0] && has[1])) return false;
							}
						}
						return true;
					}();
					if (is_valid) {
						m0[{i*M+j, j*M+k}] = __builtin_popcount(mk);
					}
				}
			}
		}
		cerr << "precomp " << C << ' ' << M << '\n';
		c_mats[C].push_back(m0);
		for (int z = 1; z < 57; z++) {
			cerr << "mul " << z << '\n';
			m0 = mul(m0, m0);
			c_mats[C].push_back(m0);
		}
	}

	int T; cin >> T;
	while (T--) {
		int C; int64_t R; cin >> C >> R;
		assert(1 <= C && C <= 7);
		std::string P, Q; cin >> P >> Q;
		assert(int(P.size()) == C);
		assert(int(Q.size()) == C);
		cout << [&]() -> int64_t {
			int M = int(c_masks[C].size());
			auto get_matches = [&](std::string s) -> std::vector<bool> {
				std::vector<bool> res(M);
				for (int i = 0; i < M; i++) {
					int m = c_masks[C][i];
					bool good = true;
					for (int d = 0; d < C; d++) {
						assert(s[d] == '*' || s[d] == '0' || s[d] == '1');
						if (s[d] == '*') {
							// allowed
						} else if (s[d] == '0' + ((m >> d) & 1)) {
							// allowed
						} else {
							good = false;
							break;
						}
					}
					res[i] = good;
				}
				return res;
			};
			std::vector<bool> top_match = get_matches(P);
			std::vector<bool> bot_match = get_matches(Q);
			int64_t ans = INF;
			if (R == 1) {
				for (int i = 0; i < M; i++) {
					if (top_match[i] && bot_match[i] && c_line_valid[C][i]) {
						setmin<int64_t>(ans, __builtin_popcount(c_masks[C][i]));
					}
				}
			} else {
				assert(R >= 2);
				std::vector<int64_t> cost(M*M, INF);
				for (int i = 0; i < M; i++) {
					for (int j = 0; j < M; j++) {
						if (top_match[i] && c_edge_valid[C][{i, j}]) {
							setmin<int64_t>(cost[i*M+j], __builtin_popcount(c_masks[C][i]) + __builtin_popcount(c_masks[C][j]));
						}
					}
				}
				for (int64_t v = R-2, b = 0; v > 0; b++, v >>= 1) {
					if (v & 1) {
						std::vector<int64_t> ncost(M*M, INF);
						for (int i = 0; i < M*M; i++) {
							if (cost[i] == INF) continue;
							for (int j = 0; j < M*M; j++) {
								int64_t t = c_mats[C][b][{i, j}];
								if (t == INF) continue;
								assert(cost[i] + t < INF);
								setmin(ncost[j], cost[i] + t);
							}
						}
						cost = std::move(ncost);
					}
				}
				for (int i = 0; i < M; i++) {
					for (int j = 0; j < M; j++) {
						if (bot_match[i] && c_edge_valid[C][{i, j}]) {
							setmin(ans, cost[j*M+i]);
						}
					}
				}
			}
			if (ans == INF) ans = -1;
			else assert(ans < INF);
			return ans;
		}() << '\n';
	}

	return 0;
}
