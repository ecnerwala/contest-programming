#include<bits/stdc++.h>
#include <random>

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
	int N, M; cin >> N >> M;
	tensor<int, 2> D({N, M});
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			cin >> D[{i,j}];
		}
	}

	vector<int> best_set; best_set.reserve(N);

	using msk_t = bitset<200>;
	struct diff_t {
		msk_t msk;
		int cnt;
		int idx;
	};
	vector<diff_t> msks; msks.reserve(N);
	mt19937 mt(48);
	for (int iter = 0; iter <= 20; iter++) {
		int base = std::uniform_int_distribution<int>(0, N-1)(mt);
		msks.clear();
		for (int i = 0; i < N; i++) {
			int min_D = D[{i,0}] - D[{base,0}];
			int max_D = D[{i,0}] - D[{base,0}];
			for (int j = 1; j < M; j++) {
				min_D = min(min_D, D[{i, j}] - D[{base, j}]);
				max_D = max(max_D, D[{i, j}] - D[{base, j}]);
			}
			if (max_D - min_D >= 2) {
				continue;
			}
			msk_t cur_msk;
			for (int j = 0; j < M; j++) {
				cur_msk[j] = (D[{i, j}] - D[{base, j}] - min_D);
			}
			msks.push_back(diff_t{cur_msk, int(cur_msk.count()), i});
		}

		sort(msks.begin(), msks.end(), [&](const diff_t& a, const diff_t& b) { return a.cnt < b.cnt; });

		vector<pair<int, int>> dp(int(msks.size()), {1, -1});
		for (int a = 0; a < int(msks.size()); a++) {
			for (int b = 0; b < a; b++) {
				if ((msks[b].msk & ~msks[a].msk).any()) {
					continue;
				}
				dp[a] = max(dp[a], {dp[b].first+1, b});
			}
		}
		int cur = int(max_element(dp.begin(), dp.end()) - dp.begin());
		if (dp[cur].first > int(best_set.size())) {
			best_set.clear();
			for (; cur != -1; cur = dp[cur].second) {
				best_set.push_back(msks[cur].idx);
			}
		}
	}

	vector<pair<int, int>> ans; ans.reserve(best_set.size());
	for (int i : best_set) {
		int s = 0;
		for (int j = 0; j < M; j++) {
			s += D[{i,j}];
		}
		ans.push_back({-s, i});
	}
	sort(ans.begin(), ans.end());
	cout << int(ans.size()) << '\n';
	for (int z = 0; z < int(ans.size()); z++) {
		cout << ans[z].second+1 << " \n"[z+1==int(ans.size())];
	}

	return 0;
}
