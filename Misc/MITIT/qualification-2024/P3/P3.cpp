#include <bits/stdc++.h>

int main() {
	using namespace std;

	int N, M; cin >> N >> M;
	std::vector<int> A(N*M);
	for (auto& a : A) { cin >> a; a--; }
	std::sort(A.begin(), A.end());
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			cout << A[(j+1)*(i+1)-1]+1 << " \n"[j+1==M];
		}
	}

	return 0;
}
