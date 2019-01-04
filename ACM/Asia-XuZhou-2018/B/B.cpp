#include <bits/stdc++.h>
using namespace std;
typedef long long LL;

#define rep(i, a, b) for(int i = a; i < (b); ++i)
#define trav(a, x) for(auto& a : x)
#define all(x) x.begin(), x.end()
#define sz(x) (int)(x).size()
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;

using ll = long long;

template <int MOD_> struct modnum {
	static const int MOD = MOD_;

private:
	int v;
public:
	modnum() : v(0) {}
	modnum(int v_) : v(int(v_ % MOD)) { if (v < 0) v += MOD; }
	int get(){
		return v;
	}
	friend std::ostream& operator << (std::ostream& o, const modnum& m) { return o << m.v; }

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

	friend modnum operator + (const modnum& a, const modnum& b) { return modnum(a) += b; }
	friend modnum operator - (const modnum& a, const modnum& b) { return modnum(a) -= b; }
	friend modnum operator * (const modnum& a, const modnum& b) { return modnum(a) *= b; }

	friend bool operator < (const modnum& a, const modnum& b) { return a.v < b.v; }
	friend bool operator > (const modnum& a, const modnum& b) { return a.v > b.v; }
	friend bool operator == (const modnum& a, const modnum& b) { return a.v == b.v; }
};
const ll MOD = int(1e9) + 7;
using num = modnum<MOD>;

num powmod(num n, ll a){
	ll f = a;
	if(a == 0) return 1;
	if(a & 1) return n * powmod(n, f-1);
	num c = powmod(n, f/2);
	return c * c;
}


num inv(num x){
	return powmod(x, MOD - 2);
}


int h(vector<int> x){
	num v = 1;
	for(ll r : x){
		v += powmod(10, r);
	}
	return v.get();
}



typedef pair<int, vector<int> > tree;


vector<tree> all_trees;

vector<vector<int> > trans;


vector<num> coeff;

vector<num> solve(vector<vector<int> > edges){
	int n = edges.size();
	int t = all_trees.size();
	vector<vector<num> > ret(n, vector<num>(t, 0));
	for(int i = 0; i < n; i++){
		ret[i][0] = 1;
	}
	for(int d = 1; d < t; d++){
		for(int i = 0; i < n; i++){
			num q = 1;
			for(int c : trans[d]){
				num res = 0;
				for(auto v : edges[i]){
					res += ret[v][c];
				}
				q = q * res;
			}
			ret[i][d] = q;
		}
	}
	vector<num> sums(t, 0);
	for(int d = 0; d < t; d++){
		for(int i = 0; i < n; i++){
			sums[d] += ret[i][d];
		}
	}
	return sums;
}

bool share(pair<int,int> a, pair<int,int> b){
	return a.first == b.first || a.first == b.second || a.second == b.first || a.second == b.second;
}

vector<vector<int> > l(vector<vector<int> > edges){
	vector<pair<int,int> > e;
	for(int i = 0; i < edges.size(); i++){
		for(int a : edges[i]){
			if(a < i) e.push_back({i, a});
		}
	}
	int n = e.size();
	vector<vector<int> > graph(n);
	for(int a = 0; a < e.size(); a++){
		for(int b = 0; b < e.size(); b++){
			if(a == b) continue;
			if(share(e[a], e[b])){
				graph[a].push_back(b);
			}
		}
	}
	return graph;
}

num countl4(vector<vector<int> > edges){
	int n = edges.size();
	vector<num> d1(n, 0);
	vector<num> d2(n, 0);
	vector<num> d22(n, 0);
	for(int i = 0; i < n; i++){
		for(int a : edges[i]) d1[i] += 1;
	}
	for(int i = 0; i < n; i++){
		for(int a : edges[i]) d2[i] += d1[a];
		for(int a : edges[i]) d22[i] += d1[a] * d1[a];
	}
	num ret = 0;
	for(int i = 0; i < n; i++){
		num val = 0;
		val = (2 * d1[i] * d1[i] - 13 * d1[i] + 21 + 4 * d2[i]) * d1[i] * (d1[i] - 1);
		val -= 13 * (d1[i] - 1) * d2[i];
		val += (d1[i] - 2) * d22[i];
		val += d2[i] * d2[i];
		ret += val;
	}
	ret = ret * inv(2);
	return ret;
}
num countl5(vector<vector<int> > edges){
	return countl4(l(edges));
}

