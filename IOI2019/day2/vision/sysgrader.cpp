#include "vision.h"
#include <cstdio>
#include <cassert>
using namespace std;

static const int MAX_GATES = 10000;
static const int MAX_INPUTS = 1000000;

static const int _AND = 0;
static const int _OR = 1;
static const int _XOR = 2;
static const int _NOT = 3;

static inline bool increasing(int a, int b, int c) {
	return a <= b && b <= c;
}

[[noreturn]] static inline void WA(string reason);

class Network {

	struct Gate {
		int type;
		vector<int> input_indexes;

		inline Gate(int _type, const vector<int>& _input_indexes):
				type(_type), input_indexes(_input_indexes) {
		}
	};

	int input_size;
	int total_inputs;
	vector<Gate> gates;

public:

	inline void init(int _input_size) {
		this->input_size = _input_size;
		this->total_inputs = 0;
		this->gates.clear();
	}

	inline int add_gate(int type, const vector<int>& input_indexes) {
		if (input_indexes.size() == 0)
			WA("Instruction with no inputs");

		if (gates.size() + 1 > MAX_GATES)
			WA("Too many instructions");

		if (total_inputs + input_indexes.size() > MAX_INPUTS)
			WA("Too many inputs");

		gates.emplace_back(type, input_indexes);
		total_inputs += input_indexes.size();
		int new_index = input_size + (int)gates.size() - 1;

		for (int input_index : input_indexes)
			if (!increasing(0, input_index, new_index-1))
				WA("Invalid index");

		return new_index;
	}

	inline void print() const {
		printf("%d\n", int(gates.size()));
		for (auto& gate : gates) {
			printf("%d ", gate.type);
			if (gate.type == _NOT) {
				printf("%d", gate.input_indexes[0]);
			} else {
				printf("%d", int(gate.input_indexes.size()));
				for (int input : gate.input_indexes)
					printf(" %d", input);
			}
			printf("\n");
		}
	}
};


static Network network;

[[noreturn]] static inline void general_output(string verdict, string reason = "") {
	const string output_secret = "b17553fd-ba5a-4140-836c-491f938c515b";
	printf("%s\n", output_secret.c_str());
	printf("%s\n", verdict.c_str());
	if ("OK" != verdict) {
		if (reason != "")
			printf("%s\n", reason.c_str());
	} else {
		network.print();
	}
	fclose(stdout);
	exit(0);
}

[[noreturn]] static inline void WA(string reason) {
	general_output("WA", reason);
}

int main() {
	int H, W, K;
	{//reading input
		const string input_secret = "c2675211-ade0-44b0-8c15-741dd835f3d2";
		char secret[1000];
		assert(1 == scanf("%999s", secret));
		if (string(secret) != input_secret)
			general_output("SV");
		assert(3 == scanf("%d%d%d", &H, &W, &K));
		fclose(stdin);
	}

	network.init(H * W);
	construct_network(H, W, K);

	general_output("OK");
}

int add_and(vector<int> Ns) {
	return network.add_gate(_AND, Ns);
}

int add_or(vector<int> Ns) {
	return network.add_gate(_OR, Ns);
}

int add_xor(vector<int> Ns) {
	return network.add_gate(_XOR, Ns);
}

int add_not(int N) {
	vector<int> Ns = {N};
	return network.add_gate(_NOT, Ns);
}
