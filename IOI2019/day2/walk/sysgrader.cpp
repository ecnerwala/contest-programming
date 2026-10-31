#include "walk.h"
#include <cstdio>
#include <cassert>
// BEGIN SECRET
#include <string>
// END SECRET

using namespace std;

int main() {
	// BEGIN SECRET
	const string input_secret = "447adec2-e0e7-4579-a2da-fed58fa3393e";
	const string output_secret = "d99f6469-b674-4a96-9c39-7bc072e22a4f";
	char secret[1000];
	assert(1 == scanf("%s", secret));
	if (string(secret) != input_secret) {
		printf("%s\n", output_secret.c_str());
		printf("SV\n");
		fclose(stdout);
		return 0;
	}
	// END SECRET
	int n, m;
	assert(2 == scanf("%d%d", &n, &m));
	vector<int> x(n), h(n);
	for (int i = 0; i < n; i++)
		assert(2 == scanf("%d%d", &x[i], &h[i]));
	vector<int> l(m), r(m), y(m);
	for (int i = 0; i < m; i++)
		assert(3 == scanf("%d%d%d", &l[i], &r[i], &y[i]));
	int s, g;
	assert(2 == scanf("%d%d", &s, &g));
	fclose(stdin);

	long long result = min_distance(x, h, l, r, y, s, g);

	// BEGIN SECRET
	printf("%s\n", output_secret.c_str());
	printf("OK\n");
	// END SECRET
	printf("%lld\n", result);
	fclose(stdout);
	return 0;
}
