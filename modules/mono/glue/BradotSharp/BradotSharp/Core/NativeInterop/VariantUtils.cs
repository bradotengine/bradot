#pragma warning disable CA1707 // Identifiers should not contain underscores
#pragma warning disable IDE1006 // Naming rule violation
// ReSharper disable InconsistentNaming

using System;
using System.Runtime.CompilerServices;
using Bradot.Collections;


#nullable enable

namespace Bradot.NativeInterop
{
    public static partial class VariantUtils
    {
        public static bradot_variant CreateFromRid(Rid from)
            => new() { Type = Variant.Type.Rid, Rid = from };

        public static bradot_variant CreateFromBool(bool from)
            => new() { Type = Variant.Type.Bool, Bool = from.ToBradotBool() };

        public static bradot_variant CreateFromInt(long from)
            => new() { Type = Variant.Type.Int, Int = from };

        public static bradot_variant CreateFromInt(ulong from)
            => new() { Type = Variant.Type.Int, Int = (long)from };

        public static bradot_variant CreateFromFloat(double from)
            => new() { Type = Variant.Type.Float, Float = from };

        public static bradot_variant CreateFromVector2(Vector2 from)
            => new() { Type = Variant.Type.Vector2, Vector2 = from };

        public static bradot_variant CreateFromVector2I(Vector2I from)
            => new() { Type = Variant.Type.Vector2I, Vector2I = from };

        public static bradot_variant CreateFromVector3(Vector3 from)
            => new() { Type = Variant.Type.Vector3, Vector3 = from };

        public static bradot_variant CreateFromVector3I(Vector3I from)
            => new() { Type = Variant.Type.Vector3I, Vector3I = from };

        public static bradot_variant CreateFromVector4(Vector4 from)
            => new() { Type = Variant.Type.Vector4, Vector4 = from };

        public static bradot_variant CreateFromVector4I(Vector4I from)
            => new() { Type = Variant.Type.Vector4I, Vector4I = from };

        public static bradot_variant CreateFromRect2(Rect2 from)
            => new() { Type = Variant.Type.Rect2, Rect2 = from };

        public static bradot_variant CreateFromRect2I(Rect2I from)
            => new() { Type = Variant.Type.Rect2I, Rect2I = from };

        public static bradot_variant CreateFromQuaternion(Quaternion from)
            => new() { Type = Variant.Type.Quaternion, Quaternion = from };

        public static bradot_variant CreateFromColor(Color from)
            => new() { Type = Variant.Type.Color, Color = from };

        public static bradot_variant CreateFromPlane(Plane from)
            => new() { Type = Variant.Type.Plane, Plane = from };

        public static bradot_variant CreateFromTransform2D(Transform2D from)
        {
            NativeFuncs.bradotsharp_variant_new_transform2d(out bradot_variant ret, from);
            return ret;
        }

        public static bradot_variant CreateFromBasis(Basis from)
        {
            NativeFuncs.bradotsharp_variant_new_basis(out bradot_variant ret, from);
            return ret;
        }

        public static bradot_variant CreateFromTransform3D(Transform3D from)
        {
            NativeFuncs.bradotsharp_variant_new_transform3d(out bradot_variant ret, from);
            return ret;
        }

        public static bradot_variant CreateFromProjection(Projection from)
        {
            NativeFuncs.bradotsharp_variant_new_projection(out bradot_variant ret, from);
            return ret;
        }

        public static bradot_variant CreateFromAabb(Aabb from)
        {
            NativeFuncs.bradotsharp_variant_new_aabb(out bradot_variant ret, from);
            return ret;
        }

        // Explicit name to make it very clear
        public static bradot_variant CreateFromCallableTakingOwnershipOfDisposableValue(bradot_callable from)
            => new() { Type = Variant.Type.Callable, Callable = from };

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static bradot_variant CreateFromCallable(Callable from)
            => CreateFromCallableTakingOwnershipOfDisposableValue(
                Marshaling.ConvertCallableToNative(from));

