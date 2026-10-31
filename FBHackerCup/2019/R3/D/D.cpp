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

const int MAXN = 1.1e6;
int N;
int P[MAXN];
vector<int> ch[MAXN];

const int S = 1 << 20;
int seg[S * 2];
int seg2[S * 2];

int st[MAXN];
int en[MAXN];
int numCh[MAXN];
void dfs(int cur, int depth, int& ind) {
	st[cur] = ind;
	ind++;
	numCh[cur] ++;
	for (int nxt : ch[cur]) {
		numCh[cur]++;
		dfs(nxt, depth + 1, ind);
	}
	en[cur] = ind;
}

int query(int l, int r) {
	int ans = 0;
	for (int a = l + S, b = r + S; a < b; a /= 2, b /= 2) {
		if (a & 1) {
			ans += seg[a];
			a++;
		}
		if (b & 1) {
			--b;
			ans += seg[b];
		}
	}
	return ans;
}

void update(int i, int v) {
	for (int a = i + S; a; a /= 2) {
		seg[a] += v;
	}
}

int query2(int i) {
	int ans = 0;
	for (int a = i + S; a; a /= 2) {
		ans += seg2[a];
	}
	return ans;
}
void update2(int l, int r, int v) {
	for (int a = l + S, b = r + S; a < b; a /= 2, b /= 2) {
		if (a & 1) {
			seg2[a] += v;
			a++;
		}
		if (b & 1) {
			--b;
			seg2[b] += v;
		}
	}
}

num go() {
	cin >> N;
	for (int i = 1; i <= N; i++) {
		cin >> P[i];
	}

	for (int i = 0; i <= N; i++) {
		ch[i] = vector<int>();
	}

	for (int i = 1; i <= N; i++) {
		ch[P[i]].push_back(i);
	}
	assert(ch[0].size() == 1);

	{
		int ind = 0;
		dfs(0, 0, ind);
		assert(ind == N+1);
	}

	memset(seg, 0, sizeof(seg));
	memset(seg2, 0, sizeof(seg2));

	for (int i = 1; i <= N; i++) {
		update(st[i], +1);
		if (numCh[i] >= 2) {
			update2(st[i]+1, en[i], +1);
		}
	}

	num curVal = 0;
	for (int i = 1; i <= N; i++) {
		assert(query(st[i], en[i]) == en[i] - st[i]);
		curVal += en[i] - st[i];
	}

	//cerr << curVal << '\n';

	num ans = 1;
	for (int i = N; i >= 1; i--) {
		ans *= curVal;
		//cerr << "remove " << i << ' ' << st[i] << ' ' << en[i] << '\n';
		// delete vertex i
		// we need to find the depth of vertex i
		//cerr << "Self " << query2(st[i])+1 << '\n';
		curVal -= query2(st[i]) + 1;
		update(st[i], -1);

		for (int cur = i; cur != 0; cur = P[cur]) {
			numCh[cur] --;
			//cerr << "Decr " << cur << ' ' << numCh[cur] << '\n';
			if (numCh[cur] == 1) {
				update2(st[cur]+1, en[cur], -1);
				curVal -= query(st[cur]+1, en[cur]);
				//cerr << "Below " << cur << ' ' << query(st[cur]+1, en[cur]) << '\n';
			}
			if (numCh[cur] == 0) {
				continue;
			} else {
				break;
			}
		}

		//cerr << curVal << '\n';
	}
	assert(curVal == 0);
	return ans;
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int T; cin >> T;
	for (int t = 1; t <= T; t++) {
		//cerr << "Case #" << t << '\n';
		num ans = go();
		cout << "Case #" << t << ": " << ans << '\n';
	}

	return 0;
}
