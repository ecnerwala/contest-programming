#include <bits/stdc++.h>
using namespace std;


#define rep(i, a, b) for(int i = a; i < (b); ++i)
#define trav(a, x) for(auto& a : x)
#define all(x) x.begin(), x.end()
#define sz(x) (int)(x).size()
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;


namespace atcoder {

namespace internal {

// @param n `0 <= n`
// @return minimum non-negative `x` s.t. `n <= 2**x`
int ceil_pow2(int n) {
    int x = 0;
    while ((1U << x) < (unsigned int)(n)) x++;
    return x;
}

// @param n `1 <= n`
// @return minimum non-negative `x` s.t. `(n & (1 << x)) != 0`
constexpr int bsf_constexpr(unsigned int n) {
    int x = 0;
    while (!(n & (1 << x))) x++;
    return x;
}

// @param n `1 <= n`
// @return minimum non-negative `x` s.t. `(n & (1 << x)) != 0`
int bsf(unsigned int n) {
#ifdef _MSC_VER
    unsigned long index;
    _BitScanForward(&index, n);
    return index;
#else
    return __builtin_ctz(n);
#endif
}

}  // namespace internal

}  // namespace atcoder
struct TwoSat {
	int N;
	vector<vi> gr;
	vi values; // 0 = false, 1 = true

	TwoSat(int n = 0) : N(n), gr(2*n) {}

	int addVar() { // (optional)
		gr.emplace_back();
		gr.emplace_back();
		return N++;
	}

	void either(int f, int j) {
		f = max(2*f, -1-2*f);
		j = max(2*j, -1-2*j);
		gr[f].push_back(j^1);
		gr[j].push_back(f^1);
	}
	void setValue(int x) { either(x, x); }

	void atMostOne(const vi& li) { // (optional)
		if (sz(li) <= 1) return;
		int cur = ~li[0];
		rep(i,2,sz(li)) {
			int next = addVar();
			either(cur, ~li[i]);
			either(cur, next);
			either(~li[i], next);
			cur = ~next;
		}
		either(cur, ~li[1]);
	}

	vi val, comp, z; int time = 0;
	int dfs(int i) {
		int low = val[i] = ++time, x; z.push_back(i);
		for(int e : gr[i]) if (!comp[e])
			low = min(low, val[e] ?: dfs(e));
		if (low == val[i]) do {
			x = z.back(); z.pop_back();
			comp[x] = low;
			if (values[x>>1] == -1)
				values[x>>1] = x&1;
		} while (x != i);
		return val[i] = low;
	}

	bool solve() {
		values.assign(N, -1);
		val.assign(2*N, 0); comp = val;
		rep(i,0,2*N) if (!comp[i]) dfs(i);
		rep(i,0,N) if (comp[2*i] == comp[2*i+1]) return 0;
		return 1;
	}
};

namespace atcoder {

template <class S, S (*op)(S, S), S (*e)()> struct segtree {
  public:
    segtree() : segtree(0) {}
    explicit segtree(int n) : segtree(std::vector<S>(n, e())) {}
    explicit segtree(const std::vector<S>& v) : _n(int(v.size())) {
        log = internal::ceil_pow2(_n);
        size = 1 << log;
        d = std::vector<S>(2 * size, e());
        for (int i = 0; i < _n; i++) d[size + i] = v[i];
        for (int i = size - 1; i >= 1; i--) {
            update(i);
        }
    }

    void set(int p, S x) {
        assert(0 <= p && p < _n);
        p += size;
        d[p] = x;
        for (int i = 1; i <= log; i++) update(p >> i);
    }

    S get(int p) const {
        assert(0 <= p && p < _n);
        return d[p + size];
    }

    S prod(int l, int r) const {
        assert(0 <= l && l <= r && r <= _n);
        S sml = e(), smr = e();
        l += size;
        r += size;

        while (l < r) {
            if (l & 1) sml = op(sml, d[l++]);
            if (r & 1) smr = op(d[--r], smr);
            l >>= 1;
            r >>= 1;
        }
        return op(sml, smr);
    }

    S all_prod() const { return d[1]; }

    template <bool (*f)(S)> int max_right(int l) const {
        return max_right(l, [](S x) { return f(x); });
    }
    template <class F> int max_right(int l, F f) const {
        assert(0 <= l && l <= _n);
        assert(f(e()));
        if (l == _n) return _n;
        l += size;
        S sm = e();
        do {
            while (l % 2 == 0) l >>= 1;
            if (!f(op(sm, d[l]))) {
                while (l < size) {
                    l = (2 * l);
                    if (f(op(sm, d[l]))) {
                        sm = op(sm, d[l]);
                        l++;
                    }
                }
                return l - size;
            }
            sm = op(sm, d[l]);
            l++;
        } while ((l & -l) != l);
        return _n;
    }

    template <bool (*f)(S)> int min_left(int r) const {
        return min_left(r, [](S x) { return f(x); });
    }
    template <class F> int min_left(int r, F f) const {
        assert(0 <= r && r <= _n);
        assert(f(e()));
        if (r == 0) return 0;
        r += size;
        S sm = e();
        do {
            r--;
            while (r > 1 && (r % 2)) r >>= 1;
            if (!f(op(d[r], sm))) {
                while (r < size) {
                    r = (2 * r + 1);
                    if (f(op(d[r], sm))) {
                        sm = op(d[r], sm);
                        r--;
                    }
                }
                return r + 1 - size;
            }
            sm = op(d[r], sm);
        } while ((r & -r) != r);
        return 0;
    }

  private:
    int _n, size, log;
    std::vector<S> d;

    void update(int k) { d[k] = op(d[2 * k], d[2 * k + 1]); }
};

}  // namespace atcoder

// vector<pair<int,int> > equations;

TwoSat ts;

int Z = 0;

map<pair<int,int>, int> cache;

int ts_false;

int op(int a, int b){
	if(a == ts_false) return b;
	if(b == ts_false) return a;
	int v = ts.addVar();
	ts.either(~a, v);
	ts.either(~b, v);
	ts.either(~a, ~b);
	return v;
}

int e(){
	return ts_false;
}

int TEST = 0;

mt19937 mt(48);
void solve(){
	int N, M;
	if(TEST){
		N = 100000;
		M = 100000;
	} else {
		cin >> N;
		cin >> M;
	}

	ts = TwoSat();


	vector<int> z;
	vector<pair<int,int> > A;
	for(int i = 0; i < N; i++){
		int S, E;
		if(TEST){
			S = mt() % 1000;
			E = mt() % 1000;
			if(S > E) swap(S, E);
		} else {
			cin >> S >> E;
		}
		E++;
		A.push_back({S, E});
		z.push_back(S);
		z.push_back(E);
	}
	sort(z.begin(), z.end());
	z.resize(unique(z.begin(), z.end()) - z.begin());
	for(auto& [x, y] : A){
		x = lower_bound(z.begin(), z.end(), x) - z.begin();
		y = lower_bound(z.begin(), z.end(), y) - z.begin();
	}

	int T = (int)z.size();
	vector<vector<int> > del(T);
	vector<vector<int> > ins(T);
	for(int i = 0; i < N; i++){
		auto [x, y] = A[i];
		assert(x < T && x < y && y < T);
		ins[x].push_back(i);
		del[y].push_back(i);
	}
	for(int i = 0; i < N; i++){
		int z = ts.addVar();
		assert(z == i);
	}
	{
		ts_false = ts.addVar();
		ts.either(~ts_false, ~ts_false);
	}
	atcoder::segtree<int, op, e> seg(vector<int>(N, ts_false));
	for(int t = 0; t < T; t++){
		for(int x : del[t]){
			seg.set(x, ts_false);
		}
		for(int x : ins[t]){
			seg.set(x, x);
		}
	}
	for(int j = 0; j < M; j++){
		int v1, v2;
		if(TEST){
			int a = mt() % N;
			int b = mt() % N;
			if(a == b) b = (b+1) % N;
			if(a > b) swap(a, b);
			v1 = a;
			v2 = b;
		} else {
			cin >> v1 >> v2;
			v1--; v2--;
		}
		ts.either(v1, v2);
	}
	bool works = ts.solve();
	cout << (works ? "YES" : "NO") << '\n';
}

int main(){
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int T;
	cin >> T;
	if(T == 0){
		TEST = 1;
		solve();
	} else {
		while(T--) solve();
	}
}
