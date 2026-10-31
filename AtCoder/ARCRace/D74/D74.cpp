#include<bits/stdc++.h>
using namespace std;

using ll = long long;
const int MAXN = 1.1e5;
int N;
int A[3 * MAXN];

ll pans[3 * MAXN];
ll sans[3 * MAXN];

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N;
	for (int i = 0; i < 3 * N; i++) {
		cin >> A[i];
	}
	multiset<int> st;
	ll sum = 0;
	for (int i = 0; i < N; i++) {
		st.insert(A[i]);
		sum += A[i];
	}
	for (int i = N; i < 2 * N; i++) {
		pans[i] = sum;
		sum += A[i];
		st.insert(A[i]);
		sum -= *st.begin();
		st.erase(st.begin());
	}
	pans[2 * N] = sum;
	st = {};
	sum = 0;
	for (int i = 2 * N; i < 3 * N; i++) {
		st.insert(-A[i]);
		sum += -A[i];
	}
	for (int i = 2 * N - 1; i >= N; i--) {
		sans[i+1] = sum;
		sum += -A[i];
		st.insert(-A[i]);
		sum -= *st.begin();
		st.erase(st.begin());
	}
	sans[N] = sum;
	ll ans = -1e18;
	for (int i = N; i <= 2 * N; i++) {
		ans = max(ans, pans[i] + sans[i]);
	}
	cout << ans << '\n';

	return 0;
}
