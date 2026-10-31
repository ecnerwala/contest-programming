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

namespace kactl {

using namespace std;

#define rep(i, a, b) for(int i = a; i < (b); ++i)
#define all(x) begin(x), end(x)
#define sz(x) (int)(x).size()
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;

struct PushRelabel {
	struct Edge {
		int dest, back;
		ll f, c;
	};
	vector<vector<Edge>> g;
	vector<ll> ec;
	vector<Edge*> cur;
	vector<vi> hs; vi H;
	PushRelabel(int n) : g(n), ec(n), cur(n), hs(2*n), H(n) {}

	void addEdge(int s, int t, ll cap, ll rcap=0) {
		if (s == t) return;
		g[s].push_back({t, sz(g[t]), 0, cap});
		g[t].push_back({s, sz(g[s])-1, 0, rcap});
	}

	void addFlow(Edge& e, ll f) {
		Edge &back = g[e.dest][e.back];
		if (!ec[e.dest] && f) hs[H[e.dest]].push_back(e.dest);
		e.f += f; e.c -= f; ec[e.dest] += f;
		back.f -= f; back.c += f; ec[back.dest] -= f;
	}
	ll calc(int s, int t) {
		int v = sz(g); H[s] = v; ec[t] = 1;
		vi co(2*v); co[0] = v-1;
		rep(i,0,v) cur[i] = g[i].data();
		for (Edge& e : g[s]) addFlow(e, e.c);

		for (int hi = 0;;) {
			while (hs[hi].empty()) if (!hi--) return -ec[s];
			int u = hs[hi].back(); hs[hi].pop_back();
			while (ec[u] > 0)  // discharge u
				if (cur[u] == g[u].data() + sz(g[u])) {
					H[u] = 1e9;
					for (Edge& e : g[u]) if (e.c && H[u] > H[e.dest]+1)
						H[u] = H[e.dest]+1, cur[u] = &e;
					if (++co[H[u]], !--co[hi] && hi < v)
						rep(i,0,v) if (hi < H[i] && H[i] < v)
							--co[H[i]], H[i] = v + 1;
					hi = H[u];
				} else if (cur[u]->c && H[u] == H[cur[u]->dest]+1)
					addFlow(*cur[u], min(ec[u], cur[u]->c));
				else ++cur[u];
		}
	}
	bool leftOfMinCut(int a) { return H[a] >= sz(g); }
};

struct Edge {
	int u, v; ll w;
};
vector<Edge> gomoryHu(int N, vector<Edge> ed) {
	vector<Edge> tree;
	vi par(N);
	rep(i,1,N) {
		PushRelabel D(N); // Dinic also works
		for (Edge t : ed) D.addEdge(t.u, t.v, t.w, t.w);
		tree.push_back({i, par[i], D.calc(i, par[i])});
		rep(j,i+1,N)
			if (par[j] == par[i] && D.leftOfMinCut(j)) par[j] = i;
	}
	return tree;
}

}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, M; cin >> N >> M;
		std::vector<kactl::Edge> edges(M);
		for (auto& e : edges) { cin >> e.u >> e.v >> e.w; e.u--, e.v--; }
		{
			auto gh = kactl::gomoryHu(N, edges);
			int64_t g = 0;
			for (auto e : gh) {
				g = std::gcd(g, e.w);
			}
			if (g != 1) {
				cout << 1 << '\n';
				cout << N << '\n';
				for (int i = 0; i < N; i++) cout << i+1 << " \n"[i+1==N];
			} else {
				tensor<bool, 2> sgn({N, N}, false);
				for (auto e : edges) { if (e.w & 1) { sgn[{e.u, e.v}] ^= 1, sgn[{e.v, e.u}] ^= 1; } }
				std::vector<bool> alive(N, true);
				std::vector<int> stk; stk.reserve(N);
				while (true) {
					int v;
					for (v = 0; v < N; v++) {
						if (!alive[v]) continue;
						int deg = 0;
						for (int u = 0; u < N; u++) {
							if (!alive[u]) continue;
							deg ^= sgn[{v, u}];
						}
						if (deg) break;
					}
					if (v == N) break;
					alive[v] = false;
					stk.push_back(v);
					for (int u = 0; u < N; u++) {
						if (!alive[u]) continue;
						if (!sgn[{v, u}]) continue;
						for (int w = 0; w < N; w++) {
							if (!alive[w]) continue;
							if (!sgn[{v, w}]) continue;
							if (u == w) continue;
							sgn[{u, w}] ^= 1;
						}
					}
				}
				std::vector<bool> side(N, false);
				while (!stk.empty()) {
					int v = stk.back(); stk.pop_back();
					assert(!alive[v]);
					for (int u = 0; u < N; u++) {
						if (!alive[u]) continue;
						if (!sgn[{v, u}]) continue;
						for (int w = 0; w < N; w++) {
							if (!alive[w]) continue;
							if (!sgn[{v, w}]) continue;
							if (u == w) continue;
							sgn[{u, w}] ^= 1;
						}
					}
					alive[v] = true;
					std::array<int, 2> cnt{0, 0};
					for (int u = 0; u < N; u++) {
						if (!alive[u]) continue;
						if (!sgn[{v, u}]) continue;
						cnt[side[u]] ^= 1;
					}
					side[v] = cnt[1] == 0 ? 1 : 0;
				}
				std::array<std::vector<int>, 2> res;
				res[0].reserve(N);
				res[1].reserve(N);
				for (int i = 0; i < N; i++) {
					res[side[i]].push_back(i);
				}
				cout << 2 << '\n';
				for (const auto& r : res) {
					cout << r.size() << '\n';
					for (int z = 0; z < int(r.size()); z++) cout << r[z]+1 << " \n"[z+1==int(r.size())];
				}
			}
		}
	}

	return 0;
}
