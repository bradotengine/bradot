using Bradot;
using Bradot.NativeInterop;

partial class Methods
{
#pragma warning disable CS0109 // Disable warning about redundant 'new' keyword
    /// <summary>
    /// Cached StringNames for the methods contained in this class, for fast lookup.
    /// </summary>
    public new class MethodName : global::Bradot.BradotObject.MethodName {
        /// <summary>
        /// Cached name for the 'MethodWithOverload' method.
        /// </summary>
        public new static readonly global::Bradot.StringName @MethodWithOverload = "MethodWithOverload";
    }
    /// <summary>
    /// Get the method information for all the methods declared in this class.
    /// This method is used by Bradot to register the available methods in the editor.
    /// Do not call this method.
    /// </summary>
    [global::System.ComponentModel.EditorBrowsable(global::System.ComponentModel.EditorBrowsableState.Never)]
    internal new static global::System.Collections.Generic.List<global::Bradot.Bridge.MethodInfo> GetBradotMethodList()
    {
        var methods = new global::System.Collections.Generic.List<global::Bradot.Bridge.MethodInfo>(3);
        methods.Add(new(name: MethodName.@MethodWithOverload, returnVal: new(type: (global::Bradot.Variant.Type)0, name: "", hint: (global::Bradot.PropertyHint)0, hintString: "", usage: (global::Bradot.PropertyUsageFlags)6, exported: false), flags: (global::Bradot.MethodFlags)1, arguments: null, defaultArguments: null));
        methods.Add(new(name: MethodName.@MethodWithOverload, returnVal: new(type: (global::Bradot.Variant.Type)0, name: "", hint: (global::Bradot.PropertyHint)0, hintString: "", usage: (global::Bradot.PropertyUsageFlags)6, exported: false), flags: (global::Bradot.MethodFlags)1, arguments: new() { new(type: (global::Bradot.Variant.Type)2, name: "a", hint: (global::Bradot.PropertyHint)0, hintString: "", usage: (global::Bradot.PropertyUsageFlags)6, exported: false),  }, defaultArguments: null));
        methods.Add(new(name: MethodName.@MethodWithOverload, returnVal: new(type: (global::Bradot.Variant.Type)0, name: "", hint: (global::Bradot.PropertyHint)0, hintString: "", usage: (global::Bradot.PropertyUsageFlags)6, exported: false), flags: (global::Bradot.MethodFlags)1, arguments: new() { new(type: (global::Bradot.Variant.Type)2, name: "a", hint: (global::Bradot.PropertyHint)0, hintString: "", usage: (global::Bradot.PropertyUsageFlags)6, exported: false), new(type: (global::Bradot.Variant.Type)2, name: "b", hint: (global::Bradot.PropertyHint)0, hintString: "", usage: (global::Bradot.PropertyUsageFlags)6, exported: false),  }, defaultArguments: null));
        return methods;
    }
#pragma warning restore CS0109
    /// <inheritdoc/>
    [global::System.ComponentModel.EditorBrowsable(global::System.ComponentModel.EditorBrowsableState.Never)]
    protected override bool InvokeBradotClassMethod(in bradot_string_name method, NativeVariantPtrArgs args, out bradot_variant ret)
    {
        if (method == MethodName.@MethodWithOverload && args.Count == 0) {
            @MethodWithOverload();
            ret = default;
            return true;
        }
        if (method == MethodName.@MethodWithOverload && args.Count == 1) {
            @MethodWithOverload(global::Bradot.NativeInterop.VariantUtils.ConvertTo<int>(args[0]));
            ret = default;
            return true;
        }
        if (method == MethodName.@MethodWithOverload && args.Count == 2) {
            @MethodWithOverload(global::Bradot.NativeInterop.VariantUtils.ConvertTo<int>(args[0]), global::Bradot.NativeInterop.VariantUtils.ConvertTo<int>(args[1]));
            ret = default;
            return true;
        }
        return base.InvokeBradotClassMethod(method, args, out ret);
    }
    /// <inheritdoc/>
    [global::System.ComponentModel.EditorBrowsable(global::System.ComponentModel.EditorBrowsableState.Never)]
    protected override bool HasBradotClassMethod(in bradot_string_name method)
    {
        if (method == MethodName.@MethodWithOverload) {
           return true;
        }
        return base.HasBradotClassMethod(method);
    }
}
