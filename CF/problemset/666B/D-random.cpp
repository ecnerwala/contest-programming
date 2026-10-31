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


struct cnd_t {
	int first;
	std::array<int, 4> second;
	friend bool operator < (const cnd_t& a, const cnd_t& b) { return a.first < b.first; }
};

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	clock_t start_time = clock();
	int N, M; cin >> N >> M;
	vector<vector<int>> adj(N);
	for (int e = 0; e < M; e++) {
		int u, v; cin >> u >> v; u--, v--;
		adj[u].push_back(v);
	}

	tensor<int, 2> dist({N, N}, -1);
	for (int i = 0; i < N; i++) {
		vector<int> q; q.reserve(N);
		dist[{i,i}] = 0;
		q.push_back(i);
		for (int z = 0; z < int(q.size()); z++) {
			int j = q[z];
			for (int k : adj[j]) {
				if (dist[{i, k}] != -1) continue;
				dist[{i, k}] = dist[{i, j}] + 1;
				q.push_back(k);
			}
		}
	}

	//using cnd_t = pair<int, array<int, 4>>;

	const cnd_t BAD{-1, {-1,-1,-1,-1}};

	cnd_t ans = BAD;
	mt19937 mt(46);
	for (int iter = 0; (clock() - start_time) / double(CLOCKS_PER_SEC) < 4.8; iter++) {
		vector<int> inds(N); iota(inds.begin(), inds.end(), 0);
		shuffle(inds.begin(), inds.end(), mt);

		vector<array<cnd_t, 3>> dp(N, array<cnd_t, 3>{BAD, BAD, BAD});
		for (int cur = 0; cur < N; cur++) {
			dp[cur][0] = cnd_t{0, {inds[cur], -1, -1, -1}};
			for (int prv = 0; prv < cur; prv++) {
				int d = dist[{inds[prv], inds[cur]}];
				if (d == -1) continue;

				for (int z = 0; z < 3; z++) {
					cnd_t nc = dp[prv][z];
					if (nc.first == -1) continue;
					assert(nc.second[z] == inds[prv]);
					// from prv to cur
					nc.first += d;
					nc.second[z+1] = inds[cur];

					if (z+1 == 3) ans = max(ans, nc);
					else dp[cur][z+1] = max(dp[cur][z+1], nc);
				}
			}
		}
	}

	cerr << ans.first << '\n';
	cout << ans.second[0]+1 << ' ' << ans.second[1]+1 << ' ' << ans.second[2]+1 << ' ' << ans.second[3]+1 << '\n';

	return 0;
}

// find 4 pts a, b, c, d
// must be far apart
//
// Shuffle and solve the dag case
