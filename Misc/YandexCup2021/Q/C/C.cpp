#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::vector<int> A(N);
	int tot = 0;
	for (auto& a : A) { cin >> a; tot += a; }
	std::vector<int> B(N);
	int totB = 0;
	for (auto& b : B) { cin >> b; totB += b; }

	int max_cnt = tot;
	for (int i = 0; i < N; i++) {
		if (B[i]) max_cnt = min(max_cnt, A[i] / B[i]);
	}

	while (tot % max_cnt) --max_cnt;

	int sz = tot / max_cnt;

	cout << max_cnt << ' ' << sz << '\n';

	std::vector<int> clone; clone.reserve(totB);
	for (int i = 0; i < N; i++) {
		for (int z = 0; z < B[i]; z++) clone.push_back(i);
		A[i] -= B[i] * max_cnt;
	}

	int cur = 0;
	for (int z = 0; z < max_cnt; z++) {
		for (int a = 0; a < sz; a++) {
			if (a < totB) cout << clone[a]+1;
			else {
				while (!A[cur]) ++cur;
				cout << cur+1;
				--A[cur];
			}
			cout << " \n"[a+1==sz];
		}
	}

	return 0;
}
