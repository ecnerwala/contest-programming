#include<bits/stdc++.h>
using namespace std;

using ll = long long;

const ll INF = 2e18;

const int MAXN = 2.1e5;
int N;
ll K;

const int MAXV = 2.1e5;
const int V = int(2e5) + 100;
int A[MAXN];

int nxtOcc[MAXV];
ll jumpDist[MAXN][61];

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N >> K;
	for (int i = 0; i < N; i++) {
		cin >> A[i];
	}

	for (int i = N-1; i >= 0; i--) {
		nxtOcc[A[i]] = i+N;
	}
	for (int i = N-1; i >= 0; i--) {
		jumpDist[i][0] = (nxtOcc[A[i]] - i) + 1;
		assert(jumpDist[i][0] <= N+1);

		nxtOcc[A[i]] = i;
	}

	for (int l = 0; l < 60; l++) {
		for (int i = 0; i < N; i++) {
			jumpDist[i][l+1] = min(INF, jumpDist[i][l] + jumpDist[(i + jumpDist[i][l]) % N][l]);
		}
	}


	for (int i = 0; i < N; i++) {
		//cerr << i << ' '; for (int l = 0; l <= 60; l++) { cerr << jumpDist[i][l] << ' '; } cerr << '\n';
	}

	ll curLoc = 0;
	for (int l = 60; l >= 0; l--) {
		if (curLoc + jumpDist[curLoc % N][l] <= N * K) {
			curLoc += jumpDist[curLoc % N][l];
		}
	}

	vector<int> ans;
	while (curLoc < N * K) {
		ll nxtLoc = (curLoc + jumpDist[curLoc % N][0]);
		if (nxtLoc <= N * K) {
			curLoc = nxtLoc;
		} else {
			ans.push_back(A[curLoc % N]);
			curLoc++;
		}
	}

	if (ans.empty()) {
		cout << '\n';
	} else {
		for (int i = 0; i < int(ans.size()); i++) {
			cout << ans[i] << " \n"[i+1==int(ans.size())];
		}
	}

	return 0;
}
