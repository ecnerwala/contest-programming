# This is a sample checker in python for Codeforces Task Cycle Closing
# Feel free to modify it as desired. It is guaranteed to work on at least Python 3.12 and above.

# You will first need to run your solution and get the output and store it to a file.

# To run this program, simply use the command 
# python3 checker.py <input file> <output file> 
# where you replace <input file>, <output file> with the name of your files
# For example, python3 checker.py in out 
# you may have to replace python3 with a suitable replacement, in case you don't have it installed.

# The checker is ONLY GUARANTEED to work if you adhere to the input restrictions and the output formats.
# Additionally, the checker prints the exact input test where you get a wrong answer for your convenience.

import sys

def main():
    if len(sys.argv) < 3:
        print(f"Invalid Template Arguments: {sys.argv[0]} <input.txt> <output.txt>")
        return 0

    # Open input file
    try:
        fin_data = open(sys.argv[1], "r").read()
    except Exception:
        print("Cannot open input file")
        return 0

    # Open output file (to read contestant's output)
    try:
        fout_data = open(sys.argv[2], "r").read()
    except Exception:
        print("Cannot open output file")
        return 0

    # Tokenize input and output streams
    fin_tokens = fin_data.split()
    fout_tokens = fout_data.split()
    fin_idx = 0
    fout_idx = 0

    def fin_next_int():
        nonlocal fin_idx
        if fin_idx >= len(fin_tokens):
            raise ValueError("Input ended unexpectedly")
        try:
            x = int(fin_tokens[fin_idx])
        except ValueError:
            raise ValueError("Input token is not an integer")
        fin_idx += 1
        return x

    def print_input(n, m, e):
        print("The input for the test case is as follows:")
        print(f"{n} {m}")
        for u, v in e:
            print(f"{u} {v}")

    # readInt from contestant output with range and variable name
    def readInt(l, r, s, test_id, n=None, m=None, e=None):
        nonlocal fout_idx
        if fout_idx < len(fout_tokens):
            tok = fout_tokens[fout_idx]
            # Try to parse as int
            try:
                x = int(tok)
            except ValueError:
                print(f"Wrong answer on test case {test_id}")
                print(f"Wrong output format, expected integer input for variable {s}, but not found ")
                print()
                # Provide the input that led to this
                if n is not None:
                    print_input(n, m, e)
                sys.exit(0)
            fout_idx += 1
            if l <= x <= r:
                return x
            else:
                print(f"Wrong answer on test case {test_id}")
                # Keep the literal "[l, r]" text as in the C++ program
                print(f"Out of range, Expected integer input for variable {s} in range [{l}, {r}], found {x}")
                print()
                if n is not None:
                    print_input(n, m, e)
                sys.exit(0)
        else:
            print(f"Wrong answer on test case {test_id}")
            print(f"Wrong format, expected integer input for variable {s}, but not found ")
            print()
            if n is not None:
                print_input(n, m, e)
            sys.exit(0)

    # Begin processing
    t = fin_next_int();

    for test_id in range(1, t + 1):
        n = fin_next_int()
        m = fin_next_int()
        e = []

        # adjacency matrix (1-indexed)
        adj = [[0] * (n + 1) for _ in range(n + 1)]

        for _ in range(m):
            u = fin_next_int()
            v = fin_next_int()
            adj[u][v] = 1
            adj[v][u] = 1
            e.append((u, v))

        op = readInt(0, 2, "op", test_id, n, m, e)

        for i in range(1, op + 1):
            k = readInt(1, n, "k", test_id, n, m, e)
            s = readInt(0, n * (n - 1) // 2, "s", test_id, n, m, e)

            add = []

            for j in range(1, s + 1):
                path = []
                for _ in range(k):
                    vtx = readInt(1, n, "v_i", test_id, n, m, e)
                    path.append(vtx)

                seen = [False] * (n + 1)
                for x in path:
                    if seen[x]:
                        print(f"Wrong answer on test case {test_id}")
                        print(f"Operation {i}, path {j}, vertex {x} repeats")
                        print()
                        print_input(n, m, e)
                        return 0
                    seen[x] = True

                # Check edges along the path exist
                for lidx in range(1, k):
                    u = path[lidx - 1]
                    v = path[lidx]
                    if adj[u][v] == 0:
                        print(f"Wrong answer on test case {test_id}")
                        print(f"Operation {i}, path {j}, edge ({u}, {v}) does not exist in the graph")
                        print()
                        print_input(n, m, e)
                        return 0

                u = path[0]
                v = path[-1]
                if u == v:
                    print(f"Wrong answer on test case {test_id}")
                    print(f"Operation {i}, path {j}, tries to add a self loop on vertex {u}")
                    print()
                    print_input(n, m, e)
                    return 0

                add.append((u, v))

            # Apply additions, checking for multi-edges
            for u, v in add:
                if adj[u][v] == 1:
                    print(f"Wrong answer on test case {test_id}")
                    print(f"Operation {i} tries to add multiple edge ({u} {v})")
                    print()
                    print_input(n, m, e)
                    return 0
                adj[u][v] = 1
                adj[v][u] = 1

        # Final completeness check
        for u in range(1, n + 1):
            for v in range(u + 1, n + 1):
                if adj[u][v] == 0:
                    print(f"Wrong answer on test case {test_id}")
                    print(f"Graph not complete, missing edge ({u}, {v})")
                    print()
                    print_input(n, m, e)
                    return 0

    extra_int_present = False
    for idx in range(fout_idx, len(fout_tokens)):
        try:
            int(fout_tokens[idx])
            extra_int_present = True
            break
        except ValueError:
            continue

    if extra_int_present:
        print("Wrong Answer")
        print("Extra information present in output file")
        return 0

    print("AC! You are the best!")
    return 0


if __name__ == "__main__":
    sys.exit(main() or 0)
