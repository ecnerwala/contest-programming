#include<bits/stdc++.h>
using namespace std;

using ll = long long;

const int MAXN = 2.1e5;
int N;
int D[MAXN], C[MAXN];
vector<int> dep[MAXN];
int deg[MAXN];

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N;
	for (int i = 0; i < N; i++) {
		cin >> D[i] >> C[i];
		int c; cin >> c;
		dep[i].resize(c);
		for (int z = 0; z < c; z++) {
			cin >> dep[i][z]; dep[i][z] --;
			deg[dep[i][z]] ++;
		}
	}
	vector<int> ans;
	priority_queue<pair<int, int>> pq;
	for (int i = 0; i < N; i++) {
		if (deg[i] == 0) {
			pq.emplace(D[i], i);
		}
	}

	ll curTime = 1e18;
	for (int z = 0; z < N; z++) {
		if (pq.empty()) {
			cout << "NO" << '\n';
			exit(0);
		}
		int i = pq.top().second; pq.pop();
		ans.push_back(i);
		curTime = min(curTime, ll(D[i]));
		curTime -= C[i];
		for (int j : dep[i]) {
			if (!(--deg[j])) {
				pq.emplace(D[j], j);
			}
		}
	}

	if (curTime < 0) {
		cout << "NO" << '\n';
		exit(0);
	}
	cout << "YES" << '\n';
	reverse(ans.begin(), ans.end());
	for (int i = 0; i < N; i++) {
		cout << ans[i]+1 << " \n"[i+1==N];
	}

	return 0;
}
