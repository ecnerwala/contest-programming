#include <bits/stdc++.h>

#define john_lennox_vs_richard_dawkins main

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

template <typename T> void setmax(T& a, T b) { if (b > a) a = b; }

int john_lennox_vs_richard_dawkins() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	const int INF = 1e9;
	using pt_t = array<int, 2>;
	using loc_t = pair<int, pt_t>;

	int X, E; loc_t S; int C; cin >> X >> E >> S.second[0] >> S.second[1] >> C;
	S.first = 0;

	// number of cans, amount of energy
	map<loc_t, pair<int, int>> important_locs;
	important_locs[S] = {0,0};
	for (int i = 0; i < C; i++) {
		loc_t l; cin >> l.second[0] >> l.second[1] >> l.first;
		important_locs[l].first++;
		for (int dx = -1; dx <= 1; dx ++) {
			for (int dy = -1; dy <= 1; dy ++) {
				if (bool(dx) != bool(dy)) {
					l.second[0] += dx;
					l.second[1] += dy;

					important_locs[l].second ++;

					l.second[0] -= dx;
					l.second[1] -= dy;
				}
			}
		}
	}

	vector<pair<loc_t, pair<int, int>>> locs(important_locs.begin(), important_locs.end());

	assert(locs[0].first == S);

	int L = int(locs.size());
	int maxE = E + C;
	tensor<int, 2> dp({L, maxE+1}, -INF);
	dp[{0,E}] = 0;
	int ans = 0;
	for (int i = 0; i < L; i++) {
		// first, add however much energy we get
		{
			auto [score, eng] = locs[i].second;
			for (int e = maxE - eng; e >= 0; e--) {
				if (dp[{i,e}] == -INF) continue;
				setmax(dp[{i,e+eng}], dp[{i,e}] + score);
			}
		}

		for (int e = 0; e <= maxE; e++) {
			setmax(ans, dp[{i,e}]);
		}

		// now, we can move to another location
		for (int j = i+1; j < L; j++) {
			int dist =
				abs(locs[j].first.second[0] - locs[i].first.second[0])
				+ abs(locs[j].first.second[1] - locs[i].first.second[1]);
			if (dist > locs[j].first.first - locs[i].first.first) continue;
			for (int e = dist; e <= maxE; e++) {
				if (dp[{i,e}] == -INF) continue;
				setmax(dp[{j,e-dist}], dp[{i,e}]);
			}
		}
	}

	cout << ans << '\n';

	return 0;
}
