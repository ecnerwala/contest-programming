#include <bits/stdc++.h>

template <typename T> void setmax(T& a, T b) { if (b > a) a = b; }
template <typename T> void setmin(T& a, T b) { if (b < a) a = b; }

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	// Length of 1 cycle
	const int C = 360 * 100 * 3;

	int N; cin >> N;
	vector<int> A; A.reserve(N+2); A.resize(N);
	for (int i = 0; i < N; i++) {
		string s; cin >> s;
		double v; cin >> v;
		A[i] = int(v * 100 + 0.5) * 3 % C;
	}
	sort(A.begin(), A.end());
	for (int i = 0; i < 2; i++) A.push_back(A[i]+C);

	int D; cin >> D; D *= 25;

	// Note that we must stay for the flash after the last photograph, so the
	// last photograph and first photograph must be at least D apart when
	// considered mod 2*C.
	//
	// Thus, we can model this problem as placing the N points onto a cycle of
	// length 2*C so that any 2 adjacent are distance at least D apart, and
	// trying to maximize the size of the maximum gap between two adjacent ones.

	auto ans = [&]() -> std::optional<int> {
		// First, prune out any locations which are far from both neighbors;
		// they essentially always can be placed outside of the target gap.
		vector<int> B; B.reserve(N+3);
		for (int i = 0; i < N; i++) {
			if (A[i+1] - A[i] >= D && A[i+2] - A[i+1] >= D) {
				continue;
			} else {
				B.push_back(A[i+1]);
			}
		}
		int M = int(B.size());

		// If all vertices are far from their neighbors, then we can have a
		// full-cycle gap. Just pick the best place to insert it.
		if (B.empty()) {
			int resp = 0;
			for (int i = 0; i < N; i++) {
				setmax(resp, A[i+1] - A[i]);
			}
			return resp + C;
		}

		for (int i = 0; i < 3; i++) B.push_back(B[i]+C);

		// Now all remaining vertices have at least 1 neighbor that must be on
		// the "opposite cycle" as them. These essentially always alternate
		// sides, so the adjacent vertices are just between B[i] and B[i+2].
		// We might be able to do a "stutter" when we find a break
		// (B[i+1] - B[i] >= D) in order to get a bigger gap (B[i-1]..B[i+2]).
		int best_gap = 0;
		int num_break = 0;
		int best_break_gap = 0;
		for (int i = 0; i < M; i++) {
			if (B[i+2] - B[i] < D) return std::nullopt;
			setmax(best_gap, B[i+2] - B[i]);
			if (B[i+2] - B[i+1] >= D) {
				num_break++;
				setmax(best_break_gap, B[i+3] - B[i]);
			}
		}

		if (num_break <= 1) {
			// If there is at most 1 break, then the parities are all fixed.

			// If M is odd, then we are forced to not stutter, so just return best_gap.
			if (M & 1) {
				return best_gap;
			} else {
				// If M is even, we are forced to stutter once, so there must be a break.
				if (num_break == 0) {
					return std::nullopt;
				} else {
					// Note that a stutter is always bigger than a non-stutter over the
					// gap, so it's still fine to use best_gap even when the stutter is forced.
					return max(best_gap, best_break_gap);
				}
			}
		} else {
			// Otherwise, stutters are always okay.
			return max(best_gap, best_break_gap);
		}
	}();

	if (ans) {
		int v = ((2 * C - *ans + D) + 25 - 1) / 25;
		cout << v << '\n';
	} else {
		cout << "not possible" << '\n';
	}

	return 0;
}
