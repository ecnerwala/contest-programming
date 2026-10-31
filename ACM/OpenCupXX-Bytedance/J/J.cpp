#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int N, Q, K, seed; cin >> N >> Q >> K >> seed;
	vector<int> A(N);
	for (int i = 0; i < N; i++) {
		cin >> A[i];
	}
	vector<int> pref(N+1);
	pref[0] = N;
	for (int i = 0; i < N; i++) pref[i+1] = min(pref[i], A[i]);
	vector<int> suff(N+1);
	suff[N] = N;
	for (int i = N-1; i >= 0; i--) suff[i] = min(suff[i+1], A[i]);

	// swap i-1 and i
	auto do_swap = [&](int i) {
		swap(A[i-1], A[i]);
		pref[i] = min(pref[i-1], A[i-1]);
		suff[i] = min(suff[i+1], A[i]);
	};

	auto query_mex = [&](int i, int j) -> int {
		return min(pref[i], suff[j+1]);
	};

	std::mt19937 gen;
	gen.seed(seed);
	int last = 0;
	while (Q--) {
		int op = gen() % K;
		int i = (gen() + last) % N;
		if (!op && i) {
			do_swap(i);
		} else {
			int j = gen() % N;
			last ^= query_mex(std::min(i, j), std::max(i, j));
		}
	}
	cout << last << '\n';

	return 0;
}
