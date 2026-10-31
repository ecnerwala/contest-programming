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
using num = modnum<int(1e9)+7>;

template <typename T> T pow(T a, long long b) {
	assert(b >= 0);
	T r = 1; while (b) { if (b & 1) r *= a; b >>= 1; a *= a; } return r;
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int N, K, M; cin >> N >> K >> M;
	vector<int> A(M);
	for (int i = 0; i < M; i++) { cin >> A[i]; A[i] --; }

	auto numBad = [&]() -> num {
		map<int, int> seen;
		int lastSeen = -1;
		int firstSeen = -1;
		for (int i = 0; i < M; i++) {
			if (seen.count(A[i])) {
				lastSeen = max(lastSeen, seen[A[i]]);
				if (firstSeen == -1) firstSeen = i;
			}
			if (i - lastSeen >= K) {
				return 0;
			}
			seen[A[i]] = i;
		}
		if (lastSeen == -1) {
			// all distinct
			assert(M < K);
			vector<vector<pair<num,num>>> dp(N+1, vector<pair<num,num>>(K));
			dp[0][0].first = 1;
			for (int i = 0; i < N; i++) {
				for (int v = K-1; v >= 1; v--) {
					dp[i+1][v].first += dp[i][v].first;
					dp[i+1][v].second += dp[i][v].second;
					if (v > 1) dp[i+1][v-1].first += dp[i+1][v].first;
					if (v > 1) dp[i+1][v-1].second += dp[i+1][v].second;
				}
				for (int v = 1; v < K; v++) {
					dp[i+1][v].first += dp[i][v-1].first * num(K-v+1);
					dp[i+1][v].second += dp[i][v-1].second * num(K-v+1);
				}
				for (int v = M; v < K; v++) {
					dp[i+1][v].second += dp[i+1][v].first;
				}
			}
			num ans = 0;
			for (int v = 0; v < K; v++) {
				ans += dp[N][v].second;
			}
			num den = 1;
			for (int i = K-M+1; i <= K; i++) den *= i;
			ans /= den;
			return ans;
		} else {
			num ans = 0;
			int suffVal = M-1 - lastSeen;
			int prefVal = firstSeen;
			assert(1 <= prefVal && prefVal < K);
			assert(1 <= suffVal && suffVal < K);
			vector<vector<num>> pdp(N-M+1, vector<num>(K));
			vector<vector<num>> sdp(N-M+1, vector<num>(K));
			pdp[0][prefVal] = 1;
			sdp[0][suffVal] = 1;
			for (int i = 0; i < N-M; i++) {
				for (int v = K-1; v >= 1; v--) {
					pdp[i+1][v] += pdp[i][v];
					sdp[i+1][v] += sdp[i][v];
					if (v > 1) pdp[i+1][v-1] += pdp[i+1][v];
					if (v > 1) sdp[i+1][v-1] += sdp[i+1][v];
				}
				for (int v = 1; v < K; v++) {
					pdp[i+1][v] += pdp[i][v-1] * num(K-v+1);
					sdp[i+1][v] += sdp[i][v-1] * num(K-v+1);
				}
			}
			for (int i = 0; i <= N-M; i++) {
				num p = 0;
				num s = 0;
				for (int v = 1; v < K; v++) {
					p += pdp[i][v];
					s += sdp[N-M-i][v];
				}
				ans += p * s;
			}
			return ans;
		}
	}();

	cout << num(N - M + 1) * pow(num(K), N - M) - numBad << '\n';


	return 0;
}
