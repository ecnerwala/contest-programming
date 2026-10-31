#include <bits/stdc++.h>
using namespace std;

template <int MOD_> struct modnum {
	static constexpr int MOD = MOD_;
	static_assert(MOD_ > 0, "MOD must be positive");

private:
	using ll = long long;

	int v;

	static int minv(int a, int m) {
		a %= m;
		assert(a);
		return a == 1 ? 1 : int(m - ll(minv(m, a)) * ll(m) / a);
	}

public:

	modnum() : v(0) {}
	modnum(ll v_) : v(int(v_ % MOD)) { if (v < 0) v += MOD; }
	explicit operator int() const { return v; }
	friend std::ostream& operator << (std::ostream& out, const modnum& n) { return out << int(n); }
	friend std::istream& operator >> (std::istream& in, modnum& n) { ll v_; in >> v_; n = modnum(v_); return in; }

	friend bool operator == (const modnum& a, const modnum& b) { return a.v == b.v; }
	friend bool operator != (const modnum& a, const modnum& b) { return a.v != b.v; }

	modnum inv() const {
		modnum res;
		res.v = minv(v, MOD);
		return res;
	}
	modnum neg() const {
		modnum res;
		res.v = v ? MOD-v : 0;
		return res;
	}

	modnum operator- () const {
		return neg();
	}
	modnum operator+ () const {
		return modnum(*this);
	}

	modnum& operator ++ () {
		v ++;
		if (v == MOD) v = 0;
		return *this;
	}
	modnum& operator -- () {
		if (v == 0) v = MOD;
		v --;
		return *this;
	}
	modnum& operator += (const modnum& o) {
		v += o.v;
		if (v >= MOD) v -= MOD;
		return *this;
	}
	modnum& operator -= (const modnum& o) {
		v -= o.v;
		if (v < 0) v += MOD;
		return *this;
	}
	modnum& operator *= (const modnum& o) {
		v = int(ll(v) * ll(o.v) % MOD);
		return *this;
	}
	modnum& operator /= (const modnum& o) {
		return *this *= o.inv();
	}

	friend modnum operator ++ (modnum& a, int) { modnum r = a; ++a; return r; }
	friend modnum operator -- (modnum& a, int) { modnum r = a; --a; return r; }
	friend modnum operator + (const modnum& a, const modnum& b) { return modnum(a) += b; }
	friend modnum operator - (const modnum& a, const modnum& b) { return modnum(a) -= b; }
	friend modnum operator * (const modnum& a, const modnum& b) { return modnum(a) *= b; }
	friend modnum operator / (const modnum& a, const modnum& b) { return modnum(a) /= b; }
};

template <typename T> T pow(T a, long long b) {
	assert(b >= 0);
	T r = 1; while (b) { if (b & 1) r *= a; b >>= 1; a *= a; } return r;
}

template <typename U, typename V> struct pairnum {
	U u;
	V v;

	pairnum() : u(0), v(0) {}
	pairnum(long long val) : u(val), v(val) {}
	pairnum(const U& u_, const V& v_) : u(u_), v(v_) {}

	friend std::ostream& operator << (std::ostream& out, const pairnum& n) { return out << '(' << n.u << ',' << ' ' << n.v << ')'; }
	friend std::istream& operator >> (std::istream& in, pairnum& n) { long long val; in >> val; n = pairnum(val); return in; }

	friend bool operator == (const pairnum& a, const pairnum& b) { return a.u == b.u && a.v == b.v; }
	friend bool operator != (const pairnum& a, const pairnum& b) { return a.u != b.u || a.v != b.v; }

	pairnum inv() const {
		return pairnum(u.inv(), v.inv());
	}
	pairnum neg() const {
		return pairnum(u.neg(), v.neg());
	}
	pairnum operator- () const {
		return pairnum(-u, -v);
	}
	pairnum operator+ () const {
		return pairnum(+u, +v);
	}

	pairnum& operator += (const pairnum& o) {
		u += o.u;
		v += o.v;
		return *this;
	}
	pairnum& operator -= (const pairnum& o) {
		u -= o.u;
		v -= o.v;
		return *this;
	}
	pairnum& operator *= (const pairnum& o) {
		u *= o.u;
		v *= o.v;
		return *this;
	}
	pairnum& operator /= (const pairnum& o) {
		u /= o.u;
		v /= o.v;
		return *this;
	}

	friend pairnum operator + (const pairnum& a, const pairnum& b) { return pairnum(a) += b; }
	friend pairnum operator - (const pairnum& a, const pairnum& b) { return pairnum(a) -= b; }
	friend pairnum operator * (const pairnum& a, const pairnum& b) { return pairnum(a) *= b; }
	friend pairnum operator / (const pairnum& a, const pairnum& b) { return pairnum(a) /= b; }
};

