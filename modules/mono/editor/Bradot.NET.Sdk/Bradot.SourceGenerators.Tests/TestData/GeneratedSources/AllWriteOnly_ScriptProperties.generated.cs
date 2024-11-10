using Bradot;
using Bradot.NativeInterop;

partial class AllWriteOnly
{
#pragma warning disable CS0109 // Disable warning about redundant 'new' keyword
    /// <summary>
    /// Cached StringNames for the properties and fields contained in this class, for fast lookup.
    /// </summary>
    public new class PropertyName : global::Bradot.BradotObject.PropertyName {
        /// <summary>
        /// Cached name for the 'WriteOnlyProperty' property.
        /// </summary>
        public new static readonly global::Bradot.StringName @WriteOnlyProperty = "WriteOnlyProperty";
        /// <summary>
        /// Cached name for the '_writeOnlyBackingField' field.
        /// </summary>
        public new static readonly global::Bradot.StringName @_writeOnlyBackingField = "_writeOnlyBackingField";
    }
    /// <inheritdoc/>
    [global::System.ComponentModel.EditorBrowsable(global::System.ComponentModel.EditorBrowsableState.Never)]
    protected override bool SetBradotClassPropertyValue(in bradot_string_name name, in bradot_variant value)
    {
        if (name == PropertyName.@WriteOnlyProperty) {
            this.@WriteOnlyProperty = global::Bradot.NativeInterop.VariantUtils.ConvertTo<bool>(value);
            return true;
        }
        if (name == PropertyName.@_writeOnlyBackingField) {
            this.@_writeOnlyBackingField = global::Bradot.NativeInterop.VariantUtils.ConvertTo<bool>(value);
            return true;
        }
        return base.SetBradotClassPropertyValue(name, value);
    }
    /// <inheritdoc/>
    [global::System.ComponentModel.EditorBrowsable(global::System.ComponentModel.EditorBrowsableState.Never)]
    protected override bool GetBradotClassPropertyValue(in bradot_string_name name, out bradot_variant value)
    {
        if (name == PropertyName.@_writeOnlyBackingField) {
            value = global::Bradot.NativeInterop.VariantUtils.CreateFrom<bool>(this.@_writeOnlyBackingField);
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
        properties.Add(new(type: (global::Bradot.Variant.Type)1, name: PropertyName.@_writeOnlyBackingField, hint: (global::Bradot.PropertyHint)0, hintString: "", usage: (global::Bradot.PropertyUsageFlags)4096, exported: false));
        properties.Add(new(type: (global::Bradot.Variant.Type)1, name: PropertyName.@WriteOnlyProperty, hint: (global::Bradot.PropertyHint)0, hintString: "", usage: (global::Bradot.PropertyUsageFlags)4096, exported: false));
        return properties;
    }
#pragma warning restore CS0109
}
