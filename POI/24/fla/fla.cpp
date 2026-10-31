#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; int X; cin >> N >> X;
	struct obstacle_t {
		int x; int64_t a, b;
	};
	vector<obstacle_t> obstacles(N+2);
	obstacles[0] = {0, 0, 0};
	for (int i = 1; i <= N; i++) {
		cin >> obstacles[i].x >> obstacles[i].a >> obstacles[i].b;
		obstacles[i].a++;
		obstacles[i].b--;
	}
	obstacles[N+1] = {X, -int64_t(1e18), int64_t(1e18)};
	for (auto& o : obstacles) {
		if ((o.a & 1) != (o.x & 1)) o.a++;
		if ((o.b & 1) != (o.x & 1)) o.b--;
	}
	for (int i = 0; i <= N; i++) {
		obstacles[i+1].a = max(obstacles[i+1].a, obstacles[i].a - (obstacles[i+1].x - obstacles[i].x));
	}
	for (int i = N; i >= 0; i--) {
		obstacles[i].a = max(obstacles[i].a, obstacles[i+1].a - (obstacles[i+1].x - obstacles[i].x));
	}
	for (int i = 0; i <= N; i++) {
		if (obstacles[i].a > obstacles[i].b) {
			cout << "NIE" << '\n';
			exit(0);
		}
	}
	cout << (obstacles[N+1].a + X) / 2 << '\n';

	return 0;
}
