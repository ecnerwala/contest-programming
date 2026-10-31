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

template <typename T> void setmax(T& a, T b) { if (b > a) a = b; }

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {

		int R, C; cin >> R >> C;
		tensor<int, 2> G({R, C});
		for (int i = 0; i < R; i++) {
			for (int j = 0; j < C; j++) {
				cin >> G[{i, j}];
			}
		}

		cerr << "Case #" << case_num << ": " << R << ' ' << C << endl;

		const int INF = 1e9;
		std::vector<int> best_row(R+C, -INF);
		std::vector<int> best_col(R+C, -INF);
		tensor<int, 3> last_vis({R, C, 2}, 0);
		int cur_id = 0;
		auto check_points = [&](std::initializer_list<std::array<int, 2>> pts) {
			cur_id++;
			int res = 0;
			for (auto [x, y] : pts) {
				auto check_vert = [&](int i, int j) {
					if (0 <= i && i + 2 < R && 0 <= j && j < C) {
						if (last_vis[{i, j, 0}] < cur_id) {
							last_vis[{i, j, 0}] += cur_id;
							res += G[{i, j}] == G[{i+1, j}] && G[{i, j}] == G[{i+2, j}];
						}
					}
				};
				auto check_horiz = [&](int i, int j) {
					if (0 <= i && i < R && 0 <= j && j + 2 < C) {
						if (last_vis[{i, j, 1}] < cur_id) {
							last_vis[{i, j, 1}] += cur_id;
							res += G[{i, j}] == G[{i, j+1}] && G[{i, j}] == G[{i, j+2}];
						}
					}
				};
				check_vert(x-2, y);
				check_vert(x-1, y);
				check_vert(x-0, y);
				check_horiz(x, y-2);
				check_horiz(x, y-1);
				check_horiz(x, y-0);
			}
			return res;
		};
		int ans = 0;
		for (int i = 0; i < R; i++) {
			for (int j = 0; j < C; j++) {
				if (i+1 < R) {
					std::swap(G[{i, j}], G[{i+1, j}]);
					int cnt = check_points({{i, j}, {i+1, j}});
					setmax(best_row[i], cnt);
					setmax(best_col[j], cnt);

					for (int ni = i-3; ni <= i+3; ni++) {
						for (int nj = j-3; nj <= j+3; nj++) {
							if (0 <= ni && ni+1 < R && 0 <= nj && nj < C) {
								std::swap(G[{ni, nj}], G[{ni+1, nj}]);
								setmax(ans, check_points({{i, j}, {i+1, j}, {ni, nj}, {ni+1, nj}}));
								std::swap(G[{ni, nj}], G[{ni+1, nj}]);
							}
							if (0 <= ni && ni < R && 0 <= nj && nj+1 < C) {
								std::swap(G[{ni, nj}], G[{ni, nj+1}]);
								setmax(ans, check_points({{i, j}, {i+1, j}, {ni, nj}, {ni, nj+1}}));
								std::swap(G[{ni, nj}], G[{ni, nj+1}]);
							}
						}
					}

					std::swap(G[{i, j}], G[{i+1, j}]);
				}
				if (j+1 < C) {
					std::swap(G[{i, j}], G[{i, j+1}]);
					int cnt = check_points({{i, j}, {i, j+1}});
					setmax(best_row[i], cnt);
					setmax(best_col[j], cnt);

					for (int ni = i-3; ni <= i+3; ni++) {
						for (int nj = j-3; nj <= j+3; nj++) {
							if (0 <= ni && ni+1 < R && 0 <= nj && nj < C) {
								std::swap(G[{ni, nj}], G[{ni+1, nj}]);
								setmax(ans, check_points({{i, j}, {i, j+1}, {ni, nj}, {ni+1, nj}}));
								std::swap(G[{ni, nj}], G[{ni+1, nj}]);
							}
							if (0 <= ni && ni < R && 0 <= nj && nj+1 < C) {
								std::swap(G[{ni, nj}], G[{ni, nj+1}]);
								setmax(ans, check_points({{i, j}, {i, j+1}, {ni, nj}, {ni, nj+1}}));
								std::swap(G[{ni, nj}], G[{ni, nj+1}]);
							}
						}
					}

					std::swap(G[{i, j}], G[{i, j+1}]);
				}
			}
		}

		auto check_gap = [&](const std::vector<int>& bests, int g) {
			int cur_best = -INF;
			for (int i = g; i < int(bests.size()); i++) {
				setmax(cur_best, best_row[i-g]);
				setmax(ans, cur_best + best_row[i]);
			}
		};
		check_gap(best_row, 4);
		check_gap(best_col, 4);

		cout << "Case #" << case_num << ": " << ans << '\n';
	}

	return 0;
}
