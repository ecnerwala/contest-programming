#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	vector<int> A(N); for (auto& a : A) cin >> a;
	vector<int> B(N); for (auto& b : B) cin >> b;
	vector<array<int, 2>> match(N, array<int, 2>{-1, -1});

	for (int z = 0; z < 2; z++) {
		unordered_map<int, int> in_stack;
		vector<int> stk; stk.reserve(N);
		for (int j = 0; j < N; j++) {
			int i = z ? N-1-j : j;
			while (!stk.empty() && stk.back() <= A[i]) {
				in_stack.erase(stk.back());
				stk.pop_back();
			}
			stk.push_back(A[i]);
			in_stack[A[i]] = i;

			if (in_stack.count(B[i])) {
				match[i][z] = in_stack[B[i]];
			}
		}
	}

	vector<int> V; V.reserve(2*N);
	for (int i = 0; i < N; i++) {
		// match[i][0] is to the left, match[i][1] is to the right
		// We can just process match[i][1] first and the match[i][0]
		if (match[i][1] != -1) {
			V.push_back(match[i][1]);
		}
		if (match[i][0] != -1 && match[i][0] != match[i][1]) {
			V.push_back(match[i][0]);
		}
	}

	// find the maximum (weakly) increasing subsequence of V
	vector<int> best; best.reserve(V.size());
	for (int v : V) {
		int mi = -1;
		int ma = int(best.size());
		while (ma - mi > 1) {
			int md = (mi + ma) / 2;
			if (v >= best[md]) mi = md;
			else ma = md;
		}
		if (ma == int(best.size())) best.push_back(v);
		else best[ma] = v;
	}

	cout << best.size() << '\n';

	return 0;
}

// 1. We can do just ops with 2 people at a time.
//
// 2. People can only increase, so we should do smaller values first.
//
// 3. If we consider the smallest B which passes, it has to come from someone's
//   original score; that original score has to "flow" from the original place to
//   the final place, without crossing anyone bigger in the middle.
//   Afterwards, no bigger ops can cross this B, which means that we can solve
//   the left and right halves separately.
//
//   This gives O(N^3) dp: for each interval dp[interval] = max_{choice of smallest B} dp[left half] + dp[right half] + 1
//
//
// Consider the maximum A; that value never decreases, so smaller values never
// cross; Then, we should do something on the left and something on the right
// then expand the A some amount left and right; there's an interval of the
// smaller stuff that's left exposed
//
//
// Each B either comes from the leftmost equal A, comes from the rightmost equal A, or is impossible
// What you need, as you sweep from left to right
//   A1 <= A2
