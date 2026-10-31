#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N; N--;
	array<int, 2> V0;
	array<vector<int>, 2> A;
	for (int z = 0; z < 2; z++) {
		string S; cin >> S;
		assert(int(S.size()) == N+1);
		A[z].reserve(N);
		V0[z] = S[0] - 'a';
		for (int i = 0; i < N; i++) {
			int v = (S[i+1] - S[i] + 3) % 3;
			assert(v == 1 || v == 2);
			v--;
			if (v) {
				A[z].push_back(i);
			}
		}
	}

	int D0 = (V0[1] - V0[0] + 3) % 3;

	//for (int z = 0; z < 2; z++) { for (int v : A[z]) { cerr << v << ' '; } cerr << '\n'; }

	// we have the positions of the 1s
	vector<pair<int64_t, int>> offset_cost(int(A[0].size()) + int(A[1].size()) + 7);
	// offset = i + 3 + A[1].size() - j
	// bigger offset is bigger i -> smaller j
	for (int i = 0, j = 0; i < int(A[0].size()) || j < int(A[1].size()); ) {
		int cur_offset = i + 3 + int(A[1].size()) - j;
		if (j < int(A[1].size()) && (i == int(A[0].size()) || A[1][j] <= A[0][i])) {
			// at smaller than cur_offset, + A[1][j]
			// at cur offset and bigger, -= A[1][j]
			offset_cost[0].first += A[1][j];
			offset_cost[0].second += 1;
			offset_cost[cur_offset].first -= 2 * A[1][j];
			offset_cost[cur_offset].second -= 2;
			j++;
		} else {
			// at <= cur_offset, -= A[0][i]
			// otherwise += A[0][i]
			offset_cost[0].first -= A[0][i];
			offset_cost[0].second -= 1;
			offset_cost[cur_offset+1].first += 2 * A[0][i];
			offset_cost[cur_offset+1].second += 2;
			i++;
		}
	}

	for (int i = 1; i < int(offset_cost.size()); i++) {
		offset_cost[i].first += offset_cost[i-1].first;
		offset_cost[i].second += offset_cost[i-1].second;
	}

	int64_t ans = 1e18;
	for (int i = 0; i < int(offset_cost.size()); i++) {
		int64_t cnd = offset_cost[i].first;
		int cur_0 = (i - 3 - int(A[1].size())) % 3;
		if (cur_0 < 0) cur_0 += 3;
		if (cur_0 != D0) continue;
		cnd -= abs(i - 3 - int(A[1].size())) * int64_t(-1);
		cnd += abs(i - 3 - int(A[0].size())) * int64_t(N);
		//cerr << i << ' ' << cnd << '\n';
		ans = min(ans, cnd);
	}
	cout << ans << '\n';

	return 0;
}

// Sequence of + and -, you can change +- to -+ and vice versa, or the first one
//
// +++
// -++
// +-+
// ++-
