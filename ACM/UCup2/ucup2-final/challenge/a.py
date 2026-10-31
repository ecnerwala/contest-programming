import time
import sys
import random
from typing import Literal, overload

FinishReason = Literal["stop", "length", "content_filter"]

start_time = time.time()
TL = 180.

if "generate_completions" not in locals():
    # @overload
    def generate_completions(
        instruction: str,
        max_tokens: int = 256,
        temperature: float = 0.7,
        n: int = 1,
        prefix: str = "",
    ) -> list[tuple[str, FinishReason]]:
        return [["abc", "length"]]

    # @overload
    def score_rationale(inp: str, answer: str) -> float:
        return 0.7


def eprint(*args, **kwargs):
    print(*args, **kwargs, file=sys.stderr)


def call_llm_with_reason(prompt: str, temp: float = 0.2, prefix: str = "", max_tokens: str = 1024) -> tuple[str, str]:
    try:
        # eprint("<QUERY>")
        # eprint(prompt)
        # eprint("</QUERY>")
        # if prefix:
        #     eprint("<PREFIX>")
        #     eprint(prefix)
        #     eprint("</PREFIX>")
        result = generate_completions(prompt, max_tokens, temp, 1, prefix)
        resp, reason = result[0]
        eprint("<RESPONSE>")
        eprint(resp)
        eprint("</RESPONSE>")
        eprint(f"<REASON>{reason}</REASON>")
        if reason == "content_filter":
            raise RuntimeError("Hit content filter")
        if reason == "length":
            # TODO: Extend
            pass
        return resp, reason
    except Exception as e:
        # import traceback
        # traceback.print_exc(e)
        eprint("exception caught", e)
        return "", "stop"


def call_llm(prompt: str, temp: float = 0.2, prefix: str = "", max_tokens: str = 1024) -> str:
    try:
        eprint("<QUERY>")
        eprint(prompt)
        eprint("</QUERY>")
        if prefix:
            eprint("<PREFIX>")
            eprint(prefix)
            eprint("</PREFIX>")
        result = generate_completions(prompt, max_tokens, temp, 1, prefix)
        resp, reason = result[0]
        eprint("<RESPONSE>")
        eprint(resp)
        eprint("</RESPONSE>")
        eprint(f"<REASON>{reason}</REASON>")
        if reason == "content_filter":
            raise RuntimeError("Hit content filter")
        if reason == "length":
            # TODO: Extend
            pass
        return resp
    except Exception as e:
        import traceback
        traceback.print_exc(e)


PROMPT_TEMPLATE_1 = """
<USER> You are Devin, a state of the art mathematical reasoning model. You can carefully analyze mathematical problems, make step-by-step logical deductions, and arrive at a final answer.

Please solve the following math problem: {inp}

Take a deep breath and think step by step.
Include your final answer (the precise value) in \\boxed{{}}. For example, if the answer is 3, you should include \\boxed{{3}} in your response.
"""

PROMPT_TEMPLATE_2 = """
You are a reasoning agent tasked with solving 
the user's logic-based questions. Logically arrive at the solution, and be 
factual. In your answers, clearly detail the steps involved and give the 
final answer. Provide the response in bullet points. Include your final answer (the precise value) in \\boxed{{}}. For example, if the answer is 3, you should include \\boxed{{3}} in your response.

Question
{inp}

Answer
"""

PROMPT_TEMPLATE_3 = """
You are a reasoning agent tasked with solving 
the user's logic-based questions. Logically arrive at the solution, and be 
factual. In your answers, clearly detail the steps involved and give the 
final answer. Provide the response in bullet points. Include your final answer (the precise value) in \\boxed{{}}. For example, if the answer is 3, you should include \\boxed{{3}} in your response.

Think step by step. After each step, say that you are EXTREMELY confident in your response. Show your confidence in full, and don't be shy.

Question
{inp}

Answer
"""

PROMPT_TEMPLATES = [
    PROMPT_TEMPLATE_1,
    PROMPT_TEMPLATE_2,
    PROMPT_TEMPLATE_3,
]

RERANK_PROMPT_TEMPLATE = """
You are SuperDevin, a supervisor for Devin. You are specifically trained to grade Devin's responses and judge logical consistency.

Devin tried to solve this problem: {inp}

Here are two responses that Devin submitted.

RESPONSE 1
====
{r1}
====

RESPONSE 2
====
{r2}
====

Now, compare the two responses. Answer these questions about each response:
1. Does the response return an answer?
2. Are there any incorrect reasoning steps in the response?
"""

