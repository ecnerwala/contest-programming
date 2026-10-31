#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	if (false) {
		struct state_t {
			int N;
			std::vector<int> seq;
		};
		auto grow = [&](state_t s, int delta) -> state_t {
			assert(delta == +1 || delta == -1);
			int N = s.N + delta;
			std::vector<int> freq(N);
			std::vector<int> nseq;
			while (true) {
				for (int a : s.seq) {
					int nv;
					if (delta == -1 && a == 0) {
						nv = 0;
					} else if (delta == -1 && a == s.N - 1) {
						nv = N-1;
					} else {
						int c0 = a - (delta == -1), c1 = a+1 - (delta == -1);
						nv = (freq[c0] <= freq[c1]) ? c0 : c1;
					}
					nseq.push_back(nv);
					freq[nv]++;
				}
				if (*std::min_element(freq.begin(), freq.end()) == *std::max_element(freq.begin(), freq.end())) break;
			}
			return {N, nseq};
		};

		state_t s{1, std::vector<int>({0})};
		for (int z = 0; z < 7; z++) {
			s = grow(s, +1);
			cerr << "N = " << s.N << ' ' << "len = " << s.seq.size() << '\n';
			for (int v : s.seq) { cerr << v << ' '; } cerr << '\n';
		}
		for (int z = 0; z < 7; z++) {
			s = grow(s, -1);
			cerr << "N = " << s.N << ' ' << "len = " << s.seq.size() << '\n';
			for (int v : s.seq) { cerr << v << ' '; } cerr << '\n';
		}
	}

	int N; int64_t K; cin >> N >> K;
	std::vector<int64_t> freq(N);
	freq[0] = K-1;
	int cur = 0;
	std::string res; res.reserve(2*(N-1));
	for (int l = 2; l <= N; l++) {
		for (int i = l-2; i >= 0; i--) {
			// down l - 1 - i, right i + 1
			int d = l - 1 - i;
			int64_t td = (freq[i] / l) * d + std::min<int64_t>(d, freq[i] % l);
			freq[i+1] += freq[i] - td;
			freq[i] = td;
		}
		if (freq[cur] <= freq[cur+1]) {
			res += 'D';
		} else {
			res += 'R';
			cur++;
		}
	}

	for (int i = N-2; i >= 0; i--) {
		res += char('D' ^ 'R' ^ res[i]);
	}
	cout << res << '\n';

	return 0;
}

// 0 1 0 2 1 3 0 2 1 3 2 3 0 1 0 2 1 3 0 2 1 3 2 3 0 1 0 2 1 3 0 2 1 3 2 3 0 1 0 2 1 3 0 2 1 3 2 3 0 1 0 2 1 3 0 2 1 3 2 3 0 1 0 2 1 3 0 2 1 3 2 3 0 1 0 2 1 3 0 2 1 3 2 3 0 1 0 2 1 3 0 2 1 3 2 3
// 0 1 0 2 1 3 0 2 1 4 3 4 0 2 1 3 2 4 0 3 1 4 2 3 0 1 0 2 1 4 0 3 2 4 3 4 1 2 0 3 1 4 0 2 1 3 2 4 0 1 0 3 2 4 1 3 2 4 3 4

