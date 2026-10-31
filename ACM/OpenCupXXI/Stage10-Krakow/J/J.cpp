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

const int dx[4] = {-1, 0, 1, 0};
const int dy[4] = {0, -1, 0, 1};

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		tensor<int, 2> V({N, N});
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				cin >> V[{i,j}];
			}
		}
		tensor<int, 2> P({N, N});
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				cin >> P[{i,j}];
			}
		}

		vector<int> C(N*N);
		vector<int64_t> W(N*N);
		vector<int> L(N*N);
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				int p = P[{i,j}];
				C[p]++;
				W[p] += V[{i,j}];
				if (i > 0 && P[{i-1,j}] != p) L[p]++;
				if (i+1 < N && P[{i+1,j}] != p) L[p]++;
				if (j > 0 && P[{i,j-1}] != p) L[p]++;
				if (j+1 < N && P[{i,j+1}] != p) L[p]++;
			}
		}
		vector<set<pair<int, int>>> best_weight(4*N*N+1);
		for (int p = 0; p < N * N; p++) {
			if (C[p]) best_weight[L[p]].insert({W[p], p});
		}
		map<int, array<int, 2>> active_lens;
		for (int l = 0; l <= 4 * N * N; l++) {
			if (!best_weight[l].empty()) {
				active_lens.insert({l, {best_weight[l].begin()->first, best_weight[l].rbegin()->first}});
			}
		}

		auto update_active = [&](int l) {
			if (best_weight[l].empty()) active_lens.erase(l);
			else active_lens[l] = {best_weight[l].begin()->first, best_weight[l].rbegin()->first};
		};

		int Q; cin >> Q;
		while (Q--) {
			{
				int i, j, p; cin >> i >> j >> p; i--, j--;
				int o = std::exchange(P[{i,j}], p);
				if (o != p) {
					if (C[o]) {
						best_weight[L[o]].erase({W[o], o});
						update_active(L[o]);
					}
					if (C[p]) {
						best_weight[L[p]].erase({W[p], p});
						update_active(L[p]);
					}

					C[o]--;
					W[o] -= V[{i,j}];
					C[p]++;
					W[p] += V[{i,j}];

					for (int d = 0; d < 4; d++) {
						int x = i + dx[d];
						int y = j + dy[d];
						if (0 <= x && x < N && 0 <= y && y < N) {
							if (P[{x,y}] == o) {
								L[o]++;
								L[p]++;
							} else if (P[{x,y}] == p) {
								L[o]--;
								L[p]--;
							} else {
								L[o]--;
								L[p]++;
							}
						}
					}

					if (C[o]) {
						best_weight[L[o]].insert({W[o], o});
						update_active(L[o]);
					}
					if (C[p]) {
						best_weight[L[p]].insert({W[p], p});
						update_active(L[p]);
					}
				}
			}

			{
				int64_t A, B, C; cin >> A >> B >> C;
				const int64_t INF = 1e18;
				int64_t ans = -INF;
				for (auto [l, w] : active_lens) {
					ans = max(ans, A * w[0] + B * l + (C * w[0]) * l);
					ans = max(ans, A * w[1] + B * l + (C * w[1]) * l);
				}
				cout << ans << " \n"[Q==0];
			}
		}
	}

	return 0;
}
