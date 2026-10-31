#include <bits/stdc++.h>

namespace maroon_rk {
using namespace std;
using ll=long long;
//#define int ll

#define rng(i,a,b) for(int i=int(a);i<int(b);i++)
#define rep(i,b) rng(i,0,b)
#define gnr(i,a,b) for(int i=int(b)-1;i>=int(a);i--)
#define per(i,b) gnr(i,0,b)
#define pb push_back
#define eb emplace_back
#define a first
#define b second
#define bg begin()
#define ed end()
#define all(x) x.bg,x.ed
#ifdef LOCAL
#define dmp(x) cerr<<__LINE__<<" "<<#x<<" "<<x<<endl
#else
#define dmp(x) void(0)
#endif

template<class t,class u> void chmax(t&a,u b){if(a<b)a=b;}
template<class t,class u> void chmin(t&a,u b){if(b<a)a=b;}

template<class t> using vc=vector<t>;
template<class t> using vvc=vc<vc<t>>;

using pi=pair<int,int>;
using vi=vc<int>;

template<class t,class u>
ostream& operator<<(ostream& os,const pair<t,u>& p){
	return os<<"{"<<p.a<<","<<p.b<<"}";
}

template<class t> ostream& operator<<(ostream& os,const vc<t>& v){
	os<<"{";
	for(auto e:v)os<<e<<",";
	return os<<"}";
}

#define mp make_pair
#define mt make_tuple
#define one(x) memset(x,-1,sizeof(x))
#define zero(x) memset(x,0,sizeof(x))
#ifdef LOCAL
void dmpr(ostream&os){os<<endl;}
template<class T,class... Args>
void dmpr(ostream&os,const T&t,const Args&... args){
	os<<t<<" ";
	dmpr(os,args...);
}
#define dmp2(...) dmpr(cerr,__LINE__,##__VA_ARGS__)
#else
#define dmp2(...) void(0)
#endif

using uint=unsigned;
using ull=unsigned long long;

template<class t,size_t n>
ostream& operator<<(ostream&os,const array<t,n>&a){
	return os<<vc<t>(all(a));
}

template<int i,class T>
void print_tuple(ostream&,const T&){
}

template<int i,class T,class H,class ...Args>
void print_tuple(ostream&os,const T&t){
	if(i)os<<",";
	os<<get<i>(t);
	print_tuple<i+1,T,Args...>(os,t);
}

template<class ...Args>
ostream& operator<<(ostream&os,const tuple<Args...>&t){
	os<<"{";
	print_tuple<0,tuple<Args...>,Args...>(os,t);
	return os<<"}";
}

template<class t>
void print(t x,int suc=1){
	cout<<x;
	if(suc==1)
		cout<<"\n";
	if(suc==2)
		cout<<" ";
}

ll read(){
	ll i;
	cin>>i;
	return i;
}

vi readvi(int n,int off=0){
	vi v(n);
	rep(i,n)v[i]=read()+off;
	return v;
}

template<class T>
void print(const vector<T>&v,int suc=1){
	rep(i,v.size())
		print(v[i],i==int(v.size())-1?suc:2);
}

string readString(){
	string s;
	cin>>s;
	return s;
}

template<class T>
T sq(const T& t){
	return t*t;
}

//#define CAPITAL
void yes(bool ex=true){
	#ifdef CAPITAL
	cout<<"YES"<<"\n";
	#else
	cout<<"Yes"<<"\n";
	#endif
	if(ex)exit(0);
}
void no(bool ex=true){
	#ifdef CAPITAL
	cout<<"NO"<<"\n";
	#else
	cout<<"No"<<"\n";
	#endif
	if(ex)exit(0);
}
void possible(bool ex=true){
	#ifdef CAPITAL
	cout<<"POSSIBLE"<<"\n";
	#else
	cout<<"Possible"<<"\n";
	#endif
	if(ex)exit(0);
}
void impossible(bool ex=true){
	#ifdef CAPITAL
	cout<<"IMPOSSIBLE"<<"\n";
	#else
	cout<<"Impossible"<<"\n";
	#endif
	if(ex)exit(0);
}

constexpr ll ten(int n){
	return n==0?1:ten(n-1)*10;
}

const ll infLL=LLONG_MAX/3;

#ifdef int
const int inf=infLL;
#else
const int inf=INT_MAX/2-100;
#endif

int topbit(signed t){
	return t==0?-1:31-__builtin_clz(t);
}
int topbit(ll t){
	return t==0?-1:63-__builtin_clzll(t);
}
int botbit(signed a){
	return a==0?32:__builtin_ctz(a);
}
int botbit(ll a){
	return a==0?64:__builtin_ctzll(a);
}
int popcount(signed t){
	return __builtin_popcount(t);
}
int popcount(ll t){
	return __builtin_popcountll(t);
}
bool ispow2(int i){
	return i&&(i&-i)==i;
}
int mask(int i){
	return (int(1)<<i)-1;
}

bool inc(int a,int b,int c){
	return a<=b&&b<=c;
}

template<class t> void mkuni(vc<t>&v){
	sort(all(v));
	v.erase(unique(all(v)),v.ed);
}

ll rand_int(ll l, ll r) { //[l, r]
	#ifdef LOCAL
	static mt19937_64 gen;
	#else
    static random_device rd;
    static mt19937_64 gen(rd());
    #endif
    return uniform_int_distribution<ll>(l, r)(gen);
}

template<class t>
int lwb(const vc<t>&v,const t&a){
	return lower_bound(all(v),a)-v.bg;
}

//sz: size of the matching
//mt[i]: matching mate of the vertex i
//NEERC2018B
//Petrozavodsk 2019w Day1 A
struct maxmatching{
	struct E{int a,b;};
	const vvc<int>&g;
	int n,sz;
	vi p,t,mt;
	vc<E> f;
	int non(int v){
		return t[v]!=sz||p[v]==-1?v:(p[v]=non(p[v]));
	}
	void R(int a,int b){
		int d=mt[a];
		mt[a]=b;
		if(d==-1||mt[d]!=a)return;
		if(f[a].b==-1){
			mt[d]=f[a].a;
			R(f[a].a,d);
		}else{
			R(f[a].a,f[a].b);
			R(f[a].b,f[a].a);
		}
	}
	bool arg(int rt){
		queue<int> q;
		t[rt]=sz;
		p[rt]=-1;
		f[rt]=E{-1,-1};
		q.push(rt);
		while(!q.empty()){
			int a=q.front();q.pop();
			for(auto b:g[a]){
				if(b==rt)continue;
				if(mt[b]==-1){
					mt[b]=a;
					R(a,b);
					return true;
				}
				if(t[b]==sz){
					int x=non(a),y=non(b);
					if(x==y)continue;
					int z=rt;
					while(x!=rt||y!=rt){
						if(y!=rt)swap(x,y);
						if(f[x].a==a&&f[x].b==b){
							z=x;
							break;
						}
						f[x]=E{a,b};
						x=non(f[mt[x]].a);
					}
					for(auto v:{non(a),non(b)}){
						while(v!=z){
							t[v]=sz;
							p[v]=z;
							q.push(v);
							v=non(f[mt[v]].a);
						}
					}
					continue;
				}
				if(t[mt[b]]==sz)continue;
				f[b]=E{-1,-1};
				t[mt[b]]=sz;
				p[mt[b]]=b;
				f[mt[b]]=E{a,-1};
				q.push(mt[b]);
			}
		}
		return false;
	}
	maxmatching(const vvc<int>&gg):g(gg),n(g.size()),sz(0),
		p(n),t(n,-1),mt(n,-1),f(n){
		rep(i,n)if(mt[i]==-1)
			sz+=arg(i);
	}
};


} // namespace maroon_rk

