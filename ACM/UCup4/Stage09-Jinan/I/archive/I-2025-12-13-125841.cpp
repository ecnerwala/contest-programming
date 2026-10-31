#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		std::string Q_S; cin >> Q_S;
		int Q; cin >> Q;

		std::string S(int(Q_S.size()) + 2*Q+2, '?');
		// inclusive
		int L = Q+1;
		int R = Q;
		int64_t ans = 0;

		struct pal_t {
			int len;
			int prv_pal;
			int cnt_suff_ccpc;
			int cnt_ge3;
			std::array<int, 26> ch{
				-1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1,
				-1, -1, -1, -1, -1,
				-1
			};
		};
		std::vector<pal_t> pals; pals.reserve(2 + S.size());
		pals.push_back({-1, -1, 0, 0});
		pals.push_back({0, 0, 0, 0});
		int cur_pref = 1;
		int cur_suff = 1;
		auto do_op = [&](char side, char v) -> void {
			if (side == 'R') {
				if (v == S[R]) {
					ans += pals[cur_suff].cnt_ge3;
				}
				R++;
				S[R] = v;
				while (v != S[R - pals[cur_suff].len - 1]) {
					cur_suff = pals[cur_suff].prv_pal;
				}
				if (pals[cur_suff].ch[v - 'a'] == -1) {
					cerr << "set ch " << cur_suff << ' ' << v << ' ' << int(pals.size()) << '\n';
					pals[cur_suff].ch[v - 'a'] = int(pals.size());
					int prv_pal = pals[cur_pref].prv_pal;
					while (prv_pal != -1 && pals[prv_pal].ch[v - 'a'] == -1) prv_pal = pals[prv_pal].prv_pal;
					prv_pal = prv_pal == -1 ? 1 : pals[prv_pal].ch[v - 'a'];
					assert(prv_pal != -1);
					cerr << prv_pal << '\n';
					pals.push_back({
						pals[cur_suff].len + 2,
						prv_pal,
						pals[prv_pal].cnt_suff_ccpc + (pals[prv_pal].len >= 3 && v == S[R-pals[prv_pal].len]),
						pals[prv_pal].cnt_ge3 + (pals[cur_suff].len + 2 >= 3)
					});
				}
				cur_suff = pals[cur_suff].ch[v - 'a'];
				ans += pals[cur_suff].cnt_suff_ccpc;
				if (pals[cur_suff].len < R-L+1) {
					ans += (v == S[R-pals[cur_suff].len]);
				} else {
					cur_pref = cur_suff;
				}
			} else if (side == 'L') {
				if (v == S[L]) {
					ans += pals[cur_pref].cnt_ge3;
				}
				L--;
				S[L] = v;
				while (v != S[L + pals[cur_pref].len + 1]) {
					cur_pref = pals[cur_pref].prv_pal;
				}
				if (pals[cur_pref].ch[v - 'a'] == -1) {
					cerr << "set ch " << cur_pref << ' ' << v << ' ' << int(pals.size()) << '\n';
					pals[cur_pref].ch[v - 'a'] = int(pals.size());
					int prv_pal = pals[cur_pref].prv_pal;
					while (prv_pal != -1 && pals[prv_pal].ch[v - 'a'] == -1) prv_pal = pals[prv_pal].prv_pal;
					prv_pal = prv_pal == -1 ? 1 : pals[prv_pal].ch[v - 'a'];
					assert(prv_pal != -1);
					cerr << prv_pal << '\n';
					pals.push_back({
						pals[cur_pref].len + 2,
						prv_pal,
						pals[prv_pal].cnt_suff_ccpc + (pals[prv_pal].len >= 3 && v == S[L+pals[prv_pal].len]),
						pals[prv_pal].cnt_ge3 + (pals[cur_pref].len + 2 >= 3)
					});
				}
				cur_pref = pals[cur_pref].ch[v - 'a'];
				ans += pals[cur_pref].cnt_suff_ccpc;
				if (pals[cur_pref].len < R-L+1) {
					ans += (v == S[L+pals[cur_pref].len]);
				} else {
					cur_suff = cur_pref;
				}
			} else assert(false);
		};
		for (char c : Q_S) {
			do_op('R', c);
		}
		std::vector<int64_t> answers; answers.reserve(Q+1);
		answers.push_back(ans);
		for (int i = 0; i < Q; i++) {
			char side, v; cin >> side >> v;
			do_op(side, v);
			answers.push_back(ans);
		}
		for (int i = 0; i <= Q; i++) cout << answers[i] << " \n"[i==Q];
	}

	return 0;
}
