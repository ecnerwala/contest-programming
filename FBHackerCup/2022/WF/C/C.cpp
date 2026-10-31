#include<bits/stdc++.h>

namespace std {

template<class Fun>
class y_combinator_result {
	Fun fun_;
public:
	template<class T>
	explicit y_combinator_result(T &&fun): fun_(std::forward<T>(fun)) {}

	template<class ...Args>
	decltype(auto) operator()(Args &&...args) {
		return fun_(std::ref(*this), std::forward<Args>(args)...);
	}
};

template<class Fun>
decltype(auto) y_combinator(Fun &&fun) {
	return y_combinator_result<std::decay_t<Fun>>(std::forward<Fun>(fun));
}

} // namespace std

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

namespace ecnerwala {

using std::swap;

struct level_ancestor {
	int N;
	std::vector<int> preorder;
	std::vector<int> idx;
	std::vector<std::pair<int, int>> heavyPar; // heavy parent, distance
	level_ancestor() : N(0) {}

	level_ancestor(const std::vector<int>& par) : N(int(par.size())), preorder(N), idx(N), heavyPar(N) {
		std::vector<std::vector<int>> ch(N);
		for (int i = 0; i < N; i++) {
			if (par[i] != -1) ch[par[i]].push_back(i);
		}
		std::vector<int> sz(N);
		int nxt_idx = 0;
		for (int i = 0; i < N; i++) {
			if (par[i] == -1) {
				std::y_combinator([&](auto self, int cur) -> void {
					sz[cur] = 1;
					for (int nxt : ch[cur]) {
						self(nxt);
						sz[cur] += sz[nxt];
					}
					if (!ch[cur].empty()) {
						auto mit = max_element(ch[cur].begin(), ch[cur].end(), [&](int a, int b) { return sz[a] < sz[b]; });
						swap(*ch[cur].begin(), *mit);
					}
				})(i);
				std::y_combinator([&](auto self, int cur, int isRoot = true) -> void {
					preorder[idx[cur] = nxt_idx++] = cur;
					if (isRoot) {
						heavyPar[idx[cur]] = {par[cur] == -1 ? -1 : idx[par[cur]], 1};
					} else {
						assert(idx[par[cur]] == idx[cur]-1);
						heavyPar[idx[cur]] = heavyPar[idx[cur]-1];
						heavyPar[idx[cur]].second++;
					}
					bool chRoot = false;
					for (int nxt : ch[cur]) {
						self(nxt, chRoot);
						chRoot = true;
					}
				})(i);
			}
		}
	}

	int get_ancestor(int a, int k) const {
		assert(k >= 0);
		a = idx[a];
		while (a != -1 && k) {
			if (k >= heavyPar[a].second) {
				k -= heavyPar[a].second;
				assert(heavyPar[a].first <= a - heavyPar[a].second);
				a = heavyPar[a].first;
			} else {
				a -= k;
				k = 0;
			}
		}
		if (a == -1) return -1;
		else return preorder[a];
	}

	int lca(int a, int b) const {
		a = idx[a], b = idx[b];
		while (true) {
			if (a > b) swap(a, b);
			assert(a <= b);
			if (a > b - heavyPar[b].second) {
				return preorder[a];
			}
			b = heavyPar[b].first;
			if (b == -1) return -1;
		}
	}

	int dist(int a, int b) const {
		a = idx[a], b = idx[b];
		int res = 0;
		while (true) {
			if (a > b) swap(a, b);
			assert(a <= b);
			if (a > b - heavyPar[b].second) {
				res += b - a;
				break;
			}
			res += heavyPar[b].second;
			b = heavyPar[b].first;
			if (b == -1) return -1;
		}
		return res;
	}

	// TODO: Add more utilities, like dist to lca, last node before lca, kth node along path
};

} // namespace ecnerwala

