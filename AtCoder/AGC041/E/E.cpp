#include<bits/stdc++.h>
using namespace std;

const int MAXN = int(5e4) + 2;
const int MAXM = int(1.1e5);
int N, M;
int T;
pair<int, int> X[MAXM];

bool ansUp[MAXM];

using bs = bitset<MAXN>;

namespace uniforming {
bs canReach[MAXN];
bool vis[MAXN];
bool findUniforming() {
	for (int t = 0; t < N; t++) canReach[t][t] = true;

	for (int i = M-1; i >= 0; i--) {
		int x = X[i].first, y = X[i].second;
		canReach[x] |= canReach[y];
		canReach[y] = canReach[x];
	}

	bs isGood;
	for (int t = 0; t < N; t++) {
		isGood[t] = true;
	}
	for (int s = 0; s < N; s++) {
		isGood &= canReach[s];
	}

	for (int t = 0; t < N; t++) {
		if (isGood[t]) {
			vis[t] = true;
			for (int i = M-1; i >= 0; i--) {
				int x = X[i].first, y = X[i].second;
				if (vis[x] || vis[y]) {
					if (!vis[y]) {
						ansUp[i] = true;
						vis[y] = true;
					}
					if (!vis[x]) {
						ansUp[i] = false;
						vis[x] = true;
					}
					assert(vis[x] && vis[y]);
				}
			}
			for (int s = 0; s < N; s++) assert(vis[s]);
			return true;
		}
	}
	return false;
}
}

namespace nonUniforming {
bool findNonUniforming() {
	if (N == 2 && M >= 1) {
		return false;
	}
	int badState = 0;
	for (int i = M-1; i >= 0; i--) {
		int x = X[i].first, y = X[i].second;
		if (y >= 3) {
			ansUp[i] = true;
			continue;
		}
		if (x == 0 && y == 1) {
			if (badState == 1) ansUp[i] = false;
			else if (badState == 2) ansUp[i] = true;
			badState = 0;
		} else if (x == 0 && y == 2) {
			if (badState == 0) ansUp[i] = false;
			else if (badState == 2) ansUp[i] = true;
			badState = 1;
		} else if (x == 1 && y == 2) {
			if (badState == 0) ansUp[i] = false;
			else if (badState == 1) ansUp[i] = true;
			badState = 2;
		} else assert(false);
	}
	return true;
}
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N >> M >> T;
	for (int i = 0; i < M; i++) {
		cin >> X[i].first >> X[i].second;
		X[i].first--, X[i].second--;
	}

	bool found;
	if (T == 1) {
		found = uniforming::findUniforming();
	} else if (T == 2) {
		found = nonUniforming::findNonUniforming();
	} else assert(false);
	if (found) {
		for (int i = 0; i < M; i++) {
			cout << "v^"[ansUp[i]];
		}
		cout << '\n';
	} else {
		cout << -1 << '\n';
	}

	return 0;
}
