#include <bits/stdc++.h>

int64_t solve(int64_t c, int64_t p, int64_t t) {
	// how many cows < c are in positions < p after time t
	// after step t, there are t+1 cows at most
	// after t=2, there are 2 cows in the prefix
	// after t=3, there are 2 or 3 cows in the prefix
	// after t=4, there are 3 cows in the prefix
	// after step t, we have a prefix of length t/2 + 1
	int64_t pref = t/2 + 1;
	if (p >= pref || c >= pref) {
		return std::min({p, c, t+1});
	}
	assert(p < pref && c < pref);

	// offset is the offset from the back of the biggest guy
	int off = (t & 1) ? 2 : 1;

	// number from the end
	p = pref - p;

	while (pref > c) {
		int64_t num_remove = std::min(pref - c, pref / 3 + (off < pref % 3));
		int64_t n_pref = pref - num_remove;
		assert(n_pref >= c);
		p -= std::min(num_remove, (p / 3 + (off < p % 3)));
		off -= int(pref % 3);
		if (off < 0) off += 3;
		pref = n_pref;
	}
	return pref-p;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int Q; cin >> Q;
	while (Q--) {
		int64_t c_lo, c_hi, p_lo, p_hi, t; cin >> c_lo >> c_hi >> p_lo >> p_hi >> t;
		c_hi++, p_hi++;
		int64_t ans = 0;
		ans += solve(c_hi, p_hi, t);
		ans -= solve(c_lo, p_hi, t);
		ans -= solve(c_hi, p_lo, t);
		ans += solve(c_lo, p_lo, t);
		cout << ans << '\n';
	}

	return 0;
}