        // Explicit name to make it very clear
        public static bradot_variant CreateFromSignalTakingOwnershipOfDisposableValue(bradot_signal from)
            => new() { Type = Variant.Type.Signal, Signal = from };

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static bradot_variant CreateFromSignal(Signal from)
            => CreateFromSignalTakingOwnershipOfDisposableValue(
                Marshaling.ConvertSignalToNative(from));

        // Explicit name to make it very clear
        public static bradot_variant CreateFromStringTakingOwnershipOfDisposableValue(bradot_string from)
            => new() { Type = Variant.Type.String, String = from };

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static bradot_variant CreateFromString(string? from)
            => CreateFromStringTakingOwnershipOfDisposableValue(Marshaling.ConvertStringToNative(from));

        public static bradot_variant CreateFromPackedByteArray(in bradot_packed_byte_array from)
        {
            NativeFuncs.bradotsharp_variant_new_packed_byte_array(out bradot_variant ret, from);
            return ret;
        }

        public static bradot_variant CreateFromPackedInt32Array(in bradot_packed_int32_array from)
        {
            NativeFuncs.bradotsharp_variant_new_packed_int32_array(out bradot_variant ret, from);
            return ret;
        }

        public static bradot_variant CreateFromPackedInt64Array(in bradot_packed_int64_array from)
        {
            NativeFuncs.bradotsharp_variant_new_packed_int64_array(out bradot_variant ret, from);
            return ret;
        }

        public static bradot_variant CreateFromPackedFloat32Array(in bradot_packed_float32_array from)
        {
            NativeFuncs.bradotsharp_variant_new_packed_float32_array(out bradot_variant ret, from);
            return ret;
        }

        public static bradot_variant CreateFromPackedFloat64Array(in bradot_packed_float64_array from)
        {
            NativeFuncs.bradotsharp_variant_new_packed_float64_array(out bradot_variant ret, from);
            return ret;
        }

        public static bradot_variant CreateFromPackedStringArray(in bradot_packed_string_array from)
        {
            NativeFuncs.bradotsharp_variant_new_packed_string_array(out bradot_variant ret, from);
            return ret;
        }

        public static bradot_variant CreateFromPackedVector2Array(in bradot_packed_vector2_array from)
        {
            NativeFuncs.bradotsharp_variant_new_packed_vector2_array(out bradot_variant ret, from);
            return ret;
        }

        public static bradot_variant CreateFromPackedVector3Array(in bradot_packed_vector3_array from)
        {
            NativeFuncs.bradotsharp_variant_new_packed_vector3_array(out bradot_variant ret, from);
            return ret;
        }

        public static bradot_variant CreateFromPackedVector4Array(in bradot_packed_vector4_array from)
        {
            NativeFuncs.bradotsharp_variant_new_packed_vector4_array(out bradot_variant ret, from);
            return ret;
        }

