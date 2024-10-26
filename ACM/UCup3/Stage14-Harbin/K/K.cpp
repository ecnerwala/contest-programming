#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; int64_t M; cin >> N >> M;
	struct crop_t {
		int w;
		std::array<int, 2> b;
	};
	std::vector<crop_t> V(N);
	for (auto& [w, b] : V) {
		cin >> w >> b[0] >> b[1];
	}
	std::sort(V.begin(), V.end(), [](auto a, auto b) { return a.w > b.w; });

	int64_t t_L = M;
	int64_t t_L_val = 0;
	for (int i = 0; i < N; i++) {
		t_L -= V[i].b[0];
		t_L_val += int64_t(V[i].w) * int64_t(V[i].b[0]);
		V[i].b[1] -= V[i].b[0];
	}
	assert(t_L >= 0);
	int64_t ans = t_L_val + int64_t(t_L) * int64_t(V[0].w);

	std::vector<int64_t> pref_R(N+1);
	std::vector<int64_t> pref_R_val(N+1);
	for (int i = 0; i < N; i++) {
		pref_R[i+1] = pref_R[i] + V[i].b[1];
		pref_R_val[i+1] = pref_R_val[i] + int64_t(V[i].b[1]) * int64_t(V[i].w);
	}
	for (int i = 1; i < N; i++) {
		if (pref_R[i] < t_L) continue;
		assert(pref_R[i] >= t_L);
		int64_t extra = std::min(t_L + V[i].b[0], pref_R[i]);
		int idx = int(std::lower_bound(pref_R.begin() + 0, pref_R.begin() + i, extra) - pref_R.begin());
		if (idx == 0) idx = 1;
		assert(1 <= idx && idx <= i);
		idx--;
		assert(pref_R[idx] <= extra);
		assert(extra <= pref_R[idx+1]);
		int64_t cnd = t_L_val - int64_t(extra - t_L) * int64_t(V[i].w) + pref_R_val[idx] + int64_t(extra - pref_R[idx]) * int64_t(V[idx].w);
		ans = std::max(ans, cnd);
	}

	cout << ans << '\n';

	return 0;
}
