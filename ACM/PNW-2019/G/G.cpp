#include<bits/stdc++.h>
using namespace std;

const int MAXN = 2.1e5;
const int MAXA = 1.1e5;
const int MAXL = int(2.1e5) + int(2.1e5) + MAXA;
int N;

vector<pair<int, int>> pulses[2][MAXA];
int cnt[2][MAXL];
int numSt[2][MAXL];

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);

	cin >> N;
	for (int i = 0; i < N; i++) {
		char c; int t, m, a; cin >> c >> t >> m >> a;
		pulses[c == 'v'][a].push_back({t + MAXA - a, t + m + MAXA - a});
	}

	for (int z = 0; z < 2; z++) {
		for (int a = 1; a < MAXA; a++) {
			if (pulses[z][a].empty()) continue;
			sort(pulses[z][a].begin(), pulses[z][a].end());
			for (int i = 0; i < int(pulses[z][a].size()); ) {
				numSt[z][pulses[z][a][i].first] ++;
				cnt[z][pulses[z][a][i].first + z]++;
				//cerr << z << ' ' << a << ' ' << "+" << pulses[z][a][i].first << '\n';
				int r = pulses[z][a][i].second;
				while (i < int(pulses[z][a].size()) && pulses[z][a][i].first <= r) {
					r = max(r, pulses[z][a][i].second);
					i++;
				}
				//cerr << z << ' ' << a << ' ' << "-" << r << '\n';
				cnt[z][r] --;
			}
		}

		for (int i = 0; i+1 < MAXL; i++) {
			cnt[z][i+1] += cnt[z][i];
		}
	}

	long long ans = 0;
	for (int i = 0; i < MAXL; i++) {
		ans += 1ll * numSt[0][i] * cnt[1][i];
		ans += 1ll * numSt[1][i] * cnt[0][i];
	}
	cout << ans << '\n';

	return 0;
}
