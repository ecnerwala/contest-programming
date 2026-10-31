#include <bits/stdc++.h>

int main() {
	{
		std::ofstream fout("t-bad.in");
		int T = 5;
		fout << T << '\n';
		int cyc_len = 2;
		for (int t = 0; t < T; t++, cyc_len++) {
			int N = 100;
			std::vector<int> labels(N);
			std::iota(labels.begin(), labels.end(), 0);
			std::vector<std::array<int, 2>> edges; edges.reserve(2*N);
			int prv = 0;
			for (int i = 1; i < N; i += (cyc_len-1)) {
				int j = std::min(i + (cyc_len - 1), N);
				std::vector<int> cyc;
				cyc.push_back(prv);
				for (int z = i; z < j; z++) cyc.push_back(z);
				assert(int(cyc.size()) >= 2);
				for (int z = 0; z+1 < int(cyc.size()); z++) {
					edges.push_back({cyc[z], cyc[z+1]});
				}
				if (cyc.size() > 2) {
					edges.push_back({cyc.front(), cyc.back()});
				}
				prv = cyc[int(cyc.size())/2];
			}
			int M = int(edges.size());
			fout << N << ' ' << M << '\n';
			for (int i = 0; i < N; i++) fout << labels[i] << " \n"[i+1==N];
			for (auto [x, y] : edges) fout << x+1 << ' ' << y+1 << '\n';
		}
	}

	{
		std::mt19937 mt(48);
		std::ofstream fout("t-rnd.in");
		int T = 5;
		fout << T << '\n';
		int cyc_len = 2;
		for (int t = 0; t < T; t++, cyc_len++) {
			int N = 100;
			std::vector<int> labels(N);
			for (auto& l : labels) l = std::uniform_int_distribution(0, N-1)(mt);
			std::vector<std::array<int, 2>> edges; edges.reserve(2*N);
			int prv = 0;
			for (int i = 1; i < N; i += (cyc_len-1)) {
				int j = std::min(i + (cyc_len - 1), N);
				std::vector<int> cyc;
				cyc.push_back(prv);
				for (int z = i; z < j; z++) cyc.push_back(z);
				assert(int(cyc.size()) >= 2);
				for (int z = 0; z+1 < int(cyc.size()); z++) {
					edges.push_back({cyc[z], cyc[z+1]});
				}
				if (cyc.size() > 2) {
					edges.push_back({cyc.front(), cyc.back()});
				}
				prv = cyc[int(cyc.size())/2];
			}
			int M = int(edges.size());
			fout << N << ' ' << M << '\n';
			for (int i = 0; i < N; i++) fout << labels[i] << " \n"[i+1==N];
			for (auto [x, y] : edges) fout << x+1 << ' ' << y+1 << '\n';
		}
	}
}
