using Bradot;
using Bradot.NativeInterop;

partial class ScriptBoilerplate
{
#pragma warning disable CS0109 // Disable warning about redundant 'new' keyword
    /// <summary>
    /// Cached StringNames for the properties and fields contained in this class, for fast lookup.
    /// </summary>
    public new class PropertyName : global::Bradot.Node.PropertyName {
        /// <summary>
        /// Cached name for the '_nodePath' field.
        /// </summary>
        public new static readonly global::Bradot.StringName @_nodePath = "_nodePath";
        /// <summary>
        /// Cached name for the '_velocity' field.
        /// </summary>
        public new static readonly global::Bradot.StringName @_velocity = "_velocity";
    }
    /// <inheritdoc/>
    [global::System.ComponentModel.EditorBrowsable(global::System.ComponentModel.EditorBrowsableState.Never)]
    protected override bool SetBradotClassPropertyValue(in bradot_string_name name, in bradot_variant value)
    {
        if (name == PropertyName.@_nodePath) {
            this.@_nodePath = global::Bradot.NativeInterop.VariantUtils.ConvertTo<global::Bradot.NodePath>(value);
            return true;
        }
        if (name == PropertyName.@_velocity) {
            this.@_velocity = global::Bradot.NativeInterop.VariantUtils.ConvertTo<int>(value);
            return true;
        }
        return base.SetBradotClassPropertyValue(name, value);
    }
    /// <inheritdoc/>
    [global::System.ComponentModel.EditorBrowsable(global::System.ComponentModel.EditorBrowsableState.Never)]
    protected override bool GetBradotClassPropertyValue(in bradot_string_name name, out bradot_variant value)
    {
        if (name == PropertyName.@_nodePath) {
            value = global::Bradot.NativeInterop.VariantUtils.CreateFrom<global::Bradot.NodePath>(this.@_nodePath);
            return true;
        }
        if (name == PropertyName.@_velocity) {
            value = global::Bradot.NativeInterop.VariantUtils.CreateFrom<int>(this.@_velocity);
            return true;
        }
        return base.GetBradotClassPropertyValue(name, out value);
    }
    /// <summary>
    /// Get the property information for all the properties declared in this class.
    /// This method is used by Bradot to register the available properties in the editor.
    /// Do not call this method.
    /// </summary>
    [global::System.ComponentModel.EditorBrowsable(global::System.ComponentModel.EditorBrowsableState.Never)]
    internal new static global::System.Collections.Generic.List<global::Bradot.Bridge.PropertyInfo> GetBradotPropertyList()
    {
        var properties = new global::System.Collections.Generic.List<global::Bradot.Bridge.PropertyInfo>();
        properties.Add(new(type: (global::Bradot.Variant.Type)22, name: PropertyName.@_nodePath, hint: (global::Bradot.PropertyHint)0, hintString: "", usage: (global::Bradot.PropertyUsageFlags)4096, exported: false));
        properties.Add(new(type: (global::Bradot.Variant.Type)2, name: PropertyName.@_velocity, hint: (global::Bradot.PropertyHint)0, hintString: "", usage: (global::Bradot.PropertyUsageFlags)4096, exported: false));
        return properties;
    }
#pragma warning restore CS0109
}
