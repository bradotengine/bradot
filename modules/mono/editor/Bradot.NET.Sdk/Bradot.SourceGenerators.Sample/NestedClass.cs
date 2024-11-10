using System;

namespace Bradot.SourceGenerators.Sample;

public partial class NestedClass : BradotObject
{
    public partial class NestedClass2 : BradotObject
    {
        public partial class NestedClass3 : BradotObject
        {
            [Signal]
            public delegate void MySignalEventHandler(string str, int num);

            [Export] private String _fieldString = "foo";
            [Export] private String PropertyString { get; set; } = "foo";

            private void Method()
            {
            }
        }
    }
}
