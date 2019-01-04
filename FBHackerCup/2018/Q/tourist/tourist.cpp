#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

int main() {
	ios_base::sync_with_stdio(0);
	int N_CASES; cin >> N_CASES;
	for (int case_num = 1; case_num <= N_CASES; case_num ++) {
		ll N, K, V; cin >> N >> K >> V;
		assert(1 <= K && K <= N);
		assert(V >= 1);
		V--;
		V *= K;
		V %= N;
		cout << "Case #" << case_num << ":";
		for (int i = 0; i < N; i++) {
			string A; cin >> A;
			if ((V <= i && i < V + K) || (V <= i + N && i + N < V + K)) {
				cout << ' ' << A;
			}
		}
		cout << '\n';
	}

	return 0;
}
