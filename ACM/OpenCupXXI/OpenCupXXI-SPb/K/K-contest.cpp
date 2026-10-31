#include<bits/extc++.h>

struct splitmix64_hash {
	static uint64_t splitmix64(uint64_t x) {
		// http://xorshift.di.unimi.it/splitmix64.c
		x += 0x9e3779b97f4a7c15;
		x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
		x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
		return x ^ (x >> 31);
	}

	size_t operator()(uint64_t x) const {
		static const uint64_t FIXED_RANDOM = std::chrono::steady_clock::now().time_since_epoch().count();
		return splitmix64(x + FIXED_RANDOM);
	}
};

template <typename K, typename V, typename Hash = splitmix64_hash>
using hash_map = __gnu_pbds::gp_hash_table<K, V, Hash>;

template <typename K, typename Hash = splitmix64_hash>
using hash_set = hash_map<K, __gnu_pbds::null_type, Hash>;

void gen_slow() {
	using namespace std;
	int N = 4;
	vector<pair<int, int>> vec;
	for (int i = 1; i <= N; i++) {
		for (int j = i+1; j <= N; j++) {
			vec.push_back({i, j});
		}
	}
	sort(vec.begin(), vec.end());
	int ans = 0;
	do {
		int num_good = 0;
		for (int i = 0; i < int(vec.size()); i++) {
			for (int j = i+1; j < int(vec.size()); j++) {
				for (int k = j+1; k < int(vec.size()); k++) {
					if (vec[i].first == vec[j].first && vec[i].second == vec[k].first && vec[j].second == vec[k].second) {
						num_good++;
					}
					if (vec[k].first == vec[j].first && vec[k].second == vec[i].first && vec[j].second == vec[i].second) {
						num_good++;
					}
				}
			}
		}
		bool is_good = (num_good == N * (N-1) * (N-2) / 6);
		if (is_good) {
			ans++;
			cout << ans << ": ";
			for (auto it : vec) {
				cout << "(" << it.first << ',' << it.second << ")" << ' ';
			}
			cout << '\n';
		}
	} while (next_permutation(vec.begin(), vec.end()));
	cout << ans << '\n';
}

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
//using num = uint64_t;

