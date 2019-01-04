#pragma warning disable 1591
using System;
using Microsoft.Quantum.Primitive;
using Microsoft.Quantum.Simulation.Core;
using Microsoft.Quantum.MetaData.Attributes;

[assembly: OperationDeclaration("Solution", "Solve (qs : Qubit[], index : Int) : ()", new string[] { }, "/home/andrew/Dropbox/programming/CF/1001-quantum/B/Operation.qs", 204L, 7L, 5L)]
#line hidden
namespace Solution
{
    public class Solve : Operation<(QArray<Qubit>,Int64), QVoid>, ICallable
    {
        public Solve(IOperationFactory m) : base(m)
        {
        }

        public class In : QTuple<(QArray<Qubit>,Int64)>, IApplyData
        {
            public In((QArray<Qubit>,Int64) data) : base(data)
            {
            }

            System.Collections.Generic.IEnumerable<Qubit> IApplyData.Qubits => ((IApplyData)Data.Item1)?.Qubits;
        }

        String ICallable.Name => "Solve";
        String ICallable.FullName => "Solution.Solve";
        protected ICallable<(Int64,Int64), Int64> MicrosoftQuantumExtensionsBitwiseAnd
        {
            get;
            set;
        }

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

        protected IUnitary<Qubit> MicrosoftQuantumPrimitiveX
        {
            get;
            set;
        }

        protected IUnitary<Qubit> MicrosoftQuantumPrimitiveZ
        {
            get;
            set;
        }

        public override Func<(QArray<Qubit>,Int64), QVoid> Body => (__in) =>
        {
            var (qs,index) = __in;
#line 10 "/home/andrew/Dropbox/programming/CF/1001-quantum/B/Operation.qs"
            if ((MicrosoftQuantumExtensionsBitwiseAnd.Apply((index, 1L)) != 0L))
            {
#line 11 "/home/andrew/Dropbox/programming/CF/1001-quantum/B/Operation.qs"
                MicrosoftQuantumPrimitiveZ.Apply(qs[0L]);
            }

#line 13 "/home/andrew/Dropbox/programming/CF/1001-quantum/B/Operation.qs"
            MicrosoftQuantumPrimitiveH.Apply(qs[0L]);
#line 14 "/home/andrew/Dropbox/programming/CF/1001-quantum/B/Operation.qs"
            MicrosoftQuantumPrimitiveCNOT.Apply((qs[0L], qs[1L]));
#line 15 "/home/andrew/Dropbox/programming/CF/1001-quantum/B/Operation.qs"
            if ((MicrosoftQuantumExtensionsBitwiseAnd.Apply((index, 2L)) != 0L))
            {
#line 16 "/home/andrew/Dropbox/programming/CF/1001-quantum/B/Operation.qs"
                MicrosoftQuantumPrimitiveX.Apply(qs[1L]);
            }

#line hidden
            return QVoid.Instance;
        }

        ;
        public override void Init()
        {
            this.MicrosoftQuantumExtensionsBitwiseAnd = this.Factory.Get<ICallable<(Int64,Int64), Int64>>(typeof(Microsoft.Quantum.Extensions.Bitwise.And));
            this.MicrosoftQuantumPrimitiveCNOT = this.Factory.Get<IUnitary<(Qubit,Qubit)>>(typeof(Microsoft.Quantum.Primitive.CNOT));
            this.MicrosoftQuantumPrimitiveH = this.Factory.Get<IUnitary<Qubit>>(typeof(Microsoft.Quantum.Primitive.H));
            this.MicrosoftQuantumPrimitiveX = this.Factory.Get<IUnitary<Qubit>>(typeof(Microsoft.Quantum.Primitive.X));
            this.MicrosoftQuantumPrimitiveZ = this.Factory.Get<IUnitary<Qubit>>(typeof(Microsoft.Quantum.Primitive.Z));
        }

        public override IApplyData __dataIn((QArray<Qubit>,Int64) data) => new In(data);
        public override IApplyData __dataOut(QVoid data) => data;
        public static System.Threading.Tasks.Task<QVoid> Run(IOperationFactory __m__, QArray<Qubit> qs, Int64 index)
        {
            return __m__.Run<Solve, (QArray<Qubit>,Int64), QVoid>((qs, index));
        }
    }
}