#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, K; cin >> N >> K;
	std::vector<int> A(N);
	for (auto& a : A) cin >> a;

	std::vector<char> S(K);
	for (auto& c : S) cin >> c;
	S.reserve(N);
	for (int i = 0; int(S.size()) < N; i++) {
		S.push_back(S[i]);
	}
	K = int(S.size());
	assert(K >= N);

	std::vector<int> run_lengths; run_lengths.reserve(K+1);
	std::vector<bool> incr; incr.reserve(K);

	run_lengths.push_back(1);
	for (char c : S) {
		if (c == '=') {
			run_lengths.back()++;
		} else if (c == '>') {
			run_lengths.push_back(1);
			incr.push_back(false);
		} else if (c == '<') {
			run_lengths.push_back(1);
			incr.push_back(true);
		} else assert(false);
	}

	const int V = (*std::max_element(A.begin(), A.end()))+1;
	std::vector<std::vector<int>> locs(V);
	std::vector<int> A_idx(N);
	for (int i = 0; i < N; i++) {
		A_idx[i] = int(locs[A[i]].size());
		locs[A[i]].push_back(i);
	}

	std::pair<std::pair<int, int>, int> best{{-1, 0}, -1};
	std::vector<int> prv(N);

	std::vector<std::vector<std::pair<int, int>>> to_insert(N);

	std::vector<int> best_val; best_val.reserve(run_lengths.size());
	std::vector<int> best_idx; best_idx.reserve(run_lengths.size());
	int lo = -1;

	for (int i = 0; i < N; i++) {
		{
			int mi = lo, ma = int(best_val.size());
			assert(lo == -1 || ma - mi >= 2);
			while (ma - mi > 1) {
				int md = (mi + ma) / 2;
				assert(md < int(incr.size()));
				if (best_val[md] != A[i] && (best_val[md] < A[i]) == incr[md]) {
					mi = md;
				} else {
					ma = md;
				}
			}
			assert(ma < int(run_lengths.size()));
			prv[i] = mi == -1 ? -1 : best_idx[mi];

			int l = run_lengths[ma];
			int my_len = std::min(l, int(locs[A[i]].size()) - A_idx[i]);
			int j = locs[A[i]][A_idx[i]+my_len-1];

			if (make_pair(ma, my_len) > best.first) {
				best.first = {ma, my_len};
				best.second = i;
			}

			if (my_len == l) {
				// actually append this event
				assert(ma < int(incr.size()));
				to_insert[j].push_back({ma, i});
			}
		}

		for (auto [len, idx] : to_insert[i]) {
			if (len == int(best_val.size())) {
				best_val.push_back(A[idx]);
				best_idx.push_back(idx);
				if (len > 0 && incr[len] != incr[len-1]) {
					lo = len-1;
				}
			} else if (A[idx] != best_val[len] && (A[idx] < best_val[len]) == incr[len]) {
				best_val[len] = A[idx];
				best_idx[len] = idx;
			}
		}
	}

	std::vector<int> B; B.reserve(N);
	{
		assert(best.second != -1);
		for (int cur = best.second, len = best.first.first; len >= 0; cur = prv[cur], len--) {
			assert(cur != -1);
			int l = len == best.first.first ? best.first.second : run_lengths[len];
			for (int z = 0; z < l; z++) {
				B.push_back(A[cur]);
			}
		}
	}

	std::reverse(B.begin(), B.end());
	cout << B.size() << '\n';
	for (int i = 0; i < int(B.size()); i++) {
		cout << B[i] << " \n"[i+1==int(B.size())];
	}

	return 0;
}
