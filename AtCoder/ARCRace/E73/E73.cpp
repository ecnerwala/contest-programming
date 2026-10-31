#include<bits/stdc++.h>
using namespace std;

using ll = long long;
const ll INF = 1e9;

const int MAXN = 2.1e5;
int N;
int X[MAXN];
int Y[MAXN];
pair<int, int> P[MAXN];
int minx[MAXN];
int maxx[MAXN];
int miny[MAXN];
int maxy[MAXN];

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N;
	for (int i = 0; i < N; i++) {
		cin >> X[i] >> Y[i];
		if (X[i] > Y[i]) swap(X[i], Y[i]);
		P[i] = {X[i], Y[i]};
	}
	sort(P, P + N);
	for (int i = 0; i < N; i++) {
		tie(X[i],Y[i]) = P[i];
	}

	ll ans = ll(*max_element(X,X+N) - *min_element(X,X+N)) * ll(*max_element(Y,Y+N) - *min_element(Y,Y+N));
	int V = *max_element(Y,Y+N) - *min_element(X,X+N);
	miny[0] = INF;
	maxy[0] = -INF;
	for (int i = 0; i < N; i++) {
		miny[i+1] = min(miny[i], Y[i]);
		maxy[i+1] = max(maxy[i], Y[i]);
	}
	minx[N] = INF;
	maxx[N] = -INF;
	for (int i = N-1; i >= 0; i--) {
		minx[i] = min(minx[i+1], X[i]);
		maxx[i] = max(maxx[i+1], X[i]);
	}
	for (int i = 0; i <= N; i++) {
		ans = min(ans, ll(V)*ll(max(maxx[i],maxy[i]) - min(minx[i],miny[i])));
	}

	cout << ans << '\n';

	return 0;
}
