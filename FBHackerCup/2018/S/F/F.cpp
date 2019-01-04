#include<bits/stdc++.h>
using namespace std;

using ll = long long;

const ll S = 1 << 20;
pair<ll, ll> maxHeight[2*S];

// returns the max inclusive
pair<ll, ll> getMax(ll a, ll b) {
	assert(a <= b);
	pair<ll, ll> res(-1, -1);
	for (a += S, b += S; a <= b; a /= 2, b /= 2) {
		if (a % 2 == 1) {
			res = max(res, maxHeight[a]);
			a++;
		}
		if (b % 2 == 0) {
			res = max(res, maxHeight[b]);
			b--;
		}
		assert(a % 2 == 0);
		assert(b % 2 == 1);
	}
	return res;
}

const ll MAXN = 1.1e6;
ll N;

const ll MAXW = 1.1e6;
const ll W = 1000010;

const ll MAXH = 1.1e6;
ll H;

ll lastHeight[MAXH];

pair<ll, ll> P[MAXN];

ll prv[MAXW], nxt[MAXW];

vector<ll> ranges[MAXN];

struct node {
	ll lazy;
	ll minVal;
} dp[2*S];
ll getMin(ll a, ll b, ll i = 1, ll st = 0, ll en = S) {
	assert(a <= b);
	assert(st <= a && a <= b && b < en);
	if (b == en-1 && st == a) {
		return dp[i].minVal;
	}
	assert(en - st >= 2);
	ll md = (st + en) / 2;
	ll res = 0;
	if (a < md) {
		res = min(res, getMin(a, min(b, md-1), 2*i, st, md));
	}
	if (b >= md) {
		res = min(res, getMin(max(md, a), b, 2*i+1, md, en));
	}
	res += dp[i].lazy;
	return res;
}
void updateSeg(ll i) {
	assert(i < S);
	dp[i].minVal = min(dp[2*i].minVal, dp[2*i+1].minVal) + dp[i].lazy;
}

void updateMin(ll a, ll b, ll v, ll i = 1, ll st = 0, ll en = S) {
	assert(a <= b);
	assert(st <= a && a <= b && b < en);
	if (b == en-1 && st == a) {
		dp[i].lazy += v;
		dp[i].minVal += v;
		return;
	}
	assert(en - st >= 2);
	ll md = (st + en) / 2;
	if (a < md) {
		updateMin(a, min(b, md-1), v, 2*i, st, md);
	}
	if (b >= md) {
		updateMin(max(md, a), b, v, 2*i+1, md, en);
	}
	updateSeg(i);
}

ll go() {
	cin >> N >> H;
	for (ll i = 1; i <= N; i++) {
		cin >> P[i].first >> P[i].second;
		assert(0 <= P[i].first && P[i].second <= W);
	}
	memset(maxHeight, -1, sizeof(maxHeight));
	for (ll i = 1; i <= N; i++) {
		assert(maxHeight[S+P[i].first].first == -1);
		maxHeight[S+P[i].first] = pair<ll, ll>(P[i].second, P[i].first);
	}
	for (ll i = S-1; i > 0; i--) {
		maxHeight[i] = max(maxHeight[2*i], maxHeight[2*i+1]);
	}
	memset(lastHeight, -1, sizeof(lastHeight));
	memset(prv, -1, sizeof(prv));
	memset(nxt, -1, sizeof(nxt));
	for (ll x = 0; x <= W; x++) {
		ll y = maxHeight[x+S].first;
		if (y == -1) continue;
		if (lastHeight[y] != -1) {
			ll my = getMax(lastHeight[y], x).first;
			assert(my >= y);
			if (my == y) {
				prv[x] = lastHeight[y];
				nxt[lastHeight[y]] = x;
			}
		}
		lastHeight[y] = x;
	}

	ll res = H + H; // starting poll
	for (ll i = 1; i+1 <= N; i++) {
		ll x1 = P[i].first, x2 = P[i+1].first;
		if (x1 > x2) swap(x1, x2);
		assert(x1 < x2);
		auto mx = getMax(x1, x2);
		res += 2 * mx.first;
		ranges[mx.second].push_back(x1);
	}
	for (ll i = 1; i <= N; i++) {
		res -= 2 * P[i].second;
	}

	for (ll x = 0; x <= W; x++) {
		ll y = maxHeight[x+S].first;
		if (y == -1) continue;
		if (prv[x] != -1) continue;
		// we'll start from the end
		vector<ll> pts;
		for (ll cur = x; cur != -1; cur = nxt[cur]) {
			pts.push_back(cur);
		}
		/*
		cerr << "ROW ";
		for (ll cur : pts) { cerr << cur << " "; } cerr << "\n";
		for (ll cur : pts) {
			cerr << cur << ": ";
			for (ll l : ranges[cur]) cerr << l << " ";
			cerr << "\n";
		}
		*/
		for (ll cur : pts) {
			for (ll& l : ranges[cur]) {
				l = ll(lower_bound(pts.begin(), pts.end(), l) - pts.begin());
			}
		}
		// let's do the slow dp first
		for (ll z = 0; z < ll(pts.size()); z++) {
			ll cur = pts[z];
			ll val = z ? getMin(0, z-1) : 0;
			val -= 2;

			dp[S+z].minVal = val;
			dp[S+z].lazy = 0;
			for (ll i = (S+z) / 2; i > 0; i /= 2) {
				updateSeg(i);
			}
			assert(getMin(0, z) == val);

			for (ll l : ranges[cur]) {
				updateMin(l, z, +2);
			}
		}
		ll best = getMin(0, ll(pts.size())-1);
		for (ll z = 0; z < ll(pts.size()); z++) {
			ll cur = pts[z];
			for (ll l : ranges[cur]) {
				updateMin(l, z, -2);
			}
		}
		res += best;
	}

	for (ll x = 0; x <= W; x++) {
		ranges[x].clear();
		ranges[x].shrink_to_fit();
	}

	return res;
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	ll N_CASES; cin >> N_CASES;
	for (ll case_num = 1; case_num <= N_CASES; case_num ++) {
		cerr << "Case #" << case_num << " START\n";
		auto res = go();
		cerr << "Case #" << case_num << ": " << res << "\n";
		cout << "Case #" << case_num << ": " << res << '\n';
	}

	return 0;
}
