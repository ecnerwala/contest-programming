#include<bits/stdc++.h>
using namespace std;

const int INF = int(1e9) + 200;

const int MAXN = 1.1e5;
int N;
int L[MAXN];
int R[MAXN];

int Lpref[MAXN];
int Lsuff[MAXN];
int Rpref[MAXN];
int Rsuff[MAXN];

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N;
	for (int i = 0; i < N; i++) {
		cin >> L[i] >> R[i];
	}
	int GL = *max_element(L, L + N);
	int GR = *min_element(R, R + N);

	Lpref[0] = Lsuff[N] = -200;
	Rpref[0] = Rsuff[N] = INF;
	for (int i = 0; i < N; i++) {
		Lpref[i+1] = max(Lpref[i], L[i]);
		Rpref[i+1] = min(Rpref[i], R[i]);
	}
	for (int i = N; i > 0; i--) {
		Lsuff[i-1] = max(Lsuff[i], L[i-1]);
		Rsuff[i-1] = min(Rsuff[i], R[i-1]);
	}
	int ans = 0;
	for (int i = 0; i < N; i++) {
		int cnd = (R[i] - L[i] + 1) + max(0, min(Rpref[i], Rsuff[i+1]) - max(Lpref[i], Lsuff[i+1]) + 1);
		ans = max(ans, cnd);
	}

	vector<pair<int, int>> vec;
	for (int i = 0; i < N; i++) {
		vec.emplace_back(L[i], R[i]);
	}
	sort(vec.begin(), vec.end());
	for (int i = N-1; i > 0; i--) {
		vec[i-1].second = min(vec[i-1].second, vec[i].second);
	}
	for (int i = 0; i+1 < N; i++) {
		ans = max(ans, max(0, GR - vec[i].first + 1) + max(0, vec[i+1].second - GL + 1));
	}

	cout << ans << '\n';
	return 0;
}

// consider global min and global max
// same side, in which case other side might as well be a single range
// if one side is empty, the other might as well be a single range too
// opposite sides, in which case we
