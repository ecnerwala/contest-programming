#include<bits/stdc++.h>
using namespace std;

const int MAXN = 1.1e6;
const int MAXM = 1.1e6;
int N, M;
int A[MAXM], B[MAXM];
int prv[MAXM];
pair<int, int> best[MAXN];

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N >> M;
	for (int m = 1; m <= M; m++) {
		cin >> A[m] >> B[m];
	}
	for (int i = 1; i <= N; i++) {
		best[i] = {M+1, M+1};
	}
	best[A[1]] = {0, 0};
	for (int m = 1; m <= M; m++) {
		prv[m] = best[A[m]].second;
		best[B[m]] = min(best[B[m]], pair<int, int>{best[A[m]].first + 1, m});
	}
	int cur = best[B[M]].second;
	vector<int> res;
	while (cur) {
		res.push_back(cur);
		cur = prv[cur];
	}
	reverse(res.begin(), res.end());
	cout << res.size() << '\n';
	for (int m : res) {
		cout << A[m] << ' ' << B[m] << '\n';
	}

	return 0;
}
