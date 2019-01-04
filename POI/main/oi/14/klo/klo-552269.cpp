#include<iostream>
#include<algorithm>

using namespace std;

typedef pair<int, int> pii;

const int MAXN = 2e5;

int N;
int A[MAXN];
pii pts[MAXN];

int res = 0;
int dp[MAXN];
int dpprv[MAXN];

int prv[MAXN];

void insert(int v, int n) {
	if(dp[0] < v) {
		int mi = 0, ma = res + 1;
		while(ma - mi > 1) {
			int md = (mi + ma) / 2;
			if(dp[md] < v) {
				mi = md;
			} else {
				ma = md;
			}
		}
		prv[n] = dpprv[mi];
		mi++;
		if(mi > res || v < dp[mi]) {
			dp[mi] = v;
			dpprv[mi] = n;
		}
		if(mi > res) res = mi;
	} else {
		prv[n] = -1;
	}
}

int rem[MAXN];

int main() {
	ios_base::sync_with_stdio(0);
	cin >> N;
	A[0] = 0;
	for(int i = 1; i <= N; i++) cin >> A[i];

	for(int i = 1; i <= N; i++) {
		pts[i - 1] = pii(i - A[i], A[i]); //nondecreasing, increasing
	}

	sort(pts, pts + N);

	for(int i = 0; i < N; i++) {
		insert(pts[i].second, pts[i].first + pts[i].second);
	}

	int numrem = 0;
	int cur = dpprv[res];
	while(cur > 0) {
		int pr = prv[cur];
		//cerr << cur << ' ' << pr << '\n';
		int dist = (cur - A[cur]) - (pr - A[pr]);
		for(int i = 1; i <= dist; i++) {
			rem[numrem++] = pr + i;
		}
		cur = pr;
	}
	cout << numrem << '\n';
	for(int i = 0; i < numrem; i++) {
		cout << rem[i] << ((i + 1 < numrem) ? ' ' : '\n');
	}
	return 0;
}
