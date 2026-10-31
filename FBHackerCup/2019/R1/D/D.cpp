#include<bits/stdc++.h>
using namespace std;

const int MAXN = 1.1e6;
int N;
int NX, NY;
int X[MAXN];
int Y[MAXN];

void read_seq(int v[MAXN]) {
	cin >> v[0] >> v[1];
	long long a, b, c, d; cin >> a >> b >> c >> d;
	for (int i = 2; i < N; i++) {
		v[i] = int((a * v[i-2] + b * v[i-1] + c) % d + 1);
	}
}

pair<int, int> P[MAXN];

int go() {
	if (NX + NY < N) return -1;
	for (int i = 0; i < N; i++) {
		P[i] = pair<int, int>(X[i], Y[i]);
	}
	X[N] = Y[N] = 0;
	sort(X, X + N + 1);
	sort(Y, Y + N + 1);
	sort(P, P + N);
	int yval = Y[N - NX];
	int xval = X[N];
	int ans = yval + xval;
	for (int i = N-1; i >= N - NY; i--) {
		yval = max(yval, P[i].second);
		xval = X[i];
		ans = min(ans, xval + yval);
	}
	return ans;
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int T; cin >> T;
	for (int t = 1; t <= T; t++) {
		cin >> N;
		cin >> NX >> NY;
		read_seq(X);
		read_seq(Y);
		int ans = go();
		cout << "Case #" << t << ": " << ans << '\n';
	}

	return 0;
}
