#include<bits/stdc++.h>
using namespace std;

const int MAXN = 1e4;
const int MAXK = MAXN * 10;

typedef pair<int, int> pii;
#define A first
#define B second

int N;
int K[MAXN];
vector<int> down[MAXN];

pii L[MAXN]; // left-most parent
pii R[MAXN]; // right-most parent

void set_left(int a, pii v) {
	if(L[a] != pii(-1, -1)) {
		return;
	}
	L[a] = v;
	for(int i = 0; i < K[a]; i++) {
		int b = down[a][i];
		if(i == 0) {
			set_left(b, v);
		} else {
			set_left(b, make_pair(a, i));
		}
	}
}

void set_right(int a, pii v) {
	if(R[a] != pii(-1, -1)) {
		return;
	}
	R[a] = v;
	for(int i = K[a] - 1; i >= 0; i--) {
		int b = down[a][i];
		if(i == K[a] - 1) {
			set_right(b, v);
		} else {
			set_right(b, make_pair(a, i + 1));
		}
	}
}

vector<int> dp[MAXN];

int dfs(int a, int b) {
	if(a == 0 && b == K[0]) return 0;
	assert(b < K[a]);
	if(dp[a][b] != -1) return dp[a][b];
	int res = 0;
	if(L[down[a][b]].first == a) {
		assert(L[down[a][b]].second == b);
		res = max(res, dfs(down[a][b], 0));
	}
	if(b + 1 < K[a]) {
		res = max(res, dfs(a, b + 1) + 1);
	} else {
		res = max(res, dfs(R[a].first, R[a].second) + 1);
	}
	//cerr << a << ' ' << b << ' ' << res << '\n';

	return dp[a][b] = res;
}

int main() {
	ios_base::sync_with_stdio(0);
	cin >> N;
	for(int i = 0; i < N - 1; i++) {
		cin >> K[i];
		for(int k = 0; k < K[i]; k++) {
			int v; cin >> v;
			v--;
			down[i].push_back(v);
		}

		dp[i].resize(K[i] + 10, -1);
		assert(int(dp[i].size()) >= K[i]);
	}

	memset(L, -1, sizeof(L));
	memset(R, -1, sizeof(R));

	assert(K[0]);

	set_left(0, pii(0, 0));
	set_right(0, pii(0, K[0]));

	//cerr << "hi\n";
	//for(int i = 0; i < N; i++) cerr << i << ' ' << L[i].A << ' ' << L[i].B << ' ' << R[i].A << ' ' << R[i].B << '\n';

	cout << dfs(0, 0) << '\n';
}
