#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int CASES; cin >> CASES;
	while (CASES--) {
		int N, T1000; cin >> N >> T1000;
		std::vector<int> E1000(N);
		for (auto& e : E1000) cin >> e;

		struct piece_t {
			double tf;
			double constant;
			std::vector<double> poly;
		};

		auto eval_at = [&](const piece_t& p, double t) -> double {
			double r = 0;
			r += p.constant;
			double v = 0;
			for (int z = int(p.poly.size()) - 1; z >= 0; z--) {
				v *= t;
				v /= z+1;
				v += p.poly[z];
			}
			v *= std::exp(-t);
			r += v;
			return r;
		};

		auto integ_from = [&](const piece_t& p, double ta, double tb) -> double {
			double r = 0;
			r += p.constant * (tb - ta);
			{
				double sum = 0;
				double vb = 0;
				double va = 0;
				for (int z = int(p.poly.size()) - 1; z >= 0; z--) {
					va *= ta;
					vb *= tb;
					va /= z+1;
					vb /= z+1;

					sum += p.poly[z];

					va += sum;
					vb += sum;
				}
				va *= std::exp(-ta);
				vb *= std::exp(-tb);
				assert(va >= vb);
				r += (va - vb);
			}
			return r;
		};

		std::vector<piece_t> cur;
		cur.push_back(piece_t{T1000 / 1000., 0, {}});
		for (auto e1000 : E1000) {

			double x0 = e1000 / 1000.;
			std::cerr << "handle " << x0 << '\n';

			double t0 = 0;
			std::vector<piece_t> nxt;
			auto it = cur.begin();
			for (; it != cur.end(); ++it) {
				const auto& p = *it;
				if (eval_at(p, t0) <= x0 / (1 + t0)) {
					// we're done
					break;
				} else if (eval_at(p, p.tf) >= (x0 + integ_from(p, t0, p.tf)) / (1 + p.tf)) {
					// take the whole thing
					x0 += integ_from(p, t0, p.tf);
					t0 = p.tf;
					continue;
				}

				// otherwise, binary search
				double mi = t0;
				double ma = p.tf;
				for (int z = 0; z < 64; z++) {
					double md = mi + (ma - mi) / 2;
					if (eval_at(p, md) >= (x0 + integ_from(p, t0, md)) / (1 + md)) {
						mi = md;
					} else {
						ma = md;
					}
				}

				// go until mi
				x0 += integ_from(p, t0, mi);
				t0 = mi;
				break;
			}

			cerr << "constant part " << x0 << ' ' << t0 << '\n';
			x0 /= (1 + t0);

			if (t0 > 0) {
				// push in the constant flow
				// constant flow
				nxt.push_back(piece_t{t0, x0, {}});
			}

			for (; it != cur.end(); ++it) {
				const auto& p = *it;
				assert(x0 >= eval_at(p, t0));
				piece_t np{
					p.tf,
					p.constant,
					p.poly
				};
				{
					double v = 0;
					for (int z = int(p.poly.size()) - 1; z >= 0; z--) {
						v += p.poly[z];
						v *= t0;
						v /= z+1;
					}
					np.poly.insert(np.poly.begin(), std::exp(t0) * (x0 - p.constant) - v);
				}
				x0 = eval_at(np, np.tf);
				t0 = np.tf;
				nxt.push_back(std::move(np));
			}

			cur = std::move(nxt);
		}

		double x = 0;
		{
			double t0 = 0;
			for (const auto& p : cur) {
				x += integ_from(p, t0, p.tf);
				t0 = p.tf;
			}
			cerr << t0 << '\n';
		}
		cout << fixed << setprecision(20) << x << '\n';
	}

	return 0;
}
