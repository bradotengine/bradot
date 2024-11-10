using System;

namespace Bradot.SourceGenerators.Sample
{
    public partial class AllWriteOnly : BradotObject
    {
        private bool _writeOnlyBackingField = false;
        public bool WriteOnlyProperty { set => _writeOnlyBackingField = value; }
    }
}
