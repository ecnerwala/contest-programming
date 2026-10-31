#include "job.h"
#include <cstdio>
#include <cassert>
// BEGIN SECRET
#include <string>
// END SECRET

using namespace std;

int main() {
	// BEGIN SECRET
	const string input_secret = "7482365a-669a-4a64-887e-b4b620a595e9";
	const string output_secret = "4915f9c2-60f4-4426-9336-0bd7a6405de6";
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
	vector<int> p(n);
	for (int i = 0; i < n; i++)
		assert(1 == scanf("%d", &p[i]));
	vector<int> u(n);
	for (int i = 0; i < n; i++)
		assert(1 == scanf("%d", &u[i]));
	vector<int> d(n);
	for (int i = 0; i < n; i++)
		assert(1 == scanf("%d", &d[i]));
	fclose(stdin);

	long long result = scheduling_cost(p, u, d);

	// BEGIN SECRET
	printf("%s\n", output_secret.c_str());
	printf("OK\n");
	// END SECRET
	printf("%lld\n", result);
	fclose(stdout);
	return 0;
}
