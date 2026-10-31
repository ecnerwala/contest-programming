import sys
from typing import Literal, overload

FinishReason = Literal["stop", "length", "content_filter"]

if "generate_completions" not in locals():
    @overload
    def generate_completions(
        instruction: str,
        max_tokens: int = 256,
        temperature: float = 0.7,
        n: int = 1,
        prefix: str = "",
    ) -> list[tuple[str, FinishReason]]:
        ...

    @overload
    def score_rationale(inp: str, answer: str) -> float:
        ...


def eprint(*args, **kwargs):
    print(*args, **kwargs, file=sys.stderr)


PROMPT_TEMPLATE = """
<USER> You are Devin, a state of the art mathematical reasoning model. You can carefully analyze mathematical problems, make step-by-step logical deductions, and arrive at a final answer.

Please solve the following math problem: {input}

Take a deep breath and think step by step.
Include your final answer (the precise value) in \\boxed{{}}. For example, if the answer is 3, you should include \\boxed{{3}} in your response.
"""

RERANK_PROMPT_TEMPLATE = """
You are SuperDevin, a supervisor for Devin. You are specifically trained to grade Devin's responses and judge logical consistency.

Devin tried to solve this problem: {input}

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
3. 
"""

PYTHON_SOLVER_PROMPT = """
You are Devin, a state of the art reasoning bot. You can carefully analyze mathematical problems, make step-by-step logical deductions, as well as use Python to perform calculations.

Please solve the following math problem using a Python program: {input}

Start by analyzing the problem in a comment.
Then, implement the solve() function. In your function, print all intermediate values using the print function.

For instance, if the problem is:
The mean of three positive integers a, b and c is 10, and the median of these
integers is 5. What is the greatest possible value for c?

You should output:

# I will brute force all possible integers. Because the mean is 10 and all numbers are positive, I only need to search for numbers up to 10 * 3 = 30.
def solve():
    best_c = -1
    for a in range(1, 31):
        for b in range(1, 31):
            for c in range(1, 31):
                if (a + b + c) == 10 * 3 and sorted([a, b, c])[1] == 5:
                    print(f"Found triple {{a}} {{b}} {{c}}")
                    best_c = max(best_c, c)
    print(f"Answer: {{best_c}}")
    return best_c
"""

def run_python_program(prog: str):
    subprocess_prints = []
    def print(*args, **kwargs):
        from builtins import print as builtin_print
        import io
        f = io.StringIO()
        builtin_print(*args, **kwargs, file=f)
        subprocess_prints.append(f.getvalue())
    exec(prog + "\n\nprint(solve())")
    return '\n'.join(subprocess_prints)


def rank_responses(inp: str, resp: list[str]):
    pass

def solve_real(inp: str) -> str:
    eprint("\n\n")
    eprint("=======================================")
    eprint("Solving input:")
    eprint(inp)
    eprint("=======================================")

    temperature = 1.0
    max_token = 1024
    n = 5
    
    result = generate_completions(
        PROMPT_TEMPLATE.format(input=inp),
        max_token,
        temperature,
        n,
        "<AGENT> "
    )
    
    best_answer = ""
    best_score = -1
    cnt = 0
    
    for answer, stop_word in result:
        eprint("=============================================")
        cnt += 1
        sys.stderr.write(f"Answer {cnt}: {answer}\n")
        sys.stderr.write(f"stop_word: {stop_word}\n")
        
        answer_score = score_rationale(inp, answer)
        eprint(f"Score: {answer_score}")
        if answer_score > best_score:
            best_answer = answer
            best_score = answer_score
            
        sys.stderr.write("=============================================\n")
        
    sys.stderr.write(f"Final Answer: {best_answer}\n")
    sys.stderr.write(f"Final Score = {best_score}\n")
    sys.stderr.write("=============================================\n")
    
    return best_answer

def my_main() -> None:
    solve_real(r"""Convert the point $(0,3)$ in rectangular coordinates to polar coordinates.
Enter your answer in the form $(r,\theta),$ where $r > 0$ and $0 \le \theta < 2 \pi.$""")
    solve_real(r"""
Given:
"oyfjdnisdr rtqwainr acxz mynzbhhx" -> "Think step by step".
Decode the following text based on the above example:
"oyekaijzdf aaptcg suaokybhai ouow aqht mynznvaatzacdfoulxxz"
""")

def solve(input: str) -> str:
    if input.strip() == "__main__":
        my_main()
        return
    return solve_real(input)
