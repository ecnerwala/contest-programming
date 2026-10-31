#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M, L; cin >> N >> M >> L;
	vector<int> A(N); for (auto& a : A) { cin >> a; a--; }
	sort(A.begin(), A.end());
	assert(N >= 1);
	vector<int> seqs; seqs.reserve(2*N);
	for (int i = 0; i+1 < int(A.size()); i++) {
		int cnt = A[i+1] - A[i] - 1;
		seqs.push_back(cnt / 2);
		seqs.push_back((cnt+1) / 2);
	}
	seqs.push_back(A.front());
	seqs.push_back(L - 1 - A.back());
	sort(seqs.begin(), seqs.end());

	assert(std::accumulate(seqs.begin(), seqs.end(), N) == L);

	int ans = 0;
	for (int i = 0; true; i++) {
		assert(seqs[i] >= ans);
		int cnt = seqs[i] - ans;
		int num = int(seqs.size()) - i;
		int v = cnt * num;
		if (M > v) {
			ans += cnt;
			M -= v;
		} else {
			ans += (M + num - 1) / num;
			M = 0;
			break;
		}
	}
	cout << ans << '\n';

	return 0;
}
