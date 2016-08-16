#include<bits/stdc++.h>
using namespace std;

const int MAXN = 3e4;

string S;
int N;

int main() {
	ios_base::sync_with_stdio(0);
	int T; cin >> T;

	for(int case_num = 1; case_num <= T; case_num ++) {
		
		cin >> S;
		N = int(S.size());
		assert(N % 2 == 0);

		int a = 0, b = 0;
		for(int i = 0; i < N / 2; i++) {
			a += (S[2 * i + 0] == 'C');
			b += (S[2 * i + 1] == 'C');
		}
		int res = min(a, b) + min(N / 2 - a, N / 2 - b);
		res += N / 2;
		res *= 5;
		cout << "Case #" << case_num << ": " << res << '\n';
	}

	return 0;
}
