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
	std::enable_if_t<(0 < D), tensor_view<T, NDIMS-1>> operator[] (int idx) const {
		std::array<int, NDIMS-1> nshape; std::copy(shape.begin()+1, shape.end(), nshape.begin());
		std::array<int, NDIMS-1> nstrides; std::copy(strides.begin()+1, strides.end(), nstrides.begin());
		T* ndata = data + (strides[0] * idx);
		return tensor_view<T, NDIMS-1>(nshape, nstrides, ndata);
	}
	template <int D = NDIMS>
	std::enable_if_t<(0 < D), tensor_view<T, NDIMS-1>> at(int idx) const {
		assert(0 <= idx && idx < shape[0]);
		return operator[](idx);
	}

	template <int D = NDIMS>
	std::enable_if_t<(0 == D), T&> operator * () const {
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
	std::enable_if_t<(0 < D), tensor_view<T, NDIMS-1>> operator[] (int idx) {
		return view()[idx];
	}
	template <int D = NDIMS>
	std::enable_if_t<(0 < D), tensor_view<T, NDIMS-1>> at(int idx) {
		return view().at(idx);
	}

	template <int D = NDIMS>
	std::enable_if_t<(0 < D), tensor_view<const T, NDIMS-1>> operator[] (int idx) const {
		return view()[idx];
	}
	template <int D = NDIMS>
	std::enable_if_t<(0 < D), tensor_view<const T, NDIMS-1>> at(int idx) const {
		return view().at(idx);
	}

	template <int D = NDIMS>
	std::enable_if_t<(0 == D), T&> operator * () {
		return *view();
	}
	template <int D = NDIMS>
	std::enable_if_t<(0 == D), const T&> operator * () const {
		return *view();
	}
};

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int N; cin >> N;
	tensor<bool, 2> is_pool({N,N});
	for (int i = 0; i < N; i++) {
		string S; cin >> S;
		for (int j = 0; j < N; j++) {
			is_pool[{i,j}] = S[j] == 'B';
		}
	}
	const array<int, 2> BAD{-1,-1};
	tensor<array<int, 2>, 2> pool_loc({N, N}, BAD);
	tensor<int, 2> pool_size({N, N}, 0);
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (is_pool[{i,j}]) {
				if (j && is_pool[{i,j-1}]) {
					pool_loc[{i,j}] = pool_loc[{i,j-1}];
				} else if (i && is_pool[{i-1,j}]) {
					pool_loc[{i,j}] = pool_loc[{i-1,j}];
				} else {
					pool_loc[{i,j}] = {i,j};
				}
				pool_size[pool_loc[{i,j}]] ++;
			} else {
				//pool_loc[{i,j}] = {i,j};
			}
		}
	}

	int ans = 0;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			ans = max(ans, pool_size[{i,j}]);
		}
	}

	auto get_pool_id = [&](array<int, 2> a) -> array<int, 2> {
		if (0 <= a[0] && a[0] < N && 0 <= a[1] && a[1] < N && is_pool[a]) {
			return pool_loc[a];
		} else {
			return BAD;
		}
	};
	vector<array<int, 2>> borders; borders.reserve(12);
	auto check_pt = [&](array<int, 2> a) {
		if (is_pool[a]) return;
		borders.clear();
		borders.push_back(get_pool_id({a[0]-1,a[1]}));
		borders.push_back(get_pool_id({a[0]+1,a[1]}));
		borders.push_back(get_pool_id({a[0],a[1]-1}));
		borders.push_back(get_pool_id({a[0],a[1]+1}));
		sort(borders.begin(), borders.end());
		borders.resize(unique(borders.begin(), borders.end()) - borders.begin());
		int cnd = 1;
		for (auto pt : borders) {
			if (pt == BAD) continue;
			cnd += pool_size[pt];
		}
		ans = max(ans, cnd);
	};

	auto check_pts = [&](array<int, 2> a, array<int, 2> b) {
		if (a == b) return;
		if (is_pool[a] || is_pool[b]) return;
		borders.clear();
		borders.push_back(get_pool_id({a[0]-1,a[1]}));
		borders.push_back(get_pool_id({a[0]+1,a[1]}));
		borders.push_back(get_pool_id({a[0],a[1]-1}));
		borders.push_back(get_pool_id({a[0],a[1]+1}));
		borders.push_back(get_pool_id({b[0]-1,b[1]}));
		borders.push_back(get_pool_id({b[0]+1,b[1]}));
		borders.push_back(get_pool_id({b[0],b[1]-1}));
		borders.push_back(get_pool_id({b[0],b[1]+1}));
		sort(borders.begin(), borders.end());
		borders.resize(unique(borders.begin(), borders.end()) - borders.begin());
		int cnd = 2;
		for (auto pt : borders) {
			if (pt == BAD) continue;
			cnd += pool_size[pt];
		}
		ans = max(ans, cnd);
	};

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			check_pt({i,j});
			if (i) check_pts({i-1,j}, {i,j});
			if (j) check_pts({i,j-1}, {i,j});
		}
	}

	vector<array<int, 2>> cnds; cnds.reserve(12);
	for (int i0 = 0; i0 < N; i0++) {
		for (int j0 = 0; j0 < N; j0++) {
			if (!(is_pool[{i0,j0}] && pool_loc[{i0,j0}] == array<int, 2>{i0,j0})) continue;
			int i1 = i0;
			while (i1+1 < N && is_pool[{i1+1, j0}]) i1++;

			int j1 = j0;
			while (j1+1 < N && is_pool[{i0, j1+1}]) j1++;

			assert((pool_size[{i0,j0}] == (i1-i0+1) * (j1-j0+1)));

			// Find the 2 distinct biggest people which are adjacent to an edge
			tuple<int, array<int, 2>, array<int, 2>> best_edge1{0, BAD, BAD};
			tuple<int, array<int, 2>, array<int, 2>> best_edge2{0, BAD, BAD};
			auto check_cnd = [&](array<int, 2> add_pt, array<int, 2> touch_pt) {
				if (!(0 <= add_pt[0] && add_pt[0] < N && 0 <= add_pt[1] && add_pt[1] < N)) return;

				touch_pt = get_pool_id(touch_pt);
				tuple<int, array<int, 2>, array<int, 2>> cnd(touch_pt == BAD ? 0 : pool_size[touch_pt], touch_pt == BAD ? add_pt : touch_pt, add_pt);
				if (get<1>(cnd) == get<1>(best_edge1) || get<1>(cnd) == get<1>(best_edge2)) return;
				if (cnd > best_edge1) {
					best_edge2 = best_edge1;
					best_edge1 = cnd;
				} else if (cnd > best_edge2) {
					best_edge2 = cnd;
				}
			};
			for (int i = i0+1; i <= i1-1; i++) {
				check_cnd({i,j0-1}, {i,j0-2});
				check_cnd({i,j1+1}, {i,j1+2});
			}
			for (int j = j0+1; j <= j1-1; j++) {
				check_cnd({i0-1,j}, {i0-2,j});
				check_cnd({i1+1,j}, {i1+2,j});
			}

			cnds.clear();
			cnds.push_back(get<2>(best_edge1));
			cnds.push_back(get<2>(best_edge2));
			if (i0-1 >= 0) {
				cnds.push_back({i0-1, j0});
				cnds.push_back({i0-1, j1});
			}
			if (i1+1 < N) {
				cnds.push_back({i1+1, j0});
				cnds.push_back({i1+1, j1});
			}
			if (j0-1 >= 0) {
				cnds.push_back({i0,j0-1});
				cnds.push_back({i1,j0-1});
			}
			if (j1+1 < N) {
				cnds.push_back({i0,j1+1});
				cnds.push_back({i1,j1+1});
			}
			sort(cnds.begin(), cnds.end());
			cnds.resize(unique(cnds.begin(), cnds.end()) - cnds.begin());
			for (int a = 0; a < int(cnds.size()); a++) {
				if (cnds[a] == BAD) continue;
				for (int b = a+1; b < int(cnds.size()); b++) {
					check_pts(cnds[a], cnds[b]);
				}
			}
		}
	}

	cout << ans << '\n';

	return 0;
}

// If you have a rectangle, if you're adjacent to a corner, you might see 2 recangles,
