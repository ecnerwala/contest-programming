#include <bits/stdc++.h>

using bs = std::bitset<2000>;
void good_to_lucky(std::vector<bs>& vecs) {
	int N = int(vecs.size());

	// run gaussian elimination, but preserve
	std::vector<int> locs(N);
	std::vector<bs> basis(N);
	for (int i = 0; i < N; i++) {
		bs flips;
		for (int j = 0; j < i; j++) {
			if (vecs[i][locs[j]]) {
				flips[locs[j]] = true;
				vecs[i] ^= basis[j];
			}
		}
		basis[i] = vecs[i];
		assert(vecs[i].any());
		locs[i] = int(vecs[i]._Find_first());
		vecs[i] ^= flips;
	}
}

void lucky_to_good(std::vector<bs>& vecs) {
	int N = int(vecs.size());

	bs flip_mask;
	std::vector<bs> basis(N);
	std::vector<int> locs(N);
	for (int i = 0; i < N; i++) {
		bs flips = vecs[i] & flip_mask;
		vecs[i] ^= flips;

		basis[i] = vecs[i];
		assert(vecs[i].any());
		locs[i] = int(vecs[i]._Find_first());

		flip_mask[locs[i]] = true;

		for (int j = i-1; j >= 0; j--) {
			if (flips[locs[j]]) {
				vecs[i] ^= basis[j];
			}
		}
	}
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		string type; cin >> type;
		int N; cin >> N;
		std::vector<bs> vecs(N);
		for (auto& v : vecs) {
			string s; cin >> s;
			assert(int(s.size()) == N);
			for (int i = 0; i < N; i++) {
				v[i] = (s[i] == '1');
			}
		}
		if (type == "good") {
			//auto orig = vecs;
			good_to_lucky(vecs);
			//auto final = vecs;
			//lucky_to_good(final);
			//assert(final == orig);
		} else if (type == "lucky") {
			//auto orig = vecs;
			lucky_to_good(vecs);
			//auto final = vecs;
			//good_to_lucky(final);
			//assert(final == orig);
		} else assert(false);

		cout << N << '\n';
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				cout << (vecs[i][j] ? '1' : '0');
			}
			cout << '\n';
		}
	}

	return 0;
}