const int P = int(1e9)+7;
using num = pairnum<modnum<P>, modnum<P>>;

num BASE(28359, 984632);

using H = pair<num, num>;

template<typename T> T combine(T a, T b){
	return T{a.first + a.second * b.first, a.second * b.second};
}

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

using SH = num;

// Either globally or in a single class:

// static char buf[850 << 20];
// void* operator new(size_t s) {
// 	static size_t i = sizeof buf;
// 	assert(s < i);
// 	return (void*)&buf[i -= s];
// }
// void operator delete(void*) {}

struct node {
	node *l, *r;
	pair<SH, SH> val;
} a[4000000];
int _x = 0;

node* get_node(){
	_x++;
	return (a + _x);
}

SH S_BASE(298347, 346345);

node* build(int lx, int rx){
	node* x = get_node();
	if(lx == rx){
		x->l = x->r = nullptr;
		x->val = {SH(0), S_BASE};
	} else {
		x->l = build(lx, (lx + rx) / 2);
		x->r = build((lx + rx) / 2 + 1, rx);
		x->val = combine(x->l->val, x->r->val);
	}
	return x;
}

pair<SH, SH> query(node* v, int lx, int rx, int qlx, int qrx){
	if(rx < qlx || qrx < lx){
		return {SH(0), SH(1)};
	} else if(qlx <= lx && rx <= qrx){
		return {v->val};
	} else {
		return combine(query(v->l, lx, (lx + rx) / 2, qlx, qrx), query(v->r, (lx + rx) / 2 + 1, rx, qlx, qrx));
	}
}

node* update(node *v, int lx, int rx, int ux, SH delta){
	if(ux < lx || rx < ux){
		return v;
	} else if(lx == rx){
		node* nv = get_node();
		nv->l = nv->r = nullptr;
		nv->val = pair<SH, SH>{v->val.first + delta, v->val.second};
		return nv;
	} else {
		node* nv = get_node();
		nv->l = update(v->l, lx, (lx + rx) / 2, ux, delta);
		nv->r = update(v->r, (lx + rx) / 2 + 1, rx, ux, delta);
		nv->val = combine(nv->l->val, nv->r->val);
		return nv;
	}
}

