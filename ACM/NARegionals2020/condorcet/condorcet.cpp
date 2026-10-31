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

	int N, M; cin >> N >> M;
	tensor<int64_t, 2> cnt({N, N});
	while (M--) {
		string s; cin >> s;
		int64_t cur_cnt; cin >> cur_cnt;
		for (int i = 0; i < N; i++) {
			for (int j = i+1; j < N; j++) {
				cnt[{s[i]-'A',s[j]-'A'}] += cur_cnt;
			}
		}
	}

	vector<int> anc(N, 0);
	auto incr_anc = [&]() -> bool {
		int i = N-1;
		while (i >= 0 && anc[i] == N-1) anc[i--] = 0;
		if (i < 0) return false;
		anc[i]++;
		return true;
	};

	int64_t ans = 1e18;

	do {
		{
			bool is_good = true;
			for (int i = 0; i < N; i++) {
				if (anc[anc[i]] == i) is_good = false;
			}
			if (!is_good) continue;
		}

		vector<int64_t> cycle_diff; cycle_diff.reserve(N);
		int64_t fixed_cost = 0;

		for (int i = 0; i < N; i++) {
			int64_t diff = cnt[{anc[i], i}] - cnt[{i, anc[i]}];
			int cur = anc[i];
			for (int j = 0; cur != i && j < N; j++) {
				cur = anc[cur];
			}
			if (cur == i) {
				cycle_diff.push_back(diff);
			} else {
				fixed_cost = max(fixed_cost, 1 - diff);
			}
		}

		assert(int(cycle_diff.size()) >= 3);
		// need to make all of cycle_diff >= 1 using ops of the form +1/+1/+1/+1/-1
		// if k votes is good, then k+2 votes is good and k+3 votes is good

		auto is_good = [&](int64_t k) -> bool {
			int64_t tot_excess = 0;
			for (int64_t a : cycle_diff) {
				int64_t excess = a + k - 1;
				if (excess < 0) return false;
				tot_excess += excess / 2;
			}
			return tot_excess >= k;
		};

		int64_t mi = -1;
		int64_t ma = 0;
		while (!is_good(ma)) ma += ma - mi;
		while (ma - mi > 1) {
			int64_t md = mi + (ma - mi) / 2;
			if (is_good(md)) ma = md;
			else mi = md;
		}
		if (mi > 0 && is_good(mi-1)) ma = mi-1;

		int64_t cnd = ma;
		cnd = max(cnd, fixed_cost);

		ans = min(ans, cnd);
	} while (incr_anc());
	cout << ans << '\n';

	return 0;
}
