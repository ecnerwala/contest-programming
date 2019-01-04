#include<bits/stdc++.h>
using namespace std;
using ll = long long;
const int MAXN = 3e5;
int N;
pair<int, int> A[MAXN];

int main() {
	ios_base::sync_with_stdio(0);
	cin >> N;
	for (int i = 0; i < N; i++) {
		cin >> A[i].first >> A[i].second;
		A[i].first -= A[i].second;
	}
	sort(A, A + N);
	ll has = 0;
	for (int i = 0; i < N; i++) {
		has = max(has, ll(A[i].first));
		has += A[i].second;
	}
	cout << has << '\n';

	return 0;
}
