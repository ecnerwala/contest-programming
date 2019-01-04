#pragma warning disable 1591
using System;
using Microsoft.Quantum.Primitive;
using Microsoft.Quantum.Simulation.Core;
using Microsoft.Quantum.MetaData.Attributes;

[assembly: OperationDeclaration("Solution", "Solve (q : Qubit, sign : Int) : ()", new string[] { }, "/home/andrew/Dropbox/programming/CF/1001-quantum/A/Operation.qs", 152L, 6L, 5L)]
[assembly: OperationDeclaration("Solution", "Test (sign : Int) : Result", new string[] { }, "/home/andrew/Dropbox/programming/CF/1001-quantum/A/Operation.qs", 279L, 14L, 5L)]
#line hidden
namespace Solution
{
    public class Solve : Operation<(Qubit,Int64), QVoid>, ICallable
    {
        public Solve(IOperationFactory m) : base(m)
        {
        }

        public class In : QTuple<(Qubit,Int64)>, IApplyData
        {
            public In((Qubit,Int64) data) : base(data)
            {
            }

            System.Collections.Generic.IEnumerable<Qubit> IApplyData.Qubits
            {
                get
                {
                    yield return Data.Item1;
                }
            }
        }

        String ICallable.Name => "Solve";
        String ICallable.FullName => "Solution.Solve";
        public override Func<(Qubit,Int64), QVoid> Body => (__in) =>
        {
            var (q,sign) = __in;
            // your code here
            ;
#line hidden
            return QVoid.Instance;
        }

        ;
        public override void Init()
        {
        }

        public override IApplyData __dataIn((Qubit,Int64) data) => new In(data);
        public override IApplyData __dataOut(QVoid data) => data;
        public static System.Threading.Tasks.Task<QVoid> Run(IOperationFactory __m__, Qubit q, Int64 sign)
        {
            return __m__.Run<Solve, (Qubit,Int64), QVoid>((q, sign));
        }
    }

    public class Test : Operation<Int64, Result>, ICallable
    {
        public Test(IOperationFactory m) : base(m)
        {
        }

        String ICallable.Name => "Test";
        String ICallable.FullName => "Solution.Test";
        protected Allocate Allocate
        {
            get;
            set;
        }

        protected ICallable<Qubit, Result> M
        {
            get;
            set;
        }

        protected Release Release
        {
            get;
            set;
        }

        protected ICallable<(Qubit,Int64), QVoid> Solve
        {
            get;
            set;
        }

        public override Func<Int64, Result> Body => (__in) =>
        {
            var sign = __in;
#line 17 "/home/andrew/Dropbox/programming/CF/1001-quantum/A/Operation.qs"
            var res = Result.Zero;
#line 18 "/home/andrew/Dropbox/programming/CF/1001-quantum/A/Operation.qs"
            var qubits = Allocate.Apply(1L);
#line 19 "/home/andrew/Dropbox/programming/CF/1001-quantum/A/Operation.qs"
            Solve.Apply((qubits[0L], sign));
#line 20 "/home/andrew/Dropbox/programming/CF/1001-quantum/A/Operation.qs"
            res = M.Apply(qubits[0L]);
#line hidden
            Release.Apply(qubits);
#line 22 "/home/andrew/Dropbox/programming/CF/1001-quantum/A/Operation.qs"
            return res;
        }

        ;
        public override void Init()
        {
            this.Allocate = this.Factory.Get<Allocate>(typeof(Microsoft.Quantum.Primitive.Allocate));
            this.M = this.Factory.Get<ICallable<Qubit, Result>>(typeof(Microsoft.Quantum.Primitive.M));
            this.Release = this.Factory.Get<Release>(typeof(Microsoft.Quantum.Primitive.Release));
            this.Solve = this.Factory.Get<ICallable<(Qubit,Int64), QVoid>>(typeof(Solution.Solve));
        }

        public override IApplyData __dataIn(Int64 data) => new QTuple<Int64>(data);
        public override IApplyData __dataOut(Result data) => new QTuple<Result>(data);
        public static System.Threading.Tasks.Task<Result> Run(IOperationFactory __m__, Int64 sign)
        {
            return __m__.Run<Test, Int64, Result>(sign);
        }
    }
}