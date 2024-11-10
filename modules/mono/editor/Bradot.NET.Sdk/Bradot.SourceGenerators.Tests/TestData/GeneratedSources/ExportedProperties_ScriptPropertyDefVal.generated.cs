partial class ExportedProperties
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
        var values = new global::System.Collections.Generic.Dictionary<global::Bradot.StringName, global::Bradot.Variant>(64);
        string __NotGenerateComplexLamdaProperty_default_value = default;
        values.Add(PropertyName.@NotGenerateComplexLamdaProperty, global::Bradot.Variant.From<string>(__NotGenerateComplexLamdaProperty_default_value));
        string __NotGenerateLamdaNoFieldProperty_default_value = default;
        values.Add(PropertyName.@NotGenerateLamdaNoFieldProperty, global::Bradot.Variant.From<string>(__NotGenerateLamdaNoFieldProperty_default_value));
        string __NotGenerateComplexReturnProperty_default_value = default;
        values.Add(PropertyName.@NotGenerateComplexReturnProperty, global::Bradot.Variant.From<string>(__NotGenerateComplexReturnProperty_default_value));
        string __NotGenerateReturnsProperty_default_value = default;
        values.Add(PropertyName.@NotGenerateReturnsProperty, global::Bradot.Variant.From<string>(__NotGenerateReturnsProperty_default_value));
        string __FullPropertyString_default_value = "FullPropertyString";
        values.Add(PropertyName.@FullPropertyString, global::Bradot.Variant.From<string>(__FullPropertyString_default_value));
        string __FullPropertyString_Complex_default_value = new string("FullPropertyString_Complex")   + global::System.Convert.ToInt32("1");
        values.Add(PropertyName.@FullPropertyString_Complex, global::Bradot.Variant.From<string>(__FullPropertyString_Complex_default_value));
        string __LamdaPropertyString_default_value = "LamdaPropertyString";
        values.Add(PropertyName.@LamdaPropertyString, global::Bradot.Variant.From<string>(__LamdaPropertyString_default_value));
        bool __PropertyBoolean_default_value = true;
        values.Add(PropertyName.@PropertyBoolean, global::Bradot.Variant.From<bool>(__PropertyBoolean_default_value));
        char __PropertyChar_default_value = 'f';
        values.Add(PropertyName.@PropertyChar, global::Bradot.Variant.From<char>(__PropertyChar_default_value));
        sbyte __PropertySByte_default_value = 10;
        values.Add(PropertyName.@PropertySByte, global::Bradot.Variant.From<sbyte>(__PropertySByte_default_value));
        short __PropertyInt16_default_value = 10;
        values.Add(PropertyName.@PropertyInt16, global::Bradot.Variant.From<short>(__PropertyInt16_default_value));
        int __PropertyInt32_default_value = 10;
        values.Add(PropertyName.@PropertyInt32, global::Bradot.Variant.From<int>(__PropertyInt32_default_value));
        long __PropertyInt64_default_value = 10;
        values.Add(PropertyName.@PropertyInt64, global::Bradot.Variant.From<long>(__PropertyInt64_default_value));
        byte __PropertyByte_default_value = 10;
        values.Add(PropertyName.@PropertyByte, global::Bradot.Variant.From<byte>(__PropertyByte_default_value));
        ushort __PropertyUInt16_default_value = 10;
        values.Add(PropertyName.@PropertyUInt16, global::Bradot.Variant.From<ushort>(__PropertyUInt16_default_value));
        uint __PropertyUInt32_default_value = 10;
        values.Add(PropertyName.@PropertyUInt32, global::Bradot.Variant.From<uint>(__PropertyUInt32_default_value));
        ulong __PropertyUInt64_default_value = 10;
        values.Add(PropertyName.@PropertyUInt64, global::Bradot.Variant.From<ulong>(__PropertyUInt64_default_value));
        float __PropertySingle_default_value = 10;
        values.Add(PropertyName.@PropertySingle, global::Bradot.Variant.From<float>(__PropertySingle_default_value));
        double __PropertyDouble_default_value = 10;
        values.Add(PropertyName.@PropertyDouble, global::Bradot.Variant.From<double>(__PropertyDouble_default_value));
        string __PropertyString_default_value = "foo";
        values.Add(PropertyName.@PropertyString, global::Bradot.Variant.From<string>(__PropertyString_default_value));
        global::Bradot.Vector2 __PropertyVector2_default_value = new(10f, 10f);
        values.Add(PropertyName.@PropertyVector2, global::Bradot.Variant.From<global::Bradot.Vector2>(__PropertyVector2_default_value));
        global::Bradot.Vector2I __PropertyVector2I_default_value = global::Bradot.Vector2I.Up;
        values.Add(PropertyName.@PropertyVector2I, global::Bradot.Variant.From<global::Bradot.Vector2I>(__PropertyVector2I_default_value));
        global::Bradot.Rect2 __PropertyRect2_default_value = new(new global::Bradot.Vector2(10f, 10f), new global::Bradot.Vector2(10f, 10f));
        values.Add(PropertyName.@PropertyRect2, global::Bradot.Variant.From<global::Bradot.Rect2>(__PropertyRect2_default_value));
        global::Bradot.Rect2I __PropertyRect2I_default_value = new(new global::Bradot.Vector2I(10, 10), new global::Bradot.Vector2I(10, 10));
        values.Add(PropertyName.@PropertyRect2I, global::Bradot.Variant.From<global::Bradot.Rect2I>(__PropertyRect2I_default_value));
        global::Bradot.Transform2D __PropertyTransform2D_default_value = global::Bradot.Transform2D.Identity;
        values.Add(PropertyName.@PropertyTransform2D, global::Bradot.Variant.From<global::Bradot.Transform2D>(__PropertyTransform2D_default_value));
        global::Bradot.Vector3 __PropertyVector3_default_value = new(10f, 10f, 10f);
        values.Add(PropertyName.@PropertyVector3, global::Bradot.Variant.From<global::Bradot.Vector3>(__PropertyVector3_default_value));
        global::Bradot.Vector3I __PropertyVector3I_default_value = global::Bradot.Vector3I.Back;
        values.Add(PropertyName.@PropertyVector3I, global::Bradot.Variant.From<global::Bradot.Vector3I>(__PropertyVector3I_default_value));
        global::Bradot.Basis __PropertyBasis_default_value = new global::Bradot.Basis(global::Bradot.Quaternion.Identity);
        values.Add(PropertyName.@PropertyBasis, global::Bradot.Variant.From<global::Bradot.Basis>(__PropertyBasis_default_value));
        global::Bradot.Quaternion __PropertyQuaternion_default_value = new global::Bradot.Quaternion(global::Bradot.Basis.Identity);
        values.Add(PropertyName.@PropertyQuaternion, global::Bradot.Variant.From<global::Bradot.Quaternion>(__PropertyQuaternion_default_value));
        global::Bradot.Transform3D __PropertyTransform3D_default_value = global::Bradot.Transform3D.Identity;
        values.Add(PropertyName.@PropertyTransform3D, global::Bradot.Variant.From<global::Bradot.Transform3D>(__PropertyTransform3D_default_value));
        global::Bradot.Vector4 __PropertyVector4_default_value = new(10f, 10f, 10f, 10f);
        values.Add(PropertyName.@PropertyVector4, global::Bradot.Variant.From<global::Bradot.Vector4>(__PropertyVector4_default_value));
        global::Bradot.Vector4I __PropertyVector4I_default_value = global::Bradot.Vector4I.One;
        values.Add(PropertyName.@PropertyVector4I, global::Bradot.Variant.From<global::Bradot.Vector4I>(__PropertyVector4I_default_value));
        global::Bradot.Projection __PropertyProjection_default_value = global::Bradot.Projection.Identity;
        values.Add(PropertyName.@PropertyProjection, global::Bradot.Variant.From<global::Bradot.Projection>(__PropertyProjection_default_value));
        global::Bradot.Aabb __PropertyAabb_default_value = new global::Bradot.Aabb(10f, 10f, 10f, new global::Bradot.Vector3(1f, 1f, 1f));
        values.Add(PropertyName.@PropertyAabb, global::Bradot.Variant.From<global::Bradot.Aabb>(__PropertyAabb_default_value));
        global::Bradot.Color __PropertyColor_default_value = global::Bradot.Colors.Aquamarine;
        values.Add(PropertyName.@PropertyColor, global::Bradot.Variant.From<global::Bradot.Color>(__PropertyColor_default_value));
        global::Bradot.Plane __PropertyPlane_default_value = global::Bradot.Plane.PlaneXZ;
        values.Add(PropertyName.@PropertyPlane, global::Bradot.Variant.From<global::Bradot.Plane>(__PropertyPlane_default_value));
        global::Bradot.Callable __PropertyCallable_default_value = new global::Bradot.Callable(global::Bradot.Engine.GetMainLoop(), "_process");
        values.Add(PropertyName.@PropertyCallable, global::Bradot.Variant.From<global::Bradot.Callable>(__PropertyCallable_default_value));
        global::Bradot.Signal __PropertySignal_default_value = new global::Bradot.Signal(global::Bradot.Engine.GetMainLoop(), "Propertylist_changed");
        values.Add(PropertyName.@PropertySignal, global::Bradot.Variant.From<global::Bradot.Signal>(__PropertySignal_default_value));
        global::ExportedProperties.MyEnum __PropertyEnum_default_value = global::ExportedProperties.MyEnum.C;
        values.Add(PropertyName.@PropertyEnum, global::Bradot.Variant.From<global::ExportedProperties.MyEnum>(__PropertyEnum_default_value));
        global::ExportedProperties.MyFlagsEnum __PropertyFlagsEnum_default_value = global::ExportedProperties.MyFlagsEnum.C;
        values.Add(PropertyName.@PropertyFlagsEnum, global::Bradot.Variant.From<global::ExportedProperties.MyFlagsEnum>(__PropertyFlagsEnum_default_value));
        byte[] __PropertyByteArray_default_value = { 0, 1, 2, 3, 4, 5, 6  };
        values.Add(PropertyName.@PropertyByteArray, global::Bradot.Variant.From<byte[]>(__PropertyByteArray_default_value));
        int[] __PropertyInt32Array_default_value = { 0, 1, 2, 3, 4, 5, 6  };
        values.Add(PropertyName.@PropertyInt32Array, global::Bradot.Variant.From<int[]>(__PropertyInt32Array_default_value));
        long[] __PropertyInt64Array_default_value = { 0, 1, 2, 3, 4, 5, 6  };
        values.Add(PropertyName.@PropertyInt64Array, global::Bradot.Variant.From<long[]>(__PropertyInt64Array_default_value));
        float[] __PropertySingleArray_default_value = { 0f, 1f, 2f, 3f, 4f, 5f, 6f  };
        values.Add(PropertyName.@PropertySingleArray, global::Bradot.Variant.From<float[]>(__PropertySingleArray_default_value));
        double[] __PropertyDoubleArray_default_value = { 0d, 1d, 2d, 3d, 4d, 5d, 6d  };
        values.Add(PropertyName.@PropertyDoubleArray, global::Bradot.Variant.From<double[]>(__PropertyDoubleArray_default_value));
        string[] __PropertyStringArray_default_value = { "foo", "bar"  };
        values.Add(PropertyName.@PropertyStringArray, global::Bradot.Variant.From<string[]>(__PropertyStringArray_default_value));
        string[] __PropertyStringArrayEnum_default_value = { "foo", "bar"  };
        values.Add(PropertyName.@PropertyStringArrayEnum, global::Bradot.Variant.From<string[]>(__PropertyStringArrayEnum_default_value));
        global::Bradot.Vector2[] __PropertyVector2Array_default_value = { global::Bradot.Vector2.Up, global::Bradot.Vector2.Down, global::Bradot.Vector2.Left, global::Bradot.Vector2.Right   };
        values.Add(PropertyName.@PropertyVector2Array, global::Bradot.Variant.From<global::Bradot.Vector2[]>(__PropertyVector2Array_default_value));
        global::Bradot.Vector3[] __PropertyVector3Array_default_value = { global::Bradot.Vector3.Up, global::Bradot.Vector3.Down, global::Bradot.Vector3.Left, global::Bradot.Vector3.Right   };
        values.Add(PropertyName.@PropertyVector3Array, global::Bradot.Variant.From<global::Bradot.Vector3[]>(__PropertyVector3Array_default_value));
        global::Bradot.Color[] __PropertyColorArray_default_value = { global::Bradot.Colors.Aqua, global::Bradot.Colors.Aquamarine, global::Bradot.Colors.Azure, global::Bradot.Colors.Beige   };
        values.Add(PropertyName.@PropertyColorArray, global::Bradot.Variant.From<global::Bradot.Color[]>(__PropertyColorArray_default_value));
        global::Bradot.BradotObject[] __PropertyBradotObjectOrDerivedArray_default_value = { null  };
        values.Add(PropertyName.@PropertyBradotObjectOrDerivedArray, global::Bradot.Variant.CreateFrom(__PropertyBradotObjectOrDerivedArray_default_value));
        global::Bradot.StringName[] __field_StringNameArray_default_value = { "foo", "bar"  };
        values.Add(PropertyName.@field_StringNameArray, global::Bradot.Variant.From<global::Bradot.StringName[]>(__field_StringNameArray_default_value));
        global::Bradot.NodePath[] __field_NodePathArray_default_value = { "foo", "bar"  };
        values.Add(PropertyName.@field_NodePathArray, global::Bradot.Variant.From<global::Bradot.NodePath[]>(__field_NodePathArray_default_value));
        global::Bradot.Rid[] __field_RidArray_default_value = { default, default, default  };
        values.Add(PropertyName.@field_RidArray, global::Bradot.Variant.From<global::Bradot.Rid[]>(__field_RidArray_default_value));
        global::Bradot.Variant __PropertyVariant_default_value = "foo";
        values.Add(PropertyName.@PropertyVariant, global::Bradot.Variant.From<global::Bradot.Variant>(__PropertyVariant_default_value));
        global::Bradot.BradotObject __PropertyBradotObjectOrDerived_default_value = default;
        values.Add(PropertyName.@PropertyBradotObjectOrDerived, global::Bradot.Variant.From<global::Bradot.BradotObject>(__PropertyBradotObjectOrDerived_default_value));
        global::Bradot.Texture __PropertyBradotResourceTexture_default_value = default;
        values.Add(PropertyName.@PropertyBradotResourceTexture, global::Bradot.Variant.From<global::Bradot.Texture>(__PropertyBradotResourceTexture_default_value));
        global::Bradot.StringName __PropertyStringName_default_value = new global::Bradot.StringName("foo");
        values.Add(PropertyName.@PropertyStringName, global::Bradot.Variant.From<global::Bradot.StringName>(__PropertyStringName_default_value));
        global::Bradot.NodePath __PropertyNodePath_default_value = new global::Bradot.NodePath("foo");
        values.Add(PropertyName.@PropertyNodePath, global::Bradot.Variant.From<global::Bradot.NodePath>(__PropertyNodePath_default_value));
        global::Bradot.Rid __PropertyRid_default_value = default;
        values.Add(PropertyName.@PropertyRid, global::Bradot.Variant.From<global::Bradot.Rid>(__PropertyRid_default_value));
        global::Bradot.Collections.Dictionary __PropertyBradotDictionary_default_value = new()  { { "foo", 10  }, { global::Bradot.Vector2.Up, global::Bradot.Colors.Chocolate   }  };
        values.Add(PropertyName.@PropertyBradotDictionary, global::Bradot.Variant.From<global::Bradot.Collections.Dictionary>(__PropertyBradotDictionary_default_value));
        global::Bradot.Collections.Array __PropertyBradotArray_default_value = new()  { "foo", 10, global::Bradot.Vector2.Up, global::Bradot.Colors.Chocolate   };
        values.Add(PropertyName.@PropertyBradotArray, global::Bradot.Variant.From<global::Bradot.Collections.Array>(__PropertyBradotArray_default_value));
        global::Bradot.Collections.Dictionary<string, bool> __PropertyBradotGenericDictionary_default_value = new()  { { "foo", true  }, { "bar", false  }  };
        values.Add(PropertyName.@PropertyBradotGenericDictionary, global::Bradot.Variant.CreateFrom(__PropertyBradotGenericDictionary_default_value));
        global::Bradot.Collections.Array<int> __PropertyBradotGenericArray_default_value = new()  { 0, 1, 2, 3, 4, 5, 6  };
        values.Add(PropertyName.@PropertyBradotGenericArray, global::Bradot.Variant.CreateFrom(__PropertyBradotGenericArray_default_value));
        return values;
    }
#endif // TOOLS
#pragma warning restore CS0109
}
