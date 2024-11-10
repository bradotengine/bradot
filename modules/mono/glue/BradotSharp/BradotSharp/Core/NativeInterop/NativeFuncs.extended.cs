#pragma warning disable CA1707 // Identifiers should not contain underscores
#pragma warning disable IDE1006 // Naming rule violation
// ReSharper disable InconsistentNaming

namespace Bradot.NativeInterop
{
    public static partial class NativeFuncs
    {
        public static bradot_variant bradotsharp_variant_new_copy(in bradot_variant src)
        {
            switch (src.Type)
            {
                case Variant.Type.Nil:
                    return default;
                case Variant.Type.Bool:
                    return new bradot_variant() { Bool = src.Bool, Type = Variant.Type.Bool };
                case Variant.Type.Int:
                    return new bradot_variant() { Int = src.Int, Type = Variant.Type.Int };
                case Variant.Type.Float:
                    return new bradot_variant() { Float = src.Float, Type = Variant.Type.Float };
                case Variant.Type.Vector2:
                    return new bradot_variant() { Vector2 = src.Vector2, Type = Variant.Type.Vector2 };
                case Variant.Type.Vector2I:
                    return new bradot_variant() { Vector2I = src.Vector2I, Type = Variant.Type.Vector2I };
                case Variant.Type.Rect2:
                    return new bradot_variant() { Rect2 = src.Rect2, Type = Variant.Type.Rect2 };
                case Variant.Type.Rect2I:
                    return new bradot_variant() { Rect2I = src.Rect2I, Type = Variant.Type.Rect2I };
                case Variant.Type.Vector3:
                    return new bradot_variant() { Vector3 = src.Vector3, Type = Variant.Type.Vector3 };
                case Variant.Type.Vector3I:
                    return new bradot_variant() { Vector3I = src.Vector3I, Type = Variant.Type.Vector3I };
                case Variant.Type.Vector4:
                    return new bradot_variant() { Vector4 = src.Vector4, Type = Variant.Type.Vector4 };
                case Variant.Type.Vector4I:
                    return new bradot_variant() { Vector4I = src.Vector4I, Type = Variant.Type.Vector4I };
                case Variant.Type.Plane:
                    return new bradot_variant() { Plane = src.Plane, Type = Variant.Type.Plane };
                case Variant.Type.Quaternion:
                    return new bradot_variant() { Quaternion = src.Quaternion, Type = Variant.Type.Quaternion };
                case Variant.Type.Color:
                    return new bradot_variant() { Color = src.Color, Type = Variant.Type.Color };
                case Variant.Type.Rid:
                    return new bradot_variant() { Rid = src.Rid, Type = Variant.Type.Rid };
            }

            bradotsharp_variant_new_copy(out bradot_variant ret, src);
            return ret;
        }

        public static bradot_string_name bradotsharp_string_name_new_copy(in bradot_string_name src)
        {
            if (src.IsEmpty)
                return default;
            bradotsharp_string_name_new_copy(out bradot_string_name ret, src);
            return ret;
        }

        public static bradot_node_path bradotsharp_node_path_new_copy(in bradot_node_path src)
        {
            if (src.IsEmpty)
                return default;
            bradotsharp_node_path_new_copy(out bradot_node_path ret, src);
            return ret;
        }

        public static bradot_array bradotsharp_array_new()
        {
            bradotsharp_array_new(out bradot_array ret);
            return ret;
        }

        public static bradot_array bradotsharp_array_new_copy(in bradot_array src)
        {
            bradotsharp_array_new_copy(out bradot_array ret, src);
            return ret;
        }

        public static bradot_dictionary bradotsharp_dictionary_new()
        {
            bradotsharp_dictionary_new(out bradot_dictionary ret);
            return ret;
        }

        public static bradot_dictionary bradotsharp_dictionary_new_copy(in bradot_dictionary src)
        {
            bradotsharp_dictionary_new_copy(out bradot_dictionary ret, src);
            return ret;
        }

        public static bradot_string_name bradotsharp_string_name_new_from_string(string name)
        {
            using bradot_string src = Marshaling.ConvertStringToNative(name);
            bradotsharp_string_name_new_from_string(out bradot_string_name ret, src);
            return ret;
        }

        public static bradot_node_path bradotsharp_node_path_new_from_string(string name)
        {
            using bradot_string src = Marshaling.ConvertStringToNative(name);
            bradotsharp_node_path_new_from_string(out bradot_node_path ret, src);
            return ret;
        }
    }
}
