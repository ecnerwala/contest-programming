#include<bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int N; cin >> N;
	int L; cin >> L;
	vector<int> T(N);
	for (int i = 0; i < N; i++) cin >> T[i];
	T.push_back(T.back()+L);
	int ans = 0;
	for (int i = 0; i < N; i++) {
		ans += min(T[i+1] - T[i], L);
	}
	cout << ans << '\n';

	return 0;
}
