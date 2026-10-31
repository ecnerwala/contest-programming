#include<bits/stdc++.h>
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

template <typename T> T pow(T a, long long b) {
	assert(b >= 0);
	T r = 1; while (b) { if (b & 1) r *= a; b >>= 1; a *= a; } return r;
}

using num = modnum<int(1e9)+7>;

const int MAXN = 5.1e3;
const int MAXM = 5.1e3;
int N, M;
int S[MAXN];

vector<int> locs[MAXN];
vector<int> H[MAXN];

int best[MAXN];
num ways[MAXN];

int ways2[MAXN];
int ways2Start[MAXN];
int ways1[MAXN];
int ways1Start[MAXN];

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N >> M;
	for (int i = 0; i < N; i++) {
		cin >> S[i];
		S[i] --;
		locs[S[i]].push_back(i);
	}
	for (int m = 0; m < M; m++) {
		int f, h; cin >> f >> h; f--;
		if (h <= int(locs[f].size())) {
			H[f].push_back(h);
		}
	}

	for (int z = 0; z <= N; z++) best[z] = 0, ways[z] = 1;

	for (int v = 0; v < N; v++) {
		for (int i = 0; i <= N; i++) {
			ways2[i] = ways2Start[i] = 0;
			ways1[i] = ways1Start[i] = 0;
		}

		for (int i : H[v]) {
			for (int j : H[v]) {
				if (i == j) continue;
				if (i + j > int(locs[v].size())) continue;
				int a = locs[v][i-1];
				int b = locs[v][int(locs[v].size())-j];
				ways2[a]++;
				ways2[b]--;

				ways2Start[a]++;
			}
		}

		for (int i : H[v]) {
			assert(i <= int(locs[v].size()));
			int a = locs[v][i-1];
			ways1[a]++;
			ways1[N]--;
			ways1Start[a]++;

			int b = locs[v][int(locs[v].size())-i];
			ways1[0]++;
			ways1[b]--;
		}

		for (int i = 0; i < N; i++) {
			ways2[i+1] += ways2[i];
			if (S[i] == v) {
				ways2[i] = 0;
			}

			ways2[i] += ways2Start[i];

			ways1[i+1] += ways1[i];
			if (S[i] == v) {
				ways1[i] = 0;
			}

			ways1[i] += ways1Start[i];

			if (ways2[i]) {
				best[i] += 2;
				ways[i] *= num(ways2[i]);
			} else if (ways1[i]) {
				best[i] += 1;
				ways[i] *= num(ways1[i]);
			} else {
				best[i] += 0;
				ways[i] *= num(S[i] != v);
			}
		}

		// no lefts
		if (!H[v].empty()) {
			best[N]++;
			ways[N] *= num(int(H[v].size()));
		}
	}

	int ans = *max_element(best, best + N + 1);
	num ansWays = 0;
	for (int i = 0; i <= N; i++) {
		if (best[i] == ans) ansWays += ways[i];
	}
	cout << ans << ' ' << ansWays << '\n';

	return 0;
}
