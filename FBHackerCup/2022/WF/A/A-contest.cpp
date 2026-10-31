#include<bits/stdc++.h>

auto sq = [](auto z) { return z * z; };

std::array<std::array<std::vector<std::pair<double, std::array<int, 2>>>, 51>, 51> by_dist;
void precomp() {
	for (int ax = 0; ax <= 50; ax++) {
		for (int ay = 0; ay <= 50; ay++) {
			for (int bx = 0; bx <= 50; bx++) {
				for (int by = 0; by <= 50; by++) {
					// weight this positively by the area fraction
					double l = std::sqrt(sq(ax - bx) + sq(ay - by)) / 2;
					by_dist[ax][ay].push_back({l, {bx, by}});
				}
			}
			std::sort(by_dist[ax][ay].begin(), by_dist[ax][ay].end());
			std::reverse(by_dist[ax][ay].begin(), by_dist[ax][ay].end());
		}
	}
}

std::tuple<int, int, int, int, int> solve(std::vector<std::array<double, 2>> P) {
	int N = int(P.size());

	//N = std::min(N, 100000);

	std::array<std::array<double, 51>, 51> probs;
	std::array<std::array<double, 51>, 51> max_rs;
	std::vector<std::pair<double, std::array<int, 2>>> by_max_r; by_max_r.reserve(51 * 51);
	for (int x = 0; x <= 50; x++) {
		for (int y = 0; y <= 50; y++) {
			double v = 0;
			double max_r2 = 0;
			for (int i = 0; i < N; i++) {
				double r2 = sq(P[i][0] - x) + sq(P[i][1] - y);
				max_r2 = std::max(max_r2, r2);
				v -= std::log(std::max(r2, 1e-18))/2;
			}
			probs[x][y] = v / N;
			max_rs[x][y] = std::sqrt(max_r2);
			by_max_r.push_back({max_rs[x][y], {x, y}});
		}
	}

	std::sort(by_max_r.begin(), by_max_r.end());

	const double EPS = 1e-9;

	double best_v = -1e99;
	std::tuple<int, int, int, int, int> ans;
	for (auto [ra, pa] : by_max_r) {
		auto [ax, ay] = pa;
		for (int r = 1; r <= 50; r++) {
			if (ra - r > EPS) continue;

			//if (probs[ax][ay] - log(r) - log(0.01) < best_v) continue;
			for (auto [l, pb] : by_dist[ax][ay]) {
				auto [bx, by] = pb;
				auto rb = max_rs[bx][by];
				if (rb - r > EPS) continue;
				if (sq(ax - bx) + sq(ay - by) >= sq(2 * r)) {
					continue;
				}
				double theta = std::acos(l / r);
				double shared_area = theta * r * r / 2 - l * std::sqrt(r * r - l * l) / 2;
				shared_area *= 4;
				shared_area /= M_PI * r * r;
				double cnd = probs[ax][ay] - std::log(2 * M_PI * r) - std::log(shared_area);
				if (cnd >= best_v) {
					best_v = cnd;
					ans = {ax, ay, bx, by, r};
				}
				break;
			}
			//if (r > ra + 5) break;
		}
	}

	std::cerr << std::exp(best_v) << '\n';
	return ans;
}

void test() {
	std::mt19937 mt(48);
	int num_good = 0;
	for (int z = 0; z < 100; z++) {
		int AX, AY, BX, BY, R;
		while (true) {
			AX = std::uniform_int_distribution(0, 50)(mt);
			AY = std::uniform_int_distribution(0, 50)(mt);
			BX = std::uniform_int_distribution(0, 50)(mt);
			BY = std::uniform_int_distribution(0, 50)(mt);
			R = std::uniform_int_distribution(1, 50)(mt);
			if (AX == BX && AY == BY) continue;
			if (sq(AX - BX) + sq(AY - BY) >= sq(2 * R)) {
				continue;
			}
			break;
		}

		int N = 1000;
		std::vector<std::array<double, 2>> P(N);
		for (auto& [x, y] : P) {
			while (true) {
				double r = std::uniform_real_distribution<double>(0, R)(mt);
				double theta = std::uniform_real_distribution<double>(0, 2 * M_PI)(mt);
				x = AX + r * std::cos(theta);
				y = AY + r * std::sin(theta);
				if (sq(x - BX) + sq(y - BY) > sq(R)) {
					continue;
				}
				break;
			}
		}
		//std::cerr << AX << ' ' << AY << ' ' << BX << ' ' << BY << ' ' << R << '\n';
		auto [ax, ay, bx, by, r] = solve(P);
		bool is_good = (ax == AX && ay == AY && bx == BX && by == BY && r == R);
		num_good += is_good;
		std::cerr << "iter " << z << ' ' << is_good << ' ' << double(num_good) / (z+1) << '\n';
		if (!is_good) {
			std::cerr << AX << ' ' << AY << ' ' << BX << ' ' << BY << ' ' << R << '\n';
			std::cerr << ax << ' ' << ay << ' ' << bx << ' ' << by << ' ' << r << '\n';
		}
	}
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	precomp();

	//test();

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {
		int N; cin >> N;
		std::vector<std::array<double, 2>> P(N);
		for (auto& [x, y] : P) cin >> x >> y;

		cerr << "Case #" << case_num << ' ' << N << '\n';

		auto ans = solve(P);

		auto [ax, ay, bx, by, r] = ans;
		cerr << "Case #" << case_num << ": " << ax << ' ' << ay << ' ' << bx << ' ' << by << ' ' << r << '\n';
		cout << "Case #" << case_num << ": " << ax << ' ' << ay << ' ' << bx << ' ' << by << ' ' << r << '\n';
	}

	return 0;
}
