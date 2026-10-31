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

using num = modnum<998244353>;

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

int main(){
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int n;
	cin >> n;
	vector<int> c(n);
	for(int i = 0; i < n; i++){
		cin >> c[i];
		c[i]--;
	}
	vector<vector<int> > edges(n);
	for(int i = 0; i < n-1; i++){
		int u, v;
		cin >> u >> v;
		u--; v--;
		edges[u].push_back(v);
		edges[v].push_back(u);
	}
	num ans = 0;
	for(int color = 0; color < n; color++){
		vector<int> balance(n, 0);
		for(int i = 0; i < n; i++){
			balance[i] = (c[i] == color) ? 1 : -1;
		}
		vector<int> best_down(n, 0);
		y_combinator(
			[&](auto self, int v, int p) -> void {
				best_down[v] = balance[v];
				for(int w : edges[v]){
					if(w == p) continue;
					self(w, v);
					best_down[v] += best_down[w];
				}
				best_down[v] = max(0, best_down[v]);
			}
		)(0, -1);
		vector<int> best_up(n, 0);
		y_combinator(
			[&](auto self, int v, int p) -> void {
				int tsum = balance[v] + best_up[v];
				for(int w : edges[v]){
					if(w == p) continue;
					tsum += best_down[w];
				}
				for(int w : edges[v]){
					if(w == p) continue;
					best_up[w] = max(0, tsum - best_down[w]);
				}
				for(int w : edges[v]){
					if(w == p) continue;
					self(w, v);
				}
			}
		)(0, -1);
		y_combinator(
			[&](auto self, int v, int p) -> pair<int, vector<num> > {
				pair<int, vector<num> > dp;
				dp.first = balance[v];
				dp.second = {1};
				int csum = best_up[v];
				for(int w : edges[v]){
					if(w == p) continue;
					csum += best_down[w];
				}
				for(int w : edges[v]){
					if(w == p) continue;
					csum -= best_down[w];
					pair<int, vector<num> > cdp = self(w, v);
					while(cdp.first < 0){
						cdp.second.insert(cdp.second.begin(), 0);
						cdp.first += 1;
					}
					if(cdp.first >= cdp.second.size()) cdp.second.resize(cdp.first+1, 0);
					cdp.second[cdp.first] += 1;
					pair<int, vector<num> > ndp = {dp.first + cdp.first, vector<num>(cdp.second.size() + ndp.second.size(), 0)};
					for(int i = 0; i < cdp.second.size(); i++){
						for(int j = 0; j < dp.second.size(); j++){
							if((cdp.first - i) + (dp.first - j) + csum <= 0) break;
							ndp.second[i + j] += cdp.second[i] * dp.second[j];
						}
					}
					while(!ndp.second.empty() && ndp.second.back() == 0) ndp.second.pop_back();
					// map<int, num> ndp;
					// for(auto [cx, cy] : cdp){
					// 	for(auto [x, y] : dp){
					// 		if((-cx + -x) + csum <= 0) break;
					// 		ndp[cx + x] += cy * y;
					// 	}
					// }
					dp = ndp;
				}
				for(int i = 0; i < (int)dp.second.size(); i++){
					int sum = dp.first - i;
					if(sum > 0){
						ans += dp.second[i];
					}
				}
				return dp;
			}
		)(0, -1);
	}
	cout << ans << '\n';
}
