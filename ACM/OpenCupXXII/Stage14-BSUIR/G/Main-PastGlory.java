import java.io.OutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.PrintWriter;
import java.io.OutputStream;
import java.util.Set;
import java.util.HashMap;
import java.io.IOException;
import java.util.InputMismatchException;
import java.io.InputStreamReader;
import java.util.function.Function;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.io.Writer;
import java.io.BufferedReader;
import java.io.InputStream;

/**
 * Built using CHelper plug-in
 * Actual solution is at the top
 *
 * @author Niyaz Nigmatullin
 */
public class Main {
    public static void main(String[] args) {
        InputStream inputStream = System.in;
        OutputStream outputStream = System.out;
        FastScanner in = new FastScanner(inputStream);
        FastPrinter out = new FastPrinter(outputStream);
        TaskG solver = new TaskG();
        solver.solve(1, in, out);
        out.close();
    }

    static class TaskG {
        static Set<Integer> validVersions;
        static Map<String, TaskG.Variable> varsByName;
        static Map<String, TaskG.FunctionDeclaration> funcsByName;

        public void solve(int testNumber, FastScanner in, FastPrinter out) {
            int functionDeclarations = Integer.parseInt(in.nextLine());
            TaskG.FunctionDeclaration[] fDecl = new TaskG.FunctionDeclaration[functionDeclarations];
            funcsByName = new HashMap<>();
            for (int i = 0; i < functionDeclarations; i++) {
                TaskG.Parser parser = new TaskG.Parser(in.nextLine());
                fDecl[i] = parser.parseFunction();
                parser.finish();
                funcsByName.put(fDecl[i].name.name, fDecl[i]);
            }
            int variableDeclarations = Integer.parseInt(in.nextLine());
            TaskG.VarDeclaration[] vDecl = new TaskG.VarDeclaration[variableDeclarations];
            validVersions = new HashSet<>();
            int freeVersion = 0;
            varsByName = new HashMap<>();
            for (int i = 0; i < variableDeclarations; i++) {
                TaskG.Parser parser = new TaskG.Parser(in.nextLine());
                vDecl[i] = parser.parseVarDecl();
                parser.finish();
                if (!vDecl[i].right.invalidateValues() || !vDecl[i].right.checkReferences()) {
                    out.println("Error in line " + (i + 1));
                    return;
                }
                TaskG.VarType rightType = vDecl[i].right.getType();
                TaskG.Variable oldVar = varsByName.get(vDecl[i].left);
                if (oldVar != null) {
                    validVersions.remove(oldVar.versionId);
                }
                int varVersion;
                if (rightType == TaskG.VarType.REFERENCE && vDecl[i].right instanceof TaskG.VarExpression) {
                    varVersion = varsByName.get(((TaskG.VarExpression) vDecl[i].right).expression.name).versionId;
                } else {
                    varVersion = freeVersion;
                    validVersions.add(freeVersion);
                    ++freeVersion;
                }
                TaskG.Variable newVar = new TaskG.Variable(varVersion, rightType);
                varsByName.put(vDecl[i].left, newVar);
            }
            out.println("Valid");
        }

        enum VarType {
            VALUE,
            REFERENCE,
            ;
        }

        static class FunctionDeclaration {
            TaskG.NameOrRefName name;
            List<TaskG.NameOrRefName> arguments;

            public FunctionDeclaration(TaskG.NameOrRefName name, List<TaskG.NameOrRefName> arguments) {
                this.name = name;
                this.arguments = arguments;
            }

        }

        static class NameOrRefName {
            String name;
            TaskG.VarType type;

            public NameOrRefName(String name, TaskG.VarType type) {
                this.name = name;
                this.type = type;
            }

        }

        static class VarExpression extends TaskG.Expression {
            TaskG.NameOrRefName expression;

            public VarExpression(TaskG.NameOrRefName name) {
                this.expression = name;
            }

            public boolean invalidateValues() {
                TaskG.Variable var = varsByName.get(expression.name);
                if (var == null || !validVersions.contains(var.versionId)) {
//                System.err.println("Bad var or invalid");
                    return false;
                }
                if (getType() == TaskG.VarType.VALUE) { // ????
                    validVersions.remove(var.versionId);
                }
                return true;
            }

            public boolean checkReferences() {
                TaskG.Variable var = varsByName.get(expression.name);
                if (var == null) {
//                System.err.println("Bad var");
                    return false;
                }
                if (expression.type == TaskG.VarType.REFERENCE) {
                    if (var.type == TaskG.VarType.REFERENCE) {
//                    System.err.println("Double reference");
                        return false;
                    }
                } else if (expression.type == TaskG.VarType.VALUE && var.type == TaskG.VarType.REFERENCE) {
                    if (!validVersions.contains(var.versionId)) {
//                    System.err.println("Reference to invalid");
                        return false;
                    }
                }
                return true;
            }

            protected TaskG.VarType getType() {
                if (expression.type == TaskG.VarType.REFERENCE) {
                    return expression.type;
                }
                TaskG.Variable var = varsByName.get(expression.name);
                if (var == null) throw new AssertionError();
                return var.type;
            }

        }

        static class VarDeclaration {
            String left;
            TaskG.Expression right;

