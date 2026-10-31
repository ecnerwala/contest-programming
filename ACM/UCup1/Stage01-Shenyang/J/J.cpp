#include <bits/stdc++.h>
using namespace std;

struct UF {
    int n;
    vector<int> par;
    UF(int _n) : n(_n) {
        for(int i = 0; i < n; i++) par.push_back(i);
    }
    int find(int a){
        if(a != par[a]) par[a] = find(par[a]);
        return par[a];
    }
    bool join(int a, int b){
        a = find(a);
        b = find(b);
        par[a] = b;
        return (a != b);
    }
};

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
	friend modnum inv(const modnum& m) { return m.inv(); }
	modnum neg() const {
		modnum res;
		res.v = v ? MOD-v : 0;
		return res;
	}
	friend modnum neg(const modnum& m) { return m.neg(); }

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

using num = modnum<998244353>;

vector<num> fact, ifact;

void init(){
	int N = 3100000;
	fact = {1};
	for(int i = 1; i < N; i++) fact.push_back(i * fact[i-1]);
	ifact.resize(N);
	ifact.back() = 1 / fact.back();
	for(int i = N - 1; i > 0; i--) ifact[i-1] = i * ifact[i];
}

num ncr(int n, int k){
	if(k < 0 || k > n) return 0;
	return fact[n] * ifact[k] * ifact[n-k];
}

#define rep(i, a, b) for(int i = a; i < (b); ++i)

int minRotation(vector<int> s) {
	int a=0, N=s.size(); for(int i = 0; i < N; i++) s.push_back(s[i]);
	rep(b,0,N) rep(k,0,N) {
		if (a+k == b || s[a+k] < s[b+k]) {b += max(0, k-1); break;}
		if (s[a+k] > s[b+k]) { a = b; break; }
	}
	return a;
}

num calc(vector<vector<int> > A){
	auto get_period = [&](vector<int> v){
		rotate(v.begin(), v.begin() + minRotation(v), v.end());
		rotate(v.begin(), v.begin() + 1, v.end());
		return 1 + minRotation(v);
	};
	auto solve_single_row = [&](vector<int> v){
		assert(v.size() % 2 == 0);
		if(v.empty()) return 1;
		int mod = v.size() + 1;
		v.insert(v.begin(), 0);
		vector<int> vis(mod, 0);
		int period = 1;
		for(int i = 1; i < mod; i++){
			if(vis[i]) continue;
			int x = i;
			vector<int> r;
			while(true){
				r.push_back(v[x]);
				x = (x * 2) % mod;
				vis[x] = 1;
				if(x == i) break;
			}
			period = lcm(period, get_period(r));
		}
		return period;
	};
	int N = A.size();
	int M = A[0].size();
	num i2 = num(1) / num(2);
	num ans = 1;
	vector<vector<int> > single_rows;
	{
		vector<int> x = A[0];
		x.erase(x.begin());
		if(M % 2 == 0) x.pop_back();
		single_rows.push_back(x);
	}
	if(N % 2 == 0){
		vector<int> x = A.back();
		x.erase(x.begin());
		if(M % 2 == 0) x.pop_back();
		single_rows.push_back(x);
	}
	{
		vector<int> x;
		for(int j = 0; j < N; j++) x.push_back(A[j][0]);
		x.erase(x.begin());
		if(N % 2 == 0) x.pop_back();
		single_rows.push_back(x);
	}
	if(M % 2 == 0){
		vector<int> x;
		for(int j = 0; j < N; j++) x.push_back(A[j][M-1]);
		x.erase(x.begin());
		if(N % 2 == 0) x.pop_back();
		single_rows.push_back(x);
	}
	for(vector<int> v : single_rows) ans *= solve_single_row(v);
	if(N % 2 == 0) N--;
	if(M % 2 == 0) M--;
	vector<vector<int> > rows_cycles;
	vector<vector<int> > cols_cycles;
	{
		vector<int> vis(N, 0);
		for(int r = 1; r < N; r++){
			if(vis[r]) continue;
			int c = r;
			vector<int> rows_cycle;
			while(true){
				rows_cycle.push_back(c);
				c = (c * 2) % N;
				vis[c] = 1;
				if(c == r) break;
			}
			rows_cycles.push_back(rows_cycle);
		}
	}
	{
		vector<int> vis(M, 0);
		for(int r = 1; r < M; r++){
			if(vis[r]) continue;
			int c = r;
			vector<int> cols_cycle;
			while(true){
				cols_cycle.push_back(c);
				c = (c * 2) % M;
				vis[c] = 1;
				if(c == r) break;
			}
			cols_cycles.push_back(cols_cycle);
		}
	}
	int R = rows_cycles.size();
	int C = cols_cycles.size();
	if(R && C){
		vector<int> row_only(R, 0);
		vector<int> col_only(C, 0);
		vector<int> row_has(R, 0);
		vector<int> col_has(C, 0);
		UF uf(R + C);
		for(int i = 0; i < R; i++){
			for(int j = 0; j < C; j++){
				map<int, int> freq;
				bool all_distinct = true;
				for(int a : rows_cycles[i]){
					for(int b : cols_cycles[j]){
						freq[A[a][b]]++;
					}
				}
				num ways = 1;
				ways *= fact[rows_cycles[i].size() * cols_cycles[j].size()];
				for(auto [c, f] : freq){
					ways *= ifact[f];
					if(f >= 2) all_distinct = false;
				}
				ans *= ways;
				if(!all_distinct) continue;
				ans *= i2;
				int row_toggle = (rows_cycles[i].size() % 2 == 0);
				int col_toggle = (cols_cycles[j].size() % 2 == 0);
				if(!row_toggle && !col_toggle) continue;
				if(row_toggle) row_has[i] = 1;
				if(col_toggle) col_has[j] = 1;
				if(row_toggle && !col_toggle){
					row_only[i] = 1;
				}
				if(!row_toggle && col_toggle){
					col_only[j] = 1;
				}
				if(row_toggle && col_toggle){
					uf.join(i, R + j);
				}
			}
		}
		int mult = 0;
		for(int x : row_has) mult += x;
		for(int x : col_has) mult += x;
		vector<int> bad(R + C);
		for(int i = 0; i < R; i++) if(row_only[i]) bad[uf.find(i)] = 1;
		for(int i = 0; i < C; i++) if(col_only[i]) bad[uf.find(R + i)] = 1;
		for(int i = 0; i < R; i++){
			if(row_has[i] && uf.find(i) == i && !bad[i]) mult--;
		}
		for(int i = 0; i < C; i++){
			if(col_has[i] && uf.find(R + i) == R + i && !bad[R + i]) mult--;
		}
		ans *= pow(num(2), mult);
	}
	return ans;
}

void solve(){
	int N, M;
	cin >> N >> M;
	vector<vector<int>> A(N, vector<int>(M));
	for(int i = 0; i < N; i++){
		for(int j = 0; j < M; j++){
			cin >> A[i][j];
		}
	}
	num a1 = calc(A);
	vector<vector<int> > B(M, vector<int>(N));
	for(int i = 0; i < N; i++){
		for(int j = 0; j < M; j++){
			B[j][i] = A[i][j];
		}
	}
	num a2 = calc(B);
	assert(a1 == a2);
	cout << a1 << '\n';
}
int main(){
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	init();
	int T;
	cin >> T;
	while(T--) solve();
}