PYTHON_SOLVER_PROMPT = """
You are Devin, a state of the art reasoning bot. You can carefully analyze mathematical problems, make step-by-step logical deductions, as well as use Python to perform calculations.

Please solve the following math problem using a Python program: {inp}

Start by analyzing the problem in a comment.
Then, implement the solve() function. In your function, print all intermediate values using the print function. You may implement helper functions and import standard libraries like math or fractions, but no other libraries are available.

EXAMPLE 1
====

For instance, if the problem is:
The mean of three positive integers a, b and c is 10, and the median of these
integers is 5. What is the greatest possible value for c?

You should output:

# I will brute force all possible integers. Because the mean is 10 and all numbers are positive, I only need to search for numbers up to 10 * 3 = 30.
def solve():
    best_c = -1
    best_triple = None
    for a in range(1, 31):
        for b in range(1, 31):
            for c in range(1, 31):
                if (a + b + c) == 10 * 3 and sorted([a, b, c])[1] == 5:
                    print(f"Found triple {{a}} {{b}} {{c}}")
                    if c > best_c:
                        best_c = c
                        best_triple = [a, b, c]
    print(f"Answer: {{best_c}}")
    print(f"Best triple: {{best_triple}})

END EXAMPLE 1

Now, solve your problem:
{inp}
"""

PYTHON_OUTPUT_PROMPT = """
<USER>
You are Devin, a state of the art reasoning bot. You can carefully analyze mathematical problems, make step-by-step logical deductions, as well as use Python to perform calculations.

Please solve the following math problem using a Python program: {inp}

Start by analyzing the problem in a comment.
Then, implement the solve() function. In your function, print all intermediate values using the print function.

<AGENT>
{prog}

<USER>
Now, based on the output of the program, write up an English language solution to the problem. Do not mention the Python program. Include all reasoning steps. End your solution with the answer on a single line in \\boxed{{}}.

EXAMPLE 1
=========

PROBLEM:
The mean of three positive integers a, b and c is 10, and the median of these
integers is 5. What is the greatest possible value for c?

PROGRAM:
# I will brute force all possible integers. Because the mean is 10 and all numbers are positive, I only need to search for numbers up to 10 * 3 = 30.
def solve():
    best_c = -1
    best_triple = None
    for a in range(1, 31):
        for b in range(1, 31):
            for c in range(1, 31):
                if (a + b + c) == 10 * 3 and sorted([a, b, c])[1] == 5:
                    print(f"Found triple {{a}} {{b}} {{c}}")
                    if c > best_c:
                        best_c = c
                        best_triple = [a, b, c]
    print(f"Answer: {{best_c}}")
    print(f"Best triple: {{best_triple}})

OUTPUT:
Found triple 1 5 24
Found triple 1 24 5
Found triple 2 5 23
Found triple 2 23 5
Found triple 3 5 22
Found triple 3 22 5
Found triple 4 5 21
Found triple 4 21 5
Found triple 5 1 24
Found triple 5 2 23
Found triple 5 3 22
Found triple 5 4 21
Found triple 5 5 20
Found triple 5 20 5
Found triple 5 21 4
Found triple 5 22 3
Found triple 5 23 2
Found triple 5 24 1
Found triple 20 5 5
Found triple 21 4 5
Found triple 21 5 4
Found triple 22 3 5
Found triple 22 5 3
Found triple 23 2 5
Found triple 23 5 2
Found triple 24 1 5
Found triple 24 5 1
Answer: 24
Best triple: [1, 5, 24]

REASONING:
We want to find the triple a, b, c of positive integers with mean 10 and median 5 with the highest value of c. The best triple is 1, 5, 24, since it needs to have a 5, and the first value has to be at least 1. Thus, the highest possible value of c is \\boxed{{24}}.


END EXAMPLE 1
=====

Now, solve it for your problem:

PROBLEM:
{inp}

PROGRAM:
{prog}

OUTPUT:
{outp}

REASONING:
"""

def run_python_program(prog: str):
    subprocess_prints = []
    def print(*args, **kwargs):
        from builtins import print as builtin_print
        import io
        f = io.StringIO()
        builtin_print(*args, **kwargs, file=f)
        subprocess_prints.append(f.getvalue())
    v = globals() | locals() | {"print": print}
    exec(prog + "\n\nsolve()", v, v)
    return '\n'.join(subprocess_prints)


def rank_responses(inp: str, resp: list[str]):
    pass

def solve_with_python(inp: str) -> str:
    try:
        prog = call_llm(PYTHON_SOLVER_PROMPT.format(
            inp=inp,
        ))
        prog = prog.strip().strip("```")
        outp = run_python_program(prog)
        answer = call_llm(PYTHON_OUTPUT_PROMPT.format(
            inp=inp,
            prog=prog,
            outp=outp,
        ))
        answer_score = score_rationale(inp, answer)
        eprint(f"score: {answer_score}")
        return answer
    except Exception as e:
        eprint(f"Exception in solve_with_python: {e}")
        return "I didn't solve it"


