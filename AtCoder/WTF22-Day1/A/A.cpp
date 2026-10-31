#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M; cin >> N >> M;
	int A, B; cin >> B >> A;
	std::vector<int> freq(N);
	for (int i = 0; i < M; i++) {
		int v; cin >> v; v--;
		freq[v]++;
	}

	std::sort(freq.begin(), freq.end());

	int ans = B-A;
	int A_rem = A;
	int num_save = 0;
	for (int i = 0; i < N; i++) {
		if (A_rem >= freq[i]) {
			num_save++;
			A_rem -= freq[i];
		}
	}
	ans = std::min(ans, N - num_save);
	std::cout << N - ans << '\n';

	return 0;
}
