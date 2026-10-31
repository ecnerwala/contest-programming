#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, K; cin >> N >> K;
	std::vector<int> I(K);
	for (auto& i : I) { cin >> i; i--; }

	std::reverse(I.begin(), I.end());

	std::vector<std::vector<int>> locs(N);
	for (int k = 0; k < K; k++) {
		locs[I[k]].push_back(k);
	}

	std::vector<bool> killed(N);
	std::vector<std::set<int>> occ(K+1);
	int num_bad = 0;
	auto is_bad = [&](int z) -> bool {
		return z >= 0 && !occ[z].empty() && !occ[z+1].empty() && *occ[z].rbegin() > *occ[z+1].begin();
	};
	int max_occ = -1;
	int num_max = 0;
	for (int i = 0; i < N; i++) {
		if (int(locs[i].size()) > max_occ) {
			max_occ = int(locs[i].size());
			num_max = 0;
		}
		num_max += int(locs[i].size()) == max_occ;

		for (int z = int(locs[i].size())-1; z >= 0; z--) {
			occ[z].insert(locs[i][z]);
		}
	}

	for (int k = 0; k < K; k++) {
		num_bad += is_bad(k);
	}

	std::vector<int> Amax;
	std::vector<int> Aother;
	for (int k = 0; k < K; k++) {
		int i = I[k];
		if (killed[i]) {
			// we can't have 2 instances of i in the last block
			break;
		}
		for (int z = int(locs[i].size())-1; z >= 0; z--) {
			num_bad -= is_bad(z);
		}
		for (int z = int(locs[i].size())-1; z >= 0; z--) {
			occ[z].erase(locs[i][z]);
			if (z > 0) occ[z-1].insert(locs[i][z]);
		}
		for (int z = int(locs[i].size())-1; z >= 0; z--) {
			num_bad += is_bad(z);
		}

		killed[i] = true;

		if (int(locs[i].size()) == max_occ) {
			num_max--;
		}

		if (num_bad == 0) {
			if (num_max > 0) {
				// Amax
				if (Amax.empty()) {
					Amax = std::vector<int>(N);
					for (int v = 0; v < N; v++) {
						Amax[v] = 1 + max_occ - (int(locs[v].size()) - killed[v]);
					}
				}
			} else {
				if (Aother.empty()) {
					Aother = std::vector<int>(N);
					for (int v = 0; v < N; v++) {
						Aother[v] = 1 + (max_occ - 1) - (int(locs[v].size()) - killed[v]);
					}
					break;
				}
			}
		}
	}

	if (Amax.empty() || (!Aother.empty() && Aother < Amax)) {
		Amax = std::move(Aother);
	}
	if (Amax.empty()) {
		cout << -1 << '\n';
	} else {
		for (int i = 0; i < N; i++) {
			cout << Amax[i] << " \n"[i+1==N];
		}
	}

	return 0;
}
