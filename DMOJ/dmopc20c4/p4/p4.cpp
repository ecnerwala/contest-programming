#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	vector<int> A(N);
	for (auto& a : A) cin >> a;

	vector<int> C(N);
	for (int i = 0; i < N; i++) {
		C[i] = A[i];
	}
	for (int i = 1; i < N; i++) {
		C[i] = max(C[i], C[i-1]);
	}
	for (int i = 0; i < N; i++) {
		C[i] -= i;
	}
	int64_t cur_ans = 0;
	vector<pair<int, int>> vals; vals.reserve(N);
	for (int i = 0; i < N; i++) {
		cur_ans += i - A[i];
		int cur_cnt = 1;
		while (!vals.empty() && C[i] >= vals.back().first) {
			cur_ans -= int64_t(vals.back().first) * int64_t(vals.back().second);
			cur_cnt += vals.back().second;
			vals.pop_back();
		}
		vals.push_back({C[i], cur_cnt});
		cur_ans += int64_t(vals.back().first) * int64_t(vals.back().second);
		cout << cur_ans << " \n"[i+1==N];
	}

	return 0;
}
