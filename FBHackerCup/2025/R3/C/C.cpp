#include<bits/stdc++.h>

using bs = std::bitset<int(1e6) + 10>;

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {
		int N, K; cin >> N >> K;
		std::vector<std::string> WC(N);
		for (auto& c : WC) cin >> c;
		auto expand = [&](std::string C) -> std::string {
			std::string w;
			int rep = -1;
			for (char c : C) {
				if ('a' <= c && c <= 'z') {
					if (rep == -1) rep = 1;
					w.append(rep, c);
					rep = -1;
				} else if ('0' <= c && c <= '9') {
					if (rep == -1) {
						rep = 0;
					}
					rep *= 10;
					rep += (c - '0');
				} else assert(false);
			}
			assert(rep == -1);
			return w;
		};
		cerr << "Case #" << case_num << ": " << N << ' ' << K << '\n';

		bs res; res[0] = true;
		std::string prv;
		for (int z = 0; z < N; z++) {
			std::string cur = expand(WC[z]);
			int tot_len = int(cur.size());
			//cerr << z << ' ' << tot_len << '\n';
			bs nres = res << tot_len;
			auto do_ap = [&](int max_overlap, int step, int num_steps) -> void {
				//cerr << "match " << max_overlap << ' ' << step << ' ' << num_steps << '\n';
				assert(max_overlap >= step);
				assert(max_overlap >= step * (num_steps - 1));
				assert(num_steps >= 1);
				bs tmp = res << (tot_len - max_overlap);
				int i = 0;
				while ((1 << (i+1)) <= num_steps) {
					tmp = tmp | (tmp << (step << i));
					i++;
				}
				nres |= tmp;
				assert(num_steps >= (1 << i));
				if (num_steps > (1 << i)) nres |= tmp << (num_steps * step - (step << i));
			};
			{
				int L = int(cur.size());
				std::vector<int> fail(L+1);
				fail[0] = -1;
				for (int i = 0; i < L; i++) {
					int cnd = fail[i];
					while (cnd >= 0 && cur[i] != cur[cnd]) cnd = fail[cnd];
					fail[i+1] = cnd+1;
				}
				int match = 0;
				for (char c : prv) {
					while (match >= 0 && c != cur[match]) match = fail[match];
					match++;
				}
				while (match > 0) {
					int step = match - fail[match];
					int num_steps = match / step;
					do_ap(match, step, num_steps + 1);
					match -= (num_steps - 1) * step;
					match = fail[match];
				}
			}
			res = nres;
			prv = cur;
		}
		int64_t ans = 0;
		for (int i = 1; i <= K; i++) {
			if (res[i]) ans += i;
		}
		cout << "Case #" << case_num << ": " << ans << '\n' << flush;
	}

	return 0;
}
