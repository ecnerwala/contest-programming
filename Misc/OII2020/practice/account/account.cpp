#include <string>

int chiedi(int K);
void rispondi(std::string S);

void indovina(int N) {
	int K = N * (N+1) / 2;
	std::string res(N, 'a');
	for (char c = char('a' + N-1); c > 'a'; c--) {
		int L = chiedi(K);
		K -= L;
		res[N-L] = c;
	}
	rispondi(res);
}
