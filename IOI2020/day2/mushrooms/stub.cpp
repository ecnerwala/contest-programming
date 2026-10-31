#include <cstdio>
#include <cstdlib>
#include <cstdarg>
#include <string>
#include "mushrooms.h"

static char fmt_buffer[100000];
#define FMT_TO_STR(fmt, result) va_list vargs; va_start(vargs, fmt); \
	vsnprintf(fmt_buffer, sizeof(fmt_buffer), fmt, vargs); \
	va_end(vargs); fmt_buffer[sizeof(fmt_buffer)-1] = 0; \
	std::string result(fmt_buffer);

static const int min_n = 2;
static const int max_n = 20000;
static const int max_qc = 20000;
static const int max_qs = 100000;
static const int species_A = 0;
static const int species_B = 1;

static int n;
static std::vector<int> species;
static int qc, qs;

static inline void error_if(bool cond, std::string message) {
	if (cond) {
		printf("%s\n", message.c_str());
		exit(0);
	}
}

static inline void wrong_if(bool cond, std::string message) {
	error_if(cond, "Wrong Answer: "+message);
}

int use_machine(std::vector<int> x) {
	const int xs = x.size();
	wrong_if(xs < 2, "Too small array for query.");
	wrong_if(xs > n, "Too large array for query.");
	qc++;
	wrong_if(qc > max_qc, "Too many queries.");
	qs += xs;
	wrong_if(qs > max_qs, "Too many total array sizes as queries.");
	for (int i = 0; i < xs; i++)
		wrong_if(x[i] < 0 || n - 1 < x[i], "Invalid value in the query array.");
	std::vector<bool> used(n, false);
	for (int i = 0; i < xs; i++) {
		wrong_if(used[x[i]], "Duplicate value in the query array.");
		used[x[i]] = true;
	}
	int diffs = 0;
	for (int i = 1; i < xs; i++)
		diffs += int(species[x[i]] != species[x[i-1]]);
	return diffs;
}

#ifdef __GNUC__
__attribute__ ((format(printf, 2, 3)))
#endif
static inline void check_input(bool cond, const char* message_fmt, ...) {
	FMT_TO_STR(message_fmt, message);
	error_if(!cond, "Invalid input: "+message);
}

int main() {
	check_input(1 == scanf("%d", &n), "Could not read n.");
	check_input(min_n <= n, "n must not be less than %d, but it is %d.", min_n, n);
	check_input(n <= max_n, "n must not be greater than %d, but it is %d.", max_n, n);
	species.resize(n);
	for (int i = 0; i < n; i++) {
		check_input(1 == scanf("%d", &species[i]), "Could not read species element [%d].", i);
		check_input(species[i]==species_A || species[i] == species_B,
					"Species elements must be %d or %d, but species element [%d] is %d.", species_A, species_B, i, species[i]);
	}
	check_input(species[0] == species_A, "Species element [%d] must be %d.", 0, species_A);
	// Postponed closing standard input in order to allow interactive usage of the grader.

	qc = 0;
	qs = 0;
	int answer = count_mushrooms(n);
	printf("%d\n%d\n", answer, qc);
	fclose(stdout);
	fclose(stdin);
	return 0;
}
