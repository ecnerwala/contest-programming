#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int N, Q, K, seed; cin >> N >> Q >> K >> seed;
	vector<int> A(N);
	for (int i = 0; i < N; i++) {
		cin >> A[i];
	}

	if (K == 1) {
		auto query_mex = [&](int i, int j) {
			int ans = N;
			while (++j < N) {
				ans = min(ans, A[j]);
			}
			return ans;
		};
		std::mt19937 gen;
		gen.seed(seed);
		int last = 0;
		while (Q--) {
			int op = gen() % K;
			int i = (gen() + last) % N;
			if (!op && i) {
				swap(A[i-1], A[i]);
			} else {
				int j = gen() % N;
				last ^= query_mex(std::min(i, j), std::max(i, j));
			}
		}
		cout << last << '\n';
		exit(0);
	}

	vector<int> invA(N);
	for (int i = 0; i < N; i++) {
		invA[A[i]] = i;
	}

	const int S = 1 << 18;
	vector<pair<int, int>> seg(S*2);

	auto updateNode = [&](int i) {
		seg[i] = {min(seg[2*i].first, seg[2*i+1].first), max(seg[2*i].second, seg[2*i+1].second)};
	};
	auto init = [&]() {
		for (int i = 0; i < S; i++) {
			int loc = i<N ? invA[i] : i;
			seg[S+i] = {loc, loc};
		}
		for (int i = S-1; i >= 0; i--) {
			updateNode(i);
		}
	};

	auto updateVal = [&](int v) {
		seg[S+v] = {invA[v], invA[v]};
		for (int a = (S+v)/2; a; a /= 2) updateNode(a);
	};

	// swap i-1 and i
	auto do_swap = [&](int i) {
		swap(A[i-1], A[i]);
		invA[A[i-1]] = i-1;
		invA[A[i]] = i;
		updateVal(A[i-1]);
		updateVal(A[i]);
	};

	auto query_mex = [&](int i, int j) -> int {
		//int cur = S;
		//while (i <= seg[cur].first && seg[cur].second <= j) cur /= 2;
		int cur = 1;
		while (cur < S) {
			if (i <= seg[2 * cur].first && seg[2 * cur].second <= j) {
				// we can move right
				cur = 2 * cur + 1;
			} else {
				cur = 2 * cur;
			}
		}
		int ans = cur-S;
		return ans;
	};

	init();
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
