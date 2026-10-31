#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::vector<int> V(N);
	for (auto& v : V) cin >> v;

	std::vector<int64_t> pref(N+1);
	for (int i = 0; i < N; i++) pref[i+1] = pref[i] + V[i];

	std::vector<int> perm(N);
	for (auto& i : perm) { cin >> i; i--; }
	std::reverse(perm.begin(), perm.end());

	std::vector<int> nxt(N+1, -1);
	std::vector<int> prv(N+1, -1);

	int64_t tot_excitement = 0;
	int num_chains = 0;
	int64_t ans = 0;

	auto round_up = [](int64_t a) -> int64_t {
		return (a + 9) / 10 * 10;
	};

	for (int i : perm) {
		num_chains++;
		nxt[i] = i+1;
		prv[i+1] = i;
		tot_excitement += round_up(pref[i+1] - pref[i]);

		for (int z = i; z <= i+1; z++) {
			if (nxt[z] != -1 && prv[z] != -1) {
				tot_excitement -= round_up(pref[nxt[z]] - pref[z]);
				tot_excitement -= round_up(pref[z] - pref[prv[z]]);

				tot_excitement += round_up(pref[nxt[z]] - pref[prv[z]]);

				num_chains--;

				nxt[prv[z]] = nxt[z];
				prv[nxt[z]] = prv[z];
				nxt[z] = prv[z] = -1;
			}
		}
		ans = std::max(ans, int64_t(tot_excitement) * num_chains);
	}
	cout << ans << '\n';

	return 0;
}
