using Bradot;

public partial class AllReadOnly : BradotObject
{
    public readonly string ReadOnlyField = "foo";
    public string ReadOnlyAutoProperty { get; } = "foo";
    public string ReadOnlyProperty { get => "foo"; }
    public string InitOnlyAutoProperty { get; init; }
}
