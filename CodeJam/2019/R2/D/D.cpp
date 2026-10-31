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

	//friend bool operator == (const modnum& a, const modnum& b) { return a.v == b.v; }
	//friend bool operator != (const modnum& a, const modnum& b) { return a.v != b.v; }

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

const int MAXM = 1.1e5;
int M;
int R[MAXM][2];
long long G[MAXM];

bool canReachLead[MAXM];
bool reachable[MAXM];
bool alive[MAXM];
vector<int> outEdges[MAXM];
vector<int> inEdges[MAXM];

void finish() {
	for (int i = 0; i < M; i++) {
		outEdges[i] = {};
		inEdges[i] = {};
	}
}

int inDeg[MAXM];
num dp[MAXM];

pair<bool, num> go() {
	cin >> M;
	for (int i = 0; i < M; i++) {
		cin >> R[i][0] >> R[i][1];
		R[i][0] --, R[i][1] --;
	}
	for (int i = 0; i < M; i++) {
		cin >> G[i];
	}

	for (int i = 0; i < M; i++) {
		outEdges[i].push_back(R[i][0]);
		outEdges[i].push_back(R[i][1]);
		inEdges[R[i][0]].push_back(i);
		inEdges[R[i][1]].push_back(i);
	}

	{
		stack<int> q;
		for (int i = 0; i < M; i++) {
			canReachLead[i] = false;
		}
		canReachLead[0] = true;
		q.push(0);
		while (!q.empty()) {
			int cur = q.top(); q.pop();
			for (int nxt : inEdges[cur]) {
				if (canReachLead[nxt]) continue;
				canReachLead[nxt] = true;
				q.push(nxt);
			}
		}
	}

	{
		stack<int> q;
		for (int i = 0; i < M; i++) {
			reachable[i] = false;
		}
		for (int i = 0; i < M; i++) {
			if (G[i] > 0) {
				reachable[i] = true;
				q.push(i);
			}
		}
		while (!q.empty()) {
			int cur = q.top(); q.pop();
			for (int nxt : outEdges[cur]) {
				if (reachable[nxt]) continue;
				reachable[nxt] = true;
				q.push(nxt);
			}
		}
	}

	for (int i = 0; i < M; i++) {
		alive[i] = reachable[i] && canReachLead[i];
	}
	for (int i = 0; i < M; i++) {
		outEdges[i] = {};
		inEdges[i] = {};
	}

	for (int i = 0; i < M; i++) {
		if (!alive[i]) continue;
		for (int z = 0; z < 2; z++) {
			int j = R[i][z];
			if (!alive[j]) continue;
			outEdges[i].push_back(j);
			if (i != 0) inEdges[j].push_back(i);
		}
	}

	if (!alive[0]) {
		return {false, 0};
	}

	{
		// check if lead => lead
		int cur = 0;
		while (outEdges[cur].size() == 1) {
			cur = outEdges[cur][0];
			if (cur == 0) break;
		}
		if (outEdges[cur].size() > 1) {
			return {true, 0};
		}
		outEdges[0].clear();
	}

	stack<int> q;
	for (int i = 0; i < M; i++) {
		if (!alive[i]) continue;
		dp[i] = G[i];
		inDeg[i] = int(inEdges[i].size());
		if (inDeg[i] == 0) {
			q.push(i);
		}
	}
	while (!q.empty()) {
		int cur = q.top(); q.pop();
		for (int nxt : outEdges[cur]) {
			inDeg[nxt] --;
			dp[nxt] += dp[cur];
			if (inDeg[nxt] == 0) {
				q.push(nxt);
			}
		}
	}
	for (int i = 0; i < M; i++) {
		if (!alive[i]) continue;
		if (inDeg[i]) return {true, 0};
	}
	return {false, dp[0]};
}

int main() {
	ios_base::sync_with_stdio(0);
	int T; cin >> T;

	for (int case_num = 1; case_num <= T; case_num ++) {

		auto res = go();
		finish();

		cout << "Case #" << case_num << ": ";
		if (res.first) {
			cout << "UNBOUNDED" << '\n';
		} else {
			cout << res.second << '\n';
		}
	}

	return 0;
}