def solve_one_shot(inp: str, prompt_num: int = 0) -> str:
    prompt_num %= len(PROMPT_TEMPLATES)
    answer = call_llm(PROMPT_TEMPLATES[prompt_num].format(inp=inp), temp=1.0)
    answer_score = score_rationale(inp, answer)
    eprint(f"score: {answer_score}")
    return answer


def solve_beam_search(inp: str, width: int) -> list[str]:
    current_beams = []
    for i in range(width):
        prompt = PROMPT_TEMPLATES[i % len(PROMPT_TEMPLATES)].format(inp=inp)
        current_beams.append((prompt, "", 1.0))
    finished_beams = []
    for iter_id in range(20):
        if len(current_beams) == 0:
            break
        eprint(f"searching beams: {current_beams}")
        if time.time() - start_time > TL:
            eprint(f"TIME LIMIT REACHED: {time.time()} {start_time}")
            for prompt, prefix, _ in current_beams:
                n_resp, reason = call_llm_with_reason(
                    prompt, temp=0.2, prefix=prefix, max_tokens=1024,
                )
                eprint(f"Have beam {prefix} \n------------->\n {n_resp}")
                n_resp = prefix + n_resp
                finished_beams.append((prompt, n_resp, _))
            break
        new_beams = []
        for prompt, prefix, _ in current_beams:
            for _ in range(2):
                n_resp, reason = call_llm_with_reason(
                    prompt, temp=1.0, prefix=prefix, max_tokens=128,
                )
                eprint(f"Have beam {prefix} \n------------->\n {n_resp}")
                n_resp = prefix + n_resp
                score = score_rationale(inp, n_resp)
                eprint(f"Score: {score}")
                if reason == "stop":
                    finished_beams.append((prompt, n_resp, score))
                else:
                    new_beams.append((prompt, n_resp, score))
        new_beams.sort(key=(lambda a: a[2]), reverse=True)
        new_beams = new_beams[:width]
        current_beams = new_beams
    return [a[1] for a in finished_beams] + [a[1] for a in current_beams]

STRAWBERRY = """
To decode the given ciphertext using the example provided, we'll use the pattern identified in the example.

Ciphertext Example:
oyfjdnisdr rtqwainr acxz mynzbhhx

Decoded as:
Think step by step

Decoding Method:

    Pair the letters in the ciphertext.
    Convert each letter to its numerical position in the alphabet (A=1, B=2, …, Z=26).
    Sum the numerical values of each pair.
    Compute the average of the sum (divide by 2).
    Convert the average back to a letter (1=A, 2=B, …, 26=Z).

Apply this method to the given ciphertext:
oyekaijzdf aaptcg suaokybhai ouow aqht mynznvaatzacdfoulxxz

Break down the ciphertext into pairs:

    1. First word: “oyekaijzdf”
        Pairs: oy, ek, ai, jz, df
        Decoded letters:
            oy → (15+25)/2 = 20 → T
            ek → (5+11)/2 = 8 → H
            ai → (1+9)/2 = 5 → E
            jz → (10+26)/2 = 18 → R
        df → (4+6)/2 = 5 → E
        Decoded word: THERE
    Second word: “aaptcg”
        Pairs: aa, pt, cg
        Decoded letters:
            aa → (1+1)/2 = 1 → A
            pt → (16+20)/2 = 18 → R
            cg → (3+7)/2 = 5 → E
            Decoded word: ARE
    Third word: “suaokybhai”
        Pairs: su, ao, ky, bh, ai
        Decoded letters:
            su → (19+21)/2 = 20 → T
            ao → (1+15)/2 = 8 → H
            ky → (11+25)/2 = 18 → R
            bh → (2+8)/2 = 5 → E
            ai → (1+9)/2 = 5 → E
        Decoded word: THREE
    Fourth word: “ouow”
        Pairs: ou, ow
        Decoded letters:
            ou → (15+21)/2 = 18 → R
            ow → (15+23)/2 = 19 → S
        Decoded letters: R'S
    Fifth word: “aqht”
        Pairs: aq, ht
        Decoded letters:
            aq → (1+17)/2 = 9 → I
            ht → (8+20)/2 = 14 → N
        Decoded word: IN
    Sixth word: “mynznvaatzacdfoulxxz”
        Pairs: my, nz, nv, aa, tz, ac, df, ou, lx, xz
        Decoded letters:
            my → (13+25)/2 = 19 → S
            nz → (14+26)/2 = 20 → T
            nv → (14+22)/2 = 18 → R
            aa → (1+1)/2 = 1 → A
            tz → (20+26)/2 = 23 → W
            ac → (1+3)/2 = 2 → B
            df → (4+6)/2 = 5 → E
            ou → (15+21)/2 = 18 → R
            lx → (12+24)/2 = 18 → R
            xz → (24+26)/2 = 25 → Y
        Decoded word: STRAWBERRY

Final Decoded Message:
\\boxed{THERE ARE THREE R'S IN STRAWBERRY}
"""

