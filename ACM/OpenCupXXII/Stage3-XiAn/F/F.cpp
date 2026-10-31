#include <bits/stdc++.h>
using namespace std;

// Floor of log_2(a); index of highest 1-bit
inline int log_2(int a) {
	return a ? (8 * sizeof(a)) - 1 - __builtin_clz(a) : -1;
}

inline int next_pow_2(int a) {
	assert(a > 0);
	return 1 << log_2(2*a-1);
}

template <typename T> T mod_inv_in_range(T a, T m) {
	// assert(0 <= a && a < m);
	T x = a, y = m;
	T vx = 1, vy = 0;
	while (x) {
		T k = y / x;
		y %= x;
		vy -= k * vx;
		std::swap(x, y);
		std::swap(vx, vy);
	}
	assert(y == 1);
	return vy < 0 ? m + vy : vy;
}

template <typename T> T mod_inv(T a, T m) {
	a %= m;
	a = a < 0 ? a + m : a;
	return mod_inv_in_range(a, m);
}

template <int MOD_> struct modnum {
	static constexpr int MOD = MOD_;
	static_assert(MOD_ > 0, "MOD must be positive");

private:
	using ll = long long;

	int v;

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
		res.v = mod_inv_in_range(v, MOD);
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
		v -= MOD-o.v;
		v = (v < 0) ? v + MOD : v;
		return *this;
	}
	modnum& operator -= (const modnum& o) {
		v -= o.v;
		v = (v < 0) ? v + MOD : v;
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

using num = modnum<int(1e9) + 7>;

using ll = int64_t;

using i128 = __int128_t;

const int N = int(4e6) + 5;

vector<num> fact, ifact;
vector<num> POW33(2*N), INV2(2*N), INV(2*N);

void init(){
	fact = {1};
	for(int i = 1; i < 2*N; i++) fact.push_back(i * fact[i-1]);
	ifact.resize(2*N);
	ifact.back() = 1 / fact.back();
	for(int i = 2*N - 1; i > 0; i--) ifact[i-1] = i * ifact[i];

	for(int i = 0; i < 2*N; i++){
		POW33[i] = pow(num(i), 33);
		if(i > 0){
			INV[i] = ifact[i] * fact[i-1];
			INV2[i] = INV[i] * INV[i];
		}
	}
}

using vi = vector<num>;
using pii = pair<num, num>;
#define sz(x) (int)(x).size()
#define rep(i, a, b) for(int i = a; i < (b); ++i)

void solve(){
	ll qa, qb, qc, qd, qe, qf;
	cin >> qa >> qb >> qc >> qd >> qe >> qf;
	ll maxX = N;
	for(ll x = 0; x < N; x++){
		ll A = qc; // <= 1e2
		ll B = -2*qc*qd + qe*(x-qb); // <= 1e9
		ll C = qc*qd*qd - qd*qe*(x-qb) + qa*(x-qb)*(x-qb) - qf; // <= 1e15
		if(x > qb && i128(B) * i128(B) - 4 * i128(A) * i128(C) < 0){
			maxX = x;
			break;
		}
	}
	maxX = max(maxX, ll(1));
	vector<pair<ll, ll> > intervals(maxX, {0, 0});
	for(ll x = 0; x < maxX; x++){
		ll A = qc; // <= 1e2
		ll B = -2*qc*qd + qe*(x-qb); // <= 1e9
		ll C = qc*qd*qd - qd*qe*(x-qb) + qa*(x-qb)*(x-qb) - qf; // <= 1e15
		ll vtx = -B / A / 2;
		auto inside = [&](ll y) -> bool {
			return A*y*y + B*y + C <= 0;
		};
		bool found = false;
		for(ll j = vtx - 2; j <= vtx + 2; j++){
			if(inside(j)){
				vtx = j;
				found = true;
				break;
			}
		}
		if(!found) continue;
		ll lows = -1;
		ll lowe = vtx;
		while(lows + 1 < lowe){
			ll lowm = (lows + lowe) / 2;
			if(inside(lowm)){
				lowe = lowm;
			} else {
				lows = lowm;
			}
		}
		ll highs = vtx;
		ll highe = N;
		while(highs + 1 < highe){
			ll highm = (highs + highe) / 2;
			if(inside(highm)){
				highs = highm;
			} else {
				highe = highm;
			}
		}
		intervals[x] = {lowe, highe};
	}
	// for(ll x = 0; x < maxX; x++){
	// 	auto [ya, yb] = intervals[x];
	// 	for(ll y = ya; y < yb; y++){
	// 		// ans += pow(num(x ^ y), 33) / (x * x) / y;
	// 		cerr << "real " << x << ' ' << y << '\n';
	// 	}
	// }
	ll maxY = 0;
	for (ll x = 0; x < maxX; x++) {
		if (intervals[x].first < intervals[x].second) {
			maxY = std::max(maxY, intervals[x].second);
		}
	}
	ll maxZ = next_pow_2(int(std::max(maxX, maxY)));
	std::vector<num> vx(INV2.begin(), INV2.begin()+maxX);
	std::vector<num> vy(INV.begin(), INV.begin()+maxY);
	std::vector<num> vz(POW33.begin(), POW33.begin()+maxZ);

	num ans = 0;
	for(int it = 0; ; it++){
		//cerr << "it " << it << '\n';
		while(!intervals.empty() && intervals.back().first == intervals.back().second){
			intervals.pop_back();
		}
		if(intervals.empty()) break;
		if(intervals.size() & 1) intervals.push_back({0, 0});
		int n = (int)intervals.size();
		vector<pair<ll, ll> > to_process;
		vector<pair<ll, ll> > new_intervals(n / 2);
		for(int x = 0; x < n; x += 2){
			auto [ys0, ye0] = intervals[x];
			auto [ys1, ye1] = intervals[x+1];
			// cerr << ys0 << ' ' << ye0 << ' ' << ys1 << ' ' << ye1 << '\n';
			ll ys = max(ys0, ys1);
			ll ye = min(ye0, ye1);
			while(ys & 1) ys++;
			while(ye & 1) ye--;
			// cerr << ys0 << ' ' << ye0 << ' ' << ys1 << ' ' << ye1 << '\n';
			auto decrease = [&](ll cx, ll fs, ll fe, ll ts, ll te){
				while(fs < ts){
					to_process.push_back({cx, fs});
					fs++;
				}
				while(fe > te){
					fe--;
					to_process.push_back({cx, fe});
				}
			};
			if(ys < ye){
				decrease(x, ys0, ye0, ys, ye);
				decrease(x+1, ys1, ye1, ys, ye);
				new_intervals[x/2] = {ys / 2, ye / 2};
			} else {
				decrease(x, ys0, ye0, ye0, ye0);
				decrease(x+1, ys1, ye1, ye1, ye1);
				new_intervals[x/2] = {0, 0};
			}
		}
		num cur_ans = 0;
		for(auto [xtop, ytop] : to_process){
			auto ztop = xtop ^ ytop;
			assert(((xtop + 1) << it) <= maxX);
			assert(((ytop + 1) << it) <= maxY);
			assert(((ztop + 1) << it) <= maxZ);

			for(int i = 0; i < (1 << it); i++){
				cur_ans += vx[(xtop << it) + i] * vy[(ytop << it) + i] * vz[(ztop << it) + i];
			}
		}
		cur_ans *= INV[(1<<it)];
		ans += cur_ans;

		{
			int step = (1 << it);
			for (int i = 0; i + 2 * step <= maxX; i += 2 * step) rep(j,i,i+step) {
				num &u = vx[j], &v = vx[j + step]; tie(u, v) =
					pii(u + v, u - v);                   // XOR /// include-line
			}
			for (int i = 0; i + 2 * step <= maxY; i += 2 * step) rep(j,i,i+step) {
				num &u = vy[j], &v = vy[j + step]; tie(u, v) =
					pii(u + v, u - v);                   // XOR /// include-line
			}
			for (int i = 0; i + 2 * step <= maxZ; i += 2 * step) rep(j,i,i+step) {
				num &u = vz[j], &v = vz[j + step]; tie(u, v) =
					pii(u + v, u - v);                   // XOR /// include-line
			}
		}
		intervals = std::move(new_intervals);
	}
	cout << ans << '\n';
}

int main(){
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	init();
	int T;
	cin >> T;
	while(T--) solve();
}
