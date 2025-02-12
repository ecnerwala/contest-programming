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
		std::vector<int> A(N);
		for (auto& a : A) cin >> a;
		std::vector<int> B(M);
		for (auto& b : B) cin >> b;

		std::vector<std::pair<std::array<int, 2>, std::vector<int>>> ops;
		ops.reserve(N+1);
		auto solve = [&]() -> bool {
			std::vector<std::array<int, 2>> ranges; ranges.reserve(N);
			std::vector<int> range_ends(N, -1);
			std::vector<bool> A_alive(N, true);
			tensor<int, 2> B_val({M+1, M+1});
			for (int i = 0; i < M; i++) {
				int cur_sum = B[i];
				int best_sum = B[i];
				for (int j = i; j < M; j++) {
					if (j > i) {
						if (cur_sum < 0) cur_sum = 0;
						cur_sum += B[j];
						best_sum = std::max(best_sum, cur_sum);
					}
					B_val[{i, j+1}] = best_sum;
				}
			}
			std::vector<int> valid_B_end(M, M);
			while (true) {
				{
					int best_sum = 0;
					int cur_st = -1;
					int cur_sum = 0;
					std::array<int, 2> best_st{-1, -1};
					for (int i = 0; i < N; i++) {
						if (A_alive[i]) {
							if (cur_st == -1 || cur_sum < 0) {
								cur_st = i;
								cur_sum = 0;
							}
							cur_sum += A[i];
							if (best_st[0] == -1 || cur_sum >= best_sum) {
								best_st = {cur_st, i+1};
								best_sum = cur_sum;
							}
						} else {
							cur_st = -1;
						}
					}
					if (best_st[0] == -1) {
						return false;
					}
					ranges.push_back(best_st);
					cerr << "mark bad " << best_st[0] << '-' << best_st[1] << '\n';
					for (int i = best_st[0]; i < best_st[1]; i++) {
						A_alive[i] = false;
					}
					range_ends[best_st[0]] = best_st[1];
					for (int i = 0; i < M; i++) {
						while (valid_B_end[i] > i && B_val[{i, valid_B_end[i]}] > best_sum) {
							valid_B_end[i]--;
						}
					}
				}
				// intervals of B < best_sum
				tensor<std::array<int, 3>, 3> dp({N+1, M+1, 2}, {-1, -1, -1});
				dp[{0, 0, 0}] = {0, 0, 0};
				for (int i = 0; i < N; i++) {
					if (range_ends[i] == -1) {
						for (int j = 0; j < M; j++) {
							if (B[j] == A[i]) {
								if (dp[{i, j, 0}][0] != -1) dp[{i+1, j+1, 0}] = {i, j, 0};
								if (dp[{i, j, 1}][0] != -1) dp[{i+1, j+1, 1}] = {i, j, 1};
							}
						}
					} else {
						int ni = range_ends[i];
						int nxt_0 = 0;
						int nxt_1 = 0;
						for (int j = 0; j < M; j++) {
							nxt_0 = std::max(nxt_0, j+1);
							nxt_1 = std::max(nxt_1, j+1);
							if (dp[{i, j, 0}][0] != -1) {
								for (; nxt_0 <= valid_B_end[j]; nxt_0++) dp[{ni, nxt_0, 0}] = {i, j, 0};
								for (; nxt_1 <= M; nxt_1++) dp[{ni, nxt_1, 1}] = {i, j, 0};
							}
							if (dp[{i, j, 1}][0] != -1) {
								for (; nxt_1 <= valid_B_end[j]; nxt_1++) dp[{ni, nxt_1, 1}] = {i, j, 1};
							}
						}
						i = ni - 1;
					}
				}
				if (dp[{N, M, 1}][0] != -1) {
					std::array<int, 3> cur{N, M, 1};
					while (cur[0] > 0) {
						std::array<int, 3> prv = dp[cur];
						assert(prv[0] != -1);
						cur = prv;
					}
					return true;
				}
			}
		};
		if (solve()) {
			cout << ops.size() << '\n';
			for (const auto& [b, c] : ops) {
				cout << b[0]+1 << ' ' << b[1] << ' ' << c.size();
				for (auto v : c) cout << ' ' << v;
				cout << '\n';
			}
		} else {
			cout << -1 << '\n';
		}
	}

	return 0;
}
