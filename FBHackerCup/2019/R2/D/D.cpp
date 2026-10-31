#include<bits/stdc++.h>
using namespace std;

using ll = long long;

const ll INF = 2e18;

const int MAXN = 1e6;
int N;
ll P[MAXN];
ll H[MAXN];
char O[MAXN];

ll go() {
	vector<pair<ll, ll>> clams;
	vector<pair<ll, ll>> rocks;
	for (int i = 0; i < N; i++) {
		if (O[i] == 'C') {
			clams.emplace_back(P[i], H[i]);
		} else if (O[i] == 'R') {
			rocks.emplace_back(H[i], P[i]);
		} else assert(false);
	}

	assert(!clams.empty());
	{
		sort(clams.begin(), clams.end());
		int clamSize = 0;
		for (int i = 0; i < int(clams.size()); i++) {
			while (clamSize > 0 && clams[i].second >= clams[clamSize - 1].second) {
				clamSize --;
			}
			clams[clamSize++] = clams[i];
		}
		clams.resize(clamSize);
		for (int i = 0; i+1 < int(clams.size()); i++) {
			assert(clams[i].first < clams[i+1].first);
			assert(clams[i].second > clams[i+1].second);
		}
	}
	assert(!clams.empty());
	//cerr << "Deduped" << '\n';

	sort(rocks.begin(), rocks.end());
	reverse(rocks.begin(), rocks.end());
	auto rockIt = rocks.begin();
	ll minRightRock = INF;
	ll maxLeftRock = -INF;

	ll ans = INF;
	vector<ll> dp(clams.size(), INF);
	dp[0] = clams[0].first - 0;

	ll bestBackTrack = INF;
	for (int i = 0; i < int(clams.size()); i++) {
		//if (i % 1000 == 0) cerr << i << '\n';
		if (i > 0) dp[i] = min(dp[i], bestBackTrack + 2 * clams[i - 1].first + clams[i].first);
		//if (dp[i] == INF) continue;
		while (rockIt != rocks.end() && rockIt->first > clams[i].second) {
			if (rockIt->second > clams.back().first) {
				minRightRock = min(minRightRock, rockIt->second);
			} else {
				maxLeftRock = max(maxLeftRock, rockIt->second);
			}
			++rockIt;
		}

		ll loc = clams[i].first;
		if (minRightRock != INF) {
			ans = min(ans, dp[i] + minRightRock - loc);
		}

		if (maxLeftRock != -INF) {
			ll rockLoc = maxLeftRock;
			assert(rockLoc <= clams.back().first);
			int nxtInd = int(lower_bound(clams.begin(), clams.end(), pair<ll, ll>(rockLoc, -1)) - clams.begin());
			assert(nxtInd < int(clams.size()));
			if (nxtInd > i) {
				dp[nxtInd] = min(dp[nxtInd], dp[i] + clams[nxtInd].first - loc);
				ans = min(ans, dp[i] + (clams.back().first - loc) + (clams.back().first - rockLoc));
			} else {
				assert(rockLoc <= loc);
				// we can take as many as we want, it just costs dp[i] + (clams[nxtLoc - 1] - loc) + (clams[nxtLoc-1] - rockLoc) + (clams[nxtLoc] - rockLoc) = dp[i] - loc - 2 * rockLoc + 2 * clams[nxtLoc - 1] + clams[nxtLoc]
				bestBackTrack = min(bestBackTrack, dp[i] - loc - 2 * rockLoc);

				ans = min(ans, dp[i] + (clams.back().first - loc) + (clams.back().first - rockLoc));
			}
		}
	}

	if (ans == INF) return -1;
	assert(ans < INF);
	return ans;
}

void read_seq(ll arr[MAXN]) {
	ll a, b, c, d;
	cin >> arr[0] >> arr[1] >> a >> b >> c >> d;
	for (int i = 2; i < N; i++) {
		arr[i] = (a * arr[i-2] + b * arr[i-1] + c) % d + 1;
	}
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int T; cin >> T;
	for (int t = 1; t <= T; t++) {
		cin >> N;
		read_seq(P);
		read_seq(H);
		cin >> O;
		cerr << "Case #" << t << ": " << "N = " << N << '\n';

		ll ans = go();

		cout << "Case #" << t << ": " << ans << '\n';
	}

	return 0;
}