namespace hos {
template <class T> bool chmax(T &t, const T &f) { if (t < f) { t = f; return true; } return false; }

unsigned xrand() {
  static unsigned x = 314159265, y = 358979323, z = 846264338, w = 327950288;
  unsigned t = x ^ x << 11; x = y; y = z; z = w; return w = w ^ w >> 19 ^ t ^ t >> 8;
}

template<class T> struct UsoMatching {
  static constexpr int MAX_N = 510;
  static constexpr int NUM_ITERS = 2;
  int n;
  T adj[MAX_N][MAX_N];
  int mate[MAX_N];

  UsoMatching(int n_) : n(n_) {
    if (n & 1) ++n;
    for (int u = 0; u < n; ++u) std::fill(adj[u], adj[u] + n, 0);
  }
  void addEdge(int u, int v, T c) {
    chmax(adj[u][v], c);
    chmax(adj[v][u], c);
  }

  int perm[MAX_N];
  int usLen;
  int us[MAX_N];
  bool on[MAX_N];
  T ds[MAX_N];

  bool dfs(int u) {
    us[usLen++] = u;
    if (on[u]) return true;
    on[u] = true;
    for (int j = 0; j < n; ++j) {
      const int v = perm[j];
      if (v != u && v != mate[u] && !on[v]) {
        const int w = mate[v];
        const T d = ds[u] + adj[u][v] - adj[v][w];
        if (ds[w] < d) {
          ds[w] = d;
          if (dfs(w)) return true;
        }
      }
    }
    --usLen;
    on[u] = false;
    return false;
  }
  T run() {
    for (int u = 0; u < n; ++u) mate[u] = u ^ 1, perm[u] = u;
    for (int iter = 0; iter < NUM_ITERS; ++iter) {
     loop:
      for (int u = 1; u < n; ++u) std::swap(perm[xrand() % (u + 1)], perm[u]);
      usLen = 0;
      std::fill(on, on + n, false);
      std::fill(ds, ds + n, 0);
      for (int j = 0; j < n; ++j) {
        if (dfs(perm[j])) {
          for (int k = usLen - 1; k--; ) if (us[k] == us[usLen - 1]) {
            const int t = mate[us[k]];
            for (int l = k; l < usLen - 2; ++l) mate[us[l]] = mate[us[l + 1]];
            mate[us[usLen - 2]] = t;
            for (int l = k; l < usLen - 1; ++l) mate[mate[us[l]]] = us[l];
            goto loop;
          }
        }
      }
    }
    T ret = 0;
    for (int u = 0; u < n; ++u) if (u < mate[u]) ret += adj[u][mate[u]];
    return ret;
  }
};

} // namespace hos

