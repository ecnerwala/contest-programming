#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int N, M, Q; cin >> N >> M >> Q;
	/*
	vector<string> G(N, string(M, '\0'));
	for (auto& r : G) {
		r.reserve(M);
		cin >> r;
		assert(int(r.size()) == M);
	}
	*/
	vector<string> G(M, string(N, '\0'));
	for (int i = 0; i < N; i++) {
		string r; cin >> r;
		for (int j = 0; j < M; j++) {
			G[j][i] = r[j];
		}
	}
	swap(N, M);

	vector<int> cc_id((N+1)*(M+1), -1);
	vector<int> pref((N+2)*(M+2), 0);
	vector<int> epref1((N+2)*(M+2), 0);
	vector<int> epref2((N+2)*(M+2), 0);
	{
		auto get_par = [&](int a) {
			while (cc_id[a] >= 0) {
				if (cc_id[cc_id[a]] >= 0) cc_id[a] = cc_id[cc_id[a]];
				a = cc_id[a];
			}
			return a;
		};
		auto merge = [&](int a, int b) {
			a = get_par(a);
			b = get_par(b);
			if (a == b) return false;
			if (cc_id[b] < cc_id[a]) swap(a, b);
			cc_id[a] += cc_id[b];
			cc_id[b] = a;
			return true;
		};

		for (int i = 0; i <= N; i++) {
			for (int j = 0; j <= M; j++) {
				if (i && (j == 0 || j == M || G[i-1][j-1] != G[i-1][j])) {
					epref1[(i+1) * (M+2) + (j+1)]++;
					merge((i-1)*(M+1)+j, i*(M+1)+j);
				}
				if (j && (i == 0 || i == N || G[i-1][j-1] != G[i][j-1])) {
					epref2[(i+1) * (M+2) + (j+1)]++;
					merge(i*(M+1)+(j-1), i*(M+1)+j);
				}
			}
		}

		for (int i = 0; i <= N; i++) {
			for (int j = 0; j <= M; j++) {
				if (cc_id[i*(M+1)+j] >= 0) {
					cc_id[i*(M+1)+j] = get_par(i*(M+1)+j);
				}
			}
		}

		for (int i = 0; i <= N; i++) {
			for (int j = 0; j <= M; j++) {
				if (cc_id[i*(M+1)+j] < 0) {
					cc_id[i*(M+1)+j] = i*(M+1)+j;
					pref[(i+1)*(M+2) + (j+1)]++;
				}
			}
		}
	}
	for (int i = 0; i <= N; i++) {
		for (int j = 0; j <= M; j++) {
			pref[(i+1)*(M+2) + (j+1)] += pref[(i+1)*(M+2) + j] + pref[i*(M+2) + (j+1)] - pref[i*(M+2) + j];
			epref1[(i+1)*(M+2) + (j+1)] += epref1[(i+1)*(M+2) + j] + epref1[i*(M+2) + (j+1)] - epref1[i*(M+2) + j];
			epref2[(i+1)*(M+2) + (j+1)] += epref2[(i+1)*(M+2) + j] + epref2[i*(M+2) + (j+1)] - epref2[i*(M+2) + j];
		}
	}

	vector<bool> has_cc((N+1)*(M+1));

	// inclusive interval
	auto get_rect = [&](const vector<int>& p, int xlo, int xhi, int ylo, int yhi) {
		xhi++, yhi++;
		assert(xlo <= xhi && ylo <= yhi);

		xlo *= (M+2), xhi *= (M+2);

		return p[xhi+yhi] - p[xhi+ylo] - p[xlo+yhi] + p[xlo+ylo];
	};

	for (int q = 0; q < Q; q++) {
		int xlo, ylo, xhi, yhi; cin >> xlo >> ylo >> xhi >> yhi;
		swap(xlo, ylo); swap(xhi, yhi);
		xlo--, ylo--;
		int ans = 1 + get_rect(pref, xlo, xhi, ylo, yhi);

		{ // subtract off cc's not completely contained
			{
				auto check_node = [&](int x, int y) {
					int c = cc_id[x * (M+1) + y];
					if (xlo <= c/(M+1) && c/(M+1) <= xhi && ylo <= c%(M+1) && c%(M+1) <= yhi) {
						if (!has_cc[c]) {
							ans--;
							has_cc[c] = true;
						}
					}
				};
				for (int y = ylo; y <= yhi; y++) {
					check_node(xlo, y);
					check_node(xhi, y);
				}
				for (int x = xlo; x <= xhi; x++) {
					check_node(x, ylo);
					check_node(x, yhi);
				}
			}
			{
				auto check_node = [&](int x, int y) {
					int c = cc_id[x * (M+1) + y];
					if (xlo <= c/(M+1) && c/(M+1) <= xhi && ylo <= c%(M+1) && c%(M+1) <= yhi) {
						has_cc[c] = false;
					}
				};
				for (int y = ylo; y <= yhi; y++) {
					check_node(xlo, y);
					check_node(xhi, y);
				}
				for (int x = xlo; x <= xhi; x++) {
					check_node(x, ylo);
					check_node(x, yhi);
				}
			}
		}

		// interior verts and interior edges; exterior ones form a cycle so they cancel
		int v_int = (xhi - xlo - 1) * (yhi - ylo - 1);
		int e1_int = get_rect(epref1, xlo+1, xhi, ylo+1, yhi-1);
		int e2_int = get_rect(epref2, xlo+1, xhi-1, ylo+1, yhi);

		ans -= v_int;
		ans += e1_int;
		ans += e2_int;

		cout << ans << '\n';
	}

	return 0;
}
