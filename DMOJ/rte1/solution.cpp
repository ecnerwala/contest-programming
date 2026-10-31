#include "problem.h"

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

struct edge {
	int u, v; double w;
	int other(int a) const { assert(u == a || v == a); return u ^ v ^ a; }
};

template <typename F_A, typename F_M> vector<double> PCG(const vector<double>& B, const F_A& f_a, const F_M& f_m) {
	int N = int(B.size());

	auto dot = [&](const vector<double>& a, const vector<double>& b) {
		double res = 0;
		for (int i = 0; i < N; i++) res += a[i] * b[i];
		return res;
	};

	vector<double> X(N, 0);
	vector<double> R = B;

	vector<double> P(N, 0); // initialize to all 0's
	double prv_rz = 1; // dummy value, doesn't matter

	vector<double> Z(N);
	vector<double> W(N);
	for (int k = 0; k <= 2 && dot(R, R) / N > 1e-6; k++) {
		f_m(R, Z);
		double rz = dot(R, Z);
		double beta = rz / prv_rz;
		//cerr << "beta " << beta << '\n';
		for (int i = 0; i < N; i++) {
			P[i] *= beta;
			P[i] += Z[i];
		}
		prv_rz = rz;

		f_a(P, W);
		double pw = dot(P, W);
		double alpha = rz / pw;
		//cerr << "alpha " << alpha << '\n';
		for (int i = 0; i < N; i++) {
			X[i] += alpha * P[i];
			R[i] -= alpha * W[i];
		}
		// TODO: break if small error
		cerr << k << ' ' << prv_rz << '\n';
	}
	//for (int i = 0; i < N; i++) { cerr << X[i] << ' '; } cerr << '\n';

	return X;
}

auto makeLDLMultiplier(int N, vector<vector<pair<int, double>>> &&lower, vector<double> &&diag) {
	return [N, lower = std::move(lower), diag = std::move(diag)] (const vector<double>& x, vector<double>& y) {
		for (int i = 0; i < N; i++) y[i] = x[i];

		for (int i = 0; i < N; i++) {
			for (auto it : lower[i]) {
				y[it.first] += y[i] * it.second;
			}
		}
		for (int i = 0; i < N; i++) {
			y[i] *= diag[i];
		}

		for (int i = N-1; i >= 0; i--) {
			for (auto it : lower[i]) {
				y[i] += y[it.first] * it.second;
			}
		}
	};
}

auto makeCholeskyInverter(int N, const vector<edge>& edges) {
	vector<unordered_map<int, double>> adj(N);
	for (auto e : edges) {
		assert(e.u < e.v);
		//cerr << "edge " << e.u << ' ' << e.v << ' ' << e.w << '\n';
		adj[e.u][e.v] += e.w;
	}

	vector<vector<pair<int, double>>> lower(N);
	vector<double> diag(N);

	for (int i = 0; i < N; i++) {
		if (adj[i].empty()) {
			continue;
		}
		double d = 0;
		for (auto it : adj[i]) d += it.second;

		double invD = 1/d;
		diag[i] = invD;

		// TODO: do this by sampling
		for (auto jt : adj[i]) {
			for (auto kt : adj[i]) {
				if (jt.first < kt.first) {
					adj[jt.first][kt.first] += jt.second * kt.second * invD;
				}
			}
		}

		lower[i] = {adj[i].begin(), adj[i].end()};
		for (auto& it : lower[i]) it.second /= d;
	}

	return makeLDLMultiplier(N, std::move(lower), std::move(diag));
}

auto makeSparseCholeskyInverter(int N, const vector<edge>& edges) {
	cerr << "Starting to make inverter" << '\n';
	const int RHO = 2;
	vector<vector<pair<int, double>>> adj(N);
	for (auto e : edges) {
		assert(e.u < e.v);
		for (int z = 0; z < RHO; z++) {
			adj[e.u].emplace_back(e.v, e.w / RHO);
		}
	}

	mt19937_64 mt(48);

	vector<vector<pair<int, double>>> lower(N);
	vector<double> diag(N);
	for (int i = 0; i < N; i++) {
		if (i % 100000 == 0) cerr << i << ' ' << adj[i].size() << '\n';
		if (adj[i].empty()) {
			continue;
		}
		for (auto it : adj[i]) {
			assert(it.first > i);
		}

		double d = 0;
		vector<double> pdf(adj[i].size());
		for (int z = 0; z < int(adj[i].size()); z++) {
			d += adj[i][z].second;
			pdf[z] = adj[i][z].second;
		}

		double invD = 1/d;
		diag[i] = invD;

		std::discrete_distribution sampler(pdf.begin(), pdf.end());

		for (auto jt : adj[i]) {
			auto kt = adj[i][sampler(mt)];

			int j = jt.first;
			int k = kt.first;

			double w = 1 / (1 / jt.second + 1 / kt.second);
			if (j < k) {
				adj[j].emplace_back(k, w);
			} else if (k < j) {
				adj[k].emplace_back(j, w);
			} else {
				// do nothing
			}
		}

		map<int, double> mp;
		for (auto it : adj[i]) mp[it.first] += it.second;
		lower[i] = {mp.begin(), mp.end()};
		for (auto& it : lower[i]) it.second /= d;
	}

	cerr << "Finished" << '\n';

	return makeLDLMultiplier(N, std::move(lower), std::move(diag));
}

vector<double> solveLap(int N, vector<edge> edges, vector<double> B) {
	mt19937 mt(48);
	vector<int> P(N);
	iota(P.begin(), P.end(), 0);
	shuffle(P.begin(), P.end(), mt);

	vector<double> permB(N);
	for (int i = 0; i < N; i++) {
		permB[P[i]] = B[i];
	}
	for (auto& e : edges) {
		e.u = P[e.u];
		e.v = P[e.v];
	}

	for (auto& e : edges) {
		if (e.u > e.v) swap(e.u, e.v);
	}

	vector<double> permY = PCG(permB,
		[&](const vector<double>& x, vector<double>& y) {
			fill(y.begin(), y.end(), 0);
			for (auto e : edges) {
				double v = e.w * (x[e.u] - x[e.v]);
				y[e.u] += v;
				y[e.v] -= v;
			}
		},
		//[&](const vector<double>& x, vector<double>& y) { copy(x.begin(), x.end(), y.begin()); }
		//makeCholeskyInverter(N, edges)
		makeSparseCholeskyInverter(N, edges)
	);

	vector<double> Y(N);
	for (int i = 0; i < N; i++) {
		Y[i] = permY[P[i]];
	}
	return Y;
}

} // namespace ecnerwala

vector<double> FindVoltages(int N, vector<int> U, vector<int> V, vector<double> W, vector<double> B) {
	vector<ecnerwala::edge> edges;
	for (int i = 0; i < int(U.size()); i++) {
		edges.push_back({U[i]-1, V[i]-1, W[i]});
	}
	return ecnerwala::solveLap(N, edges, B);
}
