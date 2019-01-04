#include<bits/stdc++.h>
using namespace std;

template <int MOD> struct modnum {
	static_assert(MOD > 0, "MOD must be positive");

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

	friend modnum operator + (const modnum& a, const modnum& b) { return modnum(a) += b; }
	friend modnum operator - (const modnum& a, const modnum& b) { return modnum(a) -= b; }
	friend modnum operator * (const modnum& a, const modnum& b) { return modnum(a) *= b; }
	friend modnum operator / (const modnum& a, const modnum& b) { return modnum(a) /= b; }
};
using num = modnum<int(1e9) + 7>;

const int MAXP = 1.1e5;
const int MAXE = 1.1e5;
int P, E;
int X[MAXP];
int Y[MAXE];

int D[MAXP][2];

const int MAXN = MAXP;
int N[2];
int vals[2][MAXN];
vector<int> adj[MAXN];

num bit[MAXN];
num query(int a) {
	assert(0 <= a && a <= N[1] + 1);
	num res = 0;
	for (; a; a -= a & -a) {
		res += bit[a];
	}
	return res;
}

void update(int a, num v) {
	assert(0 <= a && a <= N[1]);
	for (a++; a <= N[1] + 1; a += a & -a) {
		bit[a] += v;
	}
}

int main() {
	ios_base::sync_with_stdio(0);
	cin >> P >> E;
	for (int i = 0; i < P; i++) {
		cin >> X[i];
	}
	for (int j = 0; j < E; j++) {
		cin >> Y[j];
	}

	for (int i = 0, j = 0; i < P; i++) {
		while (j < E && Y[j] < X[i]) {
			assert(Y[j] != X[i]);
			j++;
		}
		assert(j == E || Y[j] > X[i]);
		if (j == 0) {
			D[i][0] = -1;
		} else {
			D[i][0] = X[i] - Y[j-1];
		}
		if (j == E) {
			D[i][1] = -1;
		} else {
			D[i][1] = Y[j] - X[i];
		}
	}

	for (int i = 0; i < P; i++) {
		if (D[i][0] == -1 || D[i][1] == -1) {
			continue;
		}
		vals[0][N[0]++] = D[i][0];
		vals[1][N[1]++] = D[i][1];
	}

	for (int z = 0; z < 2; z++) {
		sort(vals[z], vals[z] + N[z]);
		N[z] = int(unique(vals[z], vals[z] + N[z]) - vals[z]);
	}
	for (int i = 0; i < P; i++) {
		if (D[i][0] == -1 || D[i][1] == -1) {
			continue;
		}
		int x = int(lower_bound(vals[0], vals[0] + N[0], D[i][0]) - vals[0]);
		int y = int(lower_bound(vals[1], vals[1] + N[1], D[i][1]) - vals[1]);
		assert(vals[0][x] == D[i][0]);
		assert(vals[1][y] == D[i][1]);
		adj[x].push_back(y);
	}

	update(0, 1);
	for (int i = 0; i < N[0]; i++) {
		sort(adj[i].begin(), adj[i].end());
		adj[i].resize(unique(adj[i].begin(), adj[i].end()) - adj[i].begin());
		reverse(adj[i].begin(), adj[i].end());
		// biggest down
		for (int j : adj[i]) {
			update(j + 1, query(j + 1));
		}
	}
	cout << query(N[1] + 1) << '\n';

	return 0;
}
