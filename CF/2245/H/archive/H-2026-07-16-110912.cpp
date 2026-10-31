#include <bits/stdc++.h>
#include <cassert>

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

public:
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
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	auto solve_slow = [&](int M, int N, tensor<int, 2> A) -> int64_t {
		int vis_time = 1;
		tensor<int, 2> vis({M, N}, 0);
		int64_t ans = 0;
		int l = 0, r = M;
		auto check_slow = [&](int x, int y) -> void {
			// check A[{x, i}], then delete it from contention
			int tgt = std::exchange(A[{x, y}], -1);
			if (tgt == -1) return;
			//std::cerr << "check_slow " << x << ' ' << y << '\n';
			int mark = vis_time;
			vis_time += 4;
			vis[{x, y}] = mark;
			A[{x, y}] = 0;
			for (int iter = 0; iter < 3; iter++) {
				for (int i = l; i < r; i++) {
					bool on = false;
					for (int j = 0; j < N; j++) {
						if (!A[{i, j}] && vis[{i, j}] == mark+iter) {
							on = true;
						}
						if (on && vis[{i, j}] < mark) vis[{i, j}] = mark + iter + 1;
						if (A[{i, j}]) {
							on = false;
						}
					}
				}
				for (int i = l; i < r; i++) {
					bool on = false;
					for (int j = N-1; j >= 0; j--) {
						if (!A[{i, j}] && vis[{i, j}] == mark+iter) {
							on = true;
						}
						if (on && vis[{i, j}] < mark) vis[{i, j}] = mark + iter + 1;
						if (A[{i, j}]) {
							on = false;
						}
					}
				}
				for (int j = 0; j < N; j++) {
					bool on = false;
					for (int i = l; i < r; i++) {
						if (!A[{i, j}] && vis[{i, j}] == mark+iter) {
							on = true;
						}
						if (on && vis[{i, j}] < mark) vis[{i, j}] = mark + iter + 1;
						if (A[{i, j}]) {
							on = false;
						}
					}
				}
				for (int j = 0; j < N; j++) {
					bool on = false;
					for (int i = r-1; i >= l; i--) {
						if (!A[{i, j}] && vis[{i, j}] == mark+iter) {
							on = true;
						}
						if (on && vis[{i, j}] < mark) vis[{i, j}] = mark + iter + 1;
						if (A[{i, j}]) {
							on = false;
						}
					}
				}
			}
			//for (int i = l; i < r; i++) { for (int j = 0; j < N; j++) { std::cerr << vis[{i, j}] << ' '; } std::cerr << '\n'; }
			for (int i = l; i < r; i++) {
				for (int j = 0; j < N; j++) {
					if (A[{i, j}] == tgt) {
						if (vis[{i, j}] >= mark) {
							//std::cerr << "found " << x << ' ' << y << ' ' << i << ' ' << j << '\n';
							ans++;
						}
					}
				}
			}
			A[{x, y}] = -1;
		};
		for (int i = 0; i < M; i++) {
			for (int j = 0; j < N; j++) {
				if (A[{i, j}]) check_slow(i, j);
			}
		}
		return ans;
	};
	auto solve = [&](int M, int N, tensor<int, 2> A) -> int64_t {
		tensor<int, 2> row_pref({M+1, N}, 0);
		tensor<int, 2> col_pref({M, N+1}, 0);
		for (int i = 0; i < M; i++) {
			for (int j = 0; j < N; j++) {
				row_pref[{i+1, j}] = row_pref[{i, j}] + (A[{i, j}] != 0);
				col_pref[{i, j+1}] = col_pref[{i, j+1}] + (A[{i, j}] != 0);
			}
		}
		int vis_time = 1;
		tensor<int, 2> vis({M, N}, 0);
		int64_t ans = 0;
		std::vector<std::array<int, 2>> q; q.reserve(N*M);
		std::vector<std::pair<int, int>> buf; buf.reserve(2 * N * M);
		[&](this auto&& self, int l, int r, const tensor<bool, 2>& l_conn, const tensor<bool, 2>& r_conn) -> void {
			assert(r - l >= 1);
			if (r - l == 1) {
				for (int i = 0; i < N; i++) {
					if (A[{l, i}] == 0 || A[{l, i}] == -1) continue;
					bool is_first = true;
					for (int j = i+1; j < N; j++) {
						if (A[{l, j}] == 0) continue;

						if (A[{l, i}] == A[{l, j}]) {
							if (is_first || l_conn[{i, j}] || r_conn[{i, j}]) {
								ans++;
							}
						}
						is_first = false;
					}
				}
				return;
			}
			int m = l + (r - l) / 2;
			std::cerr << "solve " << l << ' ' << m << ' ' << r << '\n';
			{
				auto check_slow = [&](int x, int y) -> void {
					// check A[{x, i}], then delete it from contention
					int tgt = std::exchange(A[{x, y}], -1);
					if (tgt == -1) return;
					std::cerr << "check_slow " << x << ' ' << y << '\n';
					int mark = vis_time;
					vis_time += 4;
					vis[{x, y}] = mark;
					A[{x, y}] = 0;
					for (int iter = 0; iter < 3; iter++) {
						for (int i = l; i < r; i++) {
							bool on = false;
							for (int j = 0; j < N; j++) {
								if (!A[{i, j}] && vis[{i, j}] == mark+iter) {
									on = true;
								}
								if (on && vis[{i, j}] < mark) vis[{i, j}] = mark + iter + 1;
								if (A[{i, j}]) {
									on = false;
								}
							}
						}
						for (int i = l; i < r; i++) {
							bool on = false;
							for (int j = N-1; j >= 0; j--) {
								if (!A[{i, j}] && vis[{i, j}] == mark+iter) {
									on = true;
								}
								if (on && vis[{i, j}] < mark) vis[{i, j}] = mark + iter + 1;
								if (A[{i, j}]) {
									on = false;
								}
							}
						}
						for (int j = 0; j < N; j++) {
							bool on = false;
							for (int i = l; i < r; i++) {
								if (!A[{i, j}] && vis[{i, j}] == mark+iter) {
									on = true;
								}
								if (on && vis[{i, j}] < mark) vis[{i, j}] = mark + iter + 1;
								if (A[{i, j}]) {
									on = false;
								}
							}
						}
						for (int j = 0; j < N; j++) {
							bool on = false;
							for (int i = r-1; i >= l; i--) {
								if (!A[{i, j}] && vis[{i, j}] == mark+iter) {
									on = true;
								}
								if (on && vis[{i, j}] < mark) vis[{i, j}] = mark + iter + 1;
								if (A[{i, j}]) {
									on = false;
								}
							}
						}
					}
					//for (int i = l; i < r; i++) { for (int j = 0; j < N; j++) { std::cerr << vis[{i, j}] << ' '; } std::cerr << '\n'; }
					for (int i = l; i < r; i++) {
						for (int j = 0; j < N; j++) {
							if (A[{i, j}] == tgt) {
								if (
									vis[{i, j}] >= mark
									|| (r_conn[{y, j}] && row_pref[{r, j}] == row_pref[{i+1, j}] && row_pref[{r, y}] == row_pref[{x+1, y}])
									|| (l_conn[{y, j}] && row_pref[{l, j}] == row_pref[{i, j}] && row_pref[{l, y}] == row_pref[{x, y}])
								) {
									std::cerr << "found " << x << ' ' << y << ' ' << i << ' ' << j << '\n';
									ans++;
								}
							}
						}
					}
					A[{x, y}] = -1;
				};
				// solve things that cross the middle
				std::vector<int> l_thresh; l_thresh.reserve(N+1);
				l_thresh.push_back(l);
				l_thresh.push_back(m);
				std::vector<int> r_thresh; r_thresh.reserve(N+1);
				r_thresh.push_back(m);
				r_thresh.push_back(r);
				for (int i = 0; i < N; i++) {
					for (int x = m; x < r; x++) {
						if (A[{x, i}]) {
							// find everything we can get to
							check_slow(x, i);
							r_thresh.push_back(x+1);
							break;
						}
					}
					for (int x = m-1; x >= l; x--) {
						if (A[{x, i}]) {
							check_slow(x, i);
							l_thresh.push_back(x);
							break;
						}
					}
				}
				std::sort(l_thresh.begin(), l_thresh.end());
				l_thresh.resize(std::unique(l_thresh.begin(), l_thresh.end()) - l_thresh.begin());
				std::sort(r_thresh.begin(), r_thresh.end());
				r_thresh.resize(std::unique(r_thresh.begin(), r_thresh.end()) - r_thresh.begin());
				for (int zl = 0; zl+1 < int(l_thresh.size()); zl++) {
					for (int zr = 0; zr+1 < int(r_thresh.size()); zr++) {
						int l_lo = l_thresh[zl];
						int l_hi = l_thresh[zl+1];
						int r_lo = r_thresh[zr];
						int r_hi = r_thresh[zr+1];
						std::vector<int> y_good(N+1);
						for (int y = 0; y < N; y++) {
							y_good[y+1] = y_good[y] + (row_pref[{r_lo, y}] == row_pref[{l_hi, y}]);
						}
						//std::cerr << "merge " << l_lo << '-' << l_hi << ' ' << r_lo << '-' << r_hi << '\n';
						//std::cerr << "tot good " << y_good[N] << '\n';
						buf.clear();
						for (int x = l_lo; x < l_hi; x++) {
							int y_lo = 0;
							for (int y = 0; y < N; y++) {
								if (A[{x, y}]) {
									int y_hi = y+1;
									while (y_hi < N && !A[{x, y_hi}]) y_hi++;

									if (A[{x, y}] > 0) {
										int mi = y_good[y_lo];
										int ma = y_good[y_hi];
										if (ma > mi) {
											buf.push_back({2 * A[{x, y}] + 1, 2 * mi + 1});
											buf.push_back({2 * A[{x, y}] + 0, 2 * ma + 0});
										}
									}

									y_lo = y+1;
									y = y_hi-1;
								}
							}
						}
						for (int x = r_lo; x < r_hi; x++) {
							int y_lo = 0;
							for (int y = 0; y < N; y++) {
								if (A[{x, y}]) {
									int y_hi = y+1;
									while (y_hi < N && !A[{x, y_hi}]) y_hi++;

									if (A[{x, y}] > 0) {
										int mi = y_good[y_lo];
										int ma = y_good[y_hi];
										if (ma > mi) {
											buf.push_back({2 * A[{x, y}] + 0, 2 * mi + 1});
											buf.push_back({2 * A[{x, y}] + 1, 2 * ma + 0});
										}
									}

									y_lo = y+1;
									y = y_hi-1;
								}
							}
						}
						std::sort(buf.begin(), buf.end());
						for (int i = 0, j = 0; i < int(buf.size()); i = j) {
							while (j < int(buf.size()) && buf[j].first == buf[i].first) j++;
							std::array<int, 2> cnts{0, 0};
							int64_t num_bad = 0;
							for (int k = i; k < j; k++) {
								int dir = buf[k].second & 1;
								cnts[dir]++;
								if (dir) num_bad += cnts[0];
							}
							//std::cerr << buf[i].first << ' ' << i << ' ' << j << '\n';
							//std::cerr << " cnts " << cnts[0] << ' ' << cnts[1] << '\n';
							//std::cerr << " bad " << num_bad << '\n';
							if (!(buf[i].first & 1)) {
								ans += int64_t(cnts[0]) * int64_t(cnts[1]);
							}
							ans -= num_bad;
						}
					}
				}
			}


			{
				tensor<bool, 2> n_l_conn({N, N}, false);
				for (int i = 0; i < N; i++) {
					for (int j = i+1; j < N; j++) {
						for (int k = m-1; k >= l-1; k--) {
							if (k >= l && (A[{k, i}] || A[{k, j}])) break;
							if (k == l-1 ? l_conn[{i, j}] : (col_pref[{k, i}] == col_pref[{k, j+1}])) {
								n_l_conn[{i, j}] = n_l_conn[{j, i}] = true;
								break;
							}
						}
					}
				}
				self(m, r, n_l_conn, r_conn);
			}
			{
				tensor<bool, 2> n_r_conn({N, N}, false);
				for (int i = 0; i < N; i++) {
					for (int j = i+1; j < N; j++) {
						for (int k = m; k <= r; k++) {
							if (k < r && (A[{k, i}] || A[{k, j}])) break;
							if (k == r ? r_conn[{i, j}] : (col_pref[{k, i}] == col_pref[{k, j+1}])) {
								n_r_conn[{i, j}] = n_r_conn[{j, i}] = true;
								break;
							}
						}
					}
				}
				self(l, m, l_conn, n_r_conn);
			}
		}(0, M, tensor<bool, 2>({N, N}, false), tensor<bool, 2>({N, N}, false));
		return ans;
	};
	std::mt19937 mt(48);
	for (int z = 0; z < 2000; z++) {
		int M = std::uniform_int_distribution<int>(1, 4)(mt);
		int N = std::uniform_int_distribution<int>(1, 5)(mt);
		tensor<int, 2> A({M, N});
		for (int i = 0; i < M; i++) {
			for (int j = 0; j < N; j++) {
				A[{i, j}] = std::max(std::uniform_int_distribution<int>(-8, 2)(mt), 0);
			}
		}
		int64_t slo = solve_slow(M, N, A);
		int64_t res = solve(M, N, A);
		if (slo != res) {
			std::cerr << N << ' ' << M << '\n';
			for (int j = 0; j < N; j++) {
				for (int i = 0; i < M; i++) {
					std::cerr << A[{i, j}] << ' ';
				}
				std::cerr << '\n';
			}
			std::cerr << "slow " << slo << '\n';
			std::cerr << "mine " << res << '\n';
			assert(false);
		}
	}

	int T; std::cin >> T;
	while (T--) {
		// N is the smaller, inner dimension
		int N, M; std::cin >> N >> M;
		//std::cerr << "case " << M << ' ' << N << '\n';
		tensor<int, 2> A({M, N});
		for (int j = 0; j < N; j++) {
			for (int i = 0; i < M; i++) {
				std::cin >> A[{i, j}];
			}
		}
		std::cout << solve(M, N, A) << '\n';
	}

	return 0;
}
