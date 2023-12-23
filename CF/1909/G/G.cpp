#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M; cin >> N >> M;
	std::string S, T; cin >> S >> T;
	assert(int(S.size()) == N);
	assert(int(T.size()) == M);

	assert(M > N);

	{
		// If we pick lp = lx+ly and ls = ly+lz
		// We need T[lp:lp+delta] == T[N - ls:M - ls]
		// We're required to have the 2N >= lp + ls > N
		// lx = N - ls
		// Need max_lp >= lp > lx >= N - max_ls
		// ly = lp - lx
		// Need ly = lp - lx | M - N
		int max_lp = 0;
		while (max_lp < N && S[max_lp] == T[max_lp]) max_lp++;
		int max_ls = 0;
		while (max_ls < N && S[N-1-max_ls] == T[M-1-max_ls]) max_ls++;

		if (max_lp + max_ls <= N) {
			cout << 0 << '\n';
			exit(0);
		}
		int min_lx = N - max_ls;
		int min_lz = N - max_lp;
		assert(min_lx + min_lz < N);
		S = S.substr(min_lx, N - min_lx - min_lz);
		T = T.substr(min_lx, M - min_lx - min_lz);
		N -= min_lx + min_lz;
		M -= min_lx + min_lz;
		assert(N > 0);
		assert(int(S.size()) == N);
		assert(int(T.size()) == M);
		// Now, S should be a prefix and suffix of T
		assert(T.starts_with(S));
		assert(T.ends_with(S));
	}

	// Find two equal strings of length delta with offset dividing delta
	// T is guaranteed to be delta-periodic, so we're just trying to see if T has a smaller period
	int L = M-N;
	std::vector<int> fail(2*L+1, -1);
	int g;
	for (int i = 0; true; i++) {
		int cnd = fail[i];
		if (cnd == L) {
			g = i - L;
			break;
		}
		assert(i < 2*L);
		while (cnd >= 0 && T[i%L] != T[cnd%L]) cnd = fail[cnd];
		fail[i+1] = cnd + 1;
	}
	assert(g <= L);
	assert(L % g == 0);
	// pick multiple of g and factor of L
	int64_t ans = 0;
	for (int a = g; a <= N; a += g) {
		if (L % a) continue;
		// Now pick any two vectors which are equal mod a
		// Starting points range from 0..N-a inclusive
		ans += (N-a+1);
	}
	cout << ans << '\n';

	return 0;
}
