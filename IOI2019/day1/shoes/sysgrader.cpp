#include "shoes.h"
#include <cstdio>
#include <cassert>
// BEGIN SECRET
#include <string>
// END SECRET

using namespace std;

int main() {
	// BEGIN SECRET
	const string input_secret = "08f55b3f-c300-4051-a472-59ca2a776178";
	const string output_secret = "9ce55564-5404-428a-8d2e-0d809c85101e";
	char secret[1000];
	assert(1 == scanf("%s", secret));
	if (string(secret) != input_secret) {
		printf("%s\n", output_secret.c_str());
		printf("SV\n");
		fclose(stdout);
		return 0;
	}
	// END SECRET
	int n;
	assert(1 == scanf("%d", &n));
	vector<int> S(2 * n);
	for (int i = 0; i < 2 * n; i++)
		assert(1 == scanf("%d", &S[i]));
	fclose(stdin);

	long long result = count_swaps(S);

	// BEGIN SECRET
	printf("%s\n", output_secret.c_str());
	printf("OK\n");
	// END SECRET
	printf("%lld\n", result);
	fclose(stdout);
	return 0;
}