num countl6(vector<vector<int> > edges){
	return countl4(l(l(edges)));
}

typedef vector<num> vd;

int solveLinear(vector<vd>& A, vd& b, vd& x) {
	int n = sz(A), m = sz(x), rank = 0, br, bc;
	if (n) assert(sz(A[0]) == m);
	vi col(m); iota(all(col), 0);
	
	rep(i,0,n) {
		num v, bv = 0;
		rep(r,i,n) rep(c,i,m)
			if ((v = A[r][c]) > bv)
				br = r, bc = c, bv = v;
		if (bv == 0) {
			rep(j,i,n) if (b[j] > 0) return -1;
			break;
		}
		swap(A[i], A[br]);
		swap(b[i], b[br]);
		swap(col[i], col[bc]);
		rep(j,0,n) swap(A[j][i], A[j][bc]);
		bv = inv(A[i][i]);
		rep(j,i+1,n) {
			num fac = A[j][i] * bv;
			b[j] -= fac * b[i];
			rep(k,i+1,m) A[j][k] -= fac*A[i][k];
		}
		rank++;
	}
	
	x.assign(m, 0);
	for (int i = rank; i--;) {
		b[i] = b[i] * inv(A[i][i]);
		x[col[i]] = b[i];
		rep(j,0,i) b[j] -= A[j][i] * b[i];
	}
	//cerr << "rank " << rank << endl;
	return rank;
}


void init(){
	set<tree> stuff;
	// size, val,
	tree empty = {0, {}};
	stuff.insert(empty);
	all_trees.push_back(empty);
	while(1){
		set<tree> newstuff = stuff;
		for(tree y : stuff){
			for(tree x : stuff){
				x.first += 1 + y.first;
				x.second.push_back(h(y.second));
				sort(x.second.begin(), x.second.end());
				if(x.first > 6) continue;
				if(newstuff.find(x) == newstuff.end()){
					newstuff.insert(x);
					all_trees.push_back(x);
				}
			}
		}
		if(newstuff.size() == stuff.size()) break;
		stuff = newstuff;
	}
	//cerr << stuff.size() << endl;
	// make trans the correct thing
	
	map<int,int> l;
	
	for(int i = 0; i < all_trees.size(); i++){
		l[h(all_trees[i].second)] = i;
	}
	for(int i = 0; i < all_trees.size(); i++){
		vector<int> cur;
		for(int x : all_trees[i].second){
			cur.push_back(l[x]);
		}
		trans.push_back(cur);
		/*cout << i << " = ";
		for(int x : cur){
			cout << x << " ";
		}
		cout << endl;*/
	}
	
	vector<vd> A;
	vd b;
	vd x;
	for(int r = 0; r < 1000; r++){
		vector<vector<int> > graph(7);
		for(int i = 0; i < 7; i++){
			for(int j = i+1; j < 7; j++){
				if(rand() % 5){
					graph[i].push_back(j);
					graph[j].push_back(i);
				}
			}
		}
		vector<num> val = solve(graph);
		num rhs = countl6(graph);
		//cout << rhs << endl;
		A.push_back(val);
		b.push_back(rhs);
	}
	x.resize(all_trees.size(), 0);
	solveLinear(A, b, x);
	coeff = x;
	/*for(int r = 0; r < 1000; r++){
		int z = 30;
		vector<vector<int> > graph(z);
		for(int i = 0; i < z; i++){
			for(int j = i+1; j < z; j++){
				if(rand() % 2){
					graph[i].push_back(j);
					graph[j].push_back(i);
				}
			}
		}
		vector<num> val = solve(graph);
		num rhs = countl6(graph);
		num f = 0;
		for(int i = 0; i < val.size(); i++){
			f += val[i] * coeff[i]; 
		}
		//cout << f << " " << rhs << endl;
		assert(f == rhs);
	}*/
}

int main(){
	cin.sync_with_stdio(0); cin.tie(0); cout.tie(0);
	init();
	int T;
	cin >> T;
	for(int t = 0; t < T; t++){
		int n, m;
		cin >> n >> m;
		vector<vector<int> > edges(n);
		for(int i = 0; i < m; i++){
			int u, v;
			cin >> u >> v;
			u--; v--;
			edges[u].push_back(v);
			edges[v].push_back(u);
		}
		vector<num> vals = solve(edges);
		num ans = 0;
		for(int i = 0; i < vals.size(); i++){
			ans += vals[i] * coeff[i];
		}
		cout << ans << '\n';
	}
}
