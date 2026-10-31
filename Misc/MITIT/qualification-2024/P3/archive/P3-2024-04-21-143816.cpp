#include <bits/stdc++.h>

int main() {
	using namespace std;

	int N, M; cin >> N >> M;
	std::vector<int> A(N*M);
	for (auto& a : A) { cin >> a; a--; }
	std::sort(A.begin(), A.end());
	int L = N*M;
	std::vector<int> cnt_smaller(L);
	for (int i = 0; i < L; i++) {
		if (i == 0 || A[i] != A[i-1]) cnt_smaller[i] = i;
		else cnt_smaller[i] = cnt_smaller[i-1];
	}
	for (int i = 1; i <= N; i++) {
		int idx = 0;
		int cnt = i;
		for (int j = 0; j < M; j++) {
			idx += cnt;
			cout << A[idx-1]+1 << " \n"[j+1==M];
			if (j+1 < M) cnt = std::min(cnt, idx - (cnt_smaller[idx-1]));
		}
	}

	return 0;
}