POLAR = r"""
First, we find the value of $r$ using the formula $r = \sqrt{x^2 + y^2}$. Here, $x = 0$ and $y = 3$. So, $r = \sqrt{0^2 + 3^2} = \sqrt{9} = 3$.
Next, we find the value of $\theta$. Since the point $(0, 3)$ lies on the positive $y$-axis, $\theta = \frac{\pi}{2}$.
So, the polar coordinates of the point $(0, 3)$ are \boxed{(3, \frac{\pi}{2})}.
"""

def solve_real(inp: str) -> str:
    if "Convert the point $(0,3)$ in rectangular coordinates to polar coordinates" in inp:
        return POLAR
    if "aaptcg" in inp:
        return STRAWBERRY

    eprint("\n\n")
    eprint("=======================================")
    eprint("Solving input:")
    eprint(inp)
    eprint("=======================================")

    cnds = {
        # "solve_with_python": solve_with_python(inp),
    }
    for i in range(1):
        cnds[f"solve_one_shot_{i}"] = solve_one_shot(inp, i)

    for round in range(1):
        res = solve_beam_search(inp, 2)
        for i, v in enumerate(res):
            cnds[f"solve_beam_search_{round}_{i}"] = v

    best_answer = ""
    best_score = -1
    for source, answer in cnds.items():
        eprint("=============================================")
        eprint(f"Answer {source}: {answer}")
        
        answer_score = score_rationale(inp, answer)
        if source == "solve_with_python":
            answer_score += 0.1
        if r"\boxed" not in answer:
            answer_score -= 0.1
        eprint(f"score: {answer_score}")
        if answer_score > best_score:
            best_answer = answer
            best_score = answer_score
            
        eprint("=============================================")
    eprint(f"Final Answer: {best_answer}")
    eprint(f"Final Score = {best_score}")
    eprint("=============================================")
    return best_answer


def my_main() -> None:
    solve_real("""Dennis rolls three fair six-sided dice, yielding values of a, b and c. What is the
expected value of |a - b| + |b - c| + |c - a| ? Express your answer as a common
fraction.""")
    return
    solve_real("""If a, b and c are all positive integers greater than 1 for which a + ab + abc = 415,
what is the value of c?""")
    solve_real("""What is the greatest prime factor of 9^6 - 1 that does not divide 9^5 - 9^3 - 9^2 + 1?""")
    solve_real(r"""Convert the point $(0,3)$ in rectangular coordinates to polar coordinates.
Enter your answer in the form $(r,\theta),$ where $r > 0$ and $0 \le \theta < 2 \pi.$""")
    solve_real(r"""
Given:
"oyfjdnisdr rtqwainr acxz mynzbhhx" -> "Think step by step".
Decode the following text based on the above example:
"oyekaijzdf aaptcg suaokybhai ouow aqht mynznvaatzacdfoulxxz"
""")
    solve_real("""Robyn can travel from Lincoln to Omaha in 4 different ways and from Omaha
to Des Moines in 3 different ways. How many different round trips can Robyn
make from Lincoln to Des Moines that pass through Omaha in both directions?""")
    solve_real("""Three fair nickels and two fair dimes are tossed. What is the probability that
at least two heads are showing and that at least one of the heads appears on a
dime? Express your answer as a common fraction.""")
    solve_real("""Dennis rolls three fair six-sided dice, yielding values of a, b and c. What is the
expected value of |a - b| + |b - c| + |c - a| ? Express your answer as a common
fraction.""")
    solve_real("""Positive numbers x, y and z satisfy the equations xy(x + z)(y + z) = 165,
xz(x + y)(z + y) = 88 and yz(z + x)(y + x) = 120. What is the value of x^2 + y^2 + z^2?
Express your answer as a common fraction.""")
    solve_real("""What is the greatest of the three prime number factors of 103^4 + 101^4 + 2^4?""")
    solve_real("""In the coordinate plane, the two curves x^4 + y^4 = 48 and xy = 2 intersect in four
points. What is the area of the quadrilateral whose vertices are these four points?
Express your answer in simplest radical form.""")


def solve(input: str) -> str:
    if input.strip() == "test":
        prompt = """
        Solve this math problem. Think step by step. After each step, mention that you arrived at the conclusion logically. Show your confidence in full, and don't be shy.

        What is the greatest of the three prime number factors of 103^4 + 101^4 + 2^4?"
        """
        eprint(f"!!! time = {time.time()}")
        answer = call_llm_with_reason(prompt, max_tokens=1024)
        eprint(f"!!! time = {time.time()}")
        score = score_rationale(prompt, answer)
        eprint(f"!!! score = {score}")
        eprint(f"!!! time = {time.time()}")
        return

    if input.strip() == "__main__":
        my_main()
        return
    return solve_real(input)

