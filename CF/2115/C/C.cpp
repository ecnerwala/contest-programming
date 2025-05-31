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
		int N, M, P100; cin >> N >> M >> P100;
		double P = double(P100) / 100;
		std::vector<int> H(N);
		for (auto& h : H) cin >> h;
		int min_h = *std::min_element(H.begin(), H.end());
		int tot_ones = 0;
		for (auto h : H) tot_ones += h - min_h;
		int tot_all = min_h - 1;

		cout << fixed << setprecision(20);
		cout << [&]() -> double {
			if (tot_ones + tot_all > M) {
				return 0;
			}
			std::vector<double> endgames(tot_all+1, 0.);
			{
				// solve from the end, probability of winning from this state
				tensor<double, 2> dp({tot_all+1, N}, 0.);
				dp[{0, 0}] = 1;
				tensor<double, 2> ndp({tot_all+1, N});
				for (int m = 0; m <= M; m++) {
					if (M - m >= tot_ones && M - m <= tot_ones + tot_all) {
						endgames[tot_all + tot_ones - (M - m)] = dp[{tot_all + tot_ones - (M - m), 0}];
					}

					for (int i = 0; i <= tot_all; i++) {
						for (int j = 0; j < N; j++) {
							ndp[{i, j}] = P * dp[{std::max(i-1, 0), j}] + (1-P) * (j ? dp[{i, j-1}] : (i ? std::max(dp[{i-1, N-1}], dp[{i, j}]) : 1.));
						}
					}
					std::swap(dp, ndp);
				}
			}
			std::vector<double> endgames2(tot_ones+1, 0);
			{
				// solve from the end, probability of winning from this state
				std::vector<double> dp(tot_ones+1);
				dp[0] = 1;
				std::vector<double> ndp(tot_ones+1);
				for (int m = 0; m <= M; m++) {
					if (M - m >= tot_all && M - m <= tot_ones + tot_all) {
						endgames2[tot_all + tot_ones - (M - m)] = dp[tot_all + tot_ones - (M - m)];
					}

					for (int i = 0; i <= tot_ones; i++) {
						ndp[i] = P * dp[i] + (1-P) * dp[std::max(i-1, 0)];
					}
					std::swap(dp, ndp);
				}
			}

			tensor<double, 2> win_prob({tot_ones + 1, tot_all+1});
			for (int j = 0; j <= tot_all; j++) {
				win_prob[{0, j}] = endgames[j];
			}
			for (int i = 0; i <= tot_ones; i++) {
				win_prob[{i, 0}] = endgames2[i];
			}

			for (int i = 1; i <= tot_ones; i++) {
				for (int j = 1; j <= tot_all; j++) {
					win_prob[{i, j}] = P * win_prob[{i, j-1}] + (1-P) * win_prob[{i-1, j}];
				}
			}
			return win_prob[{tot_ones, tot_all}];
		}() << '\n';
	}

	return 0;
}

// The only time you should maybe not hit is when you have 0 ones left but not shiny; then, you need to decide if you'd rather roll for more shiny or just start a new batch
// We should solve for "endgames" backwards, and the main state forwards
// Endgame state: M rounds left, K shinies, N ones
