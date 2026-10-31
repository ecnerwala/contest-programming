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
	for (int case_num = 1; case_num <= T; case_num ++) {

		int R, C; int64_t K; cin >> R >> C >> K;
		cerr << "Case #" << case_num << " " << R << ' ' << C << '\n';
		tensor<int, 2> owners({R, C});
		for (int i = 0; i < R; i++) {
			for (int j = 0; j < C; j++) {
				cin >> owners[{i, j}];
			}
		}
		std::vector<std::vector<std::array<int, 2>>> by_owner(R*C+1);
		// insert them in increasing j-i
		for (int d = -(R-1); d <= (C-1); d++) {
			for (int i = 0, j = d; i < R; i++, j++) {
				assert(j - i == d);
				if (0 <= j && j < C) {
					by_owner[0].push_back({i, j});
					by_owner[owners[{i, j}]].push_back({i, j});
				}
			}
		}
		std::vector<int64_t> tot_cnt(std::max(R, C));
		tensor<int, 2> cur_cnt_x({R, R+C-1});
		tensor<int, 2> cur_cnt_y({C, R+C-1});
		std::vector<bool> has_sums(R+C-1);
		std::vector<int> sums; sums.reserve(R+C-1);
		for (int o = 0; o <= R*C; o++) {
			sums.clear();
			for (auto [x, y] : by_owner[o]) {
				if (!has_sums[x+y]) {
					sums.push_back(x+y);
					has_sums[x+y] = true;
				}
			}
			std::sort(sums.begin(), sums.end());
			//for (auto s : sums) has_sums[s] = false;
			// we're processing in increasing y - x
			for (auto [x, y] : by_owner[o]) {
				// first, query
				for (int ox = x; ox < R; ox++) {
					tot_cnt[ox - x] -= cur_cnt_x[{ox, x+y}];
				}
				//for (int ox = x; ox >= 0; ox--) assert((cur_cnt_x[{ox, x+y}] == 0));

				for (int oy = y; oy >= 0; oy--) {
					tot_cnt[y - oy] -= cur_cnt_y[{oy, x+y}];
				}
				//for (int oy = y; oy < C; oy++) assert((cur_cnt_y[{oy, x+y}] == 0));
				// now update
				//for (int s : sums) {
				for (int s = 0; s <= (R-1)+(C-1); s++) {
					if (s > x+y) cur_cnt_y[{y, s}]++;
					else cur_cnt_x[{x, s}]++;
				}
			}
			for (auto [x, y] : by_owner[o]) {
				//for (int s : sums) {
				for (int s = 0; s <= (R-1)+(C-1); s++) {
					if (s > x+y) cur_cnt_y[{y, s}] = 0;
					else cur_cnt_x[{x, s}] = 0;
				}
			}
			assert(tot_cnt[0] == 0);
			if (o == 0) for (auto& v : tot_cnt) v = -v;
		}
		for (auto& v : tot_cnt) v *= 2;

		K--;
		int64_t a = 0;
		while (K >= tot_cnt[a]) {
			K -= tot_cnt[a];
			a++;
		}
		cout << "Case #" << case_num << ": " << a << '\n';
	}

	return 0;
}
