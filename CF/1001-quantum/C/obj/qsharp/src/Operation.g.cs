#pragma warning disable 1591
using System;
using Microsoft.Quantum.Primitive;
using Microsoft.Quantum.Simulation.Core;
using Microsoft.Quantum.MetaData.Attributes;

[assembly: OperationDeclaration("Solution", "Solve (qs : Qubit[]) : ()", new string[] { }, "/home/andrew/Dropbox/programming/CF/1001-quantum/C/Operation.qs", 143L, 6L, 5L)]
#line hidden
namespace Solution
{
    public class Solve : Operation<QArray<Qubit>, QVoid>, ICallable
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

        public override Func<QArray<Qubit>, QVoid> Body => (__in) =>
        {
            var qs = __in;
#line 9 "/home/andrew/Dropbox/programming/CF/1001-quantum/C/Operation.qs"
            MicrosoftQuantumPrimitiveH.Apply(qs[0L]);
#line 10 "/home/andrew/Dropbox/programming/CF/1001-quantum/C/Operation.qs"
            foreach (var i in new Range(1L, (qs.Count - 1L)))
            {
#line 11 "/home/andrew/Dropbox/programming/CF/1001-quantum/C/Operation.qs"
                MicrosoftQuantumPrimitiveCNOT.Apply((qs[0L], qs[i]));
            }

#line hidden
            return QVoid.Instance;
        }

        ;
        public override void Init()
        {
            this.MicrosoftQuantumPrimitiveCNOT = this.Factory.Get<IUnitary<(Qubit,Qubit)>>(typeof(Microsoft.Quantum.Primitive.CNOT));
            this.MicrosoftQuantumPrimitiveH = this.Factory.Get<IUnitary<Qubit>>(typeof(Microsoft.Quantum.Primitive.H));
        }

        public override IApplyData __dataIn(QArray<Qubit> data) => data;
        public override IApplyData __dataOut(QVoid data) => data;
        public static System.Threading.Tasks.Task<QVoid> Run(IOperationFactory __m__, QArray<Qubit> qs)
        {
            return __m__.Run<Solve, QArray<Qubit>, QVoid>(qs);
        }
    }
}