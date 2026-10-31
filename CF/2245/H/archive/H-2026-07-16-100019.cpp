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
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	int T; std::cin >> T;
	while (T--) {
		// N is the smaller, inner dimension
		int N, M; std::cin >> N >> M;
		std::cerr << "case " << M << ' ' << N << '\n';
		tensor<int, 2> A({M, N});
		for (int j = 0; j < N; j++) {
			for (int i = 0; i < M; i++) {
				std::cin >> A[{i, j}];
			}
		}
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
		[&](this auto&& self, int l, int r, const tensor<bool, 2>& l_conn, const tensor<bool, 2>& r_conn) -> void {
			assert(r - l >= 1);
			if (r - l == 1) {
				for (int i = 0; i < N; i++) {
					if (A[{l, i}] == 0) continue;
					bool is_first = true;
					for (int j = i+1; j < N; j++) {
						if (A[{l, j}] == 0) continue;
						is_first = false;

						if (A[{l, i}] == A[{l, j}]) {
							if (is_first || l_conn[{i, j}] || r_conn[{i, j}]) {
								ans++;
							}
						}
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
					for (int iter = 0; iter < 3; iter++) {
						for (int i = l; i < r; i++) {
							bool on = false;
							for (int j = 0; j < N; j++) {
								if (((i == x && j == y) || !A[{i, j}]) && vis[{i, j}] == mark+iter) {
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
								if (((i == x && j == y) || !A[{i, j}]) && vis[{i, j}] == mark+iter) {
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
								if (((i == x && j == y) || !A[{i, j}]) && vis[{i, j}] == mark+iter) {
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
								if (((i == x && j == y) || !A[{i, j}]) && vis[{i, j}] == mark+iter) {
									on = true;
								}
								if (on && vis[{i, j}] < mark) vis[{i, j}] = mark + iter + 1;
								if (A[{i, j}]) {
									on = false;
								}
							}
						}
					}
					for (int i = l; i < r; i++) {
						for (int j = 0; j < N; j++) {
							if (A[{i, j}] == tgt && vis[{i, j}] >= mark) {
								std::cerr << "found " << x << ' ' << y << ' ' << i << ' ' << j << '\n';
								ans++;
							}
						}
					}
				};
				// solve things that cross the middle
				for (int i = 0; i < N; i++) {
					for (int x = m; x < r; x++) {
						if (A[{x, i}]) {
							// find everything we can get to
							check_slow(x, i);
							break;
						}
					}
					for (int x = m-1; x >= l; x--) {
						if (A[{x, i}]) {
							check_slow(x, i);
							break;
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
								n_l_conn[{i, j}] = true;
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
						for (int k = m; k <= r; k--) {
							if (k < r && (A[{k, i}] || A[{k, j}])) break;
							if (k == r ? r_conn[{i, j}] : (col_pref[{k, i}] == col_pref[{k, j+1}])) {
								n_r_conn[{i, j}] = true;
								break;
							}
						}
					}
				}
				self(l, m, l_conn, n_r_conn);
			}
		}(0, M, tensor<bool, 2>({N, N}, false), tensor<bool, 2>({N, N}, false));
		std::cout << ans << '\n';
	}

	return 0;
}
