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

std::optional<tensor<int, 2>> solve(std::vector<int> A) {
	int N = int(A.size());
	std::vector<int> initial_cnt(N);
	for (int i = 0; i < N; i++) initial_cnt[A[i]]++;
	for (int i = 0; i < N; i++) {
		if ((initial_cnt[i] & 1) != (N & 1)) {
			return std::nullopt;
		}
	}
	tensor<int, 2> res({N, N});
	if (N % 2 == 1) {
		// m[i][j] = f(i+j)
		// f(2*i) = A[i]
		// m[i][j] = f((i+j)/ 2 % N);
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				int c = i+j;
				if (c >= N) c -= N;
				if (c & 1) c += N;
				c /= 2;
				assert((i+j-2*c) % N == 0);
				res[{i, j}] = A[c];
			}
		}
	} else {
		std::vector<int> values; values.reserve(N);
		std::vector<int> non_values; non_values.reserve(N);
		std::vector<std::vector<int>> locs(N);
		for (int v = 0; v < N; v++) {
			locs[v].reserve(initial_cnt[v]);
			if (initial_cnt[v] != 0) values.push_back(v);
			else non_values.push_back(v);
		}
		for (int i = 0; i < N; i++) {
			locs[A[i]].push_back(i);
		}

		std::vector<int> circ_idx(N, -1);
		std::vector<int> slope_value(N, -1);
		slope_value[N-1] = values[0];
		int nxt_idx = N-1;
		for (int a : locs[values[0]]) {
			circ_idx[a] = nxt_idx--;
		}
		for (int z = 1; z < int(values.size()); z++) {
			int our_slope = (nxt_idx + nxt_idx - (int(locs[values[z]].size()) - 1)) % (N-1);
			slope_value[our_slope] = values[z];
			for (int a : locs[values[z]]) {
				circ_idx[a] = nxt_idx--;
			}
		}
		assert(nxt_idx == -1);
		for (int i = 0; i < N; i++) {
			if (slope_value[i] == -1) {
				slope_value[i] = non_values.back();
				non_values.pop_back();
			}
		}
		assert(non_values.empty());

		std::cerr << "solve " << N << '\n';
		for (int i = 0; i < N; i++) {
			std::cerr << A[i] << " \n"[i+1==N];
		}
		for (int i = 0; i < N; i++) {
			std::cerr << i << ' ' << circ_idx[i] << " \n"[i+1==N];
		}
		for (int i = 0; i < N; i++) {
			std::cerr << i << ' ' << slope_value[i] << " \n"[i+1==N];
		}

		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				if (j == i) {
					res[{i, j}] = A[i];
				} else {
					int x = circ_idx[i], y = circ_idx[j];
					int v = slope_value[((x == N-1 ? y : x) + (y == N-1 ? x : y))%N];
					assert((v == A[i]) == (v == A[j]));
					if (v == A[i] && v == A[j]) {
						res[{i, j}] = slope_value[N-1];
					}
				}
			}
		}
	}
	return res;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> A(N);
		for (auto& a : A) { cin >> a; a--; }

		auto res = solve(A);
		if (res) {
			for (int i = 0; i < N; i++) {
				for (int j = 0; j < N; j++) {
					cout << (*res)[{i,j}] + 1 << " \n"[j+1==N];
				}
			}
		} else {
			cout << -1 << '\n';
		}
	}

	return 0;
}
