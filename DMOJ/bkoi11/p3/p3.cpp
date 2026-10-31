#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	vector<int> B(N);
	for (int i = 0; i < N; i++) {
		cin >> B[i]; B[i]--;
	}
	vector<int> A(2*N-1, -1);

	vector<bool> used(2*N-1);
	used[B[0]] = true;
	A[0] = B[0];

	vector<int> lo_inds;
	vector<int> hi_inds;

	for (int i = 1; i < N; i++) {
		if (B[i] == B[i-1]) {
			lo_inds.push_back(2*i-1);
			hi_inds.push_back(2*i);
		} else if (B[i] < B[i-1]) {
			// got smaller
			lo_inds.push_back(2*i-1);
			lo_inds.push_back(2*i);

			if (!used[B[i]]) {
				used[B[i]] = true;
				A[lo_inds.back()] = B[i];
				lo_inds.pop_back();
			}
		} else if (B[i] > B[i-1]) {
			hi_inds.push_back(2*i-1);
			hi_inds.push_back(2*i);

			if (!used[B[i]]) {
				used[B[i]] = true;
				A[hi_inds.back()] = B[i];
				hi_inds.pop_back();
			}
		} else assert(false);
	}

	vector<int> unused; unused.reserve(lo_inds.size() + hi_inds.size());
	for (int i = 0; i < 2*N-1; i++) {
		if (!used[i]) unused.push_back(i);
	}
	assert(unused.size() == lo_inds.size() + hi_inds.size());
	for (int lo = 0; lo < int(lo_inds.size()); lo++) {
		A[lo_inds[lo]] = unused[lo];
	}
	for (int hi = 0; hi < int(hi_inds.size()); hi++) {
		A[hi_inds[hi]] = unused[int(unused.size())-1-hi];
	}

	for (int i = 0; i < 2*N-1; i++) {
		cout << A[i]+1 << " \n"[i+1==2*N-1];
	}

	return 0;
}
