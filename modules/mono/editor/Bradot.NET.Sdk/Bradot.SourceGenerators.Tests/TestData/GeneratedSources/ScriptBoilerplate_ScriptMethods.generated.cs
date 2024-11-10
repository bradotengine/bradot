using Bradot;
using Bradot.NativeInterop;

partial class ScriptBoilerplate
{
#pragma warning disable CS0109 // Disable warning about redundant 'new' keyword
    /// <summary>
    /// Cached StringNames for the methods contained in this class, for fast lookup.
    /// </summary>
    public new class MethodName : global::Bradot.Node.MethodName {
        /// <summary>
        /// Cached name for the '_Process' method.
        /// </summary>
        public new static readonly global::Bradot.StringName @_Process = "_Process";
        /// <summary>
        /// Cached name for the 'Bazz' method.
        /// </summary>
        public new static readonly global::Bradot.StringName @Bazz = "Bazz";
    }
    /// <summary>
    /// Get the method information for all the methods declared in this class.
    /// This method is used by Bradot to register the available methods in the editor.
    /// Do not call this method.
    /// </summary>
    [global::System.ComponentModel.EditorBrowsable(global::System.ComponentModel.EditorBrowsableState.Never)]
    internal new static global::System.Collections.Generic.List<global::Bradot.Bridge.MethodInfo> GetBradotMethodList()
    {
        var methods = new global::System.Collections.Generic.List<global::Bradot.Bridge.MethodInfo>(2);
        methods.Add(new(name: MethodName.@_Process, returnVal: new(type: (global::Bradot.Variant.Type)0, name: "", hint: (global::Bradot.PropertyHint)0, hintString: "", usage: (global::Bradot.PropertyUsageFlags)6, exported: false), flags: (global::Bradot.MethodFlags)1, arguments: new() { new(type: (global::Bradot.Variant.Type)3, name: "delta", hint: (global::Bradot.PropertyHint)0, hintString: "", usage: (global::Bradot.PropertyUsageFlags)6, exported: false),  }, defaultArguments: null));
        methods.Add(new(name: MethodName.@Bazz, returnVal: new(type: (global::Bradot.Variant.Type)2, name: "", hint: (global::Bradot.PropertyHint)0, hintString: "", usage: (global::Bradot.PropertyUsageFlags)6, exported: false), flags: (global::Bradot.MethodFlags)1, arguments: new() { new(type: (global::Bradot.Variant.Type)21, name: "name", hint: (global::Bradot.PropertyHint)0, hintString: "", usage: (global::Bradot.PropertyUsageFlags)6, exported: false),  }, defaultArguments: null));
        return methods;
    }
#pragma warning restore CS0109
    /// <inheritdoc/>
    [global::System.ComponentModel.EditorBrowsable(global::System.ComponentModel.EditorBrowsableState.Never)]
    protected override bool InvokeBradotClassMethod(in bradot_string_name method, NativeVariantPtrArgs args, out bradot_variant ret)
    {
        if (method == MethodName.@_Process && args.Count == 1) {
            @_Process(global::Bradot.NativeInterop.VariantUtils.ConvertTo<double>(args[0]));
            ret = default;
            return true;
        }
        if (method == MethodName.@Bazz && args.Count == 1) {
            var callRet = @Bazz(global::Bradot.NativeInterop.VariantUtils.ConvertTo<global::Bradot.StringName>(args[0]));
            ret = global::Bradot.NativeInterop.VariantUtils.CreateFrom<int>(callRet);
            return true;
        }
        return base.InvokeBradotClassMethod(method, args, out ret);
    }
    /// <inheritdoc/>
    [global::System.ComponentModel.EditorBrowsable(global::System.ComponentModel.EditorBrowsableState.Never)]
    protected override bool HasBradotClassMethod(in bradot_string_name method)
    {
        if (method == MethodName.@_Process) {
           return true;
        }
        if (method == MethodName.@Bazz) {
           return true;
        }
        return base.HasBradotClassMethod(method);
    }
}
