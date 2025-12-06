#include <bits/stdc++.h>

int main() {
	using namespace std;
	std::ofstream fout("t-big-100.in");
	int N = 100;
	fout << N << '\n';
	std::vector<std::string> G(4*N+1, std::string(4*N+1, '.'));
	for (int i = 0; i <= N; i++) {
		for (int j = 0; j <= N; j++) {
			G[4*i][4*j] = 'A';
		}
	}
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			G[4*i+2][4*j+2] = '4';
		}
	}
	for (auto s : G) fout << s << '\n';
}
