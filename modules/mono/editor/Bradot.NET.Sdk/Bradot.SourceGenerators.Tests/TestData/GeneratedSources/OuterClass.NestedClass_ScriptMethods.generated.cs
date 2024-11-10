using Bradot;
using Bradot.NativeInterop;

partial struct OuterClass
{
partial class NestedClass
{
#pragma warning disable CS0109 // Disable warning about redundant 'new' keyword
    /// <summary>
    /// Cached StringNames for the methods contained in this class, for fast lookup.
    /// </summary>
    public new class MethodName : global::Bradot.RefCounted.MethodName {
        /// <summary>
        /// Cached name for the '_Get' method.
        /// </summary>
        public new static readonly global::Bradot.StringName @_Get = "_Get";
    }
    /// <summary>
    /// Get the method information for all the methods declared in this class.
    /// This method is used by Bradot to register the available methods in the editor.
    /// Do not call this method.
    /// </summary>
    [global::System.ComponentModel.EditorBrowsable(global::System.ComponentModel.EditorBrowsableState.Never)]
    internal new static global::System.Collections.Generic.List<global::Bradot.Bridge.MethodInfo> GetBradotMethodList()
    {
        var methods = new global::System.Collections.Generic.List<global::Bradot.Bridge.MethodInfo>(1);
        methods.Add(new(name: MethodName.@_Get, returnVal: new(type: (global::Bradot.Variant.Type)0, name: "", hint: (global::Bradot.PropertyHint)0, hintString: "", usage: (global::Bradot.PropertyUsageFlags)131078, exported: false), flags: (global::Bradot.MethodFlags)1, arguments: new() { new(type: (global::Bradot.Variant.Type)21, name: "property", hint: (global::Bradot.PropertyHint)0, hintString: "", usage: (global::Bradot.PropertyUsageFlags)6, exported: false),  }, defaultArguments: null));
        return methods;
    }
#pragma warning restore CS0109
    /// <inheritdoc/>
    [global::System.ComponentModel.EditorBrowsable(global::System.ComponentModel.EditorBrowsableState.Never)]
    protected override bool InvokeBradotClassMethod(in bradot_string_name method, NativeVariantPtrArgs args, out bradot_variant ret)
    {
        if (method == MethodName.@_Get && args.Count == 1) {
            var callRet = @_Get(global::Bradot.NativeInterop.VariantUtils.ConvertTo<global::Bradot.StringName>(args[0]));
            ret = global::Bradot.NativeInterop.VariantUtils.CreateFrom<global::Bradot.Variant>(callRet);
            return true;
        }
        return base.InvokeBradotClassMethod(method, args, out ret);
    }
    /// <inheritdoc/>
    [global::System.ComponentModel.EditorBrowsable(global::System.ComponentModel.EditorBrowsableState.Never)]
    protected override bool HasBradotClassMethod(in bradot_string_name method)
    {
        if (method == MethodName.@_Get) {
           return true;
        }
        return base.HasBradotClassMethod(method);
    }
}
}
