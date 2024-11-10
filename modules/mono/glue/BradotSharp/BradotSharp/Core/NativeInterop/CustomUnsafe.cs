using System.Runtime.CompilerServices;

namespace Bradot.NativeInterop;

// Ref structs are not allowed as generic type parameters, so we can't use Unsafe.AsPointer<T>/AsRef<T>.
// As a workaround we create our own overloads for our structs with some tricks under the hood.

public static class CustomUnsafe
{
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static unsafe bradot_ref* AsPointer(ref bradot_ref value)
        => value.GetUnsafeAddress();

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static unsafe bradot_ref* ReadOnlyRefAsPointer(in bradot_ref value)
        => value.GetUnsafeAddress();

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static unsafe ref bradot_ref AsRef(bradot_ref* source)
        => ref *source;

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static unsafe ref bradot_ref AsRef(in bradot_ref source)
        => ref *ReadOnlyRefAsPointer(in source);

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static unsafe bradot_variant_call_error* AsPointer(ref bradot_variant_call_error value)
        => value.GetUnsafeAddress();

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static unsafe bradot_variant_call_error* ReadOnlyRefAsPointer(in bradot_variant_call_error value)
        => value.GetUnsafeAddress();

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static unsafe ref bradot_variant_call_error AsRef(bradot_variant_call_error* source)
        => ref *source;

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static unsafe ref bradot_variant_call_error AsRef(in bradot_variant_call_error source)
        => ref *ReadOnlyRefAsPointer(in source);

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static unsafe bradot_variant* AsPointer(ref bradot_variant value)
        => value.GetUnsafeAddress();

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static unsafe bradot_variant* ReadOnlyRefAsPointer(in bradot_variant value)
        => value.GetUnsafeAddress();

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static unsafe ref bradot_variant AsRef(bradot_variant* source)
        => ref *source;

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static unsafe ref bradot_variant AsRef(in bradot_variant source)
        => ref *ReadOnlyRefAsPointer(in source);

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static unsafe bradot_string* AsPointer(ref bradot_string value)
        => value.GetUnsafeAddress();

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static unsafe bradot_string* ReadOnlyRefAsPointer(in bradot_string value)
        => value.GetUnsafeAddress();

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static unsafe ref bradot_string AsRef(bradot_string* source)
        => ref *source;

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static unsafe ref bradot_string AsRef(in bradot_string source)
        => ref *ReadOnlyRefAsPointer(in source);

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static unsafe bradot_string_name* AsPointer(ref bradot_string_name value)
        => value.GetUnsafeAddress();

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static unsafe bradot_string_name* ReadOnlyRefAsPointer(in bradot_string_name value)
        => value.GetUnsafeAddress();

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static unsafe ref bradot_string_name AsRef(bradot_string_name* source)
        => ref *source;

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static unsafe ref bradot_string_name AsRef(in bradot_string_name source)
        => ref *ReadOnlyRefAsPointer(in source);

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static unsafe bradot_node_path* AsPointer(ref bradot_node_path value)
        => value.GetUnsafeAddress();

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static unsafe bradot_node_path* ReadOnlyRefAsPointer(in bradot_node_path value)
        => value.GetUnsafeAddress();

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static unsafe ref bradot_node_path AsRef(bradot_node_path* source)
        => ref *source;

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static unsafe ref bradot_node_path AsRef(in bradot_node_path source)
        => ref *ReadOnlyRefAsPointer(in source);

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static unsafe bradot_signal* AsPointer(ref bradot_signal value)
        => value.GetUnsafeAddress();

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static unsafe bradot_signal* ReadOnlyRefAsPointer(in bradot_signal value)
        => value.GetUnsafeAddress();

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static unsafe ref bradot_signal AsRef(bradot_signal* source)
        => ref *source;

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static unsafe ref bradot_signal AsRef(in bradot_signal source)
        => ref *ReadOnlyRefAsPointer(in source);

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static unsafe bradot_callable* AsPointer(ref bradot_callable value)
        => value.GetUnsafeAddress();

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static unsafe bradot_callable* ReadOnlyRefAsPointer(in bradot_callable value)
        => value.GetUnsafeAddress();

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static unsafe ref bradot_callable AsRef(bradot_callable* source)
        => ref *source;

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static unsafe ref bradot_callable AsRef(in bradot_callable source)
        => ref *ReadOnlyRefAsPointer(in source);

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static unsafe bradot_array* AsPointer(ref bradot_array value)
        => value.GetUnsafeAddress();

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static unsafe bradot_array* ReadOnlyRefAsPointer(in bradot_array value)
        => value.GetUnsafeAddress();

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static unsafe ref bradot_array AsRef(bradot_array* source)
        => ref *source;

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static unsafe ref bradot_array AsRef(in bradot_array source)
        => ref *ReadOnlyRefAsPointer(in source);

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static unsafe bradot_dictionary* AsPointer(ref bradot_dictionary value)
        => value.GetUnsafeAddress();

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static unsafe bradot_dictionary* ReadOnlyRefAsPointer(in bradot_dictionary value)
        => value.GetUnsafeAddress();

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static unsafe ref bradot_dictionary AsRef(bradot_dictionary* source)
        => ref *source;

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static unsafe ref bradot_dictionary AsRef(in bradot_dictionary source)
        => ref *ReadOnlyRefAsPointer(in source);

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static unsafe bradot_packed_byte_array* AsPointer(ref bradot_packed_byte_array value)
        => value.GetUnsafeAddress();

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static unsafe bradot_packed_byte_array* ReadOnlyRefAsPointer(in bradot_packed_byte_array value)
        => value.GetUnsafeAddress();

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static unsafe ref bradot_packed_byte_array AsRef(bradot_packed_byte_array* source)
        => ref *source;

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static unsafe ref bradot_packed_byte_array AsRef(in bradot_packed_byte_array source)
        => ref *ReadOnlyRefAsPointer(in source);

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static unsafe bradot_packed_int32_array* AsPointer(ref bradot_packed_int32_array value)
        => value.GetUnsafeAddress();

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static unsafe bradot_packed_int32_array* ReadOnlyRefAsPointer(in bradot_packed_int32_array value)
        => value.GetUnsafeAddress();

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static unsafe ref bradot_packed_int32_array AsRef(bradot_packed_int32_array* source)
        => ref *source;

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static unsafe ref bradot_packed_int32_array AsRef(in bradot_packed_int32_array source)
        => ref *ReadOnlyRefAsPointer(in source);

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static unsafe bradot_packed_int64_array* AsPointer(ref bradot_packed_int64_array value)
        => value.GetUnsafeAddress();

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static unsafe bradot_packed_int64_array* ReadOnlyRefAsPointer(in bradot_packed_int64_array value)
        => value.GetUnsafeAddress();

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static unsafe ref bradot_packed_int64_array AsRef(bradot_packed_int64_array* source)
        => ref *source;

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static unsafe ref bradot_packed_int64_array AsRef(in bradot_packed_int64_array source)
        => ref *ReadOnlyRefAsPointer(in source);

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static unsafe bradot_packed_float32_array* AsPointer(ref bradot_packed_float32_array value)
        => value.GetUnsafeAddress();

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static unsafe bradot_packed_float32_array* ReadOnlyRefAsPointer(in bradot_packed_float32_array value)
        => value.GetUnsafeAddress();

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static unsafe ref bradot_packed_float32_array AsRef(bradot_packed_float32_array* source)
        => ref *source;

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static unsafe ref bradot_packed_float32_array AsRef(in bradot_packed_float32_array source)
        => ref *ReadOnlyRefAsPointer(in source);

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static unsafe bradot_packed_float64_array* AsPointer(ref bradot_packed_float64_array value)
        => value.GetUnsafeAddress();

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static unsafe bradot_packed_float64_array* ReadOnlyRefAsPointer(in bradot_packed_float64_array value)
        => value.GetUnsafeAddress();

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static unsafe ref bradot_packed_float64_array AsRef(bradot_packed_float64_array* source)
        => ref *source;

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static unsafe ref bradot_packed_float64_array AsRef(in bradot_packed_float64_array source)
        => ref *ReadOnlyRefAsPointer(in source);

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static unsafe bradot_packed_string_array* AsPointer(ref bradot_packed_string_array value)
        => value.GetUnsafeAddress();

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static unsafe bradot_packed_string_array* ReadOnlyRefAsPointer(in bradot_packed_string_array value)
        => value.GetUnsafeAddress();

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static unsafe ref bradot_packed_string_array AsRef(bradot_packed_string_array* source)
        => ref *source;

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static unsafe ref bradot_packed_string_array AsRef(in bradot_packed_string_array source)
        => ref *ReadOnlyRefAsPointer(in source);

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static unsafe bradot_packed_vector2_array* AsPointer(ref bradot_packed_vector2_array value)
        => value.GetUnsafeAddress();

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static unsafe bradot_packed_vector2_array* ReadOnlyRefAsPointer(in bradot_packed_vector2_array value)
        => value.GetUnsafeAddress();

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static unsafe ref bradot_packed_vector2_array AsRef(bradot_packed_vector2_array* source)
        => ref *source;

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static unsafe ref bradot_packed_vector2_array AsRef(in bradot_packed_vector2_array source)
        => ref *ReadOnlyRefAsPointer(in source);

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static unsafe bradot_packed_vector3_array* AsPointer(ref bradot_packed_vector3_array value)
        => value.GetUnsafeAddress();

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static unsafe bradot_packed_vector3_array* ReadOnlyRefAsPointer(in bradot_packed_vector3_array value)
        => value.GetUnsafeAddress();

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static unsafe ref bradot_packed_vector3_array AsRef(bradot_packed_vector3_array* source)
        => ref *source;

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static unsafe ref bradot_packed_vector3_array AsRef(in bradot_packed_vector3_array source)
        => ref *ReadOnlyRefAsPointer(in source);

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static unsafe bradot_packed_vector4_array* AsPointer(ref bradot_packed_vector4_array value)
        => value.GetUnsafeAddress();

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static unsafe bradot_packed_vector4_array* ReadOnlyRefAsPointer(in bradot_packed_vector4_array value)
        => value.GetUnsafeAddress();

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static unsafe ref bradot_packed_vector4_array AsRef(bradot_packed_vector4_array* source)
        => ref *source;

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static unsafe ref bradot_packed_vector4_array AsRef(in bradot_packed_vector4_array source)
        => ref *ReadOnlyRefAsPointer(in source);

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static unsafe bradot_packed_color_array* AsPointer(ref bradot_packed_color_array value)
        => value.GetUnsafeAddress();

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static unsafe bradot_packed_color_array* ReadOnlyRefAsPointer(in bradot_packed_color_array value)
        => value.GetUnsafeAddress();

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static unsafe ref bradot_packed_color_array AsRef(bradot_packed_color_array* source)
        => ref *source;

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static unsafe ref bradot_packed_color_array AsRef(in bradot_packed_color_array source)
        => ref *ReadOnlyRefAsPointer(in source);
}