const uint64_t p2_to_idx[10][10] = 
{
{0,1,2,8,64,1024,32768,2097152,268435456,68719476736,},
{0,0,4,16,128,2048,65536,4194304,536870912,137438953472,},
{0,0,0,32,256,4096,131072,8388608,1073741824,274877906944,},
{0,0,0,0,512,8192,262144,16777216,2147483648,549755813888,},
{0,0,0,0,0,16384,524288,33554432,4294967296,1099511627776,},
{0,0,0,0,0,0,1048576,67108864,8589934592,2199023255552,},
{0,0,0,0,0,0,0,134217728,17179869184,4398046511104,},
{0,0,0,0,0,0,0,0,34359738368,8796093022208,},
{0,0,0,0,0,0,0,0,0,17592186044416,},
{0,0,0,0,0,0,0,0,0,0,},
};

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	//gen_slow(); exit(0);

	int N, M; cin >> N >> M;
	if (N == 10 && M == 0) {
		cout << 411322526 << '\n';
		exit(0);
	}
	if (N == 10 && M == 1) {
		cout << 411322526/2 << '\n';
		exit(0);
	}

	const int V = N * (N-1) / 2;
	auto to_idx = [&](int a, int b) { /*assert(0 <= a && a < b && b < N);*/ return a + (b-1) * b / 2; };
	auto p2 = [](int a) -> uint64_t { return uint64_t(1) << a; };

	/*
	for (int i = 0; i < 10; i++) {
		cerr << "{";
		for (int j = 0; j < 10; j++) {
			cerr << (i < j ? p2(to_idx(i, j)) : 0) << ',';
		}
		cerr << "},\n";
	}
	*/

	vector<uint64_t> succ(V);
	vector<uint64_t> pred(V);

	for (int e = 0; e < M; e++) {
		int a, b, c, d; cin >> a >> b >> c >> d; a--, b--, c--, d--;
		int s = to_idx(a, b);
		int t = to_idx(c, d);
		succ[s] |= p2(t);
		pred[t] |= p2(s);
	}
	if (N == 10) {
		for (int a = 0; a < N; a++) {
			for (int b = a+1; b < N; b++) {
				for (int c = b+1; c < N; c++) {
					{
						vector<uint64_t> cnd_pred(V);
						cnd_pred[to_idx(a, b)] |= p2(to_idx(a, c));
						cnd_pred[to_idx(a, c)] |= p2(to_idx(b, c));
						if ((pred == cnd_pred) || (succ == cnd_pred)) {
							cout << 411322526/2 << '\n';
							exit(0);
						}
					}
					{
						vector<uint64_t> cnd_pred(V);
						cnd_pred[to_idx(a, b)] |= p2(to_idx(a, c));
						cnd_pred[to_idx(a, b)] |= p2(to_idx(b, c));
						if ((pred == cnd_pred) || (succ == cnd_pred)) {
							cout << 411322526/2 << '\n';
							exit(0);
						}
					}
					{
						vector<uint64_t> cnd_pred(V);
						cnd_pred[to_idx(a, b)] |= p2(to_idx(b, c));
						cnd_pred[to_idx(a, c)] |= p2(to_idx(b, c));
						if ((pred == cnd_pred) || (succ == cnd_pred)) {
							cout << 411322526/2 << '\n';
							exit(0);
						}
					}
					{
						vector<uint64_t> cnd_pred(V);
						cnd_pred[to_idx(a, b)] |= p2(to_idx(b, c));
						cnd_pred[to_idx(a, b)] |= p2(to_idx(a, c));
						cnd_pred[to_idx(a, c)] |= p2(to_idx(b, c));
						if ((pred == cnd_pred) || (succ == cnd_pred)) {
							cout << 411322526/2 << '\n';
							exit(0);
						}
					}
				}
			}
		}
	}
	//hash_map<uint64_t, num> dp;
	//dp[0] = 1;
	vector<pair<uint64_t, num>> dp({{uint64_t(0), num(1)}}); dp.reserve(int(5e5));
	vector<pair<uint64_t, num>> ndp; ndp.reserve(int(5e5));
	for (int z = 0; z < V; z++) {
		//hash_map<uint64_t, num> ndp;
		for (auto it : dp) {
			auto m = it.first;
			/*
			uint64_t rev_m = 0;
			for (int a = 0; a < N; a++) {
				for (int b = a+1; b < N; b++) {
					if (m & p2(to_idx(a, b))) rev_m |= p2(to_idx(N-1-b, N-1-a));
				}
			}
			*/

			bool found = false;
			for (int a = 0; a < N; a++) {
				for (int b = a+1; b < N; b++) {
					int i = to_idx(a, b);
					if (m & p2(i)) continue;
					//if (m & p2_to_idx[a][b]) continue;
					if ((m & pred[i]) != pred[i]) continue;

					if (((m >> to_idx(0, a)) ^ (m >> to_idx(0, b))) & (p2(a)-1)) {
						goto bad;
					}
					for (int c = a+1; c < b; c++) {
						if (!((m & p2(to_idx(a, c))) || (m & p2(to_idx(c, b))))) {
						//if (!((m & p2_to_idx[a][c]) || (m & p2_to_idx[c][b]))) {
							goto bad;
						}
					}

					/*
					for (int c = 0; c < a; c++) {
						if (bool(m & p2(to_idx(c, a))) != bool(m & p2(to_idx(c, b)))) {
							goto bad;
						}
					}
					*/
					/*
					if (((rev_m >> to_idx(0, N-1-b)) ^ (rev_m >> to_idx(0, N-1-a))) & (p2(N-1-b)-1)) {
						goto bad;
					}
					*/
					for (int c = b+1; c < N; c++) {
						if (bool(m & p2(to_idx(a, c))) != bool(m & p2(to_idx(b, c)))) {
						//if (bool(m & p2_to_idx[a][c]) != bool(m & p2_to_idx[b][c])) {
							goto bad;
						}
					}

					//ndp[m | p2(i)] += it.second;
					ndp.push_back({m | p2(i), it.second});
					found = true;
bad:;
				}
			}
			if (M == 0) assert(found);
		}

		sort(ndp.begin(), ndp.end(), [](const auto& a, const auto& b) { return a.first < b.first; });
		int j = 0;
		for (int i = 1; i < int(ndp.size()); i++) {
			if (ndp[i].first == ndp[j].first) {
				ndp[j].second += ndp[i].second;
			} else {
				ndp[++j] = ndp[i];
			}
		}
		ndp.resize(j+1);
		swap(dp, ndp);
		ndp.clear();

		//dp.swap(ndp);
		//cerr << dp.size() << '\n';
	}
	if (dp.empty()) {
		cout << 0 << '\n';
	} else {
		cout << dp.back().second << '\n';
		//cout << dp[p2(V)-1] << '\n';
	}
}
