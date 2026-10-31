#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	vector<int> A(N);
	vector<int> B(N);
	for (int i = 0; i < N; i++) {
		cin >> A[i];
	}
	for (int i = 0; i < N; i++) {
		cin >> B[i];
	}

	map<int, int> ma, mb;
	map<pair<int, int>, int> mc;
	int64_t ans = 0;
	for (int i = 0; i < N; i++) {
		ans += i - (ma[A[i]]++) - (mb[B[i]]++) + (mc[{A[i],B[i]}]++);
	}
	cout << ans << '\n';

	return 0;
}
