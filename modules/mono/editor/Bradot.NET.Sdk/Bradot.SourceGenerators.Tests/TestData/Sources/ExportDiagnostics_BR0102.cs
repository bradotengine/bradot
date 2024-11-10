using Bradot;

public partial class ExportDiagnostics_BR0102 : Node
{
    public struct MyStruct { }

    [Export]
    public MyStruct {|BR0102:StructField|};

    [Export]
    public MyStruct {|BR0102:StructProperty|} { get; set; }
}
