#include<bits/stdc++.h>
using namespace std;

using ll = long long;

const int MAXN = 2.1e5;
int N;
ll A[MAXN];
pair<ll, int> starts[MAXN];
int par[MAXN][20];

int Q[MAXN];
pair<ll, ll> ans[MAXN];
pair<ll, ll> IMPOSSIBLE(-1, -1);

pair<ll, ll> intersect(pair<ll, int> a, pair<ll, int> b) {
	assert(a.first > b.first && a.second > b.second);
	return pair<ll, ll> (a.first - b.first, a.second - b.second);
}
bool geq(pair<ll, ll> a, pair<ll, ll> b) {
	return a.first * b.second >= b.first * a.second;
}

void solveDown() {
	for (int i = 0; i < N; i++) {
		starts[i] = {A[i], i};
	}
	sort(starts, starts + N);

	vector<pair<ll, int>> hull;
	for (int z = N-1; z >= 0; z--) {
		while (hull.size() >= 1 && starts[z].second > hull[hull.size()-1].second) {
			hull.pop_back();
		}
		while (hull.size() >= 2 && geq(intersect(hull[hull.size()-1], starts[z]), intersect(hull[hull.size()-2], starts[z]))) {
			hull.pop_back();
		}
		hull.push_back(starts[z]);
		int i = starts[z].second;
		int o = Q[i];
		pair<ll, int> p(A[o], o);
		if (p.first < starts[z].first) {
			// solve it
			if (o > hull[0].second) {
				ans[i] = IMPOSSIBLE;
			} else {
				int mi = -1;
				int ma = int(hull.size())-1;
				while (ma - mi > 1) {
					int md = (mi + ma) / 2;
					if (o > hull[md+1].second || geq(intersect(hull[md+1], p), intersect(hull[md], p))) {
						ma = md;
					} else {
						mi = md;
					}
				}
				ans[i] = intersect(hull[ma], p);
			}
		}
	}
}

int main() {
	if (fopen("falling.in", "r")) {
		freopen("falling.in", "r", stdin);
		freopen("falling.out", "w", stdout);
	}
	cin >> N;
	for (int i = 0; i < N; i++) {
		cin >> A[i];
	}
	for (int i = 0; i < N; i++) {
		cin >> Q[i]; Q[i] --;
	}

	solveDown();
	reverse(ans, ans + N);
	reverse(A, A + N);
	reverse(Q, Q + N);
	for (int i = 0; i < N; i++) {
		A[i] = -A[i];
		Q[i] = N-1-Q[i];
	}
	solveDown();
	reverse(ans, ans + N);

	for (int i = 0; i < N; i++) {
		if (ans[i] == IMPOSSIBLE) cout << -1 << '\n';
		else {
			ll p = ans[i].first, q = ans[i].second;
			assert(p && q);
			ll g = __gcd(p,q);
			p /= g, q /= g;
			cout << p << '/' << q << '\n';
		}
	}

	return 0;
}
