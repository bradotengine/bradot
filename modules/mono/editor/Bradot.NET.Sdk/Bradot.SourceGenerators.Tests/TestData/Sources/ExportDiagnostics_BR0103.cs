using Bradot;

public partial class ExportDiagnostics_BR0103 : Node
{
    [Export]
    public readonly string {|BR0103:ReadOnlyField|};

    [Export]
    public string {|BR0103:ReadOnlyProperty|} { get; }
}
