#include <bits/stdc++.h>
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

using ll = int64_t;

using ld = long double;
void solve(int t){
	cout << "Case #" << t << ": ";
	ll N;
	cin >> N;
	int P;
	cin >> P;
	auto get_best_d = [&](ll X) -> pair<int, ld> {
		ll max_D = 0;
		if(P > 0){
			max_D = 100 / P + 1;
		}
		ld cost = 1e18;
		int best_d = -1;
		for(int d = 0; d <= max_D; d++){
			int pgood = min(d * P, 100);
			ld c = ld(d + 1) / (ld(1) - ld(X) / ld(N) * (1 - ld(pgood) / ld(100)));
			if(c < cost){
				cost = c;
				best_d = d;
			}
		}
		return {best_d, cost};
	};
	ld ans = 0;

	ld TH = 5000000;
	y_combinator(
		[&](auto self, ll L, ll R) -> void {
			auto [d_L, _L] = get_best_d(L);
			auto [d_R, _R] = get_best_d(R);
			if(L != R && d_L != d_R){
				ll M = (L + R) / 2;
				self(L, M);
				self(M+1, R);
			} else {
				int pgood = min(d_L * P, 100);
				ld C = d_L + 1;
				ld B = 1 / ld(N) * (1 - ld(pgood) / ld(100));
				while(R >= N - TH && L <= R){
					ans += C / (1 - B * R);
					R--;
				}
				while(L <= TH && L <= R){
					ans += C / (1 - B * L);
					L++;
				}
				if(L <= R){
					if(pgood == 100){
						// B = 0
						ans += (R - L + 1) * C;
					} else {
						ans += C / B * (log(1 - B * (ld(L) - 0.5)) - log(1 - B * (ld(R) + 0.5)));
					}
				}
			}
		}
	)(0, N-1);

	cout << fixed << setprecision(10);
	cout << ans << '\n';
}

int main(){
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int T;
	cin >> T;
	for(int t = 1; t <= T; t++){
		solve(t);
		cout << flush;
	}
}
