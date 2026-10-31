#include<bits/stdc++.h>
using namespace std;

using ll = long long;

ll go() {
	int N, M; cin >> N >> M;
	string start;
	string end;
	cin >> start >> end;

	assert(int(start.size()) == 1 + N * M);
	assert(int(end.size()) == 1 + N * M);

	ll ans = 0;
	for (int i = 0; i < N; i++) {
		vector<int> spos;
		vector<int> epos;
		for (int j = 1; j <= M; j++) {
			if (start[i * M + j] == 'A') {
				spos.push_back(j);
			}
			if (end[i * M + j] == 'A') {
				epos.push_back(j);
			}
		}
		reverse(spos.begin(), spos.end());
		reverse(epos.begin(), epos.end());
		while (spos.size() < epos.size()) spos.push_back(0);
		while (epos.size() < spos.size()) epos.push_back(0);
		for (int z = 0; z < int(spos.size()); z++) {
			ans += abs(spos[z] - epos[z]);
		}
	}

	return ans;
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);

	int T; cin >> T;
	for (int t = 1; t <= T; t++) {
		ll ans = go();
		cout << "Case #" << t << ": " << ans << '\n';
	}

	return 0;
}
