#include <bits/stdc++.h>

std::string solve(std::string S) {
	using std::cerr;
	//cerr << "Running case " << S << '\n';
	int N = int(S.size());
	assert(N > 0);
	if (N <= 2) {
		std::sort(S.begin(), S.end());
		return S;
	}
	assert(N >= 3);
	std::array<char, 3> pref;
	pref[0] = S[0];
	pref[1] = S[1];
	pref[2] = 'A' ^ 'B' ^ 'C' ^ S[0] ^ S[1];
	std::vector<int> stk; stk.reserve(N);
	for (int i = 1; i+1 < N; i++) {
		assert(S[i] != S[i-1] && S[i] != S[i+1]);
		if (S[i-1] == S[i+1]) {
			// this guy is bad
			int v = i % 3;
			if (stk.empty()) {
				if (v == 0) {
					std::swap(pref[1], pref[2]);
				} else if (v == 1) {
					stk.push_back(v);
				} else if (v == 2) {
					std::swap(pref[0], pref[1]);
				} else assert(false);
			} else {
				if (v == stk.back()) stk.pop_back();
				else stk.push_back(v);
			}
		}
	}
	while (!stk.empty() && stk.back() != (N-2) % 3) stk.pop_back();
	// stk + pref now accurately represents the equivalence class
	// Now, we first want pref as early as possible, we can prepend things of the form AA, BB, and ABABAB
	std::reverse(stk.begin(), stk.end());

	std::vector<int> min_pos; min_pos.reserve(int(stk.size())+1);
	min_pos.push_back(N-1);

	auto step_back = [&](int tgt, int v) -> int {
		assert(0 <= tgt && tgt < 3);
		// special case: N-1 can eat extra things
		if (v == N-1 && tgt != (N-2) % 3) return N-1;
		v--;
		while ((v - tgt) % 3 != 0) v--;
		return v;
	};
	for (int i = 0; i < int(stk.size()); i++) {
		min_pos.push_back(step_back(stk[i], min_pos.back()));
	}
	//for (auto v : stk) { cerr << v << ' '; } cerr << '\n';
	//for (auto v : min_pos) { cerr << v << ' '; } cerr << '\n';
	//for (char c : pref) { cerr << c; } cerr << '\n';
	assert(min_pos.back() >= 1);
	auto step_back_tgt = [&](std::array<char, 3> tgt_pref, int src) -> int {
		// Apply some number of things to the
		if (tgt_pref == pref) {
			return src;
		} else if (tgt_pref[0] == pref[0]) {
			// Must do a 0
			return step_back(0, src);
		} else if (tgt_pref[2] == pref[2]) {
			// Must do a 2
			return step_back(2, src);
		} else if (tgt_pref[1] == pref[1]) {
			// Must do 020 or 202
			return std::max(
				step_back(2, step_back(0, step_back(2, src))),
				step_back(0, step_back(2, step_back(0, src)))
			);
		} else if (tgt_pref[0] == pref[1]) {
			// Must do 02
			return step_back(0, step_back(2, src));
		} else if (tgt_pref[1] == pref[0]) {
			// Must do 20
			return step_back(2, step_back(0, src));
		} else assert(false);
	};
	std::array<char, 3> tgt_pref;
	std::vector<int> common_stk;
	std::vector<int> new_stk;
	std::string ans(N, '?');
	for (char c0 = 'A'; c0 <= 'C'; c0++) {
		for (char c1 = 'A'; c1 <= 'C'; c1++) {
			if (c0 == c1) continue;
			tgt_pref = {c0, c1, char('A' ^ 'B' ^ 'C' ^ c0 ^ c1)};
			//cerr << c0 << ' ' << c1 << '\n';
			if (!(0 < step_back_tgt(tgt_pref, min_pos.back()))) continue;
			ans[0] = c0;
			ans[1] = c1;

			min_pos.push_back(step_back_tgt(tgt_pref, min_pos.back()));
			// just push something?
			if (tgt_pref == pref) {
				common_stk.push_back(-1);
				min_pos.pop_back();
			}

			for (int i = 1; i+1 < N; i++) {
				assert(min_pos.back() >= i);
				char swap_option = 'A' ^ 'B' ^ 'C' ^ ans[i-1] ^ ans[i];
				char same_option = ans[i-1];
				auto try_do_same = [&]() -> bool {
					// same means push a 0
					int a = i % 3;
					assert(int(common_stk.size()) <= int(new_stk.size()) + 1);
					if (!new_stk.empty() && a == new_stk.back()) {
						if (int(common_stk.size()) == int(new_stk.size()) + 1) {
							assert(common_stk.back() == new_stk.back());
							// Everything on our side is common
							if (i < step_back(a, min_pos.back())) {
								common_stk.pop_back();
								new_stk.pop_back();
								stk.push_back(a);
								min_pos.push_back(step_back(a, min_pos.back()));
								return true;
							} else {
								return false;
							}
						} else {
							new_stk.pop_back();
							min_pos.pop_back();
							assert(i < min_pos.back());
							return true;
						}
					} else if (new_stk.empty() && (a == 0 || a == 2)) {
						if (int(common_stk.size()) == int(new_stk.size()) + 1) {
							// undoing 1 change is just that change again
							if (i < step_back(a, min_pos.back())) {
								if (a == 0) std::swap(tgt_pref[1], tgt_pref[2]);
								else if (a == 2) std::swap(tgt_pref[0], tgt_pref[1]);
								else assert(false);

								common_stk.pop_back();
								min_pos.push_back(step_back_tgt(tgt_pref, min_pos.back()));
								return true;
							} else {
								return false;
							}
						} else {
							auto tmp_tgt_pref = tgt_pref;
							if (a == 0) std::swap(tmp_tgt_pref[1], tmp_tgt_pref[2]);
							else if (a == 2) std::swap(tmp_tgt_pref[0], tmp_tgt_pref[1]);
							if (i < step_back_tgt(tmp_tgt_pref, min_pos.end()[-2])) {
								tgt_pref = tmp_tgt_pref;
								min_pos.pop_back();
								if (tgt_pref == pref) {
									common_stk.push_back(-1);
								} else {
									min_pos.push_back(step_back_tgt(tgt_pref, min_pos.back()));
								}
								return true;
							} else {
								return false;
							}
						}
					} else {
						// push a onto the new stack
						if (int(common_stk.size()) == int(new_stk.size()) + 1 && (!stk.empty() && stk.back() == a)) {
							new_stk.push_back(a);
							stk.pop_back();
							min_pos.pop_back();
							common_stk.push_back(a);
							assert(i < min_pos.back());
							return true;
						} else {
							if (i < step_back(a, min_pos.back())) {
								new_stk.push_back(a);
								min_pos.push_back(step_back(a, min_pos.back()));
								return true;
							} else {
								return false;
							}
						}
					}
				};
				auto try_do_swap = [&]() -> bool {
					// swap mean we don't push anything, so easy to check
					if (min_pos.back() > i) {
						return true;
					}
					return false;
				};
				if (same_option < swap_option) {
					// same means push a 0
					if (try_do_same()) {
						// good
						ans[i+1] = same_option;
					} else if (try_do_swap()) {
						// swap
						ans[i+1] = swap_option;
					} else assert(false);
				} else {
					// same means push a 0
					if (try_do_swap()) {
						ans[i+1] = swap_option;
					} else if (try_do_same()) {
						ans[i+1] = same_option;
					} else assert(false);
				}
			}
			return ans;
		}
	}
	assert(false);
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int T; cin >> T;
	while (T--) {
		std::string S; cin >> S;
		cout << solve(S) << '\n';
	}

	return 0;
}