const std::array<std::array<int, 2>, 4> MOVES = {std::array<int, 2>{-1, 0}, {0, -1}, {1, 0}, {0, 1}};

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {
		int R, C; cin >> R >> C;
		tensor<int, 2> G({R, C});
		for (int r = 0; r < R; r++) {
			for (int c = 0; c < C; c++) {
				cin >> G[{r, c}];
			}
		}

		cerr << "Case #" << case_num << ": " << R << ' ' << C << '\n';

		const int V = R * C;
		std::vector<std::vector<int>> adj(V);
		for (int i = 0; i < R; i++) {
			for (int j = 0; j < C; j++) {
				for (auto [dx, dy] : MOVES) {
					int nx = i + dx;
					int ny = j + dy;
					if (0 <= nx && nx < R && 0 <= ny && ny < C) {
						if (G[{i, j}] == G[{nx, ny}]) {
							adj[i*C+j].push_back(nx*C+ny);
						}
					}
				}
			}
		}

		std::vector<int> cc_id(V, -1);
		std::vector<int> subtree_sz;
		subtree_sz.reserve(2*V);
		subtree_sz.resize(V, -1);
		std::vector<int> par; par.reserve(2*V);
		par.resize(V, -1);
		int num_nodes = V;
		{
			std::vector<int> depth(V, -1);
			std::vector<int> lowval(V, -1);
			std::vector<int> stk; stk.reserve(V);
			for (int st = 0; st < V; st++) {
				if (depth[st] == -1) {
					depth[st] = 0;
					std::y_combinator([&](auto self, int cur, int prv) -> void {
						cc_id[cur] = st;
						lowval[cur] = depth[cur];
						stk.push_back(cur);
						subtree_sz[cur] = 1;
						for (int nxt : adj[cur]) {
							if (nxt == prv) continue;
							if (depth[nxt] == -1) {
								depth[nxt] = depth[cur]+1;
								self(nxt, cur);
								lowval[cur] = std::min(lowval[cur], lowval[nxt]);
								if (lowval[nxt] >= depth[cur]) {
									int node_id = num_nodes++;
									par.push_back(cur);
									subtree_sz.push_back(0);
									while (true) {
										int v = stk.back(); stk.pop_back();
										assert(par[v] == -1);
										par[v] = node_id;
										subtree_sz[node_id] += subtree_sz[v];
										if (v == nxt) break;
									}
									subtree_sz[cur] += subtree_sz[node_id];
								}
							} else if (depth[nxt] < depth[cur]) {
								lowval[cur] = std::min(lowval[cur], depth[nxt]);
							} else {
								assert(depth[nxt] > depth[cur]);
								// do nothing with a downedge
							}
						}
					})(st, -1);
					assert(stk.size() == 1 && stk.back() == st);
					stk.pop_back();
				}
			}
		}

		ecnerwala::level_ancestor la{par};

		int64_t ans = 0;
		for (int ox = 0; ox < R; ox++) {
			for (int oy = 0; oy < C; oy++) {
				int vo = ox * C + oy;
				for (auto d0 : MOVES) {
					int nx = ox + d0[0];
					int ny = oy + d0[1];
					if (!(0 <= nx && nx < R && 0 <= ny && ny < C)) continue;
					if (G[{ox, oy}] == G[{nx, ny}]) continue;
					std::vector<int> other_ccs; other_ccs.reserve(3);
					std::vector<std::pair<int, int>> other_adj; other_adj.reserve(3);
					for (auto d1 : MOVES) {
						int px = nx + d1[0];
						int py = ny + d1[1];
						if (!(0 <= px && px < R && 0 <= py && py < C)) continue;
						if (px == ox && py == oy) continue;
						if (G[{ox, oy}] != G[{px, py}]) continue;
						int vp = px * C + py;
						if (cc_id[vo] != cc_id[vp]) {
							other_ccs.push_back(cc_id[vp]);
						} else {
							int lca = la.lca(vo, vp);
							if (lca == vo) {
								int n = la.get_ancestor(vp, la.dist(vo, vp) - 1);
								other_adj.push_back({n, subtree_sz[n]});
							} else {
								other_adj.push_back({par[vo], subtree_sz[cc_id[vo]]-subtree_sz[vo]});
							}
						}
					}
					std::sort(other_ccs.begin(), other_ccs.end());
					other_ccs.resize(std::unique(other_ccs.begin(), other_ccs.end()) - other_ccs.begin());

					std::sort(other_adj.begin(), other_adj.end());
					other_adj.resize(std::unique(other_adj.begin(), other_adj.end()) - other_adj.begin());

					int cur_sz = 1;
					for (int v : other_ccs) {
						cur_sz += subtree_sz[v];
					}
					for (auto [v, s] : other_adj) {
						cur_sz += s;
					}

					if (cur_sz >= 3) ans += cur_sz;
				}
			}
		}

		ans *= 2;
		cout << "Case #" << case_num << ": " << ans << '\n';
	}

	return 0;
}
