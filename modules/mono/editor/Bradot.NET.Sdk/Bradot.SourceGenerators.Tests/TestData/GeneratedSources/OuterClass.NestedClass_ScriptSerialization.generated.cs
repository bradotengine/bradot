using Bradot;
using Bradot.NativeInterop;

partial struct OuterClass
{
partial class NestedClass
{
    /// <inheritdoc/>
    [global::System.ComponentModel.EditorBrowsable(global::System.ComponentModel.EditorBrowsableState.Never)]
    protected override void SaveBradotObjectData(global::Bradot.Bridge.BradotSerializationInfo info)
    {
        base.SaveBradotObjectData(info);
    }
    /// <inheritdoc/>
    [global::System.ComponentModel.EditorBrowsable(global::System.ComponentModel.EditorBrowsableState.Never)]
    protected override void RestoreBradotObjectData(global::Bradot.Bridge.BradotSerializationInfo info)
    {
        base.RestoreBradotObjectData(info);
    }
}
}