        public static bradot_variant CreateFromPackedColorArray(in bradot_packed_color_array from)
        {
            NativeFuncs.bradotsharp_variant_new_packed_color_array(out bradot_variant ret, from);
            return ret;
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static bradot_variant CreateFromPackedByteArray(Span<byte> from)
        {
            using var nativePackedArray = Marshaling.ConvertSystemArrayToNativePackedByteArray(from);
            return CreateFromPackedByteArray(nativePackedArray);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static bradot_variant CreateFromPackedInt32Array(Span<int> from)
        {
            using var nativePackedArray = Marshaling.ConvertSystemArrayToNativePackedInt32Array(from);
            return CreateFromPackedInt32Array(nativePackedArray);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static bradot_variant CreateFromPackedInt64Array(Span<long> from)
        {
            using var nativePackedArray = Marshaling.ConvertSystemArrayToNativePackedInt64Array(from);
            return CreateFromPackedInt64Array(nativePackedArray);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static bradot_variant CreateFromPackedFloat32Array(Span<float> from)
        {
            using var nativePackedArray = Marshaling.ConvertSystemArrayToNativePackedFloat32Array(from);
            return CreateFromPackedFloat32Array(nativePackedArray);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static bradot_variant CreateFromPackedFloat64Array(Span<double> from)
        {
            using var nativePackedArray = Marshaling.ConvertSystemArrayToNativePackedFloat64Array(from);
            return CreateFromPackedFloat64Array(nativePackedArray);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static bradot_variant CreateFromPackedStringArray(Span<string> from)
        {
            using var nativePackedArray = Marshaling.ConvertSystemArrayToNativePackedStringArray(from);
            return CreateFromPackedStringArray(nativePackedArray);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static bradot_variant CreateFromPackedVector2Array(Span<Vector2> from)
        {
            using var nativePackedArray = Marshaling.ConvertSystemArrayToNativePackedVector2Array(from);
            return CreateFromPackedVector2Array(nativePackedArray);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static bradot_variant CreateFromPackedVector3Array(Span<Vector3> from)
        {
            using var nativePackedArray = Marshaling.ConvertSystemArrayToNativePackedVector3Array(from);
            return CreateFromPackedVector3Array(nativePackedArray);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static bradot_variant CreateFromPackedVector4Array(Span<Vector4> from)
        {
            using var nativePackedArray = Marshaling.ConvertSystemArrayToNativePackedVector4Array(from);
            return CreateFromPackedVector4Array(nativePackedArray);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static bradot_variant CreateFromPackedColorArray(Span<Color> from)
        {
            using var nativePackedArray = Marshaling.ConvertSystemArrayToNativePackedColorArray(from);
            return CreateFromPackedColorArray(nativePackedArray);
        }

        public static bradot_variant CreateFromSystemArrayOfStringName(Span<StringName> from)
        {
            if (from == null)
                return default;
            using var fromGodot = new Collections.Array(from);
            return CreateFromArray((bradot_array)fromBradot.NativeValue);
        }

        public static bradot_variant CreateFromSystemArrayOfNodePath(Span<NodePath> from)
        {
            if (from == null)
                return default;
            using var fromGodot = new Collections.Array(from);
            return CreateFromArray((bradot_array)fromBradot.NativeValue);
        }

        public static bradot_variant CreateFromSystemArrayOfRid(Span<Rid> from)
        {
            if (from == null)
                return default;
            using var fromGodot = new Collections.Array(from);
            return CreateFromArray((bradot_array)fromBradot.NativeValue);
        }

        public static bradot_variant CreateFromSystemArrayOfBradotObject(BradotObject[]? from)
        {
            if (from == null)
                return default; // Nil
            using var fromGodot = new Collections.Array(from);
            return CreateFromArray((bradot_array)fromBradot.NativeValue);
        }

        public static bradot_variant CreateFromArray(bradot_array from)
        {
            NativeFuncs.bradotsharp_variant_new_array(out bradot_variant ret, from);
            return ret;
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static bradot_variant CreateFromArray(Collections.Array? from)
            => from != null ? CreateFromArray((bradot_array)from.NativeValue) : default;

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static bradot_variant CreateFromArray<[MustBeVariant] T>(Array<T>? from)
            => from != null ? CreateFromArray((bradot_array)((Collections.Array)from).NativeValue) : default;

        public static bradot_variant CreateFromDictionary(bradot_dictionary from)
        {
            NativeFuncs.bradotsharp_variant_new_dictionary(out bradot_variant ret, from);
            return ret;
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static bradot_variant CreateFromDictionary(Dictionary? from)
            => from != null ? CreateFromDictionary((bradot_dictionary)from.NativeValue) : default;

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static bradot_variant CreateFromDictionary<[MustBeVariant] TKey, [MustBeVariant] TValue>(Dictionary<TKey, TValue>? from)
            => from != null ? CreateFromDictionary((bradot_dictionary)((Dictionary)from).NativeValue) : default;

        public static bradot_variant CreateFromStringName(bradot_string_name from)
        {
            NativeFuncs.bradotsharp_variant_new_string_name(out bradot_variant ret, from);
            return ret;
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static bradot_variant CreateFromStringName(StringName? from)
            => from != null ? CreateFromStringName((bradot_string_name)from.NativeValue) : default;

        public static bradot_variant CreateFromNodePath(bradot_node_path from)
        {
            NativeFuncs.bradotsharp_variant_new_node_path(out bradot_variant ret, from);
            return ret;
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static bradot_variant CreateFromNodePath(NodePath? from)
            => from != null ? CreateFromNodePath((bradot_node_path)from.NativeValue) : default;

        public static bradot_variant CreateFromBradotObjectPtr(IntPtr from)
        {
            if (from == IntPtr.Zero)
                return new bradot_variant();
            NativeFuncs.bradotsharp_variant_new_object(out bradot_variant ret, from);
            return ret;
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static bradot_variant CreateFromBradotObject(BradotObject? from)
            => from != null ? CreateFromBradotObjectPtr(BradotObject.GetPtr(from)) : default;

        // We avoid the internal call if the stored type is the same we want.

        public static bool ConvertToBool(in bradot_variant p_var)
            => p_var.Type == Variant.Type.Bool ?
                p_var.Bool.ToBool() :
                NativeFuncs.bradotsharp_variant_as_bool(p_var).ToBool();

        public static char ConvertToChar(in bradot_variant p_var)
            => (char)(p_var.Type == Variant.Type.Int ?
                p_var.Int :
                NativeFuncs.bradotsharp_variant_as_int(p_var));

        public static sbyte ConvertToInt8(in bradot_variant p_var)
            => (sbyte)(p_var.Type == Variant.Type.Int ?
                p_var.Int :
                NativeFuncs.bradotsharp_variant_as_int(p_var));

        public static short ConvertToInt16(in bradot_variant p_var)
            => (short)(p_var.Type == Variant.Type.Int ?
                p_var.Int :
                NativeFuncs.bradotsharp_variant_as_int(p_var));

        public static int ConvertToInt32(in bradot_variant p_var)
            => (int)(p_var.Type == Variant.Type.Int ?
                p_var.Int :
                NativeFuncs.bradotsharp_variant_as_int(p_var));

        public static long ConvertToInt64(in bradot_variant p_var)
            => p_var.Type == Variant.Type.Int ? p_var.Int : NativeFuncs.bradotsharp_variant_as_int(p_var);

        public static byte ConvertToUInt8(in bradot_variant p_var)
            => (byte)(p_var.Type == Variant.Type.Int ?
                p_var.Int :
                NativeFuncs.bradotsharp_variant_as_int(p_var));

        public static ushort ConvertToUInt16(in bradot_variant p_var)
            => (ushort)(p_var.Type == Variant.Type.Int ?
                p_var.Int :
                NativeFuncs.bradotsharp_variant_as_int(p_var));

        public static uint ConvertToUInt32(in bradot_variant p_var)
            => (uint)(p_var.Type == Variant.Type.Int ?
                p_var.Int :
                NativeFuncs.bradotsharp_variant_as_int(p_var));

        public static ulong ConvertToUInt64(in bradot_variant p_var)
            => (ulong)(p_var.Type == Variant.Type.Int ?
                p_var.Int :
                NativeFuncs.bradotsharp_variant_as_int(p_var));

        public static float ConvertToFloat32(in bradot_variant p_var)
            => (float)(p_var.Type == Variant.Type.Float ?
                p_var.Float :
                NativeFuncs.bradotsharp_variant_as_float(p_var));

        public static double ConvertToFloat64(in bradot_variant p_var)
            => p_var.Type == Variant.Type.Float ?
                p_var.Float :
                NativeFuncs.bradotsharp_variant_as_float(p_var);

        public static Vector2 ConvertToVector2(in bradot_variant p_var)
            => p_var.Type == Variant.Type.Vector2 ?
                p_var.Vector2 :
                NativeFuncs.bradotsharp_variant_as_vector2(p_var);

        public static Vector2I ConvertToVector2I(in bradot_variant p_var)
            => p_var.Type == Variant.Type.Vector2I ?
                p_var.Vector2I :
                NativeFuncs.bradotsharp_variant_as_vector2i(p_var);

        public static Rect2 ConvertToRect2(in bradot_variant p_var)
            => p_var.Type == Variant.Type.Rect2 ?
                p_var.Rect2 :
                NativeFuncs.bradotsharp_variant_as_rect2(p_var);

        public static Rect2I ConvertToRect2I(in bradot_variant p_var)
            => p_var.Type == Variant.Type.Rect2I ?
                p_var.Rect2I :
                NativeFuncs.bradotsharp_variant_as_rect2i(p_var);

        public static unsafe Transform2D ConvertToTransform2D(in bradot_variant p_var)
            => p_var.Type == Variant.Type.Transform2D ?
                *p_var.Transform2D :
                NativeFuncs.bradotsharp_variant_as_transform2d(p_var);

        public static Vector3 ConvertToVector3(in bradot_variant p_var)
            => p_var.Type == Variant.Type.Vector3 ?
                p_var.Vector3 :
                NativeFuncs.bradotsharp_variant_as_vector3(p_var);

        public static Vector3I ConvertToVector3I(in bradot_variant p_var)
            => p_var.Type == Variant.Type.Vector3I ?
                p_var.Vector3I :
                NativeFuncs.bradotsharp_variant_as_vector3i(p_var);

        public static unsafe Vector4 ConvertToVector4(in bradot_variant p_var)
            => p_var.Type == Variant.Type.Vector4 ?
                p_var.Vector4 :
                NativeFuncs.bradotsharp_variant_as_vector4(p_var);

        public static unsafe Vector4I ConvertToVector4I(in bradot_variant p_var)
            => p_var.Type == Variant.Type.Vector4I ?
                p_var.Vector4I :
                NativeFuncs.bradotsharp_variant_as_vector4i(p_var);

        public static unsafe Basis ConvertToBasis(in bradot_variant p_var)
            => p_var.Type == Variant.Type.Basis ?
                *p_var.Basis :
                NativeFuncs.bradotsharp_variant_as_basis(p_var);

        public static Quaternion ConvertToQuaternion(in bradot_variant p_var)
            => p_var.Type == Variant.Type.Quaternion ?
                p_var.Quaternion :
                NativeFuncs.bradotsharp_variant_as_quaternion(p_var);

        public static unsafe Transform3D ConvertToTransform3D(in bradot_variant p_var)
            => p_var.Type == Variant.Type.Transform3D ?
                *p_var.Transform3D :
                NativeFuncs.bradotsharp_variant_as_transform3d(p_var);

        public static unsafe Projection ConvertToProjection(in bradot_variant p_var)
            => p_var.Type == Variant.Type.Projection ?
                *p_var.Projection :
                NativeFuncs.bradotsharp_variant_as_projection(p_var);

        public static unsafe Aabb ConvertToAabb(in bradot_variant p_var)
            => p_var.Type == Variant.Type.Aabb ?
                *p_var.Aabb :
                NativeFuncs.bradotsharp_variant_as_aabb(p_var);

        public static Color ConvertToColor(in bradot_variant p_var)
            => p_var.Type == Variant.Type.Color ?
                p_var.Color :
                NativeFuncs.bradotsharp_variant_as_color(p_var);

        public static Plane ConvertToPlane(in bradot_variant p_var)
            => p_var.Type == Variant.Type.Plane ?
                p_var.Plane :
                NativeFuncs.bradotsharp_variant_as_plane(p_var);

        public static Rid ConvertToRid(in bradot_variant p_var)
            => p_var.Type == Variant.Type.Rid ?
                p_var.Rid :
                NativeFuncs.bradotsharp_variant_as_rid(p_var);

        public static IntPtr ConvertToBradotObjectPtr(in bradot_variant p_var)
            => p_var.Type == Variant.Type.Object ? p_var.Object : IntPtr.Zero;

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static BradotObject ConvertToBradotObject(in bradot_variant p_var)
            => InteropUtils.UnmanagedGetManaged(ConvertToBradotObjectPtr(p_var));

        public static string ConvertToString(in bradot_variant p_var)
        {
            switch (p_var.Type)
            {
                case Variant.Type.Nil:
                    return ""; // Otherwise, Variant -> String would return the string "Null"
                case Variant.Type.String:
                {
                    // We avoid the internal call if the stored type is the same we want.
                    return Marshaling.ConvertStringToManaged(p_var.String);
                }
                default:
                {
                    using bradot_string godotString = NativeFuncs.bradotsharp_variant_as_string(p_var);
                    return Marshaling.ConvertStringToManaged(godotString);
                }
            }
        }

        public static bradot_string_name ConvertToNativeStringName(in bradot_variant p_var)
            => p_var.Type == Variant.Type.StringName ?
                NativeFuncs.bradotsharp_string_name_new_copy(p_var.StringName) :
                NativeFuncs.bradotsharp_variant_as_string_name(p_var);

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static StringName ConvertToStringName(in bradot_variant p_var)
            => StringName.CreateTakingOwnershipOfDisposableValue(ConvertToNativeStringName(p_var));

        public static bradot_node_path ConvertToNativeNodePath(in bradot_variant p_var)
            => p_var.Type == Variant.Type.NodePath ?
                NativeFuncs.bradotsharp_node_path_new_copy(p_var.NodePath) :
                NativeFuncs.bradotsharp_variant_as_node_path(p_var);

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static NodePath ConvertToNodePath(in bradot_variant p_var)
            => NodePath.CreateTakingOwnershipOfDisposableValue(ConvertToNativeNodePath(p_var));

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static bradot_callable ConvertToNativeCallable(in bradot_variant p_var)
            => NativeFuncs.bradotsharp_variant_as_callable(p_var);

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Callable ConvertToCallable(in bradot_variant p_var)
            => Marshaling.ConvertCallableToManaged(ConvertToNativeCallable(p_var));

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static bradot_signal ConvertToNativeSignal(in bradot_variant p_var)
            => NativeFuncs.bradotsharp_variant_as_signal(p_var);

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Signal ConvertToSignal(in bradot_variant p_var)
            => Marshaling.ConvertSignalToManaged(ConvertToNativeSignal(p_var));

        public static bradot_array ConvertToNativeArray(in bradot_variant p_var)
            => p_var.Type == Variant.Type.Array ?
                NativeFuncs.bradotsharp_array_new_copy(p_var.Array) :
                NativeFuncs.bradotsharp_variant_as_array(p_var);

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Collections.Array ConvertToArray(in bradot_variant p_var)
            => Collections.Array.CreateTakingOwnershipOfDisposableValue(ConvertToNativeArray(p_var));

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Array<T> ConvertToArray<[MustBeVariant] T>(in bradot_variant p_var)
            => Array<T>.CreateTakingOwnershipOfDisposableValue(ConvertToNativeArray(p_var));

        public static bradot_dictionary ConvertToNativeDictionary(in bradot_variant p_var)
            => p_var.Type == Variant.Type.Dictionary ?
                NativeFuncs.bradotsharp_dictionary_new_copy(p_var.Dictionary) :
                NativeFuncs.bradotsharp_variant_as_dictionary(p_var);

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Dictionary ConvertToDictionary(in bradot_variant p_var)
            => Dictionary.CreateTakingOwnershipOfDisposableValue(ConvertToNativeDictionary(p_var));

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static Dictionary<TKey, TValue> ConvertToDictionary<[MustBeVariant] TKey, [MustBeVariant] TValue>(in bradot_variant p_var)
            => Dictionary<TKey, TValue>.CreateTakingOwnershipOfDisposableValue(ConvertToNativeDictionary(p_var));

        public static byte[] ConvertAsPackedByteArrayToSystemArray(in bradot_variant p_var)
        {
            using var packedArray = NativeFuncs.bradotsharp_variant_as_packed_byte_array(p_var);
            return Marshaling.ConvertNativePackedByteArrayToSystemArray(packedArray);
        }

        public static int[] ConvertAsPackedInt32ArrayToSystemArray(in bradot_variant p_var)
        {
            using var packedArray = NativeFuncs.bradotsharp_variant_as_packed_int32_array(p_var);
            return Marshaling.ConvertNativePackedInt32ArrayToSystemArray(packedArray);
        }

        public static long[] ConvertAsPackedInt64ArrayToSystemArray(in bradot_variant p_var)
        {
            using var packedArray = NativeFuncs.bradotsharp_variant_as_packed_int64_array(p_var);
            return Marshaling.ConvertNativePackedInt64ArrayToSystemArray(packedArray);
        }

        public static float[] ConvertAsPackedFloat32ArrayToSystemArray(in bradot_variant p_var)
        {
            using var packedArray = NativeFuncs.bradotsharp_variant_as_packed_float32_array(p_var);
            return Marshaling.ConvertNativePackedFloat32ArrayToSystemArray(packedArray);
        }

        public static double[] ConvertAsPackedFloat64ArrayToSystemArray(in bradot_variant p_var)
        {
            using var packedArray = NativeFuncs.bradotsharp_variant_as_packed_float64_array(p_var);
            return Marshaling.ConvertNativePackedFloat64ArrayToSystemArray(packedArray);
        }

        public static string[] ConvertAsPackedStringArrayToSystemArray(in bradot_variant p_var)
        {
            using var packedArray = NativeFuncs.bradotsharp_variant_as_packed_string_array(p_var);
            return Marshaling.ConvertNativePackedStringArrayToSystemArray(packedArray);
        }

        public static Vector2[] ConvertAsPackedVector2ArrayToSystemArray(in bradot_variant p_var)
        {
            using var packedArray = NativeFuncs.bradotsharp_variant_as_packed_vector2_array(p_var);
            return Marshaling.ConvertNativePackedVector2ArrayToSystemArray(packedArray);
        }

        public static Vector3[] ConvertAsPackedVector3ArrayToSystemArray(in bradot_variant p_var)
        {
            using var packedArray = NativeFuncs.bradotsharp_variant_as_packed_vector3_array(p_var);
            return Marshaling.ConvertNativePackedVector3ArrayToSystemArray(packedArray);
        }

        public static Vector4[] ConvertAsPackedVector4ArrayToSystemArray(in bradot_variant p_var)
        {
            using var packedArray = NativeFuncs.bradotsharp_variant_as_packed_vector4_array(p_var);
            return Marshaling.ConvertNativePackedVector4ArrayToSystemArray(packedArray);
        }

        public static Color[] ConvertAsPackedColorArrayToSystemArray(in bradot_variant p_var)
        {
            using var packedArray = NativeFuncs.bradotsharp_variant_as_packed_color_array(p_var);
            return Marshaling.ConvertNativePackedColorArrayToSystemArray(packedArray);
        }

        public static StringName[] ConvertToSystemArrayOfStringName(in bradot_variant p_var)
        {
            using var godotArray = NativeFuncs.bradotsharp_variant_as_array(p_var);
            return Marshaling.ConvertNativeBradotArrayToSystemArrayOfStringName(godotArray);
        }

        public static NodePath[] ConvertToSystemArrayOfNodePath(in bradot_variant p_var)
        {
            using var godotArray = NativeFuncs.bradotsharp_variant_as_array(p_var);
            return Marshaling.ConvertNativeBradotArrayToSystemArrayOfNodePath(godotArray);
        }

        public static Rid[] ConvertToSystemArrayOfRid(in bradot_variant p_var)
        {
            using var godotArray = NativeFuncs.bradotsharp_variant_as_array(p_var);
            return Marshaling.ConvertNativeBradotArrayToSystemArrayOfRid(godotArray);
        }

        public static T[] ConvertToSystemArrayOfBradotObject<T>(in bradot_variant p_var)
            where T : BradotObject
        {
            using var godotArray = NativeFuncs.bradotsharp_variant_as_array(p_var);
            return Marshaling.ConvertNativeBradotArrayToSystemArrayOfBradotObjectType<T>(godotArray);
        }
    }
}
