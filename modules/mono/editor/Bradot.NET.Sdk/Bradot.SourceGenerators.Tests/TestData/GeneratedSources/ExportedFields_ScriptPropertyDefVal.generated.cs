partial class ExportedFields
{
#pragma warning disable CS0109 // Disable warning about redundant 'new' keyword
#if TOOLS
    /// <summary>
    /// Get the default values for all properties declared in this class.
    /// This method is used by Bradot to determine the value that will be
    /// used by the inspector when resetting properties.
    /// Do not call this method.
    /// </summary>
    [global::System.ComponentModel.EditorBrowsable(global::System.ComponentModel.EditorBrowsableState.Never)]
    internal new static global::System.Collections.Generic.Dictionary<global::Bradot.StringName, global::Bradot.Variant> GetBradotPropertyDefaultValues()
    {
        var values = new global::System.Collections.Generic.Dictionary<global::Bradot.StringName, global::Bradot.Variant>(60);
        bool ___fieldBoolean_default_value = true;
        values.Add(PropertyName.@_fieldBoolean, global::Bradot.Variant.From<bool>(___fieldBoolean_default_value));
        char ___fieldChar_default_value = 'f';
        values.Add(PropertyName.@_fieldChar, global::Bradot.Variant.From<char>(___fieldChar_default_value));
        sbyte ___fieldSByte_default_value = 10;
        values.Add(PropertyName.@_fieldSByte, global::Bradot.Variant.From<sbyte>(___fieldSByte_default_value));
        short ___fieldInt16_default_value = 10;
        values.Add(PropertyName.@_fieldInt16, global::Bradot.Variant.From<short>(___fieldInt16_default_value));
        int ___fieldInt32_default_value = 10;
        values.Add(PropertyName.@_fieldInt32, global::Bradot.Variant.From<int>(___fieldInt32_default_value));
        long ___fieldInt64_default_value = 10;
        values.Add(PropertyName.@_fieldInt64, global::Bradot.Variant.From<long>(___fieldInt64_default_value));
        byte ___fieldByte_default_value = 10;
        values.Add(PropertyName.@_fieldByte, global::Bradot.Variant.From<byte>(___fieldByte_default_value));
        ushort ___fieldUInt16_default_value = 10;
        values.Add(PropertyName.@_fieldUInt16, global::Bradot.Variant.From<ushort>(___fieldUInt16_default_value));
        uint ___fieldUInt32_default_value = 10;
        values.Add(PropertyName.@_fieldUInt32, global::Bradot.Variant.From<uint>(___fieldUInt32_default_value));
        ulong ___fieldUInt64_default_value = 10;
        values.Add(PropertyName.@_fieldUInt64, global::Bradot.Variant.From<ulong>(___fieldUInt64_default_value));
        float ___fieldSingle_default_value = 10;
        values.Add(PropertyName.@_fieldSingle, global::Bradot.Variant.From<float>(___fieldSingle_default_value));
        double ___fieldDouble_default_value = 10;
        values.Add(PropertyName.@_fieldDouble, global::Bradot.Variant.From<double>(___fieldDouble_default_value));
        string ___fieldString_default_value = "foo";
        values.Add(PropertyName.@_fieldString, global::Bradot.Variant.From<string>(___fieldString_default_value));
        global::Bradot.Vector2 ___fieldVector2_default_value = new(10f, 10f);
        values.Add(PropertyName.@_fieldVector2, global::Bradot.Variant.From<global::Bradot.Vector2>(___fieldVector2_default_value));
        global::Bradot.Vector2I ___fieldVector2I_default_value = global::Bradot.Vector2I.Up;
        values.Add(PropertyName.@_fieldVector2I, global::Bradot.Variant.From<global::Bradot.Vector2I>(___fieldVector2I_default_value));
        global::Bradot.Rect2 ___fieldRect2_default_value = new(new global::Bradot.Vector2(10f, 10f), new global::Bradot.Vector2(10f, 10f));
        values.Add(PropertyName.@_fieldRect2, global::Bradot.Variant.From<global::Bradot.Rect2>(___fieldRect2_default_value));
        global::Bradot.Rect2I ___fieldRect2I_default_value = new(new global::Bradot.Vector2I(10, 10), new global::Bradot.Vector2I(10, 10));
        values.Add(PropertyName.@_fieldRect2I, global::Bradot.Variant.From<global::Bradot.Rect2I>(___fieldRect2I_default_value));
        global::Bradot.Transform2D ___fieldTransform2D_default_value = global::Bradot.Transform2D.Identity;
        values.Add(PropertyName.@_fieldTransform2D, global::Bradot.Variant.From<global::Bradot.Transform2D>(___fieldTransform2D_default_value));
        global::Bradot.Vector3 ___fieldVector3_default_value = new(10f, 10f, 10f);
        values.Add(PropertyName.@_fieldVector3, global::Bradot.Variant.From<global::Bradot.Vector3>(___fieldVector3_default_value));
        global::Bradot.Vector3I ___fieldVector3I_default_value = global::Bradot.Vector3I.Back;
        values.Add(PropertyName.@_fieldVector3I, global::Bradot.Variant.From<global::Bradot.Vector3I>(___fieldVector3I_default_value));
        global::Bradot.Basis ___fieldBasis_default_value = new global::Bradot.Basis(global::Bradot.Quaternion.Identity);
        values.Add(PropertyName.@_fieldBasis, global::Bradot.Variant.From<global::Bradot.Basis>(___fieldBasis_default_value));
        global::Bradot.Quaternion ___fieldQuaternion_default_value = new global::Bradot.Quaternion(global::Bradot.Basis.Identity);
        values.Add(PropertyName.@_fieldQuaternion, global::Bradot.Variant.From<global::Bradot.Quaternion>(___fieldQuaternion_default_value));
        global::Bradot.Transform3D ___fieldTransform3D_default_value = global::Bradot.Transform3D.Identity;
        values.Add(PropertyName.@_fieldTransform3D, global::Bradot.Variant.From<global::Bradot.Transform3D>(___fieldTransform3D_default_value));
        global::Bradot.Vector4 ___fieldVector4_default_value = new(10f, 10f, 10f, 10f);
        values.Add(PropertyName.@_fieldVector4, global::Bradot.Variant.From<global::Bradot.Vector4>(___fieldVector4_default_value));
        global::Bradot.Vector4I ___fieldVector4I_default_value = global::Bradot.Vector4I.One;
        values.Add(PropertyName.@_fieldVector4I, global::Bradot.Variant.From<global::Bradot.Vector4I>(___fieldVector4I_default_value));
        global::Bradot.Projection ___fieldProjection_default_value = global::Bradot.Projection.Identity;
        values.Add(PropertyName.@_fieldProjection, global::Bradot.Variant.From<global::Bradot.Projection>(___fieldProjection_default_value));
        global::Bradot.Aabb ___fieldAabb_default_value = new global::Bradot.Aabb(10f, 10f, 10f, new global::Bradot.Vector3(1f, 1f, 1f));
        values.Add(PropertyName.@_fieldAabb, global::Bradot.Variant.From<global::Bradot.Aabb>(___fieldAabb_default_value));
        global::Bradot.Color ___fieldColor_default_value = global::Bradot.Colors.Aquamarine;
        values.Add(PropertyName.@_fieldColor, global::Bradot.Variant.From<global::Bradot.Color>(___fieldColor_default_value));
        global::Bradot.Plane ___fieldPlane_default_value = global::Bradot.Plane.PlaneXZ;
        values.Add(PropertyName.@_fieldPlane, global::Bradot.Variant.From<global::Bradot.Plane>(___fieldPlane_default_value));
        global::Bradot.Callable ___fieldCallable_default_value = new global::Bradot.Callable(global::Bradot.Engine.GetMainLoop(), "_process");
        values.Add(PropertyName.@_fieldCallable, global::Bradot.Variant.From<global::Bradot.Callable>(___fieldCallable_default_value));
        global::Bradot.Signal ___fieldSignal_default_value = new global::Bradot.Signal(global::Bradot.Engine.GetMainLoop(), "property_list_changed");
        values.Add(PropertyName.@_fieldSignal, global::Bradot.Variant.From<global::Bradot.Signal>(___fieldSignal_default_value));
        global::ExportedFields.MyEnum ___fieldEnum_default_value = global::ExportedFields.MyEnum.C;
        values.Add(PropertyName.@_fieldEnum, global::Bradot.Variant.From<global::ExportedFields.MyEnum>(___fieldEnum_default_value));
        global::ExportedFields.MyFlagsEnum ___fieldFlagsEnum_default_value = global::ExportedFields.MyFlagsEnum.C;
        values.Add(PropertyName.@_fieldFlagsEnum, global::Bradot.Variant.From<global::ExportedFields.MyFlagsEnum>(___fieldFlagsEnum_default_value));
        byte[] ___fieldByteArray_default_value = { 0, 1, 2, 3, 4, 5, 6  };
        values.Add(PropertyName.@_fieldByteArray, global::Bradot.Variant.From<byte[]>(___fieldByteArray_default_value));
        int[] ___fieldInt32Array_default_value = { 0, 1, 2, 3, 4, 5, 6  };
        values.Add(PropertyName.@_fieldInt32Array, global::Bradot.Variant.From<int[]>(___fieldInt32Array_default_value));
        long[] ___fieldInt64Array_default_value = { 0, 1, 2, 3, 4, 5, 6  };
        values.Add(PropertyName.@_fieldInt64Array, global::Bradot.Variant.From<long[]>(___fieldInt64Array_default_value));
        float[] ___fieldSingleArray_default_value = { 0f, 1f, 2f, 3f, 4f, 5f, 6f  };
        values.Add(PropertyName.@_fieldSingleArray, global::Bradot.Variant.From<float[]>(___fieldSingleArray_default_value));
        double[] ___fieldDoubleArray_default_value = { 0d, 1d, 2d, 3d, 4d, 5d, 6d  };
        values.Add(PropertyName.@_fieldDoubleArray, global::Bradot.Variant.From<double[]>(___fieldDoubleArray_default_value));
        string[] ___fieldStringArray_default_value = { "foo", "bar"  };
        values.Add(PropertyName.@_fieldStringArray, global::Bradot.Variant.From<string[]>(___fieldStringArray_default_value));
        string[] ___fieldStringArrayEnum_default_value = { "foo", "bar"  };
        values.Add(PropertyName.@_fieldStringArrayEnum, global::Bradot.Variant.From<string[]>(___fieldStringArrayEnum_default_value));
        global::Bradot.Vector2[] ___fieldVector2Array_default_value = { global::Bradot.Vector2.Up, global::Bradot.Vector2.Down, global::Bradot.Vector2.Left, global::Bradot.Vector2.Right   };
        values.Add(PropertyName.@_fieldVector2Array, global::Bradot.Variant.From<global::Bradot.Vector2[]>(___fieldVector2Array_default_value));
        global::Bradot.Vector3[] ___fieldVector3Array_default_value = { global::Bradot.Vector3.Up, global::Bradot.Vector3.Down, global::Bradot.Vector3.Left, global::Bradot.Vector3.Right   };
        values.Add(PropertyName.@_fieldVector3Array, global::Bradot.Variant.From<global::Bradot.Vector3[]>(___fieldVector3Array_default_value));
        global::Bradot.Color[] ___fieldColorArray_default_value = { global::Bradot.Colors.Aqua, global::Bradot.Colors.Aquamarine, global::Bradot.Colors.Azure, global::Bradot.Colors.Beige   };
        values.Add(PropertyName.@_fieldColorArray, global::Bradot.Variant.From<global::Bradot.Color[]>(___fieldColorArray_default_value));
        global::Bradot.BradotObject[] ___fieldBradotObjectOrDerivedArray_default_value = { null  };
        values.Add(PropertyName.@_fieldBradotObjectOrDerivedArray, global::Bradot.Variant.CreateFrom(___fieldBradotObjectOrDerivedArray_default_value));
        global::Bradot.StringName[] ___fieldStringNameArray_default_value = { "foo", "bar"  };
        values.Add(PropertyName.@_fieldStringNameArray, global::Bradot.Variant.From<global::Bradot.StringName[]>(___fieldStringNameArray_default_value));
        global::Bradot.NodePath[] ___fieldNodePathArray_default_value = { "foo", "bar"  };
        values.Add(PropertyName.@_fieldNodePathArray, global::Bradot.Variant.From<global::Bradot.NodePath[]>(___fieldNodePathArray_default_value));
        global::Bradot.Rid[] ___fieldRidArray_default_value = { default, default, default  };
        values.Add(PropertyName.@_fieldRidArray, global::Bradot.Variant.From<global::Bradot.Rid[]>(___fieldRidArray_default_value));
        int[] ___fieldEmptyInt32Array_default_value = global::System.Array.Empty<int>();
        values.Add(PropertyName.@_fieldEmptyInt32Array, global::Bradot.Variant.From<int[]>(___fieldEmptyInt32Array_default_value));
        int[] ___fieldArrayFromList_default_value = new global::System.Collections.Generic.List<int>(global::System.Array.Empty<int>()).ToArray();
        values.Add(PropertyName.@_fieldArrayFromList, global::Bradot.Variant.From<int[]>(___fieldArrayFromList_default_value));
        global::Bradot.Variant ___fieldVariant_default_value = "foo";
        values.Add(PropertyName.@_fieldVariant, global::Bradot.Variant.From<global::Bradot.Variant>(___fieldVariant_default_value));
        global::Bradot.BradotObject ___fieldBradotObjectOrDerived_default_value = default;
        values.Add(PropertyName.@_fieldBradotObjectOrDerived, global::Bradot.Variant.From<global::Bradot.BradotObject>(___fieldBradotObjectOrDerived_default_value));
        global::Bradot.Texture ___fieldBradotResourceTexture_default_value = default;
        values.Add(PropertyName.@_fieldBradotResourceTexture, global::Bradot.Variant.From<global::Bradot.Texture>(___fieldBradotResourceTexture_default_value));
        global::Bradot.StringName ___fieldStringName_default_value = new global::Bradot.StringName("foo");
        values.Add(PropertyName.@_fieldStringName, global::Bradot.Variant.From<global::Bradot.StringName>(___fieldStringName_default_value));
        global::Bradot.NodePath ___fieldNodePath_default_value = new global::Bradot.NodePath("foo");
        values.Add(PropertyName.@_fieldNodePath, global::Bradot.Variant.From<global::Bradot.NodePath>(___fieldNodePath_default_value));
        global::Bradot.Rid ___fieldRid_default_value = default;
        values.Add(PropertyName.@_fieldRid, global::Bradot.Variant.From<global::Bradot.Rid>(___fieldRid_default_value));
        global::Bradot.Collections.Dictionary ___fieldBradotDictionary_default_value = new()  { { "foo", 10  }, { global::Bradot.Vector2.Up, global::Bradot.Colors.Chocolate   }  };
        values.Add(PropertyName.@_fieldBradotDictionary, global::Bradot.Variant.From<global::Bradot.Collections.Dictionary>(___fieldBradotDictionary_default_value));
        global::Bradot.Collections.Array ___fieldBradotArray_default_value = new()  { "foo", 10, global::Bradot.Vector2.Up, global::Bradot.Colors.Chocolate   };
        values.Add(PropertyName.@_fieldBradotArray, global::Bradot.Variant.From<global::Bradot.Collections.Array>(___fieldBradotArray_default_value));
        global::Bradot.Collections.Dictionary<string, bool> ___fieldBradotGenericDictionary_default_value = new()  { { "foo", true  }, { "bar", false  }  };
        values.Add(PropertyName.@_fieldBradotGenericDictionary, global::Bradot.Variant.CreateFrom(___fieldBradotGenericDictionary_default_value));
        global::Bradot.Collections.Array<int> ___fieldBradotGenericArray_default_value = new()  { 0, 1, 2, 3, 4, 5, 6  };
        values.Add(PropertyName.@_fieldBradotGenericArray, global::Bradot.Variant.CreateFrom(___fieldBradotGenericArray_default_value));
        long[] ___fieldEmptyInt64Array_default_value = global::System.Array.Empty<long>();
        values.Add(PropertyName.@_fieldEmptyInt64Array, global::Bradot.Variant.From<long[]>(___fieldEmptyInt64Array_default_value));
        return values;
    }
#endif // TOOLS
#pragma warning restore CS0109
}
