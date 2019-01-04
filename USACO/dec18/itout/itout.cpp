#include<bits/stdc++.h>
using namespace std;

using ll = long long;

// we want the kth lexicographically largest longest-increasing-subsequence
const int MAXN = 2e5;
int N;
int A[MAXN];
ll K;
const int MAXL = MAXN;
int L;
vector<pair<int, pair<pair<ll, ll>, pair<int, int>>>> val[MAXL];

bool used[MAXN];

int main() {
	if(fopen("itout.in", "r")) {
		FILE* a = freopen("itout.in", "r", stdin);
		FILE* b = freopen("itout.out", "w", stdout);
		assert(a && b);
	}
	cin >> N >> K;
	for (int i = 0; i < N; i++) {
		cin >> A[i];
	}
	L = 0;
	val[L].emplace_back(0, pair<pair<ll, ll>, pair<int, int>>{{0, 0}, {0, 0}});
	val[L].emplace_back(N+1, pair<pair<ll, ll>, pair<int, int>>{{1, 1}, {0, 0}});
	for (int i = N-1; i >= 0; i--) {
		int v = A[i];
		int mi = 0, ma = L+1;
		while (ma - mi > 1) {
			int md = (mi + ma) / 2;
			assert(v != val[md].back().first);
			if (v < val[md].back().first) {
				mi = md;
			} else {
				ma = md;
			}
		}
		if (ma == L+1) {
			L++;
			val[L].emplace_back(0, pair<pair<ll, ll>, pair<int, int>>{{0, 0}, {0, 0}});
		}
		int lo = 0, hi = int(val[mi].size())-1;
		while (hi - lo > 1) {
			int mid = (lo + hi) / 2;
			if (v < val[mi][mid].first) {
				hi = mid;
			} else {
				lo = mid;
			}
		}
		assert(v < val[mi][hi].first);
		hi = int(val[mi].size()) - 1;

		ll cnt = 0;
		for (int a = lo+1; a <= hi; a += (a & (-a))) {
			cnt += val[mi][a].second.first.second;
			if (cnt > K) cnt = K+1;
		}
		//cerr << v << ' ' << ma << ' ' << cnt << '\n';
		val[ma].emplace_back(v, pair<pair<ll, ll>, pair<int, int>>{{cnt, 0}, {lo, hi}});
		for (int a = int(val[ma].size()) - 1; a > 0; a -= (a & (-a))) {
			val[ma][a].second.first.second += cnt;
			if (val[ma][a].second.first.second > K) val[ma][a].second.first.second = K+1;
		}
	}
	for (int i = L; i > 0; i--) {
		while (val[i].back().second.first.first < K) {
			K -= val[i].back().second.first.first;
			val[i].pop_back();
		}
		//cerr << val[i].back().first << '\n';
		used[val[i].back().first] = true;
		assert(K > 0);
		assert(K <= val[i].back().second.first.first);
		int hi = val[i].back().second.second.second;
		while (int(val[i-1].size()) - 1 > hi) {
			val[i-1].pop_back();
		}
	}
	cout << N-L << '\n';
	for (int i = 1; i <= N; i++) {
		if (used[i]) continue;
		cout << i << '\n';
	}

	return 0;
}