            public VarDeclaration(String left, TaskG.Expression right) {
                this.left = left;
                this.right = right;
            }

        }

        static abstract class Expression {
            public abstract boolean invalidateValues();

            public abstract boolean checkReferences();

            protected abstract TaskG.VarType getType();

        }

        static class FunctionCall extends TaskG.Expression {
            String funcName;
            List<TaskG.Expression> callArguments;

            public FunctionCall(String funcName, List<TaskG.Expression> callArguments) {
                this.funcName = funcName;
                this.callArguments = callArguments;
            }

            public boolean invalidateValues() {
                for (TaskG.Expression e : callArguments) {
                    if (!e.invalidateValues()) {
                        return false;
                    }
                }
                return true;
            }

            public boolean checkReferences() {
                TaskG.FunctionDeclaration func = funcsByName.get(funcName);
                if (func == null || func.arguments.size() != callArguments.size()) {
//                System.err.println("Bad function or arguments count");
                    return false;
                }
                for (TaskG.Expression e : callArguments) {
                    if (!e.checkReferences()) {
                        return false;
                    }
                }
                for (int i = 0; i < callArguments.size(); i++) {
                    if (func.arguments.get(i).type != callArguments.get(i).getType()) {
//                    System.err.println("Bad type");
                        return false;
                    }
                }
                return true;
            }

            protected TaskG.VarType getType() {
                TaskG.FunctionDeclaration func = funcsByName.get(funcName);
                if (func == null) {
                    throw new AssertionError();
                }
                return func.name.type;
            }

        }

        static class Parser {
            char[] c;
            int current;

            Parser(String s) {
                c = s.toCharArray();
                current = 0;
            }

            void skip() {
                ++current;
            }

            void skip(char ch) {
                if (!next(ch)) {
                    throw new AssertionError();
                }
                skip();
            }

            void skip(String s) {
                for (char c : s.toCharArray()) {
                    skip(c);
                }
            }

            boolean next(char ch) {
                return current < c.length && ch == c[current];
            }

            boolean next(Function<Character, Boolean> is) {
                return current < c.length && is.apply(c[current]);
            }

            TaskG.FunctionDeclaration parseFunction() {
                skip('f');
                skip('n');
                skip(' ');
                TaskG.NameOrRefName name = parseNameOrRef();
                skip('(');
                List<TaskG.NameOrRefName> arguments = new ArrayList<>();
                if (!next(')')) {
                    arguments.add(parseNameOrRef());
                    while (next(',')) {
                        skip(", ");
                        arguments.add(parseNameOrRef());
                    }
                }
                skip(')');
                return new TaskG.FunctionDeclaration(name, arguments);
            }

            private TaskG.NameOrRefName parseNameOrRef() {
                TaskG.VarType type = TaskG.VarType.VALUE;
                if (next('&')) {
                    type = TaskG.VarType.REFERENCE;
                    skip('&');
                }
                String name = parseName();
                return new TaskG.NameOrRefName(name, type);
            }

            private String parseName() {
                StringBuilder sb = new StringBuilder();
                while (next(Character::isLetter)) {
                    sb.append(c[current]);
                    skip();
                }
                return sb.toString();
            }

            private TaskG.VarDeclaration parseVarDecl() {
                String left = parseName();
                skip(" := ");
                TaskG.Expression right = parseExpression();
                return new TaskG.VarDeclaration(left, right);
            }

            private TaskG.Expression parseExpression() {
                TaskG.NameOrRefName name = parseNameOrRef();
                if (next('(')) {
                    String funcName = name.name;
                    skip('(');
                    List<TaskG.Expression> callArguments = new ArrayList<>();
                    if (!next(')')) {
                        callArguments.add(parseExpression());
                        while (next(',')) {
                            skip(", ");
                            callArguments.add(parseExpression());
                        }
                    }
                    skip(')');
                    return new TaskG.FunctionCall(funcName, callArguments);
                } else {
                    return new TaskG.VarExpression(name);
                }
            }

            void finish() {
                if (current != c.length) {
                    throw new AssertionError();
                }
            }

        }

        static class Variable {
            int versionId;
            TaskG.VarType type;

            public Variable(int versionId, TaskG.VarType type) {
                this.versionId = versionId;
                this.type = type;
            }

        }

    }

    static class FastScanner extends BufferedReader {
        public FastScanner(InputStream is) {
            super(new InputStreamReader(is));
        }

        public int read() {
            try {
                int ret = super.read();
//            if (isEOF && ret < 0) {
//                throw new InputMismatchException();
//            }
//            isEOF = ret == -1;
                return ret;
            } catch (IOException e) {
                throw new InputMismatchException();
            }
        }

        public String nextLine() {
            int c = read();
            String ret = readLine();
            if (ret == null) {
                return ret;
            }
            if (c != 13) {
                return (char) c + ret;
            }
            return ret;
        }

        public String readLine() {
            try {
                return super.readLine();
            } catch (IOException e) {
                return null;
            }
        }

    }

    static class FastPrinter extends PrintWriter {
        public FastPrinter(OutputStream out) {
            super(out);
        }

        public FastPrinter(Writer out) {
            super(out);
        }

    }
}

