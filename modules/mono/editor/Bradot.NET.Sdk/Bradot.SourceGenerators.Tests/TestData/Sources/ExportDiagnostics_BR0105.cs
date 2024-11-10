using System;
using Bradot;

public partial class ExportDiagnostics_BR0105 : Node
{
    [Export]
    public int {|BR0105:this|}[int index]
    {
        get { return index; }
        set { }
    }
}
