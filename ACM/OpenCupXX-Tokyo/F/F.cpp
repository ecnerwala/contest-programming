#include<bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
	int N; cin >> N;
	vector<int> A(N); for (auto& a : A) cin >> a;
	vector<int> B(N); for (auto& b : B) cin >> b;

	int64_t ans = 0;
	for (int i = 0; i < N; i++) {
		ans += abs(A[i] - B[i]);
	}

	vector<int> ops; ops.reserve(N);

	vector<int> inds; inds.reserve(N);
	for (int i = 0; i < N; ) {
		if (B[i] < A[i]) {
			if (inds.empty() || A[i] - B[i] < B[i] - A[inds.back()]) {
				ops.push_back(i); i++;
			} else {
				ops.push_back(inds.back()); inds.pop_back();
			}
		} else {
			inds.push_back(i); i++;
		}
	}

	ops.insert(ops.end(), inds.rbegin(), inds.rend());

	cout << ans << '\n';
	for (int i = 0; i < N; i++) {
		cout << ops[i]+1 << " \n"[i+1==N];
	}

	return 0;
}
