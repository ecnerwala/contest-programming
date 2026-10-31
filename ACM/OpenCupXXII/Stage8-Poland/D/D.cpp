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

	int T; cin >> T;
	while (T--) {
		int N, M, Q;
		cin >> N >> M >> Q;
		tensor<char, 2> G({N, M});
		for (int i = 0; i < N; i++) {
			string s; cin >> s;
			for (int j = 0; j < M; j++) {
				G[{i, j}] = s[j];
			}
		}
		tensor<int, 2> is_bad({2*N+1, 2*M+1});
		for (int ia = 0; ia < N; ia++) {
			for (int ja = 0; ja < M; ja++) {
				if (G[{ia, ja}] != 'A') continue;
				for (int ib = 0; ib < N; ib++) {
					for (int jb = 0; jb < M; jb++) {
						if (G[{ib, jb}] != 'B') continue;
						is_bad[{ia - ib + N, ja - jb + M}] = true;
					}
				}
			}
		}
		std::vector<std::vector<int>> row_bad(2*N+1);
		std::vector<std::vector<int>> col_bad(2*M+1);
		for (int i = 0; i <= 2*N; i++) {
			for (int j = 0; j <= 2*M; j++) {
				if (is_bad[{i, j}]) {
					row_bad[i].push_back(j);
					col_bad[j].push_back(i);
				}
			}
		}

		string ops; cin >> ops;
		auto solve = [&]() -> bool {
			int cur_x = N, cur_y = M;
			for (char c : ops) {
				if (c == 'N')  {
					// cur_x--;
					const auto& v = col_bad[cur_y];
					auto it = std::lower_bound(v.begin(), v.end(), cur_x);
					if (it == v.begin()) return true;
					cur_x = (*std::prev(it)) + 1;
				} else if (c == 'S') {
					// cur_x ++;
					const auto& v = col_bad[cur_y];
					auto it = std::lower_bound(v.begin(), v.end(), cur_x);
					if (it == v.end()) return true;
					cur_x = (*it) - 1;
				} else if (c == 'E') {
					// cur_y ++;
					const auto& v = row_bad[cur_x];
					auto it = std::lower_bound(v.begin(), v.end(), cur_y);
					if (it == v.end()) return true;
					cur_y = (*it) - 1;
				} else if (c == 'W') {
					// cur_y --;
					const auto& v = row_bad[cur_x];
					auto it = std::lower_bound(v.begin(), v.end(), cur_y);
					if (it == v.begin()) return true;
					cur_y = (*std::prev(it)) + 1;
				} else assert(false);
			}
			return false;
		};
		bool ans = solve();
		cout << (ans ? "TAK" : "NIE") << '\n';
	}

	return 0;
}
