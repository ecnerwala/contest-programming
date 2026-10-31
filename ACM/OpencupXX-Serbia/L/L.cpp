#include<bits/stdc++.h>

template <typename T, int NDIMS> struct tensor_view {
	static_assert(NDIMS >= 0);

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

	using i64 = int64_t;

	vector<i64> p10(19);
	p10[0] = 1;
	for (int i = 1; i <= 18; i++) p10[i] = p10[i-1] * 10;

	tensor<pair<i64, i64>, 3> dp({19, 10, 10}); // len, prefmax, start -> (# steps, tot incr)
	for (int p = 0; p < 10; p++) {
		for (int s = 0; s < 10; s++) {
			dp[{0, p, s}] = {1, max(p, s)};
		}
	}
	for (int l = 1; l <= 18; l++) {
		for (int p = 0; p < 10; p++) {
			for (int s = 0; s < 10; s++) {
				pair<i64, i64> res(0, 0);
				while (s + res.second < p10[l]) {
					i64 cur = s + res.second;

					i64 np = max(p, int(cur / p10[l-1]));
					assert(0 <= np && np <= 9);

					i64 ns = cur % p10[l-1];
					assert(0 <= ns && ns <= 9);

					auto nres = dp[{l-1, int(np), int(ns)}];

					res.first += nres.first;
					res.second += nres.second;

					if (res.second == 0) {
						break;
					}
				}

				dp[{l, p, s}] = res;
			}
		}
	}

	i64 A, M, N; cin >> A >> M >> N; N--;

	i64 t = 0;
	unordered_map<i64, i64> vis;

	while (t < N) {
		//cerr << A << ' ' << t << '\n';
		if (vis.count(A)) {
			i64 cyc = t - vis[A];
			t += (N - t) / cyc * cyc;
			vis.clear();
			continue;
		}

		vis[A] = t;
		// make a step
		int l = 0;
		while (true) {
			l++;
			if (l > 18) break;
			if (A % p10[l] > 9) {
				break;
			}
			int s = int(A % p10[l]);
			if (A - s + p10[l] > M) {
				break;
			}
			int p = 0;
			for (i64 v = A / p10[l]; v; v /= 10) p = max(p, int(v % 10));
			pair<i64, i64> move = dp[{l, p, s}];

			if (t + move.first > N) break;
		}
		--l;

		int s = int(A % p10[l]);
		int p = 0;
		for (i64 v = A / p10[l]; v; v /= 10) p = max(p, int(v % 10));
		pair<i64, i64> move = dp[{l, p, s}];

		t += move.first;
		A += move.second;
		A %= M;
	}

	cout << A << '\n';
	return 0;
}
