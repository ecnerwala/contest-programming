#include<bits/stdc++.h>
using namespace std;

namespace std {

template<class Fun>
class y_combinator_result {
	Fun fun_;
public:
	template<class T>
	explicit y_combinator_result(T &&fun): fun_(std::forward<T>(fun)) {}

	template<class ...Args>
	decltype(auto) operator()(Args &&...args) {
		return fun_(std::ref(*this), std::forward<Args>(args)...);
	}
};

template<class Fun>
decltype(auto) y_combinator(Fun &&fun) {
	return y_combinator_result<std::decay_t<Fun>>(std::forward<Fun>(fun));
}

} // namespace std

int main() {
	ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
	int N; cin >> N;
	int A, B; cin >> A >> B;
	vector<int> X(N);
	for (int i = 0; i < N; i++) {
		cin >> X[i];
	}

	vector<vector<double>> init_dp(N, vector<double>(A+1));
	init_dp[0][0] = 1;

	vector<double> coeff(N);
	coeff[0] = 1./N;
	for (int i = 1; i < N; i++) {
		coeff[i] = coeff[i-1] * i / (N - i);
	}

	double ans = 0;
	std::y_combinator([&](auto self, vector<vector<double>> dp, int l, int r) {
		if (r - l == 1) {
			for (int i = 0; i < N; i++) {
				for (int j = max(0, A+1 - X[l]); j <= min(A, B - X[l]); j++) {
					ans += dp[i][j] * coeff[i];
				}
			}
			return;
		}
		int m = (l+r) / 2;
		{
			vector<vector<double>> ndp = dp;
			for (int z = l; z < m; z++) {
				for (int i = N-2; i >= 0; i--) {
					for (int j = 0; j <= A - X[z]; j++) {
						ndp[i+1][j+X[z]] += ndp[i][j];
					}
				}
			}
			self(std::move(ndp), m, r);
		}
		{
			vector<vector<double>> ndp = dp;
			for (int z = m; z < r; z++) {
				for (int i = N-2; i >= 0; i--) {
					for (int j = 0; j <= A - X[z]; j++) {
						ndp[i+1][j+X[z]] += ndp[i][j];
					}
				}
			}
			self(std::move(ndp), l, m);
		}
	})(std::move(init_dp), 0, N);

	cout << fixed << setprecision(10) << ans << '\n';

	return 0;
}
