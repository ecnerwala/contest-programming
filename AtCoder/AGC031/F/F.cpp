#include<bits/stdc++.h>
using namespace std; 
void wssert(bool b) {
	if (!b) {
		cout << "GARBAGE\n";
		exit(0);
	}
}

using ll = long long;

ll invmod(ll a, ll m) {
	assert(1 <= a && a < m);
	return a == 1 ? 1 : (m - invmod(m%a, a) * m / a);
}

const int MAXN = 51000;
const int MAXM = 51000;
const int MAXQ = 51000;
const int MAXMOD = 1.1e6;
int N, M, Q, MOD;
ll ord2;
ll inv2;
ll pow2[MAXMOD];

struct linFn {
	ll a, b;
	linFn() : a(0), b(0) {}
	explicit linFn(ll a_, ll b_ = 0) : a(a_ % ord2), b(b_ % MOD) { if (a < 0) a += ord2; if (b < 0) b += MOD; }
	linFn inv() const {
		return linFn(ord2-a, -pow2[ord2-a] * b);
	}
	friend linFn operator * (const linFn& f, const linFn& g) {
		return linFn(f.a + g.a, f.b + pow2[f.a] * g.b);
	}
};

linFn pow(linFn a, ll b) {
	linFn r(0);
	while (b) {
		if (b&1) r = r * a;
		a = a * a;
		b >>= 1;
	}
	return r;
}

linFn pot[MAXN]; // potential function
pair<pair<int, int>, ll> edges[MAXM];
vector<int> adj[MAXN];
bool vis[MAXN];

void dfs(int cur) {
	assert(!vis[cur]);
	vis[cur] = true;
	for (int e : adj[cur]) {
		int nxt = cur ^ edges[e].first.first ^ edges[e].first.second;
		if (vis[nxt]) continue;
		pot[nxt] = pot[cur] * linFn(1, edges[e].second);
		dfs(nxt);
	}
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N >> M >> Q >> MOD;
	assert(MOD >= 3);
	assert(MOD % 2 == 1);

	pow2[0] = 1;
	for (ord2 = 1; ; ord2++) {
		pow2[ord2] = pow2[ord2-1] * 2 % MOD;
		if (pow2[ord2] == 1) {
			break;
		}
	}
	assert(ord2 >= 2);

	inv2 = (MOD + 1) / 2;
	assert(inv2 == pow2[ord2-1]);
	//cerr << "ord2 = " << ord2 << '\n';

	for (int e = 0; e < M; e++) {
		int a, b; ll c; cin >> a >> b >> c;
		edges[e].first = pair<int, int>(a,b);
		edges[e].second = c;
		adj[a].push_back(e);
		adj[b].push_back(e);
	}

	dfs(1);

	//cerr << "pots\n";
	//for (int i = 1; i <= N; i++) cerr << i << ' ' << pot[i].a << ' ' << pot[i].b << '\n';
	//cerr << '\n';

	vector<linFn> poss;
	for (int e = 0; e < M; e++) {
		int a, b; linFn f;
		tie(a,b) = edges[e].first;
		f = linFn(1, edges[e].second);
		linFn x = pot[a] * f * pot[b].inv();
		linFn y = pot[b] * f * pot[a].inv();
		poss.push_back(x);
		poss.push_back(y);
		//cerr << "poss " << x.a << ' ' << x.b << '\n';
		//cerr << "poss " << y.a << ' ' << y.b << '\n';
	}

	ll mulStep = ord2;
	linFn mulBase;
	for (linFn f : poss) {
		if (f.a == 0) {
			continue;
		}
		ll nStep = __gcd(f.a, mulStep);

		if (nStep < mulStep) {
			linFn g = pow(f, invmod((f.a % mulStep) / nStep, mulStep / nStep));
			assert(g.a % mulStep == nStep);
			mulBase = g * pow(mulBase, g.a / mulStep).inv();
		}
		mulStep = nStep;

		assert(mulBase.a == mulStep % ord2);
	}
	//cerr << "mulBase " << mulStep << ' ' << mulBase.b << '\n';

	ll addBase = MOD;
	addBase = __gcd(addBase, pow(mulBase, ord2/mulStep).b);
	for (linFn f : poss) {
		assert(mulBase.a == 0 || f.a % mulBase.a == 0);
		linFn g;
		if (mulBase.a == 0) {
			assert(f.a == 0);
			g = linFn();
		} else {
			g = pow(mulBase, f.a / mulBase.a);
		}
		assert(g.a == f.a);
		//cerr << g.a << ' ' << g.b << ' ' << f.b << '\n';
		addBase = __gcd(addBase, abs(g.b - f.b));
	}
	assert(mulStep == 1 || mulStep == 2);
	cerr << "bases " << mulStep << ' ' << mulBase.b << ' ' << addBase << '\n';

	vector<ll> vals;
	for (ll i = 0, v = 1; i < ord2 / mulStep; i++) {
		vals.push_back(v % addBase);
		v = (pow2[mulBase.a] * v + 1) % MOD;
	}
	sort(vals.begin(), vals.end());
	vals.resize(size_t(unique(vals.begin(), vals.end()) - vals.begin()));

	//for (ll v : vals) { cerr << v << ' '; } cerr << '\n';

	vector<ll> divisors;
	for (ll d = 1; d <= addBase; d++) {
		if (addBase % d == 0) {
			divisors.push_back(d);
		}
	}
	unordered_map<ll, unordered_set<ll>> has;
	for (ll v : vals) {
		for (ll d : divisors) {
			has[d].insert(v % d);
		}
	}

	for (int q = 0; q < Q; q++) {
		int s, t, r; cin >> s >> t >> r;
		ll x = pot[t].b;
		ll y = (pow2[pot[s].a] * r + pot[s].b) % MOD;
		// check if y = a^nx + ba^{n-1}+ba^{n-2}+...+b for some n
		// check if y = x + ((a-1)x + b)(a^{n-1}+a^{n-2}+...+1) for some n
		y -= x;
		if (y < 0) y += MOD;
		x = ((pow2[mulBase.a]-1) * x + mulBase.b) % MOD;
		if (x < 0) x += MOD;
		//cerr << x << ' ' << y << '\n';
		// does any vx = y?
		bool good = false;

		ll g = __gcd(x, addBase);
		if (y % g == 0) {
			ll m = addBase / g;
			x /= g, y /= g;
			x %= m, y %= m;
			if (m == 1) {
				good = true;
			} else {
				if (has[m].count(y * invmod(x, m) % m)) {
					good = true;
				}
			}
		}
		cout << (good ? "YES" : "NO") << '\n';
	}

	return 0;
}
