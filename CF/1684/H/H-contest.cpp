#include <bits/stdc++.h>

std::vector<int> solve(std::string S) {
	using namespace std;
	int N = int(S.size());

	int cur_val = 0;
	for (int i = 0; i < N; i++) {
		if (S[i] == '1') {
			cur_val++;
		}
	}

	if (cur_val == 0) {
		return {};
	} else if ((cur_val & (cur_val - 1)) == 0) {
		std::vector<int> splits(N+1);
		std::iota(splits.begin(), splits.end(), 0);
		return splits;
	}


	std::vector<int> splits; splits.reserve(N+1);

	std::vector<int> pairs; pairs.reserve(N);
	std::vector<int> trips; trips.reserve(N);

	for (int i = 0; i < N; ) {
		splits.push_back(i);
		if (S[i] == '0') {
			i++;
			continue;
		} else if (S[i] == '1') {
			if (i+1==N) {
				// this is the tail
				// insert a 1
				i++;
			} else if (S[i+1] == '0') {
				// this is a 10
				pairs.push_back(i);
				i += 2;
			} else if (S[i+1] == '1') {
				if (i+2 == N) {
					// 11 at the end
					pairs.push_back(i);
					i += 2;
				} else {
					trips.push_back(i);
					i += 3;
				}
			} else assert(false);
		} else assert(false);
	}
	splits.push_back(N);

	if (cur_val == 5) {
		// we need for 10x
		for (int i = 0; i+2 < N; i++) {
			if (S[i] == '1' && S[i+1] == '0' && i+2 < N) {
				std::vector<int> res;
				for (int j = 0; j <= N; j++) {
					if (j == i+1 || j == i+2) continue;
					res.push_back(j);
				}
				return res;
			}
		}
		// otherwise, they're all together
		int i = 0;
		while (S[i] != '1') i++;
		std::vector<int> res;
		for (int j = 0; j <= N; j++) {
			if (j == i+1 || j == i+2 || j == i+3) continue;
			res.push_back(j);
		}
		return res;
	} else if (cur_val == 9) {
		if (trips.size() == 3 && pairs.size() == 0) {
			// this is the one bad case; just redo it so we have at most 1 trips, we'll have enough pairs to finish
			splits.clear();
			trips.clear();
			pairs.clear();

			for (int i = 0; i < N; ) {
				splits.push_back(i);
				if (S[i] == '0') {
					i++;
					continue;
				} else if (S[i] == '1') {
					if (i+1==N) {
						// this is the tail
						// insert a 1
						i++;
					} else if (S[i+1] == '0') {
						// this is a 10
						pairs.push_back(i);
						i += 2;
					} else if (S[i+1] == '1') {
						if (i+2 == N) {
							// 11 at the end
							pairs.push_back(i);
							i += 2;
						} else if (trips.empty()) {
							trips.push_back(i);
							i += 3;
						} else {
							pairs.push_back(i);
							i += 2;
						}
					} else assert(false);
				} else assert(false);
			}
			splits.push_back(N);
		}
	}

	cur_val--; // we want to maximize it without hitting the next power of 2

	while (!trips.empty() && __builtin_clz(cur_val) == __builtin_clz(cur_val + 4)) {
		trips.pop_back();
		cur_val += 4;
	}
	while (__builtin_clz(cur_val) == __builtin_clz(cur_val + 1)) {
		if (!trips.empty()) {
			splits.push_back(trips.back() + 1);
			trips.pop_back();
			cur_val++;
		} else if (!pairs.empty()) {
			pairs.pop_back();
			cur_val++;
		} else {
			cerr << "Bad case " << S << '\n';
			assert(false);
		}
	}
	for (auto t : trips) {
		splits.push_back(t+1);
		splits.push_back(t+2);
	}
	for (auto p : pairs) {
		splits.push_back(p+1);
	}
	return splits;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		string S; cin >> S;
		auto splits = solve(S);
		if (splits.empty()) {
			cout << -1 << '\n';
		} else {
			std::sort(splits.begin(), splits.end());
			cout << splits.size()-1 << '\n';
			for (int i = 0; i+1 < int(splits.size()); i++) {
				cout << splits[i]+1 << ' ' << splits[i+1] << '\n';
			}
		}
	}

	return 0;
}

// 10 = 1 -> 2
// 110 = 2 -> 3 -> 6
// 111 = 3 -> 4 -> 7
//
// at the end, maybe
// 1 = 1
// 11 = 2 -> 3
