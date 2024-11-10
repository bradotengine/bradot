using Bradot;

public abstract partial class AbstractGenericNode<[MustBeVariant] T> : Node
{
    [Export] // This should be included, but without type hints.
    public Bradot.Collections.Array<T> MyArray { get; set; } = new();
}
