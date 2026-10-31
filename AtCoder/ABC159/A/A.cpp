#include<bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int N, M; cin >> N >> M;
	cout << N * (N-1) / 2 + M * (M-1) / 2 << '\n';

	return 0;
}
