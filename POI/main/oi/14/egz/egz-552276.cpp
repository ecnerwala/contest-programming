#include<iostream>
#include<algorithm>
#include<vector>

using namespace std;

const int INF = 1e9;

const int MAXN = 2e5;
const int MAXM = 2e5;
const int MAXK = 2e5;

const int MAXL = MAXN;

int N, M, P, K;
vector<int> V[MAXN][2];

int cost[MAXN][2]; // min cost, removing v => decreasing

int L;
int dp[MAXN];

int init() {
	L = 0;
	dp[0] = INF;
}

void insert(int v) {
	int mi = 0, ma = L + 1;
	while(ma - mi > 1) {
		int md = (mi + ma) / 2;
		if(v <= dp[md]) {
			mi = md;
		} else {
			ma = md;
		}
	}
	mi++;
	if(mi > L) {
		L = mi;
		dp[mi] = v;
	} else if(v > dp[mi]){
		dp[mi] = v;
	}
	//cerr << v << ' ' << mi << '\n';
}

int proc(vector<int> &v) {
	sort(v.begin(), v.end());
	for(__typeof(v.begin()) it = v.begin(); it != v.end(); it++) {
		insert(*it);
	}
	return L;
}

int main() {
	cin >> N >> M >> P >> K;
	for(int i = 0; i < P; i++) {
		int n, m, d; cin >> n >> m >> d;
		n--;
		n += d; // always at the end
		V[n][d].push_back(m);
	}
	init();
	for(int i = N - 1 - 1; i >= 0; i--) {
		cost[i][0] = (N - 1 - i) - proc(V[i][0]);
	}
	init();
	for(int i = 1; i < N; i++) {
		cost[N - 1 - i][1] = i - proc(V[i][1]);
	}
	for(int i = 0; i < N - 1; i++) { cerr << cost[i][0] << ' '; } cerr << '\n';
	for(int i = 0; i < N - 1; i++) { cerr << cost[i][1] << ' '; } cerr << '\n';
	int res = 0;
	for(int i = 0, j = N - 1 - 1; i < N - 1 && j >= 0; i++) {
		while(j >= 0 && cost[i][0] + cost[j][1] > K) j--;
		if(j == 0) break;
		res = max(res, N - i - j);
	}
	cout << res << '\n';
	return 0;
}