namespace kactl {
using namespace std;

#define rep(i, a, b) for(int i = a; i < (b); ++i)
#define all(x) begin(x), end(x)
#define sz(x) (int)(x).size()
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;

const ll mod = 998244353; // faster if const

ll modpow(ll b, ll e) {
	ll ans = 1;
	for (; e; b = b * b % mod, e /= 2)
		if (e & 1) ans = ans * b % mod;
	return ans;
}
int matInv(vector<vector<ll>>& A) {
	int n = sz(A); vi col(n);
	vector<vector<ll>> tmp(n, vector<ll>(n));
	rep(i,0,n) tmp[i][i] = 1, col[i] = i;

	rep(i,0,n) {
		int r = i, c = i;
		rep(j,i,n) rep(k,i,n) if (A[j][k]) {
			r = j; c = k; goto found;
		}
		return i;
found:
		A[i].swap(A[r]); tmp[i].swap(tmp[r]);
		rep(j,0,n) swap(A[j][i], A[j][c]), swap(tmp[j][i], tmp[j][c]);
		swap(col[i], col[c]);
		ll v = modpow(A[i][i], mod - 2);
		rep(j,i+1,n) {
			ll f = A[j][i] * v % mod;
			A[j][i] = 0;
			rep(k,i+1,n) A[j][k] = (A[j][k] - f*A[i][k]) % mod;
			rep(k,0,n) tmp[j][k] = (tmp[j][k] - f*tmp[i][k]) % mod;
		}
		rep(j,i+1,n) A[i][j] = A[i][j] * v % mod;
		rep(j,0,n) tmp[i][j] = tmp[i][j] * v % mod;
		A[i][i] = 1;
	}

	for (int i = n-1; i > 0; --i) rep(j,0,i) {
		ll v = A[j][i];
		rep(k,0,n) tmp[j][k] = (tmp[j][k] - v*tmp[i][k]) % mod;
	}

	rep(i,0,n) rep(j,0,n)
		A[col[i]][col[j]] = tmp[i][j] % mod + (tmp[i][j] < 0 ? mod : 0);
	return n;
}
mt19937 mt(48);
vector<pii> generalMatching(int N, vector<pii> const& ed) {
	vector<vector<ll>> mat(N, vector<ll>(N)), A;
	for (pii pa : ed) {
		int a = pa.first, b = pa.second, r = mt() % mod;
		mat[a][b] = r, mat[b][a] = (mod - r) % mod;
	}

	int r = matInv(A = mat), M = 2*N - r, fi, fj;
	assert(r % 2 == 0);

	if (M != N) do {
		mat.resize(M, vector<ll>(M));
		rep(i,0,N) {
			mat[i].resize(M);
			rep(j,N,M) {
				int r = mt() % mod;
				mat[i][j] = r, mat[j][i] = (mod - r) % mod;
			}
		}
	} while (matInv(A = mat) != M);

	vi has(M, 1); vector<pii> ret;
	rep(it,0,M/2) {
		rep(i,0,M) if (has[i])
			rep(j,i+1,M) if (A[i][j] && mat[i][j]) {
				fi = i; fj = j; goto done;
		} assert(0); done:
		if (fj < N) ret.emplace_back(fi, fj);
		has[fi] = has[fj] = 0;
		rep(sw,0,2) {
			ll a = modpow(A[fi][fj], mod-2);
			rep(i,0,M) if (has[i] && A[i][fj]) {
				ll b = A[i][fj] * a % mod;
				rep(j,0,M) A[i][j] = (A[i][j] - A[fi][j] * b) % mod;
			}
			swap(fi,fj);
		}
	}
	return ret;
}
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
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int T; cin >> T;
	while (T--) {
		int V, E; cin >> V >> E;
		vector<pair<int, int>> edges(E);
		for (auto& it : edges) {
			cin >> it.first >> it.second; it.first--, it.second--;
		}
		/*
		hos::UsoMatching<int> um(V);
		for (auto it : edges) {
			um.addEdge(it.first, it.second, 1);
		}
		int M = um.run();
		vector<pair<int, int>> matching; matching.reserve(M);
		for (int u = 0; u < V; u++) {
			if (u < um.mate[u] && um.adj[u][um.mate[u]] > 0) {
				matching.emplace_back(u, um.mate[u]);
			}
		}
		*/
		maroon_rk::vvc<int> g(V);
		for (auto it : edges) {
			g[it.first].push_back(it.second);
			g[it.second].push_back(it.first);
		}
		maroon_rk::maxmatching mm(g);
		int M = mm.sz;
		vector<pair<int, int>> matching; matching.reserve(M);
		for (int u = 0; u < V; u++) {
			if (u < mm.mt[u]) {
				matching.emplace_back(u, mm.mt[u]);
			}
		}
		//auto matching = kactl::generalMatching(V, edges);
		vector<int> vertex_id(V, M);
		for (int a = 0; a < M; a++) {
			vertex_id[matching[a].first] = a;
			vertex_id[matching[a].second] = ~a;
		}
		{ // exactly M
			kactl::TwoSat ts(M);
			for (auto it : edges) {
				assert(!(vertex_id[it.first] == M && vertex_id[it.second] == M));
				if (vertex_id[it.first] == M) {
					ts.setValue(vertex_id[it.second]);
				} else if (vertex_id[it.second] == M) {
					ts.setValue(vertex_id[it.first]);
				} else {
					ts.either(vertex_id[it.first], vertex_id[it.second]);
				}
			}
			if (ts.solve()) {
				vector<int> res; res.reserve(M);
				for (int a = 0; a < M; a++) {
					if (ts.values[a]) res.push_back(matching[a].first);
					else res.push_back(matching[a].second);
				}

				sort(res.begin(), res.end());
				cout << res.size() << '\n';
				for (int i = 0; i < int(res.size()); i++) {
					cout << res[i]+1 << " \n"[i+1==int(res.size())];
				}
				if (res.empty()) cout << '\n';
				goto found;
			}
		}
		{ // with a random vertex
			kactl::TwoSat ts(V);
			for (auto it : edges) {
				ts.either(it.first, it.second);
			}
			vector<int> unmatched; unmatched.reserve(V - 2 * M);
			for (int u = 0; u < V; u++) {
				if (mm.mt[u] == -1) {
					unmatched.push_back(u);
				} else if (u < mm.mt[u]) {
					ts.either(~u, ~mm.mt[u]);
				}
			}
			ts.atMostOne(unmatched);
			if (ts.solve()) {
				vector<int> res; res.reserve(M+1);
				for (int u = 0; u < V; u++) if (ts.values[u]) res.push_back(u);

				sort(res.begin(), res.end());
				cout << res.size() << '\n';
				for (int i = 0; i < int(res.size()); i++) {
					cout << res[i]+1 << " \n"[i+1==int(res.size())];
				}
				if (res.empty()) cout << '\n';
				goto found;
			}
		}
		for (int v = 0; v < V; v++) {
			if (!(v < mm.mt[v])) continue;
			kactl::TwoSat ts(M);
			for (auto it : edges) {
				if (v == it.first || v == it.second) continue;
				assert(!(vertex_id[it.first] == M && vertex_id[it.second] == M));
				if (vertex_id[it.first] == M) {
					ts.setValue(vertex_id[it.second]);
				} else if (vertex_id[it.second] == M) {
					ts.setValue(vertex_id[it.first]);
				} else {
					ts.either(vertex_id[it.first], vertex_id[it.second]);
				}
			}
			if (ts.solve()) {
				vector<int> res; res.reserve(M + (v >= 0));
				if (v >= 0) res.push_back(v);
				for (int a = 0; a < M; a++) {
					if (ts.values[a]) res.push_back(matching[a].first);
					else res.push_back(matching[a].second);
				}

				sort(res.begin(), res.end());
				cout << res.size() << '\n';
				for (int i = 0; i < int(res.size()); i++) {
					cout << res[i]+1 << " \n"[i+1==int(res.size())];
				}
				if (res.empty()) cout << '\n';
				goto found;
			}
		}
		cout << "not smol" << '\n';
found:;
	}

	return 0;
}
