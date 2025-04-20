#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

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
	for (int z = 0; z < 10; z++) {
		s = grow(s, +1);
		cerr << "N = " << s.N << ' ' << "len = " << s.seq.size() << '\n';
		for (int v : s.seq) { cerr << v << ' '; } cerr << '\n';
	}

	return 0;
}
