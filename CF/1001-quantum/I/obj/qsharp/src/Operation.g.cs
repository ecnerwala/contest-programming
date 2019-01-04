#pragma warning disable 1591
using System;
using Microsoft.Quantum.Primitive;
using Microsoft.Quantum.Simulation.Core;
using Microsoft.Quantum.MetaData.Attributes;

[assembly: OperationDeclaration("Solution", "Solve (N : Int, Uf : ((Qubit[], Qubit) => ())) : Bool", new string[] { }, "/home/andrew/Dropbox/programming/CF/1001-quantum/I/Operation.qs", 171L, 6L, 5L)]
#line hidden
namespace Solution
{
    public class Solve : Operation<(Int64,ICallable), Boolean>, ICallable
    {
        public Solve(IOperationFactory m) : base(m)
        {
        }

        public class In : QTuple<(Int64,ICallable)>, IApplyData
        {
            public In((Int64,ICallable) data) : base(data)
            {
            }

            System.Collections.Generic.IEnumerable<Qubit> IApplyData.Qubits => ((IApplyData)Data.Item2)?.Qubits;
        }

        String ICallable.Name => "Solve";
        String ICallable.FullName => "Solution.Solve";
        protected Allocate Allocate
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

        protected Release Release
        {
            get;
            set;
        }

        protected ICallable<QArray<Qubit>, QVoid> ResetAll
        {
            get;
            set;
        }

        protected IUnitary<Qubit> MicrosoftQuantumPrimitiveZ
        {
            get;
            set;
        }

        public override Func<(Int64,ICallable), Boolean> Body => (__in) =>
        {
            var (N,Uf) = __in;
#line 9 "/home/andrew/Dropbox/programming/CF/1001-quantum/I/Operation.qs"
            var result = false;
#line 10 "/home/andrew/Dropbox/programming/CF/1001-quantum/I/Operation.qs"
            var qubits = Allocate.Apply((N + 1L));
#line 11 "/home/andrew/Dropbox/programming/CF/1001-quantum/I/Operation.qs"
            var xs = qubits?.Slice(new Range(0L, (N - 1L)));
#line 12 "/home/andrew/Dropbox/programming/CF/1001-quantum/I/Operation.qs"
            var y = qubits[N];
#line 14 "/home/andrew/Dropbox/programming/CF/1001-quantum/I/Operation.qs"
            foreach (var i in new Range(0L, (N - 1L)))
            {
#line 15 "/home/andrew/Dropbox/programming/CF/1001-quantum/I/Operation.qs"
                MicrosoftQuantumPrimitiveH.Apply(xs[i]);
            }

#line 18 "/home/andrew/Dropbox/programming/CF/1001-quantum/I/Operation.qs"
            MicrosoftQuantumPrimitiveH.Apply(y);
#line 19 "/home/andrew/Dropbox/programming/CF/1001-quantum/I/Operation.qs"
            MicrosoftQuantumPrimitiveZ.Apply(y);
#line 21 "/home/andrew/Dropbox/programming/CF/1001-quantum/I/Operation.qs"
            Uf.Apply((xs, y));
#line 23 "/home/andrew/Dropbox/programming/CF/1001-quantum/I/Operation.qs"
            foreach (var i in new Range(0L, (N - 1L)))
            {
#line 24 "/home/andrew/Dropbox/programming/CF/1001-quantum/I/Operation.qs"
                MicrosoftQuantumPrimitiveH.Apply(xs[i]);
            }

#line 27 "/home/andrew/Dropbox/programming/CF/1001-quantum/I/Operation.qs"
            result = true;
            // assume it's constant
#line 28 "/home/andrew/Dropbox/programming/CF/1001-quantum/I/Operation.qs"
            foreach (var i in new Range(0L, (N - 1L)))
            {
                // any 1 means it's false
#line 30 "/home/andrew/Dropbox/programming/CF/1001-quantum/I/Operation.qs"
                if ((M.Apply(xs[i]) == Result.One))
                {
#line 31 "/home/andrew/Dropbox/programming/CF/1001-quantum/I/Operation.qs"
                    result = false;
                }
            }

#line 34 "/home/andrew/Dropbox/programming/CF/1001-quantum/I/Operation.qs"
            ResetAll.Apply(qubits);
#line hidden
            Release.Apply(qubits);
#line 36 "/home/andrew/Dropbox/programming/CF/1001-quantum/I/Operation.qs"
            return result;
        }

        ;
        public override void Init()
        {
            this.Allocate = this.Factory.Get<Allocate>(typeof(Microsoft.Quantum.Primitive.Allocate));
            this.MicrosoftQuantumPrimitiveH = this.Factory.Get<IUnitary<Qubit>>(typeof(Microsoft.Quantum.Primitive.H));
            this.M = this.Factory.Get<ICallable<Qubit, Result>>(typeof(Microsoft.Quantum.Primitive.M));
            this.Release = this.Factory.Get<Release>(typeof(Microsoft.Quantum.Primitive.Release));
            this.ResetAll = this.Factory.Get<ICallable<QArray<Qubit>, QVoid>>(typeof(Microsoft.Quantum.Primitive.ResetAll));
            this.MicrosoftQuantumPrimitiveZ = this.Factory.Get<IUnitary<Qubit>>(typeof(Microsoft.Quantum.Primitive.Z));
        }

        public override IApplyData __dataIn((Int64,ICallable) data) => new In(data);
        public override IApplyData __dataOut(Boolean data) => new QTuple<Boolean>(data);
        public static System.Threading.Tasks.Task<Boolean> Run(IOperationFactory __m__, Int64 N, ICallable Uf)
        {
            return __m__.Run<Solve, (Int64,ICallable), Boolean>((N, Uf));
        }
    }
}