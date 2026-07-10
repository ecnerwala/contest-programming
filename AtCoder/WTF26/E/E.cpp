#include <bits/stdc++.h>
#include <cassert>

namespace maroon_rk {
#ifndef LOCAL
#pragma GCC optimize ("O3")
#pragma GCC optimize ("unroll-loops")
#endif

using namespace std;

using ll=long long;
#define int ll

bool dbg=false;

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
#define si(x) int(x.size())
#ifdef LOCAL
#define dmp(x) cerr<<__LINE__<<" "<<#x<<" "<<x<<endl
#else
#define dmp(x) void(0)
#endif

template<class t,class u> bool chmax(t&a,u b){if(a<b){a=b;return true;}else return false;}
template<class t,class u> bool chmin(t&a,u b){if(b<a){a=b;return true;}else return false;}

template<class t> using vc=vector<t>;
template<class t> using vvc=vc<vc<t>>;

using pi=pair<int,int>;
using vi=vc<int>;
using vvi=vc<vc<int>>;

template<class t,class u>
ostream& operator<<(ostream& os,const pair<t,u>& p){
	return os<<"{"<<p.a<<","<<p.b<<"}";
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

ll rand_int(ll l, ll r) { //[l, r]
	//#ifdef LOCAL
	static mt19937_64 gen;
	/*#else
	static mt19937_64 gen(chrono::steady_clock::now().time_since_epoch().count());
	#endif*/
	assert(l<=r); //最適化していると↓が l>r でも動いてしまうという事故があったので
	return uniform_int_distribution<ll>(l, r)(gen);
}
ll rand_int(ll k){ //[0,k)
	return rand_int(0,k-1);
}
pi rand_range(int n){
	int l=rand_int(n);
	int r=rand_int(n);
	if(l>r)swap(l,r);
	r++;
	return pi(l,r);
}
string rand_string(int n,char lw,char up){
	string s(n,'?');
	rep(i,n)s[i]=rand_int(lw,up);
	return s;
}

int current_run_id,run_batch_size=1000;
int calc_random_limit(){
	return current_run_id/run_batch_size+1;
}
template<class t>
void generate_single(t&a){
	a=rand_int(1,calc_random_limit());
}
void generate_single(string&a){
	int n;generate_single(n);
	a=rand_string(n,'a','b');
}
template<class t,class u>
void generate_single(pair<t,u>&a){
	generate_single(a.a);
	generate_single(a.b);
}
//https://trap.jp/post/1224/
template<class... Args>
void input(Args&... a){
	if(dbg){
		(generate_single(a),...);
	}else{
		#ifdef USE_FAST_IO
		sc.read(a...);
		#else
		(cin >> ... >> a);
		#endif
	}
}
#define INT(...) int __VA_ARGS__;input(__VA_ARGS__)
#define LL(...) ll __VA_ARGS__;input(__VA_ARGS__)
#define ULL(...) ull __VA_ARGS__;input(__VA_ARGS__)
#define STR(...) string __VA_ARGS__;input(__VA_ARGS__)
#define CHR(...) char __VA_ARGS__;input(__VA_ARGS__)
#define DBL(...) double __VA_ARGS__;input(__VA_ARGS__)
#define LD(...) ld __VA_ARGS__;input(__VA_ARGS__)
#define overload3(a,b,c,d,...) d
#define VI2(name,size) vi name(size);rep(i_##name,size)input(name[i_##name]);
#define VI3(name,size,offset) vi name(size);rep(i_##name,size)input(name[i_##name]),name[i_##name]+=offset;
#define VI(...) overload3(__VA_ARGS__,VI3,VI2)(__VA_ARGS__)
#define VPI(name,size) vc<pi> name(size);rep(i_##name,size)input(name[i_##name].a,name[i_##name].b);
#define VVI(name,sizeN,sizeM) vvi name(sizeN,vi(sizeM));\
rep(i_##name,sizeN)rep(j_##name,sizeM)input(name[i_##name][j_##name]);
#define VS(name,size) vc<string> name(size);rep(i_##name,size)input(name[i_##name]);
#define VMI(name,size) vc<mint> name(size);rep(i_##name,size){INT(tmp_##name);name[i_##name]=tmp_##name;};

#define overload5(a,b,c,d,e,f,...) f
#define VVC4(type,name,sizeN,sizeM) vvc<type> name(sizeN,vc<type>(sizeM));
#define VVC5(type,name,sizeN,sizeM,ini) vvc<type> name(sizeN,vc<type>(sizeM,ini));
#define VVC(...) overload5(__VA_ARGS__,VVC5,VVC4)(__VA_ARGS__)

#define VRANGE(name,size) VPI(name,size);for(auto&[l,r]:name)l--;

template<class T>
T vvvc(T v){
	return v;
}

template<class T,class...Args>
auto vvvc(int n,T v,Args...args){
	return vector(n,vvvc(v,args...));
}

void printsuc(int suc){
	#ifdef USE_FAST_IO
		if(suc==1)pr.write('\n');
		if(suc==2)pr.write(' ');
	#else
		if(suc==1){
			if(dbg)cout<<endl;
			else{
				#ifdef LOCAL
				cout<<endl;
				#else
				cout<<"\n";
				#endif
			}
		}
		if(suc==2)
			cout<<" ";
	#endif
}

template<class t>
void print_single(t x,int suc=1){
	#ifdef USE_FAST_IO
	pr.write(x);
	#else
	cout<<x;
	#endif
	printsuc(suc);
}

template<class t,class u>
void print_single(const pair<t,u>&p,int suc=1){
	print_single(p.a,2);
	print_single(p.b,suc);
}

template<class T>
void print_single(const vector<T>&v,int suc=1){
	rep(i,v.size())
		print_single(v[i],i==int(v.size())-1?3:2);
	printsuc(suc);
}

template<class T,size_t N>
void print_single(const array<T,N>&v,int suc=1){
	rep(i,N)
		print_single(v[i],i==int(N)-1?3:2);
	printsuc(suc);
}

template<class T>
void print(const T&t){
	print_single(t);
}

template<class T,class ...Args>
void print(const T&t,const Args&...args){
	print_single(t,2);
	print(args...);
}

template<class T>
void printvv(const vvc<T>&vs){
	for(const auto&row:vs)print(row);
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

void YES(bool ex=true){
	cout<<"YES\n";
	if(ex)exit(0);
	#ifdef LOCAL
	cout.flush();
	#endif
}
void NO(bool ex=true){
	cout<<"NO\n";
	if(ex)exit(0);
	#ifdef LOCAL
	cout.flush();
	#endif
}
void Yes(bool ex=true){
	cout<<"Yes\n";
	if(ex)exit(0);
	#ifdef LOCAL
	cout.flush();
	#endif
}
void No(bool ex=true){
	cout<<"No\n";
	if(ex)exit(0);
	#ifdef LOCAL
	cout.flush();
	#endif
}
//#define CAPITAL
/*
void yes(bool ex=true){
	#ifdef CAPITAL
	cout<<"YES"<<"\n";
	#else
	cout<<"Yes"<<"\n";
	#endif
	if(ex)exit(0);
	#ifdef LOCAL
	cout.flush();
	#endif
}
void no(bool ex=true){
	#ifdef CAPITAL
	cout<<"NO"<<"\n";
	#else
	cout<<"No"<<"\n";
	#endif
	if(ex)exit(0);
	#ifdef LOCAL
	cout.flush();
	#endif
}*/
void possible(bool ex=true){
	#ifdef CAPITAL
	cout<<"POSSIBLE"<<"\n";
	#else
	cout<<"Possible"<<"\n";
	#endif
	if(ex)exit(0);
	#ifdef LOCAL
	cout.flush();
	#endif
}
void impossible(bool ex=true){
	#ifdef CAPITAL
	cout<<"IMPOSSIBLE"<<"\n";
	#else
	cout<<"Impossible"<<"\n";
	#endif
	if(ex)exit(0);
	#ifdef LOCAL
	cout.flush();
	#endif
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
int topbit(ull t){
	return t==0?-1:63-__builtin_clzll(t);
}
int botbit(signed a){
	return a==0?32:__builtin_ctz(a);
}
int botbit(ll a){
	return a==0?64:__builtin_ctzll(a);
}
int botbit(ull a){
	return a==0?64:__builtin_ctzll(a);
}
int popcount(signed t){
	return __builtin_popcount(t);
}
int popcount(ll t){
	return __builtin_popcountll(t);
}
int popcount(ull t){
	return __builtin_popcountll(t);
}
int bitparity(ll t){
	return __builtin_parityll(t);
}
bool ispow2(int i){
	return i&&(i&-i)==i;
}
ll mask(int i){
	return (ll(1)<<i)-1;
}
inline ull umask(int i){
	return (ull(1)<<i)-1;
}
inline ull umask(int l,int r){
	return l==64?0:r-l==64?-1:umask(r-l)<<l;
}
ll minp2(ll n){
	if(n<=1)return 1;
	else return ll(1)<<(topbit(n-1)+1);
}

bool inc(int a,int b,int c){
	return a<=b&&b<=c;
}

template<class S> void mkuni(S&v){
	sort(all(v));
	v.erase(unique(all(v)),v.ed);
}

template<class t> bool isuni(vc<t> v){
	int s=si(v);
	mkuni(v);
	return si(v)==s;
}

bool isperm(const vi&p){
	int n=si(p);
	vc<bool> used(n);
	for(auto v:p){
		if(!inc(0,v,n-1))return false;
		if(used[v])return false;
		used[v]=true;
	}
	return true;
}

template<class t>
void myshuffle(vc<t>&a){
	rep(i,si(a))swap(a[i],a[rand_int(0,i)]);
}

template<class S,class u>
int lwb(const S&v,const u&a){
	return lower_bound(all(v),a)-v.bg;
}
template<class t,class u>
bool bis(const vc<t>&v,const u&a){
	return binary_search(all(v),a);
}

//VERIFY: yosupo
//KUPC2017J
//AOJDSL1A
//without rank
struct unionfind{
	vi p,s;
	int c;
	unionfind(int n):p(n,-1),s(n,1),c(n){}
	void clear(){
		fill(all(p),-1);
		fill(all(s),1);
		c=si(p);
	}
	int find(int a){
		return p[a]==-1?a:(p[a]=find(p[a]));
	}
	//set b to a child of a
	bool unite(int a,int b){
		a=find(a);
		b=find(b);
		if(a==b)return false;
		p[b]=a;
		s[a]+=s[b];
		c--;
		return true;
	}
	bool same(int a,int b){
		return find(a)==find(b);
	}
	int sz(int a){
		return s[find(a)];
	}
};

vvc<int> readGraph(int n,int m){
	vvc<int> g(n);
	rep(i,m){
		int a,b;
		cin>>a>>b;
		//sc.read(a,b);
		a--;b--;
		g[a].pb(b);
		g[b].pb(a);
	}
	return g;
}

vvc<int> initUG(int n,const vc<pi>&es){
	vvc<int> g(n);
	for(auto [a,b]:es){
		g[a].pb(b);
		g[b].pb(a);
	}
	return g;
}

vvc<int> rand_tree(int n){
	vvc<int> t(n);
	unionfind uf(n);
	while(uf.c>1){
		int a=rand_int(n);
		int b=rand_int(n);
		if(uf.unite(a,b)){
			t[a].pb(b);
			t[b].pb(a);
		}
	}
	return t;
}

vvc<int> readTree(int n){
	if(dbg){
		return rand_tree(n);
	}else{
		return readGraph(n,n-1);
	}
}

vi readRooted(int n){
	assert(!dbg);
	vi par(n,-1);
	rng(i,1,n){
		input(par[i]);
		par[i]--;
		assert(inc(0,par[i],i-1));
	}
	return par;
}

void printTree(const vvc<int> t){
	int n=si(t);
	int degsum=0;
	rep(i,n)degsum+=si(t[i]);
	if(degsum==n-1){
		//directed
		rep(i,si(t))for(auto j:t[i]){
			print(i+1,j+1);
		}
	}else if(degsum==2*(n-1)){
		//undirected
		rep(i,si(t))for(auto j:t[i])if(i<j){
			print(i+1,j+1);
		}
	}else{
		assert(false);
	}
}

template<class t>
vc<t> presum(const vc<t>&a){
	vc<t> s(si(a)+1);
	rep(i,si(a))s[i+1]=s[i]+a[i];
	return s;
}
vc<ll> presum(const vi&a){
	vc<ll> s(si(a)+1);
	rep(i,si(a))s[i+1]=s[i]+a[i];
	return s;
}
//BIT で数列を管理するときに使う (CF850C)
template<class t>
vc<t> predif(vc<t> a){
	gnr(i,1,si(a))a[i]-=a[i-1];
	return a;
}
template<class t>
vvc<ll> imos(const vvc<t>&a){
	int n=si(a),m=si(a[0]);
	vvc<ll> b(n+1,vc<ll>(m+1));
	rep(i,n)rep(j,m)
		b[i+1][j+1]=b[i+1][j]+b[i][j+1]-b[i][j]+a[i][j];
	return b;
}

//verify してないや
void transvvc(int&n,int&m){
	swap(n,m);
}
template<class t,class... Args>
void transvvc(int&n,int&m,vvc<t>&a,Args&...args){
	assert(si(a)==n);
	vvc<t> b(m,vi(n));
	rep(i,n){
		assert(si(a[i])==m);
		rep(j,m)b[j][i]=a[i][j];
	}
	a.swap(b);
	transvvc(n,m,args...);
}
//CF854E
void rotvvc(int&n,int&m){
	swap(n,m);
}
template<class t,class... Args>
void rotvvc(int&n,int&m,vvc<t>&a,Args&...args){
	assert(si(a)==n);
	vvc<t> b(m,vi(n));
	rep(i,n){
		assert(si(a[i])==m);
		rep(j,m)b[m-1-j][i]=a[i][j];
	}
	a.swap(b);
	rotvvc(n,m,args...);
}

//ソートして i 番目が idx[i]
//CF850C
template<class t>
vi sortidx(const vc<t>&a){
	int n=si(a);
	vi idx(n);iota(all(idx),0);
	sort(all(idx),[&](int i,int j){return a[i]<a[j];});
	return idx;
}
//vs[i]=a[idx[i]]
//例えば sortidx で得た idx を使えば単にソート列になって返ってくる
//CF850C
template<class t>
vc<t> a_idx(const vc<t>&a,const vi&idx){
	int n=si(a);
	assert(si(idx)==n);
	vc<t> vs(n);
	rep(i,n)vs[i]=a[idx[i]];
	return vs;
}
//CF850C
vi invperm(const vi&p){
	int n=si(p);
	vi q(n);
	rep(i,n)q[p[i]]=i;
	return q;
}

template<class t,class s=t>
s SUM(const vc<t>&a){
	return accumulate(all(a),s(0));
}
ll SUM(const vi&a){
	return accumulate(all(a),ll(0));
}
template<class t,size_t K,class s=t>
s SUM(const array<t,K>&a){
	return accumulate(all(a),s(0));
}

template<class t>
t MAX(const vc<t>&a){
	return *max_element(all(a));
}

template<class t>
pair<t,int> MAXi(const vc<t>&a){
	auto itr=max_element(all(a));
	return mp(*itr,itr-a.bg);
}

template<class A>
auto MIN(const A&a){
	return *min_element(all(a));
}

template<class t>
pair<t,int> MINi(const vc<t>&a){
	auto itr=min_element(all(a));
	return mp(*itr,itr-a.bg);
}

vi vid(int n){
	vi res(n);iota(all(res),0);
	return res;
}

template<class S>
void soin(S&s){
	sort(all(s));
}

template<class S,class F>
void soin(S&s,F&&f){
	sort(all(s),forward<F>(f));
}

template<class S>
S soout(S s){
	soin(s);
	return s;
}

template<class S>
void rein(S&s){
	reverse(all(s));
}

template<class S>
S reout(S s){
	rein(s);
	return s;
}

template<class t,class u>
pair<t,u>&operator+=(pair<t,u>&a,pair<t,u> b){
	a.a+=b.a;a.b+=b.b;return a;}
template<class t,class u>
pair<t,u>&operator-=(pair<t,u>&a,pair<t,u> b){
	a.a-=b.a;a.b-=b.b;return a;}
template<class t,class u>
pair<t,u> operator+(pair<t,u> a,pair<t,u> b){return mp(a.a+b.a,a.b+b.b);}
template<class t,class u>
pair<t,u> operator-(pair<t,u> a,pair<t,u> b){return mp(a.a-b.a,a.b-b.b);}
template<class t,class u,class v>
pair<t,u>&operator*=(pair<t,u>&a,v b){
	a.a*=b;a.b*=b;return a;}
template<class t,class u,class v>
pair<t,u> operator*(pair<t,u> a,v b){return a*=b;}
template<class t,class u>
pair<t,u> operator-(pair<t,u> a){return mp(-a.a,-a.b);}
namespace std{
template<class t,class u>
istream&operator>>(istream&is,pair<t,u>&a){
	return is>>a.a>>a.b;
}
}

template<class t,size_t n>
array<t,n>&operator+=(array<t,n>&a,const array<t,n>&b){
	rep(i,n)a[i]+=b[i];
	return a;
}
template<class t,size_t n>
array<t,n>&operator-=(array<t,n>&a,const array<t,n>&b){
	rep(i,n)a[i]-=b[i];
	return a;
}
template<class t,size_t n,class v>
array<t,n>&operator*=(array<t,n>&a,v b){
	rep(i,n)a[i]*=b;
	return a;
}
template<class t,size_t n>
array<t,n> operator+(array<t,n> a,const array<t,n>&b){return a+=b;}
template<class t,size_t n>
array<t,n> operator-(array<t,n> a,const array<t,n>&b){return a-=b;}
template<class t,size_t n,class v>
array<t,n> operator*(array<t,n> a,v b){return a*=b;}

template<class t>
t gpp(vc<t>&vs){
	assert(si(vs));
	t res=move(vs.back());
	vs.pop_back();
	return res;
}

template<class t,class u>
void pb(vc<t>&a,const vc<u>&b){
	a.insert(a.ed,all(b));
}

template<class t,class...Args>
vc<t> cat(vc<t> a,Args&&...b){
	(pb(a,forward<Args>(b)),...);
	return a;
}

template<class t,class u>
vc<t>& operator+=(vc<t>&a,u x){
	for(auto&v:a)v+=x;
	return a;
}

template<class t,class u>
vc<t> operator+(vc<t> a,u x){
	return a+=x;
}

template<class t>
vc<t>& operator+=(vc<t>&a,const vc<t>&b){
	a.resize(max(si(a),si(b)));
	rep(i,si(b))a[i]+=b[i];
	return a;
}

template<class t>
vc<t> operator+(const vc<t>&a,const vc<t>&b){
	vc<t> c(max(si(a),si(b)));
	rep(i,si(a))c[i]+=a[i];
	rep(i,si(b))c[i]+=b[i];
	return c;
}

template<class t,class u>
vc<t>& operator-=(vc<t>&a,u x){
	for(auto&v:a)v-=x;
	return a;
}
template<class t,class u>
vc<t> operator-(vc<t> a,u x){
	return a-=x;
}
template<class t>
vc<t>& operator-=(vc<t>&a,const vc<t>&b){
	a.resize(max(si(a),si(b)));
	rep(i,si(b))a[i]-=b[i];
	return a;
}
template<class t>
vc<t> operator-(const vc<t>&a,const vc<t>&b){
	vc<t> c(max(si(a),si(b)));
	rep(i,si(a))c[i]+=a[i];
	rep(i,si(b))c[i]-=b[i];
	return c;
}
template<class t,class u>
vc<t>& operator*=(vc<t>&a,u x){
	for(auto&v:a)v*=x;
	return a;
}
template<class t,class u>
vc<t> operator*(vc<t> a,u x){
	return a*=x;
}

template<class t,class u>
vc<t>& operator/=(vc<t>&a,u x){
	for(auto&v:a)v/=x;
	return a;
}
template<class t,class u>
vc<t> operator/(vc<t> a,u x){
	return a/=x;
}

template<class t>
vc<t>& operator<<=(vc<t>&a,int k){
	assert(k>=0);
	a.insert(a.bg,k,t(0));
	return a;
}
template<class t>
vc<t> operator<<(vc<t> a,int k){
	return a<<=k;
}

template<class t>
vc<t>& operator>>=(vc<t>&a,int k){
	if(si(a)<=k)a.clear();
	else a.erase(a.bg,a.bg+k);
	return a;
}
template<class t>
vc<t> operator>>(vc<t> a,int k){
	return a>>=k;
}

//消した要素の個数を返してくれる
//not verified
template<class t,class u>
int remval(vc<t>&a,const u&v){
	auto itr=remove(all(a),v);
	int res=a.ed-itr;
	a.erase(itr,a.ed);
	return res;
}
//消した要素の個数を返してくれる
//UCUP 2-8-F
template<class t,class F>
int remif(vc<t>&a,F f){
	auto itr=remove_if(all(a),f);
	int res=a.ed-itr;
	a.erase(itr,a.ed);
	return res;
}
template<class t>
void rempos(vc<t>&a,int i){
	assert(inc(0,i,si(a)-1));
	a.erase(a.bg+i);
}

template<class VS,class u>
void fila(VS&vs,const u&a){
	fill(all(vs),a);
}

template<class t,class u>
int findid(const vc<t>&vs,const u&a){
	auto itr=find(all(vs),a);
	if(itr==vs.ed)return -1;
	else return itr-vs.bg;
}

template<class t>
void rtt(vc<t>&vs,int i){
	rotate(vs.bg,vs.bg+i,vs.ed);
}

//Multiuni2023-8 C
//f(lw)=false,...,f(n-1)=false,f(n)=true,...,f(up)=true,
//のときに n を返す
template<class F>
int find_min_true(int lw,int up,F f){
	while(up-lw>1){
		const int mid=(lw+up)/2;
		if(f(mid))up=mid;
		else lw=mid;
	}
	return up;
}
//f(lw)=true,f(up)=false
template<class F>
int find_max_true(int lw,int up,F f){
	while(up-lw>1){
		const int mid=(lw+up)/2;
		if(f(mid))lw=mid;
		else up=mid;
	}
	return lw;
}

template<class t> using pqmin=priority_queue<t,vc<t>,greater<t>>;
template<class t> using pqmax=priority_queue<t>;
using T=tuple<int,int,int>;

//mint107 は verify してねえ
//#define DYNAMIC_MOD

struct modinfo{uint mod,root;
#ifdef DYNAMIC_MOD
//barret redunction なのでどんな値でもいい
constexpr modinfo(uint m,uint r):mod(m),root(r),im(0){set_mod(m);}
ull im;
constexpr void set_mod(uint m){
	mod=m;
	im=ull(-1)/m+1;
}
uint product(uint a,uint b)const{
	ull z=ull(a)*b;
	uint x=((unsigned __int128)z*im)>>64;
	uint v=uint(z)-x*mod;
	return v<mod?v:v+mod;
}
#endif
};
template<modinfo const&ref>
struct modular{
	static constexpr uint const &mod=ref.mod;
	static modular root(){return modular(ref.root);}
	uint v;
	//modular(initializer_list<uint>ls):v(*ls.bg){}
	modular(ll vv=0){s(vv%mod+mod);}
	modular& s(uint vv){
		v=vv<mod?vv:vv-mod;
		return *this;
	}
	modular operator-()const{return modular()-*this;}
	modular& operator+=(const modular&rhs){return s(v+rhs.v);}
	modular&operator-=(const modular&rhs){return s(v+mod-rhs.v);}
	modular&operator*=(const modular&rhs){
		#ifndef DYNAMIC_MOD
		v=ull(v)*rhs.v%mod;
		#else
		v=ref.product(v,rhs.v);
		#endif
		return *this;
	}
	modular&operator/=(const modular&rhs){return *this*=rhs.inv();}
	modular operator+(const modular&rhs)const{return modular(*this)+=rhs;}
	modular operator-(const modular&rhs)const{return modular(*this)-=rhs;}
	modular operator*(const modular&rhs)const{return modular(*this)*=rhs;}
	modular operator/(const modular&rhs)const{return modular(*this)/=rhs;}
	modular pow(ll n)const{
		if(n<0)return inv().pow(-n);
		modular res(1),x(*this);
		while(n){
			if(n&1)res*=x;
			x*=x;
			n>>=1;
		}
		return res;
	}
	modular inv()const{return pow(mod-2);}
	/*modular inv()const{
		int x,y;
		int g=extgcd<ll>(v,mod,x,y);
		assert(g==1);
		if(x<0)x+=mod;
		return modular(x);
	}*/
	friend modular operator+(ll x,const modular&y){
		return modular(x)+y;
	}
	friend modular operator-(ll x,const modular&y){
		return modular(x)-y;
	}
	friend modular operator*(ll x,const modular&y){
		return modular(x)*y;
	}
	friend modular operator/(ll x,const modular&y){
		return modular(x)/y;
	}
	friend ostream& operator<<(ostream&os,const modular&m){
		return os<<m.v;
	}
	friend istream& operator>>(istream&is,modular&m){
		ll x;is>>x;
		m=modular(x);
		return is;
	}
	bool operator<(const modular&r)const{return v<r.v;}
	bool operator==(const modular&r)const{return v==r.v;}
	bool operator!=(const modular&r)const{return v!=r.v;}
	explicit operator bool()const{
		return v;
	}
};

#ifndef DYNAMIC_MOD
extern constexpr modinfo base{998244353,3};
//extern constexpr modinfo base{1000000007,5};
//extern constexpr modinfo base{1000000009,13};
//extern constexpr modinfo base{2147483579,1689685080};//2^31 未満の最大の安全素数
//modinfo base{1,0};
#ifdef USE_GOOD_MOD
static_assert(base.mod==998244353);
#endif
#else
modinfo base(1,0);
extern constexpr modinfo base107(1000000007,0);
using mint107=modular<base107>;
#endif
using mint=modular<base>;

//atcoder-library をまあまあコピーして使っている

//N() が単位元

//merge で片方が inactive のときはもう片方をそのまま返す，
//といったときに，lazy の情報までコピーして渡さないようにする

//クエリできる点が[0,s)だったのを[0,n)に変えた (UCUP1-21D)
//ch,max_rightは動くと思う
//ちゃんと test してないから assert とかが壊れたらごめん

//VERIFY:
//https://atcoder.jp/contests/practice2/tasks/practice2_k
template<class N,bool Beats=false>
struct seglazy{
	vc<N> x;
	int n,L,s;
	seglazy(){}
	template<class T>
	seglazy(const vc<T>& a){
		n=a.size();
		L=0;
		while((1<<L)<n)L++;
		s=1<<L;
		x.resize(s*2);
		rep(i,n)x[s+i]=N(a[i]);
		gnr(i,1,s)upd(i);
	}
	seglazy(int nn){
		n=nn;
		L=0;
		while((1<<L)<n)L++;
		s=1<<L;
		x.assign(s*2,N());
		gnr(i,1,s)upd(i);
	}
	template<class t>
	void init(const vc<t>&a){
		n=a.size();
		L=0;
		while((1<<L)<n)L++;
		s=1<<L;
		x.resize(s*2);
		rep(i,n)
			x[s+i]=N(a[i]);
		rng(i,n,s)
			x[s+i]=N();
		gnr(i,1,s)
			x[i]=N::merge(x[i*2],x[i*2+1]);
	}
	void upd(int i){
		x[i]=N::merge(x[i*2],x[i*2+1]);
	}
	void push(int i){
		x[i].push(x[i*2],x[i*2+1]);
	}
	//空ノードなしで動く ver が segbeats_norec に書いてある
	N composite(int l,int r){
		assert(0<=l&&l<=r&&r<=n);
		if(l==r)return N();
		
		l+=s;
		r+=s;
		
		for (int i = L; i >= 1; i--) {
			if (((l >> i) << i) != l) push(l >> i);
			if (((r >> i) << i) != r) push((r - 1) >> i);
		}
		
		N sml,smr;
		while (l < r) {
			if (l & 1) sml = N::merge(sml, x[l++]);
			if (r & 1) smr = N::merge(x[--r], smr);
			l >>= 1;
			r >>= 1;
		}

		return N::merge(sml, smr);
	}
	//[l,r)内の極大区間について f(i) を呼ぶ
	//UCUP3-26-L
	template<class F>
	void foreach_range(int l,int r,F f){
		assert(0<=l&&l<=r&&r<=n);
		if(l==r)return;
		
		l+=s;
		r+=s;
		
		for (int i = L; i >= 1; i--) {
			if (((l >> i) << i) != l) push(l >> i);
			if (((r >> i) << i) != r) push((r - 1) >> i);
		}
		
		while (l < r) {
			if (l & 1) f(l++);
			if (r & 1) f(--r);
			l >>= 1;
			r >>= 1;
		}
	}
	//UCUP1-21 D
	//JSC2024FinalD
	template<class F,class... Args>
	void ch_beats(int i,F f,Args&&... args){
		int ini=i;
		while(1){
			if((x[i].*f)(forward<Args>(args)...)){
				while(i>ini&&(i&1)){
					i>>=1;
					upd(i);
				}
				if(i==ini)break;
				i++;
			}else{
				assert(i<s);
				push(i);
				i*=2;
			}
		}
	}
	template<class F,class... Args>
	void ch(int l, int r, F f,Args&&... args) {
		assert(0<=l&&l<=r&&r<=n);
		if (l == r) return;

		l+=s;
		r+=s;

		for (int i = L; i >= 1; i--) {
			if (((l >> i) << i) != l) push(l >> i);
			if (((r >> i) << i) != r) push((r - 1) >> i);
		}
		
		static int buf[2][30];
		int cnt[2]{};
		{
			int l2 = l, r2 = r;
			while (l < r) {
				if (l & 1){
					//(x[l++].*f)(forward<Args>(args)...);
					buf[0][cnt[0]++]=l++;
				}
				if (r & 1){
					//(x[--r].*f)(forward<Args>(args)...);
					buf[1][cnt[1]++]=--r;
				}
				l >>= 1;
				r >>= 1;
			}
			l = l2;
			r = r2;
		}
		if constexpr(Beats){
			rep(i,cnt[0])ch_beats(buf[0][i],f,forward<Args>(args)...);
			per(i,cnt[1])ch_beats(buf[1][i],f,forward<Args>(args)...);
		}else{
			rep(i,cnt[0])(x[buf[0][i]].*f)(forward<Args>(args)...);
			per(i,cnt[1])(x[buf[1][i]].*f)(forward<Args>(args)...);
		}

		for (int i = 1; i <= L; i++) {
			if (((l >> i) << i) != l) upd(l >> i);
			if (((r >> i) << i) != r) upd((r - 1) >> i);
		}
	}
	template<class F,class... Args>
	void chall(F f,Args&&... args){
		if constexpr(Beats){
			ch_beats(1,f,forward<Args>(args)...);
		}else{
			(x[1].*f)(forward<Args>(args)...);
		}
	}
	N getall(){return x[1];}
	template <class F,class... Args> 
	pair<int,N> max_right(int l,F f,Args&&... args){
		assert(0<=l&&l<=n);
		if(l==n)return mp(n,N());
		l+=s;
		
		for (int i = L; i >= 1; i--) push(l >> i);
		N sm;
		assert((sm.*f)(forward<Args>(args)...));
		do {
			while (l % 2 == 0) l >>= 1;
			if (!(N::merge(sm,x[l]).*f)(forward<Args>(args)...)){
				while (l < s) {
					push(l);
					l = (2 * l);
					N tmp=N::merge(sm,x[l]);
					if ((tmp.*f)(forward<Args>(args)...)) {
						sm = tmp;
						l++;
					}
				}
				return mp(l - s,sm);
			}
			sm = N::merge(sm, x[l]);
			l++;
		} while ((l & -l) != l);
		return mp(n,sm);
	}
	//XXI Opencup Krakow M
	template <class F,class... Args> 
	pair<int,N> min_left(int r,F f,Args&&... args){
		assert(0<=r&&r<=n);
        if(r==0)return mp(0,N());
        r+=s;
        for (int i = L; i >= 1; i--) push((r - 1) >> i);
        N sm;
        do {
            r--;
            while (r > 1 && (r % 2)) r >>= 1;
            if (!(N::merge(x[r],sm).*f)(forward<Args>(args)...)) {
                while (r < s) {
                    push(r);
                    r = (2 * r + 1);
                    N tmp=N::merge(x[r],sm);
                    if ((tmp.*f)(forward<Args>(args)...)) {
                        sm = tmp;
                        r--;
                    }
                }
                return mp(r + 1 - s,sm);
            }
            sm = N::merge(x[r], sm);
        } while ((r & -r) != r);
        return mp(0,sm);
    }
	template<class F,class...Args>
	void point_change(int p,F f,Args&&...args){
		assert(0 <= p && p < n);
		p += s;
		for (int i = L; i >= 1; i--) push(p >> i);
		(x[p].*f)(forward<Args>(args)...);
		for (int i = 1; i <= L; i++) upd(p >> i);
	}
	void point_merge(int p,const N&t){
		assert(0 <= p && p < n);
		p += s;
		for (int i = L; i >= 1; i--) push(p >> i);
		x[p]=N::merge(x[p],t);
		for (int i = 1; i <= L; i++) upd(p >> i);
	}
	N point_get(int p){
		assert(0 <= p && p < n);
		p += s;
		for (int i = L; i >= 1; i--) push(p >> i);
		return x[p];
	}
	void point_set(int p,N val){
		assert(0 <= p && p < n);
		p += s;
		for (int i = L; i >= 1; i--) push(p >> i);
		x[p]=val;
		for (int i = 1; i <= L; i++) upd(p >> i);
	}
	void enumerater(int l,int r,int i,int b,int e,vc<N>&dst){
		if(e<=l||r<=b)
			return;
		if(l+1==r){
			dst.pb(x[i]);
			return;
		}
		push(i);
		int m=(l+r)/2;
		enumerater(l,m,i*2,b,e,dst);
		enumerater(m,r,i*2+1,b,e,dst);
	}
	void enumerate(int b,int e,vc<N>&dst){
		assert(0<=b&&b<=e&&e<=n);
		return enumerater(0,s,1,b,e,dst);
	}
};

//Range add,sum
struct AddSum{
	mint len,sum,lz;
	AddSum():len(0),sum(0),lz(0){}
	AddSum(mint inilen,mint inisum):len(inilen),sum(inisum),lz(0){}
	void add(mint v){
		sum+=v*len;
		lz+=v;
		return;
	}
	void push(AddSum&x,AddSum&y){
		x.add(lz);
		y.add(lz);
		lz=0;
	}
	static AddSum merge(AddSum x,AddSum y){
		return AddSum(x.len+y.len,x.sum+y.sum);
	}
};

template<class t>
struct BIT{
	vc<t> raw,buf;
	int s;
	BIT(int n=0){init(n);}
	BIT(const vc<t>&a){init(a);}
	void init(int n){
		s=n;
		raw.clear();raw.resize(s);
		buf.clear();buf.resize(s);
	}
	void init(const vc<t>&a){
		s=si(a);
		raw=a;
		buf.resize(s);
		rep(i,s)buf[i]=a[i];
		rep(i,s){
			int j=i+((i+1)&(-i-1));
			if(j<s)buf[j]+=buf[i];
		}
	}
	void add(int i,t v){
		if(s<=i)return;
		raw[i]+=v;
		for(;i<s;i+=(i+1)&(-i-1))
			buf[i]+=v;
	}
	t get(int i){
		t res=t();
		for(;i>=0;i-=(i+1)&(-i-1))
			res+=buf[i];
		return res;
	}
	t sum(int b,int e){
		return get(e-1)-get(b-1);
	}
	void add_range(int b,int e,t v){
		add(b,v);
		add(e,-v);
	}
	int kth(int k){
		int res=0;
		for(int i=topbit(s);i>=0;i--){
			int w=res+(1<<i);
			if(w<=s&&buf[w-1]<=k){
				k-=buf[w-1];
				res=w;
			}
		}
		return res;
	}
	//UCUP 2-9-I
	int kth_helper(int i,int k){
		return kth(k+get(i-1));
	}
	t operator[](int i)const{
		return raw[i];
	}
};

//min cartesian tree
vi mincartesiantree(const vi&a){
	int n=si(a);
	vi par(n,-1);
	vi s;
	rep(i,n){
		int last=-1;
		while(si(s)&&a[s.back()]>=a[i]){
			last=s.back();
			s.pop_back();
		}
		if(last!=-1)par[last]=i;
		if(si(s))par[i]=s.back();
		s.pb(i);
	}
	return par;
}

struct Range{
	int k,l,r;
};
ostream&operator<<(ostream&os,Range r){
	return os<<"R{"<<r.k<<","<<r.l<<","<<r.r<<"}";
}

mint fast_sub(int m,vi bd){
	int n=si(bd);
	assert(n>0);
	
	if(m%2==0){
		mint ans=0;
		
		vi vs;
		for(int i=0;i<n;){
			if(bitparity(bd[i])==0){
				vs.pb(0);
				i++;
			}else{
				int j=i+1;
				while(bitparity(bd[j])==0)j++;
				int val=j-i;
				bool bad=bd[i]==bd[j];
				rng(k,i+1,j)if(popcount(bd[i]^bd[k])!=1)bad=false;
				if(bad)val++;
				vs.pb(val);
				i=j+1;
			}
		}

		rep(i,si(vs))
			ans+=vs[i]*(i+1)*(si(vs)-i);
		
		return ans;
	}
	
	vi halls(n);
	rep(i,n)halls[i]=(m-1)/2-popcount(bd[i])/2;

	mint baseans=0;
	{
		vi vs;
		vi ds;
		for(int i=0;i<n;){
			if(bitparity(bd[i])==0){
				vs.pb(0);
				ds.pb(halls[i]);
				i++;
			}else{
				int j=i+1;
				while(bitparity(bd[j])==0)j++;
				int val=j-i;
				int d=-1;
				rng(k,i,j+1)d+=halls[k];
				vs.pb(val);
				ds.pb(d);
				i=j+1;
			}
		}
		
		vi h=presum(ds);
		vi sum=presum(vs);

		vi ys=h;mkuni(ys);
		BIT<int> cnt(si(ys)),wei(si(ys));
		rep(i,si(sum)){
			int j=lwb(ys,h[i]);
			int c=cnt.get(j),w=wei.get(j);
			baseans+=(sum[i]*c-w);
			cnt.add(j,1);
			wei.add(j,sum[i]);
		}
	}
	
	vc<Range> ws;
	vi cuts{0};
	vc<pi> ext;
	{
		for(int i=0;i<n;){
			if(bitparity(bd[i])==0){
				rep(_,halls[i])ws.pb({+1,i,i});
				i++;
			}else{
				int j=i+1;
				while(bitparity(bd[j])==0)j++;
				vi xs;
				rng(k,i,j+1)rep(_,halls[k])xs.pb(k);
				if(xs.empty()){
					ws.pb({-1,i,j});
				}else{
					xs.insert(xs.bg,i);
					xs.pb(j);

					ws.pb({-1,i,i});
					rep(k,si(xs)-1)ws.pb({+1,xs[k],xs[k+1]});
					ws.pb({-1,j,j});
					
					bool bad=bd[i]==bd[j];
					rng(k,i+1,j)if(popcount(bd[i]^bd[k])!=1)bad=false;
					if(bad){
						assert(si(xs)>=4);
						ext.eb(si(ws)-si(xs)+1,si(ws)-3);
					}
				}
				i=j+1;
			}
			cuts.pb(si(ws));
		}
	}

	int len=si(ws);
	vi h(len+1);
	rep(i,len)h[i+1]=h[i]+ws[i].k;
	h-=MIN(h);
	
	int H=MAX(h);
	
	vvc<int> pos(H);
	rep(i,len)pos[min(h[i],h[i+1])].pb(i);
	
	vi cc(len+1);
	for(auto c:cuts)cc[c]++;
	rep(i,len)cc[i+1]+=cc[i];
	
	vi prelow(len),sufup(len);
	vi prelowsum(len),sufupsum(len);
	vi left(len,-1),right(len,len);
	vi same(len);
	
	mint X=0;
	
	vc<T> qss[2];
	
	rep(lv,H){
		vi ls=pos[lv];
		
		rep(i,si(ls)){
			int x=ls[i];
			if(ws[x].k==+1){
				prelow[x]+=cc[x]-(i==0?0:cc[ls[i-1]]);
				prelowsum[x]=prelow[x];
				if(i>=2)prelowsum[x]+=prelowsum[ls[i-2]];
			}
		}
		per(i,si(ls)){
			int x=ls[i];
			if(ws[x].k==+1){
				sufup[x]+=(i+1<si(ls)?cc[ls[i+1]]:si(cuts))-cc[x];
				sufupsum[x]=sufup[x];
				if(i+2<si(ls))sufupsum[x]+=sufupsum[ls[i+2]];
			}
		}
		
		rep(i,si(ls)-1){
			if(ws[ls[i]].k==-1){
				qss[0].eb(ls[i],lv,ws[ls[i+1]].l-ws[ls[i]].r);
			}else{
				qss[1].eb(ls[i+1]+1,lv+1,ws[ls[i+1]].l-ws[ls[i]].r);
			}
		}
		if(ws[ls[0]].k==-1)rempos(ls,0);
		if(ls.empty())continue;
		if(ws[ls.back()].k==-1)ls.pop_back();
		
		int k=(si(ls)-1)/2;
		vi a(k),b(k);
		rep(i,k){
			a[i]=ws[ls[i*2+1]].l-ws[ls[i*2+0]].r;
			b[i]=ws[ls[i*2+2]].l-ws[ls[i*2+1]].r;
		}
		vi c(k+1);
		c[0]=SUM(b);
		rep(i,k)c[i+1]=c[i]+a[i]-b[i];
		
		{
			mint W=0;
			
			using A=array<int,2>;
			vc<A> t(k+1,A{-1,-1});
			vi par=mincartesiantree(c);
			int root=-1;
			rep(i,k+1)if(par[i]==-1)root=i;
			else t[par[i]][par[i]<i]=i;
			
			struct D{
				mint lwei,lcnt,lval,rwei,rcnt,rval;
			};
			auto dfs=[&](auto self,int i,int l,int r)->D{
				D res;
				if(i==-1){
					int j=l-1;
					if(inc(0,j,k-1)){
						res.lwei=a[j];
						res.rwei=b[j];
					}
					return res;
				}
				int x=ls[i*2];
				D L=self(self,t[i][0],l,i);
				D R=self(self,t[i][1],i+1,r);
				
				W+=L.lval*(sufup[x]+R.rcnt);
				W+=R.rval*(L.lcnt+prelow[x]);
				
				res.lwei=L.lwei+R.lwei;
				res.rwei=L.rwei+R.rwei;
				
				res.lcnt=L.lcnt+prelow[x]+R.lcnt;
				res.rcnt=L.rcnt+sufup[x]+R.rcnt;
				
				res.lval=L.lval+(L.lcnt+prelow[x])*R.lwei+R.lval;
				res.rval=L.rval+(sufup[x]+R.rcnt)*L.rwei+R.rval;
				
				return res;
			};
			dfs(dfs,root,0,k+1);
			
			X+=W;
		}
		
		{
			vi st;
			rep(i,k+1){
				while(si(st)&&c[st.back()]>c[i])st.pop_back();
				if(si(st)){
					left[ls[i*2]]=ls[st.back()*2];
					same[ls[i*2]]=(c[i]==c[st.back()]);
				}
				st.pb(i);
			}
		}
		{
			vi st;
			per(i,k+1){
				while(si(st)&&c[st.back()]>c[i])st.pop_back();
				if(si(st))right[ls[i*2]]=ls[st.back()*2];
				st.pb(i);
			}
		}
	}
	
	mint Y=0;
	vi dp(len);
	
	for(auto [l,r]:ext){
		int L=left[l];
		int R=right[r];
		int val=prelowsum[l];
		if(L!=-1){
			val-=prelowsum[L];
			if(same[l])
				val+=dp[L];
		}
		int mul=sufupsum[r];
		if(R<len)mul-=sufupsum[R];
		
		Y+=val*mul;
		dp[r]=val;
	}
	
	mint Z=0;
	rep(flip,2){
		vc<T> qs;
		vi xs;
		auto add=[&](int x,int y,int v){
			if(flip){
				x=len-x;
				y=H-y;
			}
			qs.eb(y,v,x);
			if(v>=0)xs.pb(x);
		};
		
		for(auto [x,y,v]:qss[flip])
			add(x,y,v);
		rep(i,si(cuts)){
			add(cuts[i],h[cuts[i]],-1);
		}
		mkuni(xs);
		int s=si(xs);
		
		for(auto&[y,v,x]:qs){
			x=lwb(xs,x);
		}
		soin(qs);
		
		seglazy<AddSum> seg(s);
		for(auto [y,v,x]:qs){
			if(v==-1){
				auto w=seg.composite(0,x);
				Z+=w.sum;
				seg.ch(x,s,&AddSum::add,1);
			}else{
				seg.point_set(x,AddSum(v,0));
			}
		}
	}
	
	return baseans+X*2+Y+Z*2;
}

mint fast_all(vc<string> ss){
	int n=si(ss),m=si(ss[0]);
	vi bd(n);
	rep(i,n)rep(j,m)if(ss[i][j]=='O')bd[i]|=1<<j;
	
	vi ps(n+1);
	rep(i,n)ps[i+1]=ps[i]^bitparity(bd[i]);
	
	mint ans=0;
	rep(tar,2){
		int l=inf,r=-inf;
		rep(i,n+1)if(ps[i]==tar){
			chmin(l,i);
			chmax(r,i);
		}
		if(l<r){
			ans+=fast_sub(m,vi(bd.bg+l,bd.bg+r));
		}
	}
	
	return ans;
}

int slv(int N, int M, vector<bitset<10>> S) {
	vc<string> ss(N);
	for (int i = 0; i < N; i++) {
		ss[i].resize(M);
		for (int j = 0; j < M; j++) {
			ss[i][j] = S[i][j] ? 'O' : '.';
		}
	}
	mint ans=fast_all(ss);
	return ans.v;
}
/*

signed main(signed argc,char*argv[]){
	if(argc>1&&strcmp(argv[1],"D")==0)dbg=true;
	
	cin.tie(0);
	ios::sync_with_stdio(0);
	cout<<fixed<<setprecision(20);
	
	if(dbg){
		while(1){
			if(current_run_id%run_batch_size==0){
				cerr<<"Current Run "<<current_run_id<<endl;
			}
			slv();
			current_run_id++;
		}
	}else{
		INT(t);rep(_,t)
		slv();
	}
}
*/

#undef int

#undef rng
#undef rep
#undef gnr
#undef per
#undef pb
#undef eb
#undef a
#undef b
#undef bg
#undef ed
#undef all
#undef si
#ifdef LOCAL
#undef dmp(x) cerr<<__LINE__<<" "<<#x<<" "<<x<<endl
#else
#undef dmp
#endif

}

/** Binary-indexed tree
 *
 *  A binary indexed tree with N nodes of type T provides the
 *  following two functions for 0 <= i <= N:
 *
 *      prefix(int i) -> prefix_iterator<T>
 *      suffix(int i) -> suffix_iterator<T>
 *
 *  such that size(suffix(i) intersect prefix(j)) = (1 if i < j else 0).
 *  Furthermore, the resulting lists always have size at most log_2(N).
 *
 *  This can be used to implement either point-update/(prefix|suffix)-query or
 *  (prefix|suffix)-update/point-query over a virtual array of size N of a
 *  commutative monoid. This can be generalized to implement
 *  point-update/range-query or range-update/point-query over a virtual array
 *  of size N of a commutative group.
 *
 *  With 0-indexed data, prefixes are more natural:
 *   * For range update/query, use for_prefix for the ranges and for_suffix for the points.
 *   * For prefix update/query, no change.
 *   * For suffix update/query, use for_prefix(point + 1); 1-index the data.
 */
template <typename T> class binary_indexed_tree {
private:
	std::vector<T> dat;
public:
	binary_indexed_tree() {}
	explicit binary_indexed_tree(size_t N) : dat(N) {}
	binary_indexed_tree(size_t N, const T& t) : dat(N, t) {}

	size_t size() const { return dat.size(); }
	const std::vector<T>& data() const { return dat; }
	std::vector<T>& data() { return dat; }

private:
	template <typename I, typename S = I> struct iterator_range {
	private:
		I begin_;
		S end_;
	public:
		iterator_range() : begin_(), end_() {}
		iterator_range(const I& begin__, const S& end__) : begin_(begin__), end_(end__) {}
		iterator_range(I&& begin__, S&& end__) : begin_(begin__), end_(end__) {}
		I begin() const { return begin_; }
		S end() const { return end_; }
	};

public:
	class const_suffix_iterator {
	private:
		const T* dat;
		int a;
		const_suffix_iterator(const T* dat_, int a_) : dat(dat_), a(a_) {}
		friend class binary_indexed_tree;
	public:
		friend bool operator != (const const_suffix_iterator& i, const const_suffix_iterator& j) {
			assert(j.dat == nullptr);
			return i.a < j.a;
		}
		const_suffix_iterator& operator ++ () {
			a |= a+1;
			return *this;
		}
		const T& operator * () const {
			return dat[a];
		}
	};
	using const_suffix_range = iterator_range<const_suffix_iterator>;
	const_suffix_range suffix(int a) const {
		assert(0 <= a && a <= int(dat.size()));
		return const_suffix_range{const_suffix_iterator{dat.data(), a}, const_suffix_iterator{nullptr, int(dat.size())}};
	}

	class suffix_iterator {
	private:
		T* dat;
		int a;
		suffix_iterator(T* dat_, int a_) : dat(dat_), a(a_) {}
		friend class binary_indexed_tree;
	public:
		friend bool operator != (const suffix_iterator& i, const suffix_iterator& j) {
			assert(j.dat == nullptr);
			return i.a < j.a;
		}
		suffix_iterator& operator ++ () {
			a |= a+1;
			return *this;
		}
		T& operator * () const {
			return dat[a];
		}
	};
	using suffix_range = iterator_range<suffix_iterator>;
	suffix_range suffix(int a) {
		assert(0 <= a && a <= int(dat.size()));
		return suffix_range{suffix_iterator{dat.data(), a}, suffix_iterator{nullptr, int(dat.size())}};
	}

	class const_prefix_iterator {
	private:
		const T* dat;
		int a;
		const_prefix_iterator(const T* dat_, int a_) : dat(dat_), a(a_) {}
		friend class binary_indexed_tree;
	public:
		friend bool operator != (const const_prefix_iterator& i, const const_prefix_iterator& j) {
			assert(j.dat == nullptr);
			return i.a > 0;
		}
		const_prefix_iterator& operator ++ () {
			a &= a-1;
			return *this;
		}
		const T& operator * () const {
			return dat[a-1];
		}
	};
	using const_prefix_range = iterator_range<const_prefix_iterator>;
	const_prefix_range prefix(int a) const {
		return const_prefix_range{const_prefix_iterator{dat.data(), a}, const_prefix_iterator{nullptr, 0}};
	}

	class prefix_iterator {
	private:
		T* dat;
		int a;
		prefix_iterator(T* dat_, int a_) : dat(dat_), a(a_) {}
		friend class binary_indexed_tree;
	public:
		friend bool operator != (const prefix_iterator& i, const prefix_iterator& j) {
			assert(j.dat == nullptr);
			return i.a > 0;
		}
		prefix_iterator& operator ++ () {
			a &= a-1;
			return *this;
		}
		T& operator * () const {
			return dat[a-1];
		}
	};
	using prefix_range = iterator_range<prefix_iterator>;
	prefix_range prefix(int a) {
		return prefix_range{prefix_iterator{dat.data(), a}, prefix_iterator{nullptr, 0}};
	}
};

template <typename F> struct reverse_comparator_t {
	F f;
	template <typename Arg1, typename Arg2> constexpr bool operator() (Arg1&& arg1, Arg2&& arg2) & {
		return f(std::forward<Arg2>(arg2), std::forward<Arg1>(arg1));
	}
	template <typename Arg1, typename Arg2> constexpr bool operator() (Arg1&& arg1, Arg2&& arg2) const& {
		return f(std::forward<Arg2>(arg2), std::forward<Arg1>(arg1));
	}
	template <typename Arg1, typename Arg2> constexpr bool operator() (Arg1&& arg1, Arg2&& arg2) && {
		return std::move(f)(std::forward<Arg2>(arg2), std::forward<Arg1>(arg1));
	}
	template <typename Arg1, typename Arg2> constexpr bool operator() (Arg1&& arg1, Arg2&& arg2) const&& {
		return std::move(f)(std::forward<Arg2>(arg2), std::forward<Arg1>(arg1));
	}
};

template <typename F> constexpr reverse_comparator_t<std::decay_t<F>> reverse_comparator(F&& f) {
	return { std::forward<F>(f) };
}

class CartesianTree {
public:
	struct Node {
		int l, m, r; // inclusive ranges
		std::array<int, 2> c;
	};
	std::vector<Node> nodes;
	int root = -1;

	CartesianTree() {}

	Node& operator [] (int idx) { return nodes[idx]; }
	const Node& operator [] (int idx) const { return nodes[idx]; }

	int size() const { return int(nodes.size()); }

private:
	CartesianTree(std::vector<Node>&& nodes_, int root_) : nodes(std::move(nodes_)), root(root_) {}

public:

	// min-cartesian-tree, with earlier cells tiebroken earlier
	template <typename T, typename Comp = std::less<T>>
	static CartesianTree build_min_tree(const std::vector<T>& v, Comp comp = Comp()) {
		std::vector<Node> nodes(v.size()*2+1);
		std::vector<int> stk; stk.reserve(v.size());
		int root = -1;
		for (int i = 0; i <= int(v.size()); i++) {
			int cur = 2*i;
			nodes[cur].l = i;
			nodes[cur].r = i-1;
			nodes[cur].m = i-1;
			nodes[cur].c = {-1, -1};
			while (!stk.empty() && (i == int(v.size()) || comp(v[i], v[nodes[stk.back()].m]))) {
				int nxt = stk.back(); stk.pop_back();
				nodes[nxt].c[1] = cur;
				nodes[nxt].r = nodes[cur].r;
				cur = nxt;
			}
			if (i == int(v.size())) {
				root = cur;
				break;
			}
			nodes[2*i+1].l = nodes[cur].l;
			nodes[2*i+1].m = i;
			nodes[2*i+1].c[0] = cur;
			stk.push_back(2*i+1);
		}
		return {std::move(nodes), root};
	}

	// max-cartesian-tree, with earlier cells tiebroken earlier
	template <typename T, typename Comp = std::less<T>>
	static CartesianTree build_max_tree(const std::vector<T>& v, Comp comp = Comp()) {
		return build_min_tree(v, reverse_comparator(comp));
	}
};

template <typename T> T mod_inv_in_range(T a, T m) {
	// assert(0 <= a && a < m);
	T x = a, y = m;
	// abs coeff of a in x and y (they're always opposite sign)
	T vx = 1, vy = 0;
	bool swap = false;
	while (x) {
		T k = y / x;
		y %= x;
		vy += k * vx;
		std::swap(x, y);
		std::swap(vx, vy);
		swap ^= 1;
	}
	assert(y == 1);
	return swap ? vy : m - vy;
}

template <typename T> struct extended_gcd_result {
	T gcd;
	T coeff_a, coeff_b;
};
template <typename T> extended_gcd_result<T> extended_gcd(T a, T b) {
	T x = a, y = b;
	// coeff of a and b in x and y
	T ax = 1, ay = 0;
	T bx = 0, by = 1;
	while (x) {
		T k = y / x;
		y %= x;
		ay -= k * ax;
		by -= k * bx;
		std::swap(x, y);
		std::swap(ax, ay);
		std::swap(bx, by);
	}
	return {y, ay, by};
}

template <typename T> T mod_inv(T a, T m) {
	a %= m;
	a = a < 0 ? a + m : a;
	return mod_inv_in_range(a, m);
}

template <int MOD_> struct modnum {
	// Uses subtraction to support MOD up to 2^31 - 1
	static constexpr int MOD = MOD_;
	static_assert(MOD_ > 0, "MOD must be positive");
	struct is_reduced_tag {};

	int v;
	modnum() : v(0) {}
	modnum(int v_, is_reduced_tag) : v(v_) { assert(0 <= v && v < MOD); }
	static modnum from_reduced(int v) { return modnum(v, is_reduced_tag{}); }

	modnum(int v_) : v(int(v_ % MOD)) { if (v < 0) v += MOD; }
	modnum(unsigned v_) : v(int(v_ % MOD)) { }
	modnum(int64_t v_) : v(int(v_ % MOD)) { if (v < 0) v += MOD; }
	modnum(uint64_t v_) : v(int(v_ % MOD)) { }

	explicit operator int() const { return v; }
	int as_signed() const { return MOD-v > v ? v : v - MOD; }
	friend std::ostream& operator << (std::ostream& out, modnum n) { return out << int(n); }
	friend std::istream& operator >> (std::istream& in, modnum& n) { int64_t v_; in >> v_; n = modnum(v_); return in; }

	friend bool operator == (modnum a, modnum b) { return a.v == b.v; }
	friend bool operator != (modnum a, modnum b) { return a.v != b.v; }

	modnum inv() const { return from_reduced(mod_inv_in_range(v, MOD)); }
	friend modnum inv(modnum m) { return m.inv(); }
	modnum neg() const { return from_reduced(v ? MOD-v : 0); }
	friend modnum neg(modnum m) { return m.neg(); }

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
	modnum& operator += (modnum o) {
		v -= MOD-o.v;
		v = (v < 0) ? v + MOD : v;
		return *this;
	}
	modnum& operator -= (modnum o) {
		v -= o.v;
		v = (v < 0) ? v + MOD : v;
		return *this;
	}
	modnum& operator *= (modnum o) {
		v = int(int64_t(v) * int64_t(o.v) % MOD);
		return *this;
	}
	modnum& operator /= (modnum o) {
		return *this *= o.inv();
	}

	friend modnum operator ++ (modnum& a, int) { modnum r = a; ++a; return r; }
	friend modnum operator -- (modnum& a, int) { modnum r = a; --a; return r; }
	friend modnum operator + (modnum a, modnum b) { return modnum(a) += b; }
	friend modnum operator - (modnum a, modnum b) { return modnum(a) -= b; }
	friend modnum operator * (modnum a, modnum b) { return modnum(a) *= b; }
	friend modnum operator / (modnum a, modnum b) { return modnum(a) /= b; }
};

struct mod_goldilocks {
	static constexpr uint64_t MOD = 0xffffffff00000001ull;
	static constexpr uint64_t EPS = -MOD;
	// We have 2^32 is a primitive 6th root of unity.
	// Note that omega_8 + omega_8^7 == 2^24 - 2^72 == sqrt(2)
	// We'll pick the root so that 2^24 - 2^72 is our primitive 384th root of unity.
	static constexpr uint64_t PRIMITIVE_ROOT = 2717;
	struct is_reduced_tag {};
	uint64_t v;
	mod_goldilocks() : v(0) {}
	mod_goldilocks(uint64_t v_, is_reduced_tag) : v(v_) { assert(v < MOD); }
	mod_goldilocks(int64_t a) : v(a < 0 ? a+MOD : a) {}
	mod_goldilocks(int a) : mod_goldilocks(int64_t(a)) {}
	mod_goldilocks(uint64_t a) : v(a >= MOD ? a-MOD : a) {}
	mod_goldilocks(unsigned a) : mod_goldilocks(uint64_t(a)) {}
	mod_goldilocks(__int128_t a) : v(a % MOD < 0 ? uint64_t(MOD - a % MOD) : uint64_t(a % MOD)) {}
	mod_goldilocks(__uint128_t a) : v(uint64_t(a % MOD)) {}

	static mod_goldilocks from_reduced(uint64_t v) {
		return mod_goldilocks(v, is_reduced_tag{});
	}

	explicit operator uint64_t () const { return v; }
	int64_t as_signed() const { return MOD-v > v ? v : int64_t(v - MOD); }
	friend std::ostream& operator << (std::ostream& out, mod_goldilocks n) { return out << uint64_t(n); }

	friend bool operator == (mod_goldilocks a, mod_goldilocks b) { return a.v == b.v; }
	friend bool operator != (mod_goldilocks a, mod_goldilocks b) { return a.v != b.v; }

	mod_goldilocks operator+ () const { return *this; }

	// returns a-b, assuming -MOD <= a-b, e.g. b <= MOD
	static uint64_t sub_mod_raw(uint64_t a, uint64_t b) {
#if defined(__x86_64__)
		// TODO: We could try to write this using intrinsics, but GCC sometimes produces the wrong code.
		uint64_t res_wrapped = a;
		uint64_t adjustment = b;
		asm (
			// AT&T syntax: SRC DST
			"sub %[y], %[x]\n\t"
			// Trick from plonky2 implementation:
			// After sub, flag CF is set iff we underflowed. We want to correct by EPS == 2^32 - 1 iff C is set.
			// sbb (subtract with borrow) computes DST <- DST - SRC - CF
			// Thus, we can use the 32-bit form of sbb on a dummy register to load CF ? EPS : 0.
			// Here, we'll just reuse the original register holding b.
			"sbb %k[y], %k[y]\n\t"
			: [x] "+r"(res_wrapped),
			[y] "+r"(adjustment)
			:
			: "cc"
		);
#else
		uint64_t res_wrapped = a - b;
		uint64_t adjustment = (res_wrapped > a) ? EPS : 0;
#endif
		return res_wrapped - adjustment;
	}

	// Reduce lo + 2^64 * mi + 2^96 * hi, where hi <= MOD
	static uint64_t reduce_u160_raw(uint64_t lo, uint32_t mi, uint64_t hi) {
		// result = lo - hi + EPS * mi
		// 0 <= lo <= 2^64 - 1 = MOD + EPS - 1
		// 0 <= EPS * mi <= (2^32 - 1) * EPS = MOD - 1 - EPS
		// 0 <= hi <= MOD
		// -MOD <= lo - hi + EPS * mi <= 2*MOD-2
		// so we do have some leeway
		return sub_mod_raw(sub_mod_raw(lo, hi), MOD-(uint64_t(mi)<<32)+mi);
	}

	static uint64_t reduce_u128_raw(__uint128_t v) {
		uint64_t hi = uint64_t(v >> 64);
		uint64_t lo = uint64_t(v);
		uint32_t hi_hi = uint32_t(hi >> 32);
		uint32_t hi_lo = uint32_t(hi);
		return reduce_u160_raw(lo, hi_lo, hi_hi);
	}

	mod_goldilocks neg() const { return from_reduced(v ? MOD-v : 0); }
	friend mod_goldilocks neg(const mod_goldilocks& m) { return m.neg(); }
	mod_goldilocks operator- () const { return neg(); }

	mod_goldilocks& operator ++ () {
		++ v;
		if (v == MOD) v = 0;
		return *this;
	}
	mod_goldilocks& operator -- () {
		if (v == 0) v = MOD;
		-- v;
		return *this;
	}
	mod_goldilocks& operator += (mod_goldilocks o) {
		v = sub_mod_raw(v, MOD-o.v);
		return *this;
	}
	mod_goldilocks& operator -= (mod_goldilocks o) {
		v = sub_mod_raw(v, o.v);
		return *this;
	}
	mod_goldilocks& operator *= (mod_goldilocks o) {
		v = reduce_u128_raw(__uint128_t(v) * __uint128_t(o.v));
		return *this;
	}

	friend mod_goldilocks operator ++ (mod_goldilocks& a, int) { mod_goldilocks r = a; ++a; return r; }
	friend mod_goldilocks operator -- (mod_goldilocks& a, int) { mod_goldilocks r = a; --a; return r; }
	friend mod_goldilocks operator + (mod_goldilocks a, mod_goldilocks b) { return mod_goldilocks(a) += b; }
	friend mod_goldilocks operator - (mod_goldilocks a, mod_goldilocks b) { return mod_goldilocks(a) -= b; }
	friend mod_goldilocks operator * (mod_goldilocks a, mod_goldilocks b) { return mod_goldilocks(a) *= b; }

	mod_goldilocks inv() const { return from_reduced(mod_inv_in_range(v, MOD)); }
	friend mod_goldilocks inv(mod_goldilocks m) { return m.inv(); }
	mod_goldilocks& operator /= (mod_goldilocks o) {
		return *this *= o.inv();
	}
	friend mod_goldilocks operator / (mod_goldilocks a, mod_goldilocks b) { return mod_goldilocks(a) /= b; }
};

template <typename T> T power(T a, long long b) {
	assert(b >= 0);
	T r = 1; while (b) { if (b & 1) r *= a; b >>= 1; a *= a; } return r;
}

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);
	using bs = std::bitset<10>;
	using num = modnum<998244353>;
	auto solve = [&](int N, int M, const std::vector<bs>& S) -> num {
		assert(int(S.size()) == N);
		auto popcount = [](bs a) -> int {
			return int(a.count());
		};
		std::vector<int> pref(N+1);
		for (int i = 0; i < N; i++) {
			pref[i+1] = (pref[i] + popcount(S[i])) & 1;
		}
		std::vector<std::array<int, 2>> pref_cnt(N+1);
		std::vector<std::array<int, 2>> suff_cnt(N+1);
		for (int i = 0; i <= N; i++) {
			pref_cnt[i] = i > 0 ? pref_cnt[i-1] : std::array<int, 2>{0, 0};
			pref_cnt[i][pref[i]]++;
		}
		for (int i = N; i >= 0; i--) {
			suff_cnt[i] = i < N ? suff_cnt[i+1] : std::array<int, 2>{0, 0};
			suff_cnt[i][pref[i]]++;
		}

		std::vector<int> odds; odds.reserve(N);
		for (int i = 0; i < N; i++) {
			if (pref[i+1] != pref[i]) {
				odds.push_back(i);
			}
		}
		int K = int(odds.size());
		if (K == 0) return 0; // everything's even already
		assert(K >= 1);

		std::vector<std::optional<int>> extra_cost_lo(N, std::nullopt);
		std::vector<std::optional<int>> extra_cost_hi(N, std::nullopt);
		std::vector<bool> extra_cost(K-1);
		for (int i = 0; i+1 < K; i++) {
			int a = odds[i], b = odds[i+1];
			bool has_extra = S[a] == S[b] && (popcount(S[a]) < M);
			for (int m = a+1; m < b; m++) {
				int v = popcount(S[m] ^ S[a]);
				assert(v & 1);
				if (v > 1) has_extra = false;
			}
			extra_cost[i] = has_extra;
			if (has_extra) {
				extra_cost_lo[a] = i;
				extra_cost_hi[b] = i;
			}
		}

		// Now, it's time to do the funny matching thing
		//for (int i = 0; i+1 < K; i++) { std::cerr << extra_cost[i]; } std::cerr << '\n';

		int M_cap = M - (M & 1);
		num tot_ans = 0;
		for (const bool side : {0, 1}) {
			num ans = 0;
			//std::cerr << '\n' << "doing side " << side << '\n';
			// we're going to find the matching for lo..hi
			struct pt_t {
				bool is_need;
				int loc;
				std::optional<int> is_lo_ex;
				std::optional<int> is_hi_ex;
			};
			std::vector<pt_t> pts; pts.reserve(N*M);
			std::vector<int> valid_ends; valid_ends.reserve(N*M + 1);
			valid_ends.push_back(0);
			for (int i = 0; i < N; i++) {
				if (pref[i] == side) {
					valid_ends.back()++;
				}
				int cnt = popcount(S[i]);
				if (cnt > M_cap) {
					// need 1 here
					pts.push_back({true, i, std::nullopt, std::nullopt});
					valid_ends.push_back(0);
				} else {
					int extras = M_cap - cnt;
					for (int z = 0; z < extras; z++) {
						auto is_lo_ex = z == 0 ? extra_cost_lo[i] : std::nullopt;
						auto is_hi_ex = z == extras-1 ? extra_cost_hi[i] : std::nullopt;
						if (is_hi_ex) {
							// we mark them on the same parity cause it's simpler
							assert(!pts.empty());
							pts.back().is_hi_ex = is_hi_ex;
						}
						pts.push_back({false, i, is_lo_ex, std::nullopt});
						valid_ends.push_back(0);
					}
				}
			}
			if (pref[N] == side) valid_ends.back()++;

			int L = int(pts.size());
			assert(int(valid_ends.size()) == L+1);
			assert(std::accumulate(valid_ends.begin(), valid_ends.end(), 0) == pref_cnt[N][side]);
			std::vector<int> pref_valid_ends(L+2);
			for (int i = 0; i <= L; i++) pref_valid_ends[i+1] = pref_valid_ends[i] + valid_ends[i];
			//std::cerr << "valid "; for (int i = 0; i <= L; i++) { std::cerr << valid_ends[i] << ' '; } std::cerr << '\n';

			// greedy match forwards and backwards
			{
				std::vector<int> loc_bal(N+1);
				loc_bal[0] = 0;
				for (int i = 0; i < N; i++) {
					loc_bal[i+1] = loc_bal[i] + (M_cap-popcount(S[i]));
				}
				{
					int a = *std::min_element(loc_bal.begin(), loc_bal.end());
					for (auto& v : loc_bal) v -= a;
				}
				int MX = *std::max_element(loc_bal.begin(), loc_bal.end()) + 1;
				binary_indexed_tree<std::pair<int, num>> bit(MX);
				int cur_fail_cnt = 0;
				//int64_t tot_pairs = 0;
				for (int i = 0; i <= N; i++) {
					if (pref[i] != side) {
						cur_fail_cnt++;
						continue;
					}
					for (auto it : bit.prefix(loc_bal[i]+1)) {
						//tot_pairs += it.first;
						ans += num(cur_fail_cnt) * num(it.first) - it.second;
					}
					for (auto& it : bit.suffix(loc_bal[i])) {
						it.first ++;
						it.second += num(cur_fail_cnt);
					}
				}
				//std::cerr << "tot_pairs " << tot_pairs << '\n';
			}

			//std::cerr << "baseline side " << ans << '\n';

			struct edge_t {
				int dest;
				int cost;
			};
			std::vector<edge_t> prv(L, {-1, 0});
			std::vector<edge_t> nxt(L, {-1, 0});
			for (bool l_type : {false, true}) {
				std::vector<int> extra_pts;
				int k = 0;
				std::vector<int> stk;
				for (int i = 0; i < L; i++) {
					for (; k <= pts[i].loc; k++) {
						if (pref[k] != side) {
							//std::cerr << "save at " << k << '\n';
							extra_pts.push_back(k);
						}
					}
					if (pts[i].is_need == l_type) {
						stk.push_back(i);
					} else {
						if (!stk.empty()) {
							int j = stk.back();
							stk.pop_back();
							int cost = pts[i].loc - pts[j].loc;
							while (!extra_pts.empty() && extra_pts.back() > pts[j].loc) {
								cost--;
								extra_pts.pop_back();
							}
							//std::cerr << "match ";
							//std::cerr << pts[j].is_need << '@' << pts[j].loc << ' ';
							//std::cerr << pts[i].is_need << '@' << pts[i].loc << ' ';
							//std::cerr << cost << '\n';
							prv[i] = {j, cost};
							nxt[j] = {i, cost};
						}
					}
				}
			}

			std::vector<num> ways_lo(K);
			std::vector<num> ways_hi(K);
			std::vector<int> tie_pair(K, -1);
			std::vector<int> chain; chain.reserve(L);
			for (int st = 0; st < L; st++) {
				if (prv[st].dest != -1) continue;
				chain.clear();
				for (int i = st; i != -1; i = nxt[i].dest) {
					chain.push_back(i);
				}
				int C = int(chain.size());
				//std::cerr << "have chain " << C << '\n';
				//std::cerr << ' '; for (int i : chain) { std::cerr << pts[i].is_need << '@' << pts[i].loc << '/' << pts[i].is_lo_ex.value_or(-1) << '/' << pts[i].is_hi_ex.value_or(-1) << ' '; } std::cerr << '\n';

				std::vector<int> chain_vals(C); // TODO: p sure this should be int
				chain_vals[0] = 0;
				for (int z = 0; z+1 < int(chain.size()); z++) {
					assert(pts[chain[z]].is_need != pts[chain[z+1]].is_need);
					chain_vals[z+1] = chain_vals[z] +
						(pts[chain[z]].is_need ? -1 : +1) * nxt[chain[z]].cost;
				}
				//std::cerr << ' '; for (auto v : chain_vals) { std::cerr << v << ' '; } std::cerr << '\n';
				std::vector<std::array<int, 2>> ends(C+1);
				for (int z = 0; z <= C; z++) {
					int cur_ends = pref_valid_ends[z == C ? L+1 : chain[z] + 1]
						- pref_valid_ends[z == 0 ? 0 : chain[z-1] + 1];
					if (z == C ? !pts[chain[z-1]].is_need : pts[chain[z]].is_need) {
						// we're a peak
						ends[z][true] += cur_ends;
					} else {
						ends[z][false] += cur_ends;
					}
				}
				//std::cerr << ' '; for (auto v : ends) { std::cerr << v[1] << ' '; } std::cerr << '\n';
				//std::cerr << ' '; for (auto v : ends) { std::cerr << v[0] << ' '; } std::cerr << '\n';
				std::vector<std::array<int, 2>> pref_ends = ends;
				std::vector<std::array<int, 2>> suff_ends = ends;
				for (int z = 0; z+1 <= C; z++) {
					pref_ends[z+1][0] += pref_ends[z][0];
					pref_ends[z+1][1] += pref_ends[z][1];
				}
				for (int z = C; z-1 >= 0; z--) {
					suff_ends[z-1][0] += suff_ends[z][0];
					suff_ends[z-1][1] += suff_ends[z][1];
				}
				assert(pref_ends[C][0] + pref_ends[C][1] == pref_valid_ends.back());
				num chain_ans_under = 0;
				num chain_ans_over = 0;
				num chain_ans = 0;
				for (int z = 0; z < C; z++) {
					num v = num(chain_vals[z] * ((pts[chain[z]].is_need) ? +1 : -1));
					chain_ans_under += num(pref_ends[z][0]) * num(suff_ends[z+1][0]) * v;
					chain_ans_over += num(pref_ends[z][1]) * num(suff_ends[z+1][1]) * v;
					chain_ans += num(pref_ends[z][0]) * num(suff_ends[z+1][1]) * v;
				}
				//std::cerr << "chain_ans all " << chain_ans << '\n';
				std::vector<int> want_vals; want_vals.reserve((C+1)/2);
				for (int z = 0; z < C; z++) {
					if (!pts[chain[z]].is_need) {
						assert(int(want_vals.size()) == (z/2));
						// we prefer not to pick the extra ones
						want_vals.push_back(chain_vals[z]);
					}
				}
				auto ct = CartesianTree::build_min_tree(want_vals);
				if (!want_vals.empty()) {
					bool x = !pts[chain[0]].is_need ? 0 : 1;
					assert(!pts[chain[x]].is_need);
					for (int z = x; z < C; z += 2) {
						assert(!pts[chain[z]].is_need);
						int wz = z / 2;
						assert(ct[2*wz+1].m == wz);
						int l = ct[2*wz+1].l * 2 + x;
						int r = ct[2*wz+1].r * 2 + x;
						assert(0 <= l && l <= z && z <= r && r < C);
						num used_times = 1;
						used_times *= pref_ends[z][0] - (l == 0 ? 0 : pref_ends[l-1][0]);
						used_times *= suff_ends[z+1][1] - (r+1 == C ? 0 : suff_ends[r+2][1]);
						//std::cerr << "remove " << z << ' ' << chain_vals[z] << ' ' << l << ' ' << r << ' ' << used_times << '\n';
						chain_ans += used_times * num(chain_vals[z]);
					}
					std::vector<bool> is_par(want_vals.size(), true);
					for (int i = 0; i < int(want_vals.size()); i++) {
						{
							int j = ct[2*i+1].c[0];
							assert(!(j & 1) || want_vals[i] < want_vals[j/2]);
						}
						int j = ct[2*i+1].c[1];
						if (j & 1) assert(want_vals[i] <= want_vals[j/2]);
						if ((j & 1) && want_vals[i] == want_vals[j/2]) {
							is_par[j/2] = false;
						}
					}
					std::vector<int> buf; buf.reserve(C+2);
					for (int z = x; z < C; z += 2) {
						assert(!pts[chain[z]].is_need);
						int wz = z / 2;
						if (!is_par[wz]) continue;
						//std::cerr << "have tie" << '\n';
						assert(ct[2*wz+1].m == wz);
						int l = ct[2*wz+1].l * 2 + x;
						int r = ct[2*wz+1].r * 2 + x;
						assert(0 <= l && l <= z && z <= r && r < C);
						buf.clear();
						buf.push_back(l-1);
						for (int v = z; true; ) {
							buf.push_back(v);
							if (!(ct[2*(v/2)+1].c[1] & 1)) break;
							int nv = (ct[2*(v/2)+1].c[1]/2) * 2 + x;
							if (want_vals[v/2] != want_vals[nv/2]) break;
							v = nv;
						}
						buf.push_back(r+1);
						for (int i = 1; i+1 < int(buf.size()); i++) {
							int j = i;
							while (j+2 < int(buf.size()) && pts[chain[buf[j]]].is_hi_ex && pts[chain[buf[j+1]]].is_lo_ex) {
								tie_pair[*pts[chain[buf[j]]].is_hi_ex] = *pts[chain[buf[j+1]]].is_lo_ex;
								j++;
							}
							int cl = buf[i-1];
							int cr = buf[j+1];
							for (int k = i; k <= j; k++) {
								int v = buf[k];
								if (pts[chain[v]].is_lo_ex) {
									ways_lo[*pts[chain[v]].is_lo_ex] = pref_ends[v][0] - (cl == -1 ? 0 : pref_ends[cl][0]);
								}
								if (pts[chain[v]].is_hi_ex) {
									ways_hi[*pts[chain[v]].is_hi_ex] = suff_ends[v+1][1] - (cr == C ? 0 : suff_ends[cr+1][1]);
								}
							}
							i = j;
						}
						/*
						// now, we have several tied things
						for (int v = z; true; ) {
							if (pts[chain[v]].is_lo_ex) {
								ways_lo[*pts[chain[v]].is_lo_ex] = pref_ends[v][0] - (l == 0 ? 0 : pref_ends[l-1][0]);
							}
							if (pts[chain[v]].is_hi_ex) {
								ways_hi[*pts[chain[v]].is_hi_ex] = suff_ends[v+1][1] - (r+1 == C ? 0 : suff_ends[r+2][1]);
							}

							if (!(ct[2*(v/2)+1].c[1] & 1)) break;
							int nv = (ct[2*(v/2)+1].c[1]/2) * 2 + x;
							if (want_vals[v/2] != want_vals[nv/2]) break;
							std::cerr << "going " << v << ' ' << nv << '\n';
							assert(pts[chain[v]].is_hi_ex);
							assert(pts[chain[nv]].is_lo_ex);
							// mark v and nv as tied
							tie_pair[*pts[chain[v]].is_hi_ex] = *pts[chain[nv]].is_lo_ex;
							v = nv;
						}
						*/
					}
				}
				//std::cerr << "chain_ans_under " << chain_ans_under << '\n';
				//std::cerr << "chain_ans_over " << chain_ans_over << '\n';
				//std::cerr << "chain_ans " << chain_ans << '\n';
				ans += chain_ans + chain_ans_under + chain_ans_over;
			}
			num horiz_ans = 0;
			for (int k = 0; k < K; k++) {
				//std::cerr << "extra " << k << ' ' << ways_lo[k] << ' ' << ways_hi[k] << '\n';
				horiz_ans += ways_lo[k] * ways_hi[k];
			}
			for (int k = 0; k < K; k++) {
				if (tie_pair[k] != -1) {
					int l = tie_pair[k];
					//std::cerr << "tie pair " << k << ' ' << l << '\n';
					horiz_ans -= ways_lo[k] * ways_hi[l];
				}
			}
			//std::cerr << "horiz_ans " << horiz_ans << '\n';
			tot_ans += horiz_ans;
			//std::cerr << "side ans " << ans << '\n';
			tot_ans += ans;
		}

		return tot_ans;
	};

	std::mt19937 mt(48);
	for (int iter = 0; iter < 1000; iter++) {
		int N = 20;
		int M = 3;
		std::vector<bs> S(N);
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < M; j++) {
				S[i][j] = mt() & 1;
			}
		}
		std::cerr << iter << '\n';
		auto mine = solve(N, M, S);
		auto riku = maroon_rk::slv(N, M, S);
		if (mine != riku) {
			std::cerr << "bad " << mine << ' ' << riku << '\n';
			std::cerr << N << ' ' << M << '\n';
			for (int i = 0; i < N; i++) {
				for (int j = 0; j < M; j++) {
					std::cerr << (S[i][j] ? 'O' : '.');
				}
				std::cerr << '\n';
			}
			assert(false);
		}
	}

	int T; std::cin >> T;
	while (T--) {
		int N, M; std::cin >> N >> M;
		assert(M <= int(bs().size()));
		std::vector<bs> S(N);
		for (int i = 0; i < N; i++) {
			std::string r; std::cin >> r;
			assert(int(r.size()) == M);
			for (int j = 0; j < M; j++) {
				assert(r[j] == 'O' || r[j] == '.');
				S[i][j] = r[j] == 'O';
			}
		}
		std::cout << solve(N, M, S) << '\n';
	}


	return 0;
}
