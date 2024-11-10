using Bradot;
using Bradot.NativeInterop;

partial class AbstractGenericNode<T>
{
#pragma warning disable CS0109 // Disable warning about redundant 'new' keyword
    /// <summary>
    /// Cached StringNames for the properties and fields contained in this class, for fast lookup.
    /// </summary>
    public new class PropertyName : global::Bradot.Node.PropertyName {
        /// <summary>
        /// Cached name for the 'MyArray' property.
        /// </summary>
        public new static readonly global::Bradot.StringName @MyArray = "MyArray";
    }
    /// <inheritdoc/>
    [global::System.ComponentModel.EditorBrowsable(global::System.ComponentModel.EditorBrowsableState.Never)]
    protected override bool SetBradotClassPropertyValue(in bradot_string_name name, in bradot_variant value)
    {
        if (name == PropertyName.@MyArray) {
            this.@MyArray = global::Bradot.NativeInterop.VariantUtils.ConvertToArray<T>(value);
            return true;
        }
        return base.SetBradotClassPropertyValue(name, value);
    }
    /// <inheritdoc/>
    [global::System.ComponentModel.EditorBrowsable(global::System.ComponentModel.EditorBrowsableState.Never)]
    protected override bool GetBradotClassPropertyValue(in bradot_string_name name, out bradot_variant value)
    {
        if (name == PropertyName.@MyArray) {
            value = global::Bradot.NativeInterop.VariantUtils.CreateFromArray(this.@MyArray);
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
        properties.Add(new(type: (global::Bradot.Variant.Type)28, name: PropertyName.@MyArray, hint: (global::Bradot.PropertyHint)0, hintString: "", usage: (global::Bradot.PropertyUsageFlags)4102, exported: true));
        return properties;
    }
#pragma warning restore CS0109
}
