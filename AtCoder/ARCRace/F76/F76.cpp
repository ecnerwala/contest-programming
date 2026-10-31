#include<bits/stdc++.h>
using namespace std;

const int MAXN = 2.1e5;
const int MAXM = 2.1e5;
int N, M;
int L[MAXN];
int R[MAXN];
vector<int> evts[MAXN];

const int S = 1 << 18;
int segVal[2*S];
int segMax[2*S];
void update(int i) {
	segMax[i] = (i >= S ? 0 : max(segMax[2*i], segMax[2*i+1])) + segVal[i];
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N >> M;
	for (int i = 0; i < N; i++) {
		cin >> L[i] >> R[i];
	}
	for (int i = 0; i < N; i++) {
		evts[L[i]].push_back(R[i]);
	}
	int ans = max(0, N-M);
	for (int r = 0; r <= M+1; r++) {
		segVal[S+r] = -(M+1-r);
	}

	for (int i = 2 * S - 1; i; --i) update(i);

	for (int l = 0; l <= M; l++) {
		for (int r : evts[l]) {
			for (int b = S+r+1; 1 < b; b /= 2) {
				if (b & 1) {
					--b;
					segVal[b] ++;
					update(b);
				}
			}
			for (int b = S+r; b; b /= 2) {
				update(b);
			}
		}
		ans = max(ans, segMax[1] - l);
	}
	cout << ans << '\n';

	return 0;
}
