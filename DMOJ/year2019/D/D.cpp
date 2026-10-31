#include<bits/stdc++.h>
using namespace std;

using ll = long long;

const int MAXN = 1.1e5;
int N, M;
ll S[MAXN];
vector<pair<int, pair<char, ll>>> adj[MAXN];
ll dist[2][MAXN][2];

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N >> M;
	for (int i = 1; i <= N; i++) {
		cin >> S[i];
	}

	for (int e = 0; e < M; e++) {
		int a, b; char t; cin >> a >> b >> t;
		ll x = 0;
		if (t == 'A' || t == 'B') {
			cin >> x;
		}
		adj[a].push_back({b, {t, x}});
	}

	for (int z = 0; z < 2; z++) {
		using state = pair<int, bool>;
		priority_queue<pair<ll, state>, vector<pair<ll, state>>, greater<pair<ll, state>>> pq;
		memset(dist[z], -1, sizeof(dist[z]));

		auto checkState = [&](int ns, bool np, ll nd) {
			assert(nd >= 0);
			if (dist[z][ns][np] == -1 || nd < dist[z][ns][np]) {
				dist[z][ns][np] = nd;
				pq.emplace(nd, state(ns, np));
			}
		};

		checkState(1, z, 0);
		while (!pq.empty()) {
			ll d = pq.top().first;
			int cur = pq.top().second.first;
			bool pressed = pq.top().second.second;
			pq.pop();

			assert(dist[z][cur][pressed] != -1);
			assert(dist[z][cur][pressed] <= d);
			if (dist[z][cur][pressed] != d) continue;

			if (pressed == true) {
				checkState(cur, false, d);
			} else {
				checkState(cur, true, d+1);
			}

			for (auto it : adj[cur]) {
				int nxt = it.first;
				char t = it.second.first;
				ll x = it.second.second;
				if (t == 'A') {
					if (x == 0) {
						checkState(nxt, pressed, d);
					} else {
						checkState(nxt, true, d + x);
					}
				} else if (t == 'B') {
					if (pressed) {
						checkState(nxt, true, d + x);
					}
				} else if (t == 'C') {
					if (!pressed) {
						checkState(nxt, false, d);
					}
				} else assert(false);
			}
		}
	}

	ll res = 0;
	ll numGainDown = 0;
	ll numFreeUp = 0;
	ll numBoth = 0;
	bool needsUp = false;
	for (int i = 1; i <= N; i++) {
		ll d = min(dist[0][i][0], dist[1][i][1]);
		res += S[i] * d;
		assert(dist[1][i][0] <= d && d <= dist[0][i][1]);
		bool gains = dist[1][i][0] < d;
		bool free = dist[0][i][1] == d;

		if (gains && free) numBoth += S[i];
		else if (gains) numGainDown += S[i];
		else if (free) numFreeUp += S[i];

		if (dist[1][i][1] < dist[0][i][0]) {
			needsUp = true;
			assert(gains == false && free == false);
		}
		//assert(!gains); // batch 1
	}


	cerr << numGainDown << ' ' << numFreeUp << ' ' << numBoth << '\n';
	// res goes down by one for each pair you can make
	if (needsUp && numGainDown == 0 && numFreeUp == 0 && numBoth == 0) {
		// you just take the hit
		res++;
	}

	res -= min(min(numGainDown + numBoth, numFreeUp + numBoth), (numGainDown + numFreeUp + numBoth) / 2);

	cout << res << '\n';
	return 0;
}
