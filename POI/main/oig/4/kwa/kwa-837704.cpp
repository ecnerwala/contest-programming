#include<bits/stdc++.h>
using namespace std;

typedef long long ll;
const ll INF = 1e9 + 200;

const int MAXN = 1e3 + 200;
int N;
ll A[MAXN][MAXN];
int main() {
	ios_base::sync_with_stdio(0);
	cin >> N;
	ll sum = 0;
	for (int i = 0; i < N; i++) {
		ll cur = 0;
		for (int j = 0; j < N; j++) {
			cin >> A[i][j];
			cur += A[i][j];
		}
		sum = max(cur+INF+1, sum);
	}
	for (int i = 0; i < N; i++) {
		ll cur = 0;
		for (int j = 0; j < N; j++) {
			cur += A[i][j];
		}
		assert(cur < sum);
		for (int j = 0; j < N; j++) {
			if (A[i][j] == 0) A[i][j] = sum - cur;
		}
	}
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (j) cout << ' ';
			cout << A[i][j];
		}
		cout << '\n';
	}
	return 0;
}
