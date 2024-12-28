#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> A(N);
		for (auto& a : A) { cin >> a; a--; }
		for (auto a : A) cerr << a << '\n';

		std::vector<std::vector<int>> inds(1 << 16);

		std::vector<std::array<int, 8>> freq(N+1);
		freq[0].fill(0);

		int msk = 0;
		std::vector<int> valid_msk; valid_msk.reserve(N);
		for (int i = 0; i < N; i++) {
			freq[i+1] = freq[i];
			freq[i+1][A[i]]++;
			msk += 1 << (2 * A[i]);
			if (((msk >> (2 * A[i])) & 3) == 3) {
				msk -= 3 << (2 * A[i]);
			}
			if (inds[msk].empty()) valid_msk.push_back(msk);
			inds[msk].push_back(i);
		}
		int64_t ans = 0;
		for (auto st_msk : valid_msk) {
			for (auto en_msk : valid_msk) {
				if ((inds[st_msk][0] - inds[en_msk][0]) % 3 != 0) continue;
				cerr << "trying msks" << '\n';

				std::array<int, 8> num_needed; num_needed.fill(0);
				for (int z = 0; z < 8; z++) {
					int st_val = (st_msk >> (2 * z)) & 3;
					int en_val = (en_msk >> (2 * z)) & 3;
					int val = en_val - st_val - num_needed[z];
					val %= 3;
					if (val < 0) val += 3;
					if (val) {
						if (z + 2 >= 8) {
							goto bad;
						}
						num_needed[z] += val;
						num_needed[z+1] += val;
						num_needed[z+2] += val;
					}
					cerr << st_val << ' ' << en_val << ' ' << num_needed[z] << '\n';
				}
				for (int i = 0, j = 0; j < int(inds[en_msk].size()); j++) {
					int en = inds[en_msk][j];
					while (i < int(inds[st_msk].size())) {
						int st = inds[st_msk][i];
						bool good = st < en;
						for (int z = 0; good && z < 8; z++) {
							if (freq[en][z] - freq[st][z] < num_needed[z]) { good = false; break; }
						}
						if (!good) break;
						cerr << "good " << st << ' ' << en << '\n';
						i++;
					}
					ans += i;
				}
bad:
				continue;
			}
		}
		cout << ans << '\n';
	}

	return 0;
}
