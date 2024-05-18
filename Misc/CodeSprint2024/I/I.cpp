#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	auto solve_code = [&](std::vector<double>& v) -> double {
		std::sort(v.begin(), v.end());
		assert(!v.empty());
		std::vector<double> o; o.reserve(v.size()-1);
		int vidx = 0;
		int oidx = 0;
		double ans = 0;
		auto pop = [&]() -> double {
			if (vidx != int(v.size()) && (oidx == int(o.size()) || v[vidx] < o[oidx])) {
				return v[vidx++];
			} else {
				return o[oidx++];
			}
		};
		for (int z = 0; z < int(v.size())-1; z++) {
			double a = pop();
			double b = pop();
			ans += a + b;
			o.push_back(a+b);
		}
		return ans;
	};

	int N; cin >> N;

	std::vector<double> vA(N);
	std::vector<double> vB(N);
	std::vector<double> vAB(N*N);
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			double v; cin >> v;
			vAB[i*N+j] = v;
			vA[i] += v;
			vB[j] += v;
		}
	}

	cout << fixed << setprecision(10) << solve_code(vA) + solve_code(vB) - solve_code(vAB) << '\n';

	return 0;
}
