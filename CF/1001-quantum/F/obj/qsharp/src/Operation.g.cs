#pragma warning disable 1591
using System;
using Microsoft.Quantum.Primitive;
using Microsoft.Quantum.Simulation.Core;
using Microsoft.Quantum.MetaData.Attributes;

[assembly: OperationDeclaration("Solution", "Solve (qs : Qubit[], bits0 : Bool[], bits1 : Bool[]) : Int", new string[] { }, "/home/andrew/Dropbox/programming/CF/1001-quantum/F/Operation.qs", 176L, 6L, 5L)]
#line hidden
namespace Solution
{
    public class Solve : Operation<(QArray<Qubit>,QArray<Boolean>,QArray<Boolean>), Int64>, ICallable
    {
        public Solve(IOperationFactory m) : base(m)
        {
        }

        public class In : QTuple<(QArray<Qubit>,QArray<Boolean>,QArray<Boolean>)>, IApplyData
        {
            public In((QArray<Qubit>,QArray<Boolean>,QArray<Boolean>) data) : base(data)
            {
            }

            System.Collections.Generic.IEnumerable<Qubit> IApplyData.Qubits => ((IApplyData)Data.Item1)?.Qubits;
        }

        String ICallable.Name => "Solve";
        String ICallable.FullName => "Solution.Solve";
        protected ICallable<Qubit, Result> M
        {
            get;
            set;
        }

        public override Func<(QArray<Qubit>,QArray<Boolean>,QArray<Boolean>), Int64> Body => (__in) =>
        {
            var (qs,bits0,bits1) = __in;
#line 9 "/home/andrew/Dropbox/programming/CF/1001-quantum/F/Operation.qs"
            foreach (var i in new Range(0L, (qs.Count - 1L)))
            {
#line 10 "/home/andrew/Dropbox/programming/CF/1001-quantum/F/Operation.qs"
                if ((bits0[i] != bits1[i]))
                {
#line 11 "/home/andrew/Dropbox/programming/CF/1001-quantum/F/Operation.qs"
                    var res = M.Apply(qs[i]);
#line 12 "/home/andrew/Dropbox/programming/CF/1001-quantum/F/Operation.qs"
                    if (((bits0[i] == true) == (res == Result.One)))
                    {
#line 13 "/home/andrew/Dropbox/programming/CF/1001-quantum/F/Operation.qs"
                        return 0L;
                    }
                    else
                    {
#line 15 "/home/andrew/Dropbox/programming/CF/1001-quantum/F/Operation.qs"
                        return 1L;
                    }
                }
            }

#line 19 "/home/andrew/Dropbox/programming/CF/1001-quantum/F/Operation.qs"
            return -(1L);
        }

        ;
        public override void Init()
        {
            this.M = this.Factory.Get<ICallable<Qubit, Result>>(typeof(Microsoft.Quantum.Primitive.M));
        }

        public override IApplyData __dataIn((QArray<Qubit>,QArray<Boolean>,QArray<Boolean>) data) => new In(data);
        public override IApplyData __dataOut(Int64 data) => new QTuple<Int64>(data);
        public static System.Threading.Tasks.Task<Int64> Run(IOperationFactory __m__, QArray<Qubit> qs, QArray<Boolean> bits0, QArray<Boolean> bits1)
        {
            return __m__.Run<Solve, (QArray<Qubit>,QArray<Boolean>,QArray<Boolean>), Int64>((qs, bits0, bits1));
        }
    }
}