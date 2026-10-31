#include "shoes.h"
#include <cstdio>
#include <cassert>

using namespace std;

int main() {
	int n;
	assert(1 == scanf("%d", &n));
	vector<int> S(2 * n);
	for (int i = 0; i < 2 * n; i++)
		assert(1 == scanf("%d", &S[i]));
	fclose(stdin);

	long long result = count_swaps(S);

	printf("%lld\n", result);
	fclose(stdout);
	return 0;
}