int main(){
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int N, Q;
	cin >> N >> Q;
	vector<int> A(N);
	for(int& x : A){
		cin >> x;
	}
	vector<vector<int> > r(Q);
	for(int i = 0; i < Q; i++){
		int u, v;
		cin >> u >> v;
		u--; v--;
		r[i] = {u, v};
	}
	int INF = int(1e8);
	vector<int> ans(Q, INF);
	auto flip = [&](){
		reverse(A.begin(), A.end());
		for(auto& v : r){
			v[0] = N-1-v[0];
			v[1] = N-1-v[1];
		}
	};

	mt19937 mt(48);
	vector<SH> random_function(2 * N + 1);
	for(int i = 0; i <= 2 * N; i++){
		random_function[i] = SH(mt(), mt());
		// random_function[i] = SH(i, i);
	}

	for(int it = 0; it < 2; it++){
		flip();
		vector<int> type(N);
		for(int i = 0; i < N; i++){
			if(A[i] == 2){
				type[i] = 0;
			} else {
				type[i] = A[(i+N-1) % N];
			}
		}
		vector<int> nxt(N);
		for(int i = 0; i < N; i++){
			nxt[i] = (i + A[i]) % N;
		}
		const int B = 20;
		vector<vector<int> > jump(B, vector<int>(N));
		vector<vector<H> > jump_hash(B, vector<H>(N));
		jump[0] = nxt;
		for(int i = 0; i < N; i++){
			jump_hash[0][i] = H{num(type[i]), BASE};
		}
		for(int j = 1; j < B; j++){
			for(int i = 0; i < N; i++){
				jump[j][i] = jump[j-1][jump[j-1][i]];
				jump_hash[j][i] = combine(jump_hash[j-1][i], jump_hash[j-1][jump[j-1][i]]);
			}
		}
		for(int q = 0; q < Q; q++){
			int x = r[q][0];
			int y = r[q][1];
			if(jump_hash[B-1][x] == jump_hash[B-1][y]) continue;
			int len = 0;
			for(int b = B-1; b >= 0; b--){
				if(jump_hash[b][x] == jump_hash[b][y]){
					x = jump[b][x];
					y = jump[b][y];
					len += 1 << b;
					assert(jump_hash[b][x] != jump_hash[b][y]);
				}
			}
			assert(type[x] != type[y]);
			ans[q] = min(ans[q], len + (A[x] == A[y]));
		}
		if(A == vector<int>(N, 2)) continue;

		// only one functional graph cycle
		vector<int> jumps_till_1(N, INF);
		for(int g = 0; g < 3; g++) for(int i = 0; i < N; i++){
			if(A[i] == 1){
				jumps_till_1[i] = 0;
			} else {
				int prv = (i + N-2) % N;
				jumps_till_1[i] = min(jumps_till_1[i], jumps_till_1[prv] + 1);
			}
		}
		vector<int> extra(N, N);
		for(int i = 0; i < N; i++){
			int prv = (i + N-1) % N;
			if(A[i] == 2){
				extra[i] = 0;
			} else if(A[i] == 1 && jumps_till_1[prv] < INF){
				extra[i] = min(jumps_till_1[prv] + 1, N);
			}
		}
		// extra in [0, N]
		int root = jump[B-1][0];

		auto jump_k = [&](int v, int k) -> int {
			for(int j = B-1; j >= 0; j--){
				if(v & (1 << j)){
					v ^= (1 << j);
					k = jump[k][v];
				}
			}
			return v;
		};
		vector<vector<int> > ch(N);
		for(int i = 0; i < N; i++){
			if(i != root){
				ch[nxt[i]].push_back(i);
			}
		}
		vector<int> depth(N);
		y_combinator([&](auto self, int v, int d) -> void {
			depth[v] = d;
			for(int w : ch[v]){
				self(w, d+1);
			}
		})(root, 0);

		vector<int> key(N);
		for(int i = 0; i < N; i++){
			key[i] = extra[i] + N - depth[i]; // in [0, 2N]
		}

		int L = 0;
		int R = 2 * N;
		node* tree = build(L, R);
		vector<node*> pseg(N);
		y_combinator([&](auto self, int v) -> void {
			// add (v, extra)
			tree = update(tree, L, R, key[v], random_function[extra[v]]);
			// cerr << "add " << v << ' ' << key[v] << ' ' << extra[v] << '\n';
			pseg[v] = tree;
			// for(int i = L; i <= R; i++){
			// 	cerr << query(tree, L, R, i, i).first << ' ';
			// }
			// cerr << '\n';
			for(int w : ch[v]){
				self(w);
			}
			// subtract (extra)
			tree = update(tree, L, R, key[v], -random_function[extra[v]]);
			// cerr << "subtr " << v << ' ' << key[v] << ' ' << extra[v] << '\n';
		})(root);
		auto get_hash = [&](int v, int st, int len){
			assert(st + len - 1 <= R);
			return query(pseg[v], L, R, st, st + len - 1);
		};
		auto get_ans = [&](int x, int y) -> int {
			// tree only
			int stx = N - depth[x];
			int sty = N - depth[y];
			// check first mismatch from A for x, substring from B for y
			int len = R - max(stx, sty) + 1;
			int s = 0; // same
			int e = len; // diff
			while(s + 1 < e){
				int m = (s + e) / 2;
				if(get_hash(x, stx, m) == get_hash(y, sty, m)){
					s = m;
				} else {
					e = m;
				}
			}
			if(e < len){
				return e;
			} else {
				return N;
			}
		};
		for(int q = 0; q < Q; q++){
			int x = r[q][0];
			int y = r[q][1];
			ans[q] = min(ans[q], get_ans(x, y));
			ans[q] = min(ans[q], depth[x] + 1 + get_ans(jump_k(x, depth[x] + 1), jump_k(y, depth[x] + 1)));
			ans[q] = min(ans[q], depth[y] + 1 + get_ans(jump_k(x, depth[y] + 1), jump_k(y, depth[y] + 1)));
		}
	}
	for(int q = 0; q < Q; q++){
		if(ans[q] >= N) ans[q] = -1;
		cout << ans[q] << '\n';
	}
}
