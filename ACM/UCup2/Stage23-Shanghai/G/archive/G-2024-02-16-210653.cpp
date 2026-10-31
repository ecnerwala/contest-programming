#include <bits/stdc++.h>
#include <ext/pb_ds/priority_queue.hpp>

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
#ifdef _GLIBCXX_DEBUG
		return data[flatten_index_checked(idx)];
#else
		return data[flatten_index(idx)];
#endif
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

template <typename flow_t = int, typename cost_t = int64_t>
struct MCMF_Dinic {
	int N;
	std::vector<std::vector<int>> adj;
	struct edge_t {
		int dest;
		flow_t cap;
		cost_t cost;
	};
	std::vector<edge_t> edges;

	std::vector<char> seen;
	std::vector<cost_t> pi;

	explicit MCMF_Dinic(int N_) : N(N_), adj(N), pi(N, 0) {}

	void add_edge(int from, int to, flow_t cap, cost_t cost) {
		assert(cap >= 0);
		assert(cost + pi[from] - pi[to] >= 0); // TODO: Remove this restriction
		int e = int(edges.size());
		edges.emplace_back(edge_t{to, cap, cost});
		edges.emplace_back(edge_t{from, 0, -cost});
		adj[from].push_back(e);
		adj[to].push_back(e+1);
	}

	static constexpr cost_t INF_COST = std::numeric_limits<cost_t>::max() / 4;
	static constexpr flow_t INF_FLOW = std::numeric_limits<flow_t>::max() / 4;
	std::vector<cost_t> dist;
	__gnu_pbds::priority_queue<std::pair<cost_t, int>> q;
	std::vector<typename decltype(q)::point_iterator> its;
	cost_t dijkstra(int s, int t) {
		dist.assign(N, INF_COST);
		dist[s] = 0;

		its.assign(N, q.end());
		its[s] = q.push({-(dist[s] - pi[s]), s});

		while (!q.empty()) {
			int i = q.top().second; q.pop();
			cost_t d = dist[i];
			for (int e : adj[i]) {
				if (edges[e].cap) {
					int j = edges[e].dest;
					cost_t nd = d + edges[e].cost;
					if (nd < dist[j]) {
						dist[j] = nd;
						if (its[j] == q.end()) {
							its[j] = q.push({-(dist[j] - pi[j]), j});
						} else {
							q.modify(its[j], {-(dist[j] - pi[j]), j});
						}
					}
				}
			}
		}

		std::swap(pi, dist);
		return pi[t];
	}

	std::vector<int> buf;
	std::vector<int> level;
	flow_t dinic_dfs(int cur, int t, flow_t f) {
		if (cur == t) return f;
		flow_t cur_f = 0;
		assert(f > 0);
		for (; buf[cur] < int(adj[cur].size()); buf[cur]++) {
			int e = adj[cur][buf[cur]];
			int nxt = edges[e].dest;
			if (level[nxt] == level[cur] + 1 && edges[e].cap > 0 && edges[e].cost == pi[nxt] - pi[cur]) {
				flow_t v = dinic_dfs(nxt, t, std::min(f, edges[e].cap));
				edges[e].cap -= v;
				edges[e^1].cap += v;
				f -= v;
				cur_f += v;
				if (f == 0) break;
			}
		}
		return cur_f;
	}
	flow_t dinic(int s, int t) {
		flow_t tot_flow = 0;
		while (true) {
			buf.clear();
			buf.reserve(N);
			level.assign(N, -1);
			buf.push_back(s);
			level[s] = 0;
			for (int z = 0; z < int(buf.size()); z++) {
				int cur = buf[z];
				for (int e : adj[cur]) {
					int nxt = edges[e].dest;
					if (edges[e].cap > 0 && edges[e].cost == pi[nxt] - pi[cur] && level[nxt] == -1) {
						level[nxt] = level[cur] + 1;
						buf.push_back(nxt);
					}
				}
			}
			if (level[t] == -1) break;
			buf.assign(N, 0);
			tot_flow += dinic_dfs(s, t, INF_FLOW);
		}
		return tot_flow;
	}

	std::vector<std::pair<flow_t, cost_t>> all_flows(int s, int t, cost_t max_cost = INF_COST - 1) {
		assert(s != t);
		std::vector<std::pair<flow_t, cost_t>> res;
		while (dijkstra(s, t) <= max_cost) {
			assert(res.empty() || pi[t] > res.back().second);
			flow_t f = dinic(s, t);
			res.push_back({f, pi[t]});
		}
		return res;
	}

