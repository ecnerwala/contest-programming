#include <bits/stdc++.h>

template <typename T> T mod_inv_in_range(T a, T m) {
	// assert(0 <= a && a < m);
	T x = a, y = m;
	// coeff of a in x and y
	T vx = 1, vy = 0;
	while (x) {
		T k = y / x;
		y %= x;
		vy -= k * vx;
		std::swap(x, y);
		std::swap(vx, vy);
	}
	assert(y == 1);
	return vy < 0 ? m + vy : vy;
}

template <typename T> struct extended_gcd_result {
	T gcd;
	T coeff_a, coeff_b;
};
template <typename T> extended_gcd_result<T> extended_gcd(T a, T b) {
	T x = a, y = b;
	// coeff of a and b in x and y
	T ax = 1, ay = 0;
	T bx = 0, by = 1;
	while (x) {
		T k = y / x;
		y %= x;
		ay -= k * ax;
		by -= k * bx;
		std::swap(x, y);
		std::swap(ax, ay);
		std::swap(bx, by);
	}
	return {y, ay, by};
}

template <typename T> T mod_inv(T a, T m) {
	a %= m;
	a = a < 0 ? a + m : a;
	return mod_inv_in_range(a, m);
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int CASES; cin >> CASES;
	while (CASES--) {
		int N, M; cin >> N >> M;
		std::string S; cin >> S;
		std::string T; cin >> T;
		std::array<std::vector<int>, 2> max_match;
		for (int z = 0; z < 2; z++) {
			std::vector<int> z_fn(N, -1);
			z_fn[0] = N;
			for (int best_i = 1, best_j = 1, i = 1; i < N; i++) {
				int j = std::min(best_j, i + z_fn[i - best_i]);
				j = std::max(j, i);
				if (j < best_j) {
					assert(S[j] != S[j-i]);
				}

				while (j < N && S[j] == S[j-i]) {
					j++;
				}
				if (j >= best_j) {
					best_i = i;
					best_j = j;
				}
				z_fn[i] = j - i;
			}
			max_match[z].resize(M+1);
			for (int best_i = 0, best_j = 0, i = 0; i < M; i++) {
				int j = std::min(best_j, i + (i - best_i >= N ? 0 : z_fn[i - best_i]));
				j = std::max(j, i);
				while (j < M && j-i < N && T[j] == S[j-i]) {
					j++;
				}
				if (j >= best_j) {
					best_i = i;
					best_j = j;
				}
				max_match[z][i] = j - i;
			}
			max_match[z][M] = 0;
			std::reverse(S.begin(), S.end());
			std::reverse(T.begin(), T.end());
		}
		std::reverse(max_match[1].begin(), max_match[1].end());
		//for (auto v : max_match[0]) { cerr << v << ' '; } cerr << '\n';
		//for (auto v : max_match[1]) { cerr << v << ' '; } cerr << '\n';
		// find out if S is periodic
		int period;
		{
			std::vector<int> fail(N+1, -1);
			for (int i = 0; i < N; i++) {
				int cur = fail[i];
				while (cur != -1 && S[cur] != S[i]) {
					cur = fail[cur];
				}
				fail[i+1] = cur+1;
			}
			period = N - fail[N];
			if (N % period != 0) {
				period = -1;
			}
		}
		bool T_is_period = false;
		if (period != -1 && M % period == 0) {
			T_is_period = true;
			for (int i = 0; i < M; i++) {
				if (T[i] != S[i % period]) {
					T_is_period = false;
					break;
				}
			}
		}
		std::vector<int> q1; q1.reserve(M+1);
		std::vector<int> q2; q2.reserve(M+1);
		auto solve = [&](int i) -> bool {
			int j = N-i;
			if (period != -1 && i % period == 0) {
				assert(j % period == 0);
				if (!T_is_period) return false;
				int g = std::gcd(i, j);
				if (M % g != 0) return false;
				int a = i / g;
				int b = j / g;
				int l = M / g;
				// can l be written as ax + by
				// y = modinv(b, a) * l % a
				return l >= (int64_t(mod_inv(b, a)) * l % a) * b;
			}
			// otherwise, just check things
			q1.clear(); q2.clear();
			q1.push_back(0);
			for (int a = 0, b = 0; true; ) {
				int va = a < int(q1.size()) ? q1[a] : M+1;
				int vb = b < int(q2.size()) ? q2[b] : M+1;
				int v = std::min(va, vb);
				if (va == v) a++;
				if (vb == v) b++;
				if (v == M) return true;
				if (v > M) return false;

				if (v + i <= M && max_match[0][v] >= i) q1.push_back(v+i);
				if (v + j <= M && max_match[1][v+j] >= j) q2.push_back(v+j);
			}
		};
		std::string ans; ans.reserve(N-1);
		for (int i = 1; i < N; i++) {
			bool res = solve(i);
			ans += "01"[res];
		}
		cout << ans << '\n';
	}

	return 0;
}
