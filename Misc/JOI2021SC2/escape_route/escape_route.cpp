#include <bits/stdc++.h>
#include <queue>

#include "escape_route.h"

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

template <typename T> using min_pq = std::priority_queue<T, std::vector<T>, std::greater<T>>;

std::vector<long long> calculate_necessary_time(
	int N, int M, long long S, int Q, std::vector<int> A, std::vector<int> B,
	std::vector<long long> L, std::vector<long long> C, std::vector<int> U,
	std::vector<int> V, std::vector<long long> T
) {
	using namespace std;
	using ll = long long;

	struct edge_t {
		int dest;
		ll L, C;
	};
	vector<vector<edge_t>> adj(N);
	for (int e = 0; e < M; e++) {
		adj[A[e]].push_back(edge_t{B[e], L[e], C[e]});
		adj[B[e]].push_back(edge_t{A[e], L[e], C[e]});
	}

	tensor<vector<pair<ll, ll>>, 2> shortest_paths({N,N});
	vector<vector<ll>> back_dists(2*M);
	vector<vector<ll>> forwards_dists(2*M);
	for (int e = 0; e < M; e++) {
		for (int z = 0; z < 2; z++, swap(A[e], B[e])) {
			// dijkstra backwards from A[e] and forwards from B[e]
			{
				auto& back_dist = back_dists[2*e+z] = vector<ll>(N, S+1);
				vector<bool> vis(N, false);
				back_dist[A[e]] = L[e];
				for (int iter = 0; iter < N; iter++) {
					int cur = -1;
					ll d = S+1;
					for (int cnd = 0; cnd < N; cnd++) if (!vis[cnd] && back_dist[cnd] < d) {
						cur = cnd;
						d = back_dist[cnd];
					}
					if (cur == -1) break;
					vis[cur] = true;
					for (auto [nxt, l, c] : adj[cur]) {
						if (C[e] - d > c) continue;
						if (C[e] - d - l < 0) continue;
						ll cnd = d + l;
						if (cnd < back_dist[nxt]) {
							back_dist[nxt] = cnd;
						}
					}
				}
			}
			{
				auto& forwards_dist = forwards_dists[2*e+z] = vector<ll>(N, S+1);
				vector<bool> vis(N, false);
				forwards_dist[B[e]] = 0;
				for (int iter = 0; iter < N; iter++) {
					int cur = -1;
					ll d = S+1;
					for (int cnd = 0; cnd < N; cnd++) if (!vis[cnd] && forwards_dist[cnd] < d) {
						cur = cnd;
						d = forwards_dist[cnd];
					}
					if (cur == -1) break;
					vis[cur] = true;
					for (auto [nxt, l, c] : adj[cur]) {
						if (C[e] + d >= c-l) continue;
						ll cnd = d+l;
						if (cnd < forwards_dist[nxt]) {
							forwards_dist[nxt] = cnd;
						}
					}
				}
			}
		}
	}

	auto compress_paths = [](vector<pair<ll, ll>>& paths) -> void {
		int l = 0;
		for (int z = 0; z < int(paths.size()); z++) {
			auto [t, d] = paths[z];
			if (l > 0 && t == paths[l-1].first && d >= paths[l-1].second) {
				continue;
			}
			while (l > 0 && d <= paths[l-1].second) l--;
			paths[l++] = paths[z];
		}
		paths.resize(l);
	};

	vector<pair<ll, int>> bottlenecks; bottlenecks.reserve(M);
	for (int i = 0; i < N; i++) {
		bottlenecks.clear();
		for (int e = 0; e < 2*M; e++) {
			if (back_dists[e][i] > S) continue;
			bottlenecks.push_back({C[e/2] - back_dists[e][i], e});
		}
		sort(bottlenecks.begin(), bottlenecks.end());
		for (int j = 0; j < N; j++) {
			if (i == j) continue;
			auto& paths = shortest_paths[{i,j}];
			for (auto [t, e] : bottlenecks) {
				if (forwards_dists[e][j] > S) continue;
				pair<ll, ll> cnd(t, back_dists[e][i] + forwards_dists[e][j]);
				if (!paths.empty() && paths.back().first == t && paths.back().second <= cnd.second) continue;
				while (!paths.empty() && paths.back().second >= cnd.second) paths.pop_back();
				paths.push_back(cnd);
			}
		}
	}

	tensor<ll, 2> day_dist({N, N}, S * (N+1));
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (i == j) day_dist[{i,j}] = 0;
			else if (!shortest_paths[{i,j}].empty()) day_dist[{i,j}] = shortest_paths[{i,j}].front().second;
		}
	}

	for (int k = 0; k < N; k++) {
		for (int i = 0; i < N; i++) {
			ll day_ik = (day_dist[{i,k}] + S - 1) / S * S;
			for (int j = 0; j < N; j++) {
				day_dist[{i,j}] = min(day_dist[{i,j}], day_ik + day_dist[{k,j}]);
			}
		}
	}

	tensor<vector<pair<ll, ll>>, 2> day_paths({N,N});
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (i == j) continue;
			for (int k = 0; k < N; k++) {
				if (shortest_paths[{i,k}].empty()) continue;
				day_paths[{i,j}].push_back({shortest_paths[{i,k}].back().first, day_dist[{k,j}]});
			}
			sort(day_paths[{i,j}].begin(), day_paths[{i,j}].end(), [](const auto& a, const auto& b) { return a.first < b.first; });
			day_paths[{i,j}].push_back({0, day_dist[{i,j}]});
			day_paths[{i,j}].push_back({S, day_dist[{i,j}]});
			compress_paths(day_paths[{i,j}]);
		}
	}

	auto query = [&](int u, int v, ll t0) -> ll {
		{
			const auto& paths = shortest_paths[{u, v}];
			auto it = lower_bound(paths.begin(), paths.end(), pair<ll, ll>{t0, -1});
			if (it != paths.end()) {
				return it->second;
			}
		}
		{
			const auto& paths = day_paths[{u, v}];
			auto it = lower_bound(paths.begin(), paths.end(), pair<ll, ll>{t0, -1});
			assert(it != paths.end());
			return S - t0 + it->second;
		}
	};

	vector<ll> ans(Q);
	for (int q = 0; q < Q; q++) {
		ans[q] = query(U[q], V[q], T[q]);
	}
	return ans;
}

// We want O(MN^2 + QN) runtime I think.