	std::pair<flow_t, cost_t> max_flow(int s, int t, cost_t max_cost = INF_COST - 1) {
		assert(s != t);
		flow_t tot_flow = 0; cost_t tot_cost = 0;
		while (dijkstra(s, t) <= max_cost) {
			flow_t cur_flow = dinic(s, t);
			tot_flow += cur_flow;
			tot_cost += cur_flow * pi[t];
		}
		return {tot_flow, tot_cost};
	}
};

tensor<int, 2> test_graph(int N, std::vector<std::array<int, 2>> edges) {
	tensor<int, 2> A({N, N});
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (i == j) A[{i, j}] = 0;
			else {
				MCMF_Dinic<int, int> flower(N);
				for (auto [u, v] : edges) {
					flower.add_edge(u, v, 1, 0);
					flower.add_edge(v, u, 1, 0);
				}
				A[{i, j}] = flower.max_flow(i, j).first;
			}
		}
	}
	return A;
}

bool is_valid_edges(int N, std::vector<std::array<int, 2>> edges) {
	for (auto& [u, v] : edges) {
		if (u > v) std::swap(u, v);
		if (!(0 <= u && u < v && v < N)) return false;
	}
	return true;
}

std::optional<std::vector<std::array<int, 2>>> solve(int N, tensor<int, 2> A) {
	std::vector<std::array<int, 2>> edges;
	auto solve = [&]() -> bool {
		// First, ensure that the groups which are >= 3, >= 2, and >= 1 are all cliques
		std::vector<std::vector<int>> group_ids(4);
		group_ids[0] = std::vector<int>(N, 0);
		for (int i = 0; i < N; i++) {
			if (A[{i, i}] != 0) return false;
			A[{i, i}] = 3;
		}
		for (int min_d = 1; min_d <= 3; min_d++) {
			std::vector<int> group_id(N);
			for (int i = 0; i < N; i++) {
				for (int j = 0; j < N; j++) {
					if (j == i || A[{i, j}] >= min_d) {
						group_id[i] = j;
						break;
					}
				}
			}
			for (int i = 0; i < N; i++) {
				for (int j = 0; j < N; j++) {
					if ((group_id[i] == group_id[j]) != (A[{i, j}] >= min_d)) {
						return false;
					}
				}
			}
			group_ids[min_d] = std::move(group_id);
		}

		std::vector<int> group_3_sz(N);
		for (int i = 0; i < N; i++) {
			group_3_sz[group_ids[3][i]]++;
		}
		std::vector<int> buf_3; buf_3.reserve(N);
		auto solve_3_group = [&](int st) -> std::array<std::array<int, 2>, 2> {
			assert(group_ids[3][st] == st);
			buf_3.clear();
			for (int i = 0; i < N; i++) {
				if (group_ids[3][i] == st) {
					buf_3.push_back(i);
				}
			}
			assert(int(buf_3.size()) == group_3_sz[st]);
			int s = int(buf_3.size());
			assert(s >= 1);
			if (s == 1) {
				return {std::array<int, 2>{st, st}, {st, st}};
			} else if (s == 2) {
				edges.push_back({buf_3[0], buf_3[1]});
				return {std::array<int, 2>{buf_3[0], buf_3[1]}, {buf_3[0], buf_3[1]}};
			} else if (s == 3) {
				edges.push_back({buf_3[0], buf_3[1]});
				edges.push_back({buf_3[0], buf_3[2]});
				edges.push_back({buf_3[1], buf_3[2]});
				return {std::array<int, 2>{buf_3[0], buf_3[1]}, {buf_3[0], buf_3[2]}};
			} else {
				for (int i = 0; i < (s-1); i++) {
					edges.push_back({buf_3[i], buf_3[(i+1) % (s-1)]});
					edges.push_back({buf_3[i], buf_3[s-1]});
				}
				return {std::array<int, 2>{st, st}, {st, st}};
			}
		};

		std::vector<int> buf_2; buf_2.reserve(N);
		std::vector<int> goods; goods.reserve(N);
		auto solve_2_group = [&](int st) -> bool {
			assert(group_ids[2][st] == st);
			buf_2.clear();
			goods.clear();
			for (int i = 0; i < N; i++) {
				if (group_ids[2][i] == st) {
					if (group_ids[3][i] == i) {
						if (group_3_sz[i] != 2 && group_3_sz[i] != 3) {
							goods.push_back(i);
						} else {
							buf_2.push_back(i);
						}
					}
				}
			}
			if (!buf_2.empty()) {
				if (goods.size() < 2) {
					return false;
				}
				std::array<int, 2> prv = solve_3_group(goods.back())[1];
				goods.pop_back();
				for (auto i : buf_2) {
					auto [lo, hi] = solve_3_group(buf_2[i]);
					edges.push_back({prv[0], lo[0]});
					edges.push_back({prv[1], lo[1]});
					prv = hi;
				}
				for (auto i : goods) {
					solve_3_group(i);
					edges.push_back({prv[0], i});
					prv[0] = i;
				}
				edges.push_back({prv[0], prv[1]});
				return true;
			} else {
				if (goods.size() == 1) {
					solve_3_group(goods.back());
					return true;
				} else if (goods.size() == 2) {
					int a = goods[0], b = goods[1];
					if (group_3_sz[a] > group_3_sz[b]) {
						std::swap(a, b);
					}
					if (group_3_sz[b] == 1) {
						return false;
					}
					solve_3_group(a);
					solve_3_group(b);
					auto [u, v] = edges.back();
					edges.pop_back();
					edges.push_back({u, a});
					edges.push_back({v, a});
					return true;
				} else {
					for (int z = 0; z < int(goods.size()); z++) {
						solve_3_group(goods[z]);
						edges.push_back({goods[z], goods[(z+1) % int(goods.size())]});
					}
					return true;
				}
			}
		};

		auto solve_1_group = [&](int st) -> bool {
			int prv = -1;
			for (int i = 0; i < N; i++) {
				if (group_ids[1][i] == st) {
					if (group_ids[2][i] == i) {
						if (!solve_2_group(i)) return false;
						if (prv != -1) edges.push_back({prv, i});
						prv = i;
					}
				}
			}
			return true;
		};
		for (int i = 0; i < N; i++) {
			if (group_ids[1][i] == i) {
				if (!solve_1_group(i)) {
					return false;
				}
			}
		}
		return true;
	};
	bool good = solve();
	if (good) return edges;
	else return std::nullopt;
}

