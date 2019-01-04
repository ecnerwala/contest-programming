#pragma warning disable 1591
using System;
using Microsoft.Quantum.Primitive;
using Microsoft.Quantum.Simulation.Core;
using Microsoft.Quantum.MetaData.Attributes;

[assembly: OperationDeclaration("Solution", "Solve (qs : Qubit[]) : Int", new string[] { }, "/home/andrew/Dropbox/programming/CF/1001-quantum/E/Operation.qs", 144L, 6L, 5L)]
#line hidden
namespace Solution
{
    public class Solve : Operation<QArray<Qubit>, Int64>, ICallable
    {
        public Solve(IOperationFactory m) : base(m)
        {
        }

        String ICallable.Name => "Solve";
        String ICallable.FullName => "Solution.Solve";
        protected IUnitary<(Qubit,Qubit)> MicrosoftQuantumPrimitiveCNOT
        {
            get;
            set;
        }

        protected IUnitary<Qubit> MicrosoftQuantumPrimitiveH
        {
            get;
            set;
        }

        protected ICallable<Qubit, Result> M
        {
            get;
            set;
        }

        public override Func<QArray<Qubit>, Int64> Body => (__in) =>
        {
            var qs = __in;
#line 9 "/home/andrew/Dropbox/programming/CF/1001-quantum/E/Operation.qs"
            var result = 0L;
#line 10 "/home/andrew/Dropbox/programming/CF/1001-quantum/E/Operation.qs"
            MicrosoftQuantumPrimitiveCNOT.Apply((qs[0L], qs[1L]));
#line 11 "/home/andrew/Dropbox/programming/CF/1001-quantum/E/Operation.qs"
            if ((M.Apply(qs[1L]) == Result.One))
            {
#line 12 "/home/andrew/Dropbox/programming/CF/1001-quantum/E/Operation.qs"
                result = (result + 2L);
            }

#line 14 "/home/andrew/Dropbox/programming/CF/1001-quantum/E/Operation.qs"
            MicrosoftQuantumPrimitiveH.Apply(qs[0L]);
#line 15 "/home/andrew/Dropbox/programming/CF/1001-quantum/E/Operation.qs"
            if ((M.Apply(qs[0L]) == Result.One))
            {
#line 16 "/home/andrew/Dropbox/programming/CF/1001-quantum/E/Operation.qs"
                result = (result + 1L);
            }

#line 18 "/home/andrew/Dropbox/programming/CF/1001-quantum/E/Operation.qs"
            return result;
        }

        ;
        public override void Init()
        {
            this.MicrosoftQuantumPrimitiveCNOT = this.Factory.Get<IUnitary<(Qubit,Qubit)>>(typeof(Microsoft.Quantum.Primitive.CNOT));
            this.MicrosoftQuantumPrimitiveH = this.Factory.Get<IUnitary<Qubit>>(typeof(Microsoft.Quantum.Primitive.H));
            this.M = this.Factory.Get<ICallable<Qubit, Result>>(typeof(Microsoft.Quantum.Primitive.M));
        }

        public override IApplyData __dataIn(QArray<Qubit> data) => data;
        public override IApplyData __dataOut(Int64 data) => new QTuple<Int64>(data);
        public static System.Threading.Tasks.Task<Int64> Run(IOperationFactory __m__, QArray<Qubit> qs)
        {
            return __m__.Run<Solve, QArray<Qubit>, Int64>(qs);
        }
    }
}