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
	std::vector<int64_t> freq; freq.reserve(N);
	std::vector<int64_t> nfreq; nfreq.reserve(N);
	freq.push_back(K-1);
	int cur = 0;
	std::string res; res.reserve(2*(N-1));
	for (int z = 0; z < N-1; z++) {
		nfreq.assign(freq.size() + 1, 0);
		int64_t cnt = freq.back() * int(freq.size());
		for (int i = 0; i < int(freq.size()); i++) {
			// down nfreq.size() - 1 - i, right i + 1
			int d = int(nfreq.size() - 1 - i);
			int64_t td = (freq[i] / int(nfreq.size())) * d + std::min<int64_t>(d, freq[i] % int(nfreq.size()));
			nfreq[i] += td;
			nfreq[i+1] += freq[i] - td;
		}
		for (int i = 0; i < int(nfreq.size()); i++) {
			nfreq[i] = cnt / int(nfreq.size()) + (i < cnt % int(nfreq.size()));
		}
		for (int j = int(freq.size()) - 2; j >= 0; j--) {
			assert(freq[j] >= freq[j+1]);
			assert(freq[j] <= freq[j+1] + 1);
			if (freq[j] > freq[j+1]) {
				for (int i = 0; i <= j; i++) {
					nfreq[(nfreq[i] <= nfreq[i+1]) ? i : i+1]++;
				}
			}
		}

		if (nfreq[cur] <= nfreq[cur+1]) {
			res += 'D';
		} else {
			res += 'R';
			cur++;
		}

		std::swap(freq, nfreq);
	}

	for (int i = N-2; i >= 0; i--) {
		res += char('D' ^ 'R' ^ res[i]);
	}
	cout << res << '\n';

	return 0;
}

// 0 1 0 2 1 3 0 2 1 3 2 3 0 1 0 2 1 3 0 2 1 3 2 3 0 1 0 2 1 3 0 2 1 3 2 3 0 1 0 2 1 3 0 2 1 3 2 3 0 1 0 2 1 3 0 2 1 3 2 3 0 1 0 2 1 3 0 2 1 3 2 3 0 1 0 2 1 3 0 2 1 3 2 3 0 1 0 2 1 3 0 2 1 3 2 3
// 0 1 0 2 1 3 0 2 1 4 3 4 0 2 1 3 2 4 0 3 1 4 2 3 0 1 0 2 1 4 0 3 2 4 3 4 1 2 0 3 1 4 0 2 1 3 2 4 0 1 0 3 2 4 1 3 2 4 3 4

