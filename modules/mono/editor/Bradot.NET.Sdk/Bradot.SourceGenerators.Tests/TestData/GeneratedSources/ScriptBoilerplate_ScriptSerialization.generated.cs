using Bradot;
using Bradot.NativeInterop;

partial class ScriptBoilerplate
{
    /// <inheritdoc/>
    [global::System.ComponentModel.EditorBrowsable(global::System.ComponentModel.EditorBrowsableState.Never)]
    protected override void SaveBradotObjectData(global::Bradot.Bridge.BradotSerializationInfo info)
    {
        base.SaveBradotObjectData(info);
        info.AddProperty(PropertyName.@_nodePath, global::Bradot.Variant.From<global::Bradot.NodePath>(this.@_nodePath));
        info.AddProperty(PropertyName.@_velocity, global::Bradot.Variant.From<int>(this.@_velocity));
    }
    /// <inheritdoc/>
    [global::System.ComponentModel.EditorBrowsable(global::System.ComponentModel.EditorBrowsableState.Never)]
    protected override void RestoreBradotObjectData(global::Bradot.Bridge.BradotSerializationInfo info)
    {
        base.RestoreBradotObjectData(info);
        if (info.TryGetProperty(PropertyName.@_nodePath, out var _value__nodePath))
            this.@_nodePath = _value__nodePath.As<global::Bradot.NodePath>();
        if (info.TryGetProperty(PropertyName.@_velocity, out var _value__velocity))
            this.@_velocity = _value__velocity.As<int>();
    }
}
