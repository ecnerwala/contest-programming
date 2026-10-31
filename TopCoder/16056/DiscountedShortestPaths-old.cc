#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

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

int64_t dp[1<<18][20];

class DiscountedShortestPaths {

public:

	long long minimumCostSum(int N, vector <int> a, vector <int> b, vector <int> c, vector <int> d) {
		sort(d.begin(), d.end());
		while (int(d.size()) >= N-1) {
			int v0 = d[0];
			for (auto& v : d) v -= v0;
			for (auto& v : c) v = max(0, v-v0);
			while (!d.empty() && *d.begin() == 0) d.erase(d.begin());
		}
		int D = int(d.size());
		assert(D < N-1);

		const int64_t INF = 1e18;
		int M = int(a.size());
		tensor<int64_t, 2> dist({N,N}, INF);
		for (int i = 0; i < N; i++) dist[{i,i}] = 0;
		vector<vector<pair<int, pair<int, int>>>> adj(N);

		for (int e = 0; e < M; e++) {
			setmin<int64_t>(dist[{a[e], b[e]}], c[e]);
			setmin<int64_t>(dist[{b[e], a[e]}], c[e]);
			int idx = int(lower_bound(d.begin(), d.end(), c[e]) - d.begin());
			adj[a[e]].push_back({b[e], {c[e], idx}});
			adj[b[e]].push_back({a[e], {c[e], idx}});

		}

		for (int k = 0; k < N; k++) {
			for (int i = 0; i < N; i++) {
				for (int j = 0; j < N; j++) {
					setmin(dist[{i,j}], dist[{i,k}] + dist[{k,j}]);
				}
			}
		}

		int64_t ans = 0;
		for (int i = 0; i < N; i++) {
			for (int m = 0; m < (1 << D); m++) {
				for (int j = 0; j < N; j++) {
					dp[m][j] = INF;
				}
			}
			dp[0][i] = 0;
			for (int m = 0; m < (1 << D); m++) {
				for (int j = 0; j < N; j++) {
					for (int k = 0; k < N; k++) {
						setmin(dp[m][j], dp[m][k] + dist[{k,j}]);
					}
				}

				int rem = ((1<<D)-1-m);
				for (int j = 0; j < N; j++) {
					for (auto it : adj[j]) {
						// we can use an edge with a coupon
						int k = it.first;
						int v = it.second.first;
						int idx = it.second.second;
						if ((rem & ((1 << idx) - 1))) {
							int p = 31 - __builtin_clz((1 << idx) - 1);
							setmin(dp[m|(1<<p)][k], dp[m][j] + v - d[p]);
						}

						if ((rem >> idx)) {
							int p = idx + __builtin_ctz(rem >> idx);
							setmin(dp[m|(1<<p)][k], dp[m][j]);
						}
					}
					// we could also just not use anything
					if (rem) {
						int p = __builtin_ctz(rem);
						setmin(dp[m|(1<<p)][j], dp[m][j]);
					}
				}
			}
			for (int j = i+1; j < N; j++) {
				ans += dp[(1<<D)-1][j];
			}
		}
		return ans;
	}
};

void my_tests() {
	// FILL IN TESTS
	int N = 20;
	vector<int> A;
	vector<int> B;
	vector<int> C;
	for (int i = 0; i < N; i++) {
		for (int j = i+1; j < N; j++) {
			A.push_back(i);
			B.push_back(j);
			C.push_back(int(5e8));
		}
	}
	vector<int> D(20);
	iota(D.begin(), D.end(), int(5e8) - 10);
	ofstream fout("tbig.in");
	fout << N << '\n';
	for (int i = 0; i < int(A.size()); i++) {fout << ",{"[i==0] << A[i]; } fout << '}' << '\n';
	for (int i = 0; i < int(B.size()); i++) {fout << ",{"[i==0] << B[i]; } fout << '}' << '\n';
	for (int i = 0; i < int(C.size()); i++) {fout << ",{"[i==0] << C[i]; } fout << '}' << '\n';
	for (int i = 0; i < int(D.size()); i++) {fout << ",{"[i==0] << D[i]; } fout << '}' << '\n';

	DiscountedShortestPaths().minimumCostSum(N, A, B, C, D);
}

}  // namespace ecnerwala

using ecnerwala::DiscountedShortestPaths;

// vim:ft=cpp
