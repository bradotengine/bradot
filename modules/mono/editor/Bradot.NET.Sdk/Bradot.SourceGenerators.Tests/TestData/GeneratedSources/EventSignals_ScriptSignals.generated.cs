using Bradot;
using Bradot.NativeInterop;

partial class EventSignals
{
#pragma warning disable CS0109 // Disable warning about redundant 'new' keyword
    /// <summary>
    /// Cached StringNames for the signals contained in this class, for fast lookup.
    /// </summary>
    public new class SignalName : global::Bradot.BradotObject.SignalName {
        /// <summary>
        /// Cached name for the 'MySignal' signal.
        /// </summary>
        public new static readonly global::Bradot.StringName @MySignal = "MySignal";
    }
    /// <summary>
    /// Get the signal information for all the signals declared in this class.
    /// This method is used by Bradot to register the available signals in the editor.
    /// Do not call this method.
    /// </summary>
    [global::System.ComponentModel.EditorBrowsable(global::System.ComponentModel.EditorBrowsableState.Never)]
    internal new static global::System.Collections.Generic.List<global::Bradot.Bridge.MethodInfo> GetBradotSignalList()
    {
        var signals = new global::System.Collections.Generic.List<global::Bradot.Bridge.MethodInfo>(1);
        signals.Add(new(name: SignalName.@MySignal, returnVal: new(type: (global::Bradot.Variant.Type)0, name: "", hint: (global::Bradot.PropertyHint)0, hintString: "", usage: (global::Bradot.PropertyUsageFlags)6, exported: false), flags: (global::Bradot.MethodFlags)1, arguments: new() { new(type: (global::Bradot.Variant.Type)4, name: "str", hint: (global::Bradot.PropertyHint)0, hintString: "", usage: (global::Bradot.PropertyUsageFlags)6, exported: false), new(type: (global::Bradot.Variant.Type)2, name: "num", hint: (global::Bradot.PropertyHint)0, hintString: "", usage: (global::Bradot.PropertyUsageFlags)6, exported: false),  }, defaultArguments: null));
        return signals;
    }
#pragma warning restore CS0109
    private global::EventSignals.MySignalEventHandler backing_MySignal;
    /// <inheritdoc cref="global::EventSignals.MySignalEventHandler"/>
    public event global::EventSignals.MySignalEventHandler @MySignal {
        add => backing_MySignal += value;
        remove => backing_MySignal -= value;
}
    protected void EmitSignalMySignal(string str, int num)
    {
        EmitSignal(SignalName.MySignal, str, num);
    }
    /// <inheritdoc/>
    [global::System.ComponentModel.EditorBrowsable(global::System.ComponentModel.EditorBrowsableState.Never)]
    protected override void RaiseBradotClassSignalCallbacks(in bradot_string_name signal, NativeVariantPtrArgs args)
    {
        if (signal == SignalName.@MySignal && args.Count == 2) {
            backing_MySignal?.Invoke(global::Bradot.NativeInterop.VariantUtils.ConvertTo<string>(args[0]), global::Bradot.NativeInterop.VariantUtils.ConvertTo<int>(args[1]));
            return;
        }
        base.RaiseBradotClassSignalCallbacks(signal, args);
    }
    /// <inheritdoc/>
    [global::System.ComponentModel.EditorBrowsable(global::System.ComponentModel.EditorBrowsableState.Never)]
    protected override bool HasBradotClassSignal(in bradot_string_name signal)
    {
        if (signal == SignalName.@MySignal) {
           return true;
        }
        return base.HasBradotClassSignal(signal);
    }
}
