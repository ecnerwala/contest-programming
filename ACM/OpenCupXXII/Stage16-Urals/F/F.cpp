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

template <typename T> void setmin(T& a, T b) { if (b < a) a = b; }

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int D, S_full; cin >> D >> S_full;
	std::vector<int> A(S_full);
	for (auto& a : A) cin >> a;

	int S = std::min(S_full, 3);
	tensor<std::pair<int64_t, int>, 2> days({D, S});

	for (int i = 0; i < S_full; i++) {
		for (int d = 0; d < D; d++) {
			std::pair<int64_t, int> v; cin >> v.first; v.second = i;
			if (i < S) {
				days[{d, i}] = v;
				for (int z = i-1; z >= 0; z--) {
					if (days[{d, z+1}] < days[{d, z}]) std::swap(days[{d, z+1}], days[{d, z}]);
					else break;
				}
			} else {
				if (v < days[{d, S-1}]) {
					days[{d, S-1}] = v;
					for (int z = S-2; z >= 0; z--) {
						if (days[{d, z+1}] < days[{d, z}]) std::swap(days[{d, z+1}], days[{d, z}]);
						else break;
					}
				}
			}
		}
	}


	const int64_t INF = 1e18;
	tensor<std::pair<int64_t, int>, 3> dp({D, S, D+1}, {INF, -1});

	for (int z = 0; z < S; z++) {
		auto [v, p] = days[{0, z}];
		dp[{0, z, 1}].first = v;
	}

	for (int d = 1; d < D; d++) {
		for (int z = 0; z < S; z++) {
			auto [v, p] = days[{d, z}];
			for (int x = 0; x < S; x++) {
				auto pp = days[{d-1, x}].second;
				for (int i = 1; i <= A[pp]; i++) {
					int j = (p == pp ? i+1 : 1);
					if (j > A[p]) break;
					setmin(dp[{d, z, j}], {dp[{d-1, x, i}].first + v, 3*i + x});
				}
			}
		}
	}

	int64_t ans = INF;
	std::array<int, 3> best_state{-1, -1, -1};
	for (int z = 0; z < S; z++) {
		for (int i = 1; i <= D; i++) {
			std::array<int, 3> cnd{D-1, z, i};
			if (dp[cnd].first < ans) {
				ans = dp[cnd].first;
				best_state = cnd;
			}
		}
	}
	std::vector<int> path(D);
	for (int d = D-1; d >= 0; d--) {
		assert(best_state[0] == d);
		int z = best_state[1];
		path[d] = days[{d, z}].second;
		auto ns = dp[best_state].second;
		best_state[0] --;
		best_state[1] = ns % 3;
		best_state[2] = ns / 3;
	}

	cout << ans << '\n';
	for (int d = 0; d < D; d++) {
		cout << path[d]+1 << " \n"[d+1==D];
	}

	return 0;
}
