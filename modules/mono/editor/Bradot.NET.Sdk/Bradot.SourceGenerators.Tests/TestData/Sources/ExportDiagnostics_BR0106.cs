using Bradot;

public interface MyInterface
{
    public int MyProperty { get; set; }
}

public partial class ExportDiagnostics_BR0106_OK : Node, MyInterface
{
    [Export]
    public int MyProperty { get; set; }
}

public partial class ExportDiagnostics_BR0106_KO : Node, MyInterface
{
    [Export]
    int MyInterface.{|BR0106:MyProperty|} { get; set; }
}
