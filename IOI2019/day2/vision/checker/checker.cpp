#include "testlib.h"
#include <vector>
#include <algorithm>
using namespace std;

#define fori(i, n) for (int i=0; i<n; i++)
#define forv(i, v) fori(i, sz(v))
#define allOf(c) ((c).begin()), ((c).end())

typedef pair<int,int> PII;
typedef vector<int> VI;

template<class T>
inline int sz(const T& c) {
	return c.size();
}

template<class T>
inline void sort_unify(vector<T>& v) {
	sort(allOf(v));
	v.resize(unique(allOf(v))-v.begin());
}


const string output_secret = "b17553fd-ba5a-4140-836c-491f938c515b";

const int MAX_GATES = 10000;
const int MAX_INPUTS = 1000000;

#define AND 0
#define OR 1
#define XOR 2
#define NOT 3

/**
 * Similar to sort_unify, but keeps the parity of the elements.
 * Examples:
 *   [] -> []
 *   [1] -> [1]
 *   [1,1] -> []
 *   [1,1,1] -> [1]
 *   [1,2,1] -> [2]
 */
template<class T>
inline void sort_xorify(vector<T>& v) {
	sort(allOf(v));
	int j = 0;
	forv(i, v)
		if (j > 0 && v[j-1] == v[i])
			j--;
		else
			v[j++] = v[i];
	v.resize(j);
}


class Circuit {
	struct Gate {
		int type;
		VI inputs;
		int output;
		
		inline void read(int maxInputIndex, int maxNumInputs) {
			type = ouf.readInt(0, NOT, "gate type");
			ouf.readSpace();
			if (type == NOT) {
				inputs.push_back(ouf.readInt(0, maxInputIndex, "NOT-gate input"));
			} else {
				int num_inputs = ouf.readInt(1, maxNumInputs, "number of gate inputs");
				fori(j, num_inputs) {
					ouf.readSpace();
					inputs.push_back(ouf.readInt(0, maxInputIndex, "gate input"));
				}
			}
			ouf.readEoln();
		}
		
		inline int evaluate(const VI& states) const {
		    switch (type) {
		    	case NOT:
		    		return 1 - states[inputs[0]];
		        case AND:
					for (auto index : inputs)
						if (!states[index])
							return 0;
		            return 1;
		        case OR:
					for (auto index : inputs)
						if (states[index])
							return 1;
		            return 0;
		        case XOR: {
		        	int r = 0;
					for (auto index : inputs)
						r ^= states[index];
					return r;
		        }
		        default:
		            throw "Unknown gate type";
		    }
		}

		template<class T>
		inline T evaluateBitwise(const vector<T>& states) const {
		    switch (type) {
		    	case NOT:
		    		return ~states[inputs[0]];
		        case AND: {
		    		T r = 0;
		    		r = ~r;
					for (auto index : inputs)
						r &= states[index];
		            return r;
		        }
		        case OR: {
		    		T r = 0;
					for (auto index : inputs)
						r |= states[index];
		            return r;
		        }
		        case XOR: {
		        	T r = 0;
					for (auto index : inputs)
						r ^= states[index];
					return r;
		        }
		        default:
		            throw "Unknown gate type";
		    }
		}
	};
	
	int num_inputs;
	vector<Gate> gates;
	int statesSize;
	int output;
	
	inline void removeRedundantGateInputsAndBypassRedundantSingleInputGates() {
		vector<bool> bypassable(100, false);
		bypassable[AND] = bypassable[OR] = bypassable[XOR] = true;
		VI repr(statesSize);
		forv(i, repr)
			repr[i] = i;
		for (auto& g : gates) {
			forv(i, g.inputs)
				g.inputs[i] = repr[g.inputs[i]];
			switch (g.type) {
			case AND:
			case OR:
				sort_unify(g.inputs);
				break;
			case XOR:
				sort_xorify(g.inputs);
				break;
			}
			if (bypassable[g.type] && sz(g.inputs)==1)
				repr[g.output] = g.inputs[0];
		}
		output = repr[output];
	}

	inline void removeUselessGates() {
		vector<bool> used(statesSize, false);
		used[output] = true;
		for (int i = sz(gates); --i >= 0;)
			if (used[gates[i].output])
				for (int j : gates[i].inputs)
					used[j] = true;
		int j = 0;
		forv(i, gates)
			if (used[gates[i].output])
				gates[j++] = gates[i];
		gates.resize(j);
	}

	inline void postConstruct() {
		removeRedundantGateInputsAndBypassRedundantSingleInputGates();
		removeUselessGates();
	}

public:
	
	Circuit(int num_inputs): num_inputs(num_inputs), statesSize(-1), output(-1) {
	}
	
	inline void read() {
		int num_gates = ouf.readInt(1, MAX_GATES, "num_gates");
		ouf.readEoln();
		gates.resize(num_gates);
		int cur = num_inputs;
		int inputsLeft = MAX_INPUTS;
        fori(i, num_gates) {
        	Gate& g = gates[i];
			g.read(cur-1, inputsLeft);
			g.output = cur++;
            inputsLeft -= sz(g.inputs);
		}
        statesSize = cur;
        output = cur-1;
        postConstruct();
	}
	