void run_tests() {
	std::mt19937_64 mt(48);
	for (int z = 0; z < 100; z++) {
		int N = std::uniform_int_distribution(1, 10)(mt);
		double p = std::uniform_real_distribution<double>(0, 5)(mt) / N;
		p = std::min(p, 1.);
		std::cerr << "Test " << N << ' ' << p << '\n';
		std::vector<std::array<int, 2>> edges;
		for (int i = 0; i < N; i++) {
			for (int j = i+1; j < N; j++) {
				if (std::bernoulli_distribution(p)(mt)) {
					edges.push_back({i, j});
				}
			}
		}
		auto A_orig = test_graph(N, edges);
		bool valid = true;
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				if (A_orig[{i, j}] > 3) {
					valid = false;
				}
			}
		}
		if (!valid) {
			std::cerr << "Skipping" << '\n';
			continue;
		}

		std::cerr << N << '\n';
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				std::cerr << A_orig[{i, j}] << ' ';
			}
			std::cerr << '\n';
		}
		auto new_edges = solve(N, A_orig);
		if (!new_edges) {
			std::cerr << "Failed to find solution" << '\n';
			std::cerr << N << '\n';
			for (int i = 0; i < N; i++) {
				for (int j = 0; j < N; j++) {
					std::cerr << A_orig[{i, j}] << ' ';
				}
				std::cerr << '\n';
			}
			assert(false);
		}

		if (!is_valid_edges(N, *new_edges)) {
			std::cerr << N << '\n';
			for (int i = 0; i < N; i++) {
				for (int j = 0; j < N; j++) {
					std::cerr << A_orig[{i, j}] << ' ';
				}
				std::cerr << '\n';
			}
			std::cerr << new_edges->size() << '\n';
			for (auto [u, v] : *new_edges) {
				std::cerr << u << ' ' << v << '\n';
			}
			assert(false);
		}

		auto A_new = test_graph(N, *new_edges);
		bool matches = true;
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				if (A_new[{i, j}] != A_orig[{i, j}]) {
					matches = false;
				}
			}
		}
		if (!matches) {
			std::cerr << N << '\n';
			for (int i = 0; i < N; i++) {
				for (int j = 0; j < N; j++) {
					std::cerr << A_orig[{i, j}] << ' ';
				}
				std::cerr << '\n';
			}
			std::cerr << new_edges->size() << '\n';
			for (auto [u, v] : *new_edges) {
				std::cerr << u << ' ' << v << '\n';
			}
			std::cerr << N << '\n';
			for (int i = 0; i < N; i++) {
				for (int j = 0; j < N; j++) {
					std::cerr << A_new[{i, j}] << ' ';
				}
				std::cerr << '\n';
			}
			assert(false);
		}
	}
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	run_tests();

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		tensor<int, 2> A({N, N});
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				cin >> A[{i, j}];
			}
		}

		auto res = solve(N, A);
		if (res) {
			const auto& edges = *res;
			cout << "Yes" << '\n';
			cout << edges.size() << '\n';
			for (auto [u, v] : edges) {
				cout << u+1 << ' ' << v+1 << '\n';
			}
		} else {
			cout << "No" << '\n';
		}

	}

	return 0;
}
