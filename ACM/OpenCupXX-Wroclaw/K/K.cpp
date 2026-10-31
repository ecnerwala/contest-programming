#include<bits/stdc++.h>
using namespace std;

const double PI = acos(-1.);

template <typename T> T sq(T a) { return a * a; }
double p15(double x) { return x * sqrt(x); }

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int Q; cin >> Q;
	while (Q--) {
		using i64 = int64_t;

		i64 Xs, Ys, Zs; cin >> Xs >> Ys >> Zs;
		i64 Xd, Yd, Zd, R; cin >> Xd >> Yd >> Zd >> R;
		i64 A, B, C, D; cin >> A >> B >> C >> D;

		double ans = PI * double(sq(A*Xs + B*Ys + C*Zs + D)) * double(sq(R)) * sqrt((sq(Xs - Xd) + sq(Ys - Yd) + sq(Zs - Zd) - sq(R)) * (sq(A) + sq(B) + sq(C))) / p15(sq(A * (Xs - Xd) + B * (Ys - Yd) + C * (Zs - Zd)) - sq(R) * (sq(A) + sq(B) + sq(C)));
		cout << fixed << setprecision(9) << ans << '\n';
	}

	return 0;
}