	class Evaluator {
		const Circuit& circuit;
		VI states;

	public:
		inline Evaluator(const Circuit& c, int initialInputValues): circuit(c) {
			states.resize(circuit.statesSize, initialInputValues);
		}
		inline void setInput(int index, int value) {
			if (index<0 || index>=circuit.num_inputs)
				throw "Invalid index";
			states[index] = value;
		}
		inline int evaluate() {
		    for (auto &gate : circuit.gates)
		        states[gate.output] = gate.evaluate(states);
		    return states[circuit.output];
		}
	};

	inline Evaluator* newEvaluator(int initialInputValues=0) const {
		return new Evaluator(*this, initialInputValues);
	}


	class BatchEvaluator {
		typedef unsigned long long ULL;
		const Circuit& circuit;
		vector<ULL> states;

		inline void validateEvaluationIndex(int evaluationIndex) const {
			if (evaluationIndex < 0 || evaluationIndex >= MAX_SIZE)
				throw "Invalid evaluation index";
		}

	public:
		static const int MAX_SIZE = 64;

		inline BatchEvaluator(const Circuit& c): circuit(c) {
			states.resize(circuit.statesSize);
		}

		inline void zeroInit() {
			fill(allOf(states), 0);
		}

		inline void setInput(int evaluationIndex, int index, int value) {
			validateEvaluationIndex(evaluationIndex);
			if (index<0 || index>=circuit.num_inputs)
				throw "Invalid index";
			ULL b = 1;
			b <<= evaluationIndex;
			if (value==0)
				states[index] &= ~b;
			else
				states[index] |= b;
		}
		inline void evaluate() {
		    for (auto &gate : circuit.gates)
		        states[gate.output] = gate.evaluateBitwise(states);
		}

		inline int getOutput(int evaluationIndex) const {
			validateEvaluationIndex(evaluationIndex);
		    return (int)((states[circuit.output] >> evaluationIndex) & 1);
		}
	};

	inline BatchEvaluator* newBatchEvaluator() const {
		return new BatchEvaluator(*this);
	}
};




int H, W, K, HW;

struct CircuitEvaluationInstance {
	int cellA, cellB, rowA, colA, rowB, colB, expected, computed;
	inline CircuitEvaluationInstance(int cellA, int cellB): cellA(cellA), cellB(cellB) {
	    rowA = cellA / W;
	    colA = cellA % W;
	    rowB = cellB / W;
	    colB = cellB % W;
		expected = abs(rowA - rowB) + abs(colA - colB) == K ? 1 : 0;
		computed=-1;
	}
};

int main(int argc, char * argv[]) {
    registerChecker("vision", argc, argv);

    vector<PII> queries;
    {//Reading input
		inf.readLine();
		H = inf.readInt();
		W = inf.readInt();
		K = inf.readInt();
		HW = H*W;
		int T = inf.readInt();
		if (T == -1) {//query all pairs
			fori(i, HW)
				fori(j, i)
					queries.emplace_back(j, i);
		} else {//queries are in input
			int mask = inf.readInt();
			fori(i, T) {
				int cellA = inf.readInt() ^ mask;
				quitif(cellA<0 || cellA>=HW, _fail, "invalid cell reference %d in query pair %d", cellA, i+1);
				int cellB = inf.readInt() ^ mask;
				quitif(cellB<0 || cellB>=HW, _fail, "invalid cell reference %d in query pair %d", cellB, i+1);
				quitif(cellA == cellB, _fail, "Bad query: identical cells");
				if (cellA > cellB)
					swap(cellA, cellB);
				queries.emplace_back(cellA, cellB);
			}
		}
	}

    //Reading solution output file
    ouf.readSecret(output_secret);
    ouf.readGraderResult();
    Circuit circuit(HW);
    circuit.read();
    ouf.readEof();
	
    //Creating batches of queries
	vector<vector<PII>> queryBatches;
	forv(i, queries) {
		if (i % Circuit::BatchEvaluator::MAX_SIZE == 0)
			queryBatches.push_back({});
		queryBatches.back().push_back(queries[i]);
	}

    //Testing the circuit with query batches
	auto batchEvaluator = circuit.newBatchEvaluator();
	for (auto& queryBatch : queryBatches) {
		batchEvaluator->zeroInit();
		forv(i, queryBatch) {
			batchEvaluator->setInput(i, queryBatch[i].first, 1);
			batchEvaluator->setInput(i, queryBatch[i].second, 1);
		}
		batchEvaluator->evaluate();
		forv(i, queryBatch) {
			CircuitEvaluationInstance oci(queryBatch[i].first, queryBatch[i].second);
			oci.computed = batchEvaluator->getOutput(i);
			quitif(oci.computed != oci.expected, _wa, "on inputs (%d, %d), (%d, %d), expected %d, but computed %d", oci.rowA, oci.colA, oci.rowB, oci.colB, oci.expected, oci.computed);
		}
	}
    delete batchEvaluator;
    quit(_ok);
}
