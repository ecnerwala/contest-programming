#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;

		std::vector<int> A(2*N-1);
		for (auto& a : A) { cin >> a; a--; }
		A.front() = A.back() = 0;

		int pref = 0;
		while (pref < int(A.size()) && A[pref] != -1) pref++;
		int suff = int(A.size());
		while (suff > 0 && A[suff-1] != -1) suff--;

		if (pref == int(A.size())) {
			for (int i = 0; i < int(A.size()); i++) {
				cout << A[i]+1 << " \n"[i+1==int(A.size())];
			}
			continue;
		}

		std::vector<bool> used(N);
		for (auto a : A) {
			if (a != -1) used[a] = true;
		}

		std::vector<int> pref_path; pref_path.reserve(N);
		for (int i = 0; i < pref; i++) {
			if (pref_path.size() >= 2 && A[i] == pref_path.end()[-2]) {
				pref_path.pop_back();
			} else {
				pref_path.push_back(A[i]);
			}
		}

		std::vector<int> suff_path; suff_path.reserve(N);
		for (int i = int(A.size()) - 1; i >= suff; i--) {
			if (suff_path.size() >= 2 && A[i] == suff_path.end()[-2]) {
				suff_path.pop_back();
			} else {
				suff_path.push_back(A[i]);
			}
		}

		int z = 0;
		while (z < int(pref_path.size()) && z < int(suff_path.size()) && pref_path[z] == suff_path[z]) z++;
		z--; // z is the lca
		assert(z >= 0);

		std::vector<int> path; path.reserve(N);
		for (int i = int(pref_path.size()) - 1; i > z; i--) {
			path.push_back(pref_path[i]);
		}
		path.push_back(pref_path[z]);
		for (int i = z+1; i < int(suff_path.size()); i++) {
			path.push_back(suff_path[i]);
		}
		std::reverse(path.begin(), path.end());

		std::vector<int> unused; unused.reserve(N);
		for (int i = 0; i < N; i++) {
			if (!used[i]) unused.push_back(i);
		}

		int idx = pref;
		auto it = unused.begin();
		while (it != unused.end() || path.size() > 1) {
			if (it == unused.end() || (path.size() >= 2 && path.end()[-2] < *it)) {
				A[idx++] = path.end()[-2];
				path.pop_back();
			} else {
				A[idx++] = *it;
				path.push_back(*it);
				++it;
			}
		}
		assert(idx == suff+1);

		for (int i = 0; i < int(A.size()); i++) {
			cout << A[i]+1 << " \n"[i+1==int(A.size())];
		}
	}

	return 0;
}
