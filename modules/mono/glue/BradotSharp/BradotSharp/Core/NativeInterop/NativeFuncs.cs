#pragma warning disable CA1707 // Identifiers should not contain underscores
#pragma warning disable IDE1006 // Naming rule violation
// ReSharper disable InconsistentNaming

using System;
using System.Runtime.CompilerServices;
using Bradot.SourceGenerators.Internal;


namespace Bradot.NativeInterop
{
    /*
     * IMPORTANT:
     * The order of the methods defined in NativeFuncs must match the order
     * in the array defined at the bottom of 'glue/runtime_interop.cpp'.
     */

    [GenerateUnmanagedCallbacks(typeof(UnmanagedCallbacks))]
    public static unsafe partial class NativeFuncs
    {
        private static bool initialized;

        // ReSharper disable once ParameterOnlyUsedForPreconditionCheck.Global
        public static void Initialize(IntPtr unmanagedCallbacks, int unmanagedCallbacksSize)
        {
            if (initialized)
                throw new InvalidOperationException("Already initialized.");
            initialized = true;

            if (unmanagedCallbacksSize != sizeof(UnmanagedCallbacks))
                throw new ArgumentException("Unmanaged callbacks size mismatch.", nameof(unmanagedCallbacksSize));

            _unmanagedCallbacks = Unsafe.AsRef<UnmanagedCallbacks>((void*)unmanagedCallbacks);
        }

        private partial struct UnmanagedCallbacks
        {
        }

        // Custom functions

        internal static partial bradot_bool bradotsharp_dotnet_module_is_initialized();

        public static partial IntPtr bradotsharp_method_bind_get_method(in bradot_string_name p_classname,
            in bradot_string_name p_methodname);

        public static partial IntPtr bradotsharp_method_bind_get_method_with_compatibility(
            in bradot_string_name p_classname, in bradot_string_name p_methodname, ulong p_hash);

        public static partial delegate* unmanaged<bradot_bool, IntPtr> bradotsharp_get_class_constructor(
            in bradot_string_name p_classname);

        public static partial IntPtr bradotsharp_engine_get_singleton(in bradot_string p_name);


        internal static partial Error bradotsharp_stack_info_vector_resize(
            ref DebuggingUtils.bradot_stack_info_vector p_stack_info_vector, int p_size);

        internal static partial void bradotsharp_stack_info_vector_destroy(
            ref DebuggingUtils.bradot_stack_info_vector p_stack_info_vector);

        internal static partial void bradotsharp_internal_editor_file_system_update_files(in bradot_packed_string_array p_script_paths);

        internal static partial void bradotsharp_internal_script_debugger_send_error(in bradot_string p_func,
            in bradot_string p_file, int p_line, in bradot_string p_err, in bradot_string p_descr,
            bradot_error_handler_type p_type, in DebuggingUtils.bradot_stack_info_vector p_stack_info_vector);

        internal static partial bradot_bool bradotsharp_internal_script_debugger_is_active();

        internal static partial IntPtr bradotsharp_internal_object_get_associated_gchandle(IntPtr ptr);

        internal static partial void bradotsharp_internal_object_disposed(IntPtr ptr, IntPtr gcHandleToFree);

        internal static partial void bradotsharp_internal_refcounted_disposed(IntPtr ptr, IntPtr gcHandleToFree,
            bradot_bool isFinalizer);

        internal static partial Error bradotsharp_internal_signal_awaiter_connect(IntPtr source,
            in bradot_string_name signal,
            IntPtr target, IntPtr awaiterHandlePtr);

        internal static partial void bradotsharp_internal_tie_native_managed_to_unmanaged(IntPtr gcHandleIntPtr,
            IntPtr unmanaged, in bradot_string_name nativeName, bradot_bool refCounted);

        internal static partial void bradotsharp_internal_tie_user_managed_to_unmanaged(IntPtr gcHandleIntPtr,
            IntPtr unmanaged, bradot_ref* scriptPtr, bradot_bool refCounted);

        internal static partial void bradotsharp_internal_tie_managed_to_unmanaged_with_pre_setup(
            IntPtr gcHandleIntPtr, IntPtr unmanaged);

        internal static partial IntPtr bradotsharp_internal_unmanaged_get_script_instance_managed(IntPtr p_unmanaged,
            out bradot_bool r_has_cs_script_instance);

        internal static partial IntPtr bradotsharp_internal_unmanaged_get_instance_binding_managed(IntPtr p_unmanaged);

        internal static partial IntPtr bradotsharp_internal_unmanaged_instance_binding_create_managed(IntPtr p_unmanaged,
            IntPtr oldGCHandlePtr);

        internal static partial void bradotsharp_internal_new_csharp_script(bradot_ref* r_dest);

        internal static partial bradot_bool bradotsharp_internal_script_load(in bradot_string p_path, bradot_ref* r_dest);

        internal static partial void bradotsharp_internal_reload_registered_script(IntPtr scriptPtr);

        internal static partial void bradotsharp_array_filter_bradot_objects_by_native(in bradot_string_name p_native_name,
            in bradot_array p_input, out bradot_array r_output);

        internal static partial void bradotsharp_array_filter_bradot_objects_by_non_native(in bradot_array p_input,
            out bradot_array r_output);

        public static partial void bradotsharp_ref_new_from_ref_counted_ptr(out bradot_ref r_dest,
            IntPtr p_ref_counted_ptr);

        public static partial void bradotsharp_ref_destroy(ref bradot_ref p_instance);

        public static partial void bradotsharp_string_name_new_from_string(out bradot_string_name r_dest,
            in bradot_string p_name);

        public static partial void bradotsharp_node_path_new_from_string(out bradot_node_path r_dest,
            in bradot_string p_name);

        public static partial void
            bradotsharp_string_name_as_string(out bradot_string r_dest, in bradot_string_name p_name);

        public static partial void bradotsharp_node_path_as_string(out bradot_string r_dest, in bradot_node_path p_np);

        public static partial bradot_packed_byte_array bradotsharp_packed_byte_array_new_mem_copy(byte* p_src,
            int p_length);

        public static partial bradot_packed_int32_array bradotsharp_packed_int32_array_new_mem_copy(int* p_src,
            int p_length);

        public static partial bradot_packed_int64_array bradotsharp_packed_int64_array_new_mem_copy(long* p_src,
            int p_length);

        public static partial bradot_packed_float32_array bradotsharp_packed_float32_array_new_mem_copy(float* p_src,
            int p_length);

        public static partial bradot_packed_float64_array bradotsharp_packed_float64_array_new_mem_copy(double* p_src,
            int p_length);

        public static partial bradot_packed_vector2_array bradotsharp_packed_vector2_array_new_mem_copy(Vector2* p_src,
            int p_length);

        public static partial bradot_packed_vector3_array bradotsharp_packed_vector3_array_new_mem_copy(Vector3* p_src,
            int p_length);

        public static partial bradot_packed_vector4_array bradotsharp_packed_vector4_array_new_mem_copy(Vector4* p_src,
            int p_length);

        public static partial bradot_packed_color_array bradotsharp_packed_color_array_new_mem_copy(Color* p_src,
            int p_length);

        public static partial void bradotsharp_packed_string_array_add(ref bradot_packed_string_array r_dest,
            in bradot_string p_element);

        public static partial void bradotsharp_callable_new_with_delegate(IntPtr p_delegate_handle, IntPtr p_trampoline,
            IntPtr p_object, out bradot_callable r_callable);

        internal static partial bradot_bool bradotsharp_callable_get_data_for_marshalling(in bradot_callable p_callable,
            out IntPtr r_delegate_handle, out IntPtr r_trampoline, out IntPtr r_object, out bradot_string_name r_name);

        internal static partial bradot_variant bradotsharp_callable_call(in bradot_callable p_callable,
            bradot_variant** p_args, int p_arg_count, out bradot_variant_call_error p_call_error);

        internal static partial void bradotsharp_callable_call_deferred(in bradot_callable p_callable,
            bradot_variant** p_args, int p_arg_count);

        internal static partial Color bradotsharp_color_from_ok_hsl(float p_h, float p_s, float p_l, float p_alpha);

        // BRNative functions

        // gdnative.h

        public static partial void bradotsharp_method_bind_ptrcall(IntPtr p_method_bind, IntPtr p_instance, void** p_args,
            void* p_ret);

        public static partial bradot_variant bradotsharp_method_bind_call(IntPtr p_method_bind, IntPtr p_instance,
            bradot_variant** p_args, int p_arg_count, out bradot_variant_call_error p_call_error);

        // variant.h

        public static partial void
            bradotsharp_variant_new_string_name(out bradot_variant r_dest, in bradot_string_name p_s);

        public static partial void bradotsharp_variant_new_copy(out bradot_variant r_dest, in bradot_variant p_src);

        public static partial void bradotsharp_variant_new_node_path(out bradot_variant r_dest, in bradot_node_path p_np);

        public static partial void bradotsharp_variant_new_object(out bradot_variant r_dest, IntPtr p_obj);

        public static partial void bradotsharp_variant_new_transform2d(out bradot_variant r_dest, in Transform2D p_t2d);

        public static partial void bradotsharp_variant_new_basis(out bradot_variant r_dest, in Basis p_basis);

        public static partial void bradotsharp_variant_new_transform3d(out bradot_variant r_dest, in Transform3D p_trans);

        public static partial void bradotsharp_variant_new_projection(out bradot_variant r_dest, in Projection p_proj);

        public static partial void bradotsharp_variant_new_aabb(out bradot_variant r_dest, in Aabb p_aabb);

        public static partial void bradotsharp_variant_new_dictionary(out bradot_variant r_dest,
            in bradot_dictionary p_dict);

        public static partial void bradotsharp_variant_new_array(out bradot_variant r_dest, in bradot_array p_arr);

        public static partial void bradotsharp_variant_new_packed_byte_array(out bradot_variant r_dest,
            in bradot_packed_byte_array p_pba);

        public static partial void bradotsharp_variant_new_packed_int32_array(out bradot_variant r_dest,
            in bradot_packed_int32_array p_pia);

        public static partial void bradotsharp_variant_new_packed_int64_array(out bradot_variant r_dest,
            in bradot_packed_int64_array p_pia);

        public static partial void bradotsharp_variant_new_packed_float32_array(out bradot_variant r_dest,
            in bradot_packed_float32_array p_pra);

        public static partial void bradotsharp_variant_new_packed_float64_array(out bradot_variant r_dest,
            in bradot_packed_float64_array p_pra);

        public static partial void bradotsharp_variant_new_packed_string_array(out bradot_variant r_dest,
            in bradot_packed_string_array p_psa);

        public static partial void bradotsharp_variant_new_packed_vector2_array(out bradot_variant r_dest,
            in bradot_packed_vector2_array p_pv2a);

        public static partial void bradotsharp_variant_new_packed_vector3_array(out bradot_variant r_dest,
            in bradot_packed_vector3_array p_pv3a);

        public static partial void bradotsharp_variant_new_packed_vector4_array(out bradot_variant r_dest,
            in bradot_packed_vector4_array p_pv4a);

        public static partial void bradotsharp_variant_new_packed_color_array(out bradot_variant r_dest,
            in bradot_packed_color_array p_pca);

        public static partial bradot_bool bradotsharp_variant_as_bool(in bradot_variant p_self);

        public static partial Int64 bradotsharp_variant_as_int(in bradot_variant p_self);

        public static partial double bradotsharp_variant_as_float(in bradot_variant p_self);

        public static partial bradot_string bradotsharp_variant_as_string(in bradot_variant p_self);

        public static partial Vector2 bradotsharp_variant_as_vector2(in bradot_variant p_self);

        public static partial Vector2I bradotsharp_variant_as_vector2i(in bradot_variant p_self);

        public static partial Rect2 bradotsharp_variant_as_rect2(in bradot_variant p_self);

        public static partial Rect2I bradotsharp_variant_as_rect2i(in bradot_variant p_self);

        public static partial Vector3 bradotsharp_variant_as_vector3(in bradot_variant p_self);

        public static partial Vector3I bradotsharp_variant_as_vector3i(in bradot_variant p_self);

        public static partial Transform2D bradotsharp_variant_as_transform2d(in bradot_variant p_self);

        public static partial Vector4 bradotsharp_variant_as_vector4(in bradot_variant p_self);

        public static partial Vector4I bradotsharp_variant_as_vector4i(in bradot_variant p_self);

        public static partial Plane bradotsharp_variant_as_plane(in bradot_variant p_self);

        public static partial Quaternion bradotsharp_variant_as_quaternion(in bradot_variant p_self);

        public static partial Aabb bradotsharp_variant_as_aabb(in bradot_variant p_self);

        public static partial Basis bradotsharp_variant_as_basis(in bradot_variant p_self);

        public static partial Transform3D bradotsharp_variant_as_transform3d(in bradot_variant p_self);

        public static partial Projection bradotsharp_variant_as_projection(in bradot_variant p_self);

        public static partial Color bradotsharp_variant_as_color(in bradot_variant p_self);

        public static partial bradot_string_name bradotsharp_variant_as_string_name(in bradot_variant p_self);

        public static partial bradot_node_path bradotsharp_variant_as_node_path(in bradot_variant p_self);

        public static partial Rid bradotsharp_variant_as_rid(in bradot_variant p_self);

        public static partial bradot_callable bradotsharp_variant_as_callable(in bradot_variant p_self);

        public static partial bradot_signal bradotsharp_variant_as_signal(in bradot_variant p_self);

        public static partial bradot_dictionary bradotsharp_variant_as_dictionary(in bradot_variant p_self);

        public static partial bradot_array bradotsharp_variant_as_array(in bradot_variant p_self);

        public static partial bradot_packed_byte_array bradotsharp_variant_as_packed_byte_array(in bradot_variant p_self);

        public static partial bradot_packed_int32_array bradotsharp_variant_as_packed_int32_array(in bradot_variant p_self);

        public static partial bradot_packed_int64_array bradotsharp_variant_as_packed_int64_array(in bradot_variant p_self);

        public static partial bradot_packed_float32_array bradotsharp_variant_as_packed_float32_array(
            in bradot_variant p_self);

        public static partial bradot_packed_float64_array bradotsharp_variant_as_packed_float64_array(
            in bradot_variant p_self);

        public static partial bradot_packed_string_array bradotsharp_variant_as_packed_string_array(
            in bradot_variant p_self);

        public static partial bradot_packed_vector2_array bradotsharp_variant_as_packed_vector2_array(
            in bradot_variant p_self);

        public static partial bradot_packed_vector3_array bradotsharp_variant_as_packed_vector3_array(
            in bradot_variant p_self);

        public static partial bradot_packed_vector4_array bradotsharp_variant_as_packed_vector4_array(
            in bradot_variant p_self);

        public static partial bradot_packed_color_array bradotsharp_variant_as_packed_color_array(in bradot_variant p_self);

        public static partial bradot_bool bradotsharp_variant_equals(in bradot_variant p_a, in bradot_variant p_b);

        // string.h

        public static partial void bradotsharp_string_new_with_utf16_chars(out bradot_string r_dest, char* p_contents);

        // string_name.h

        public static partial void bradotsharp_string_name_new_copy(out bradot_string_name r_dest,
            in bradot_string_name p_src);

        // node_path.h

        public static partial void bradotsharp_node_path_new_copy(out bradot_node_path r_dest, in bradot_node_path p_src);

        // array.h

        public static partial void bradotsharp_array_new(out bradot_array r_dest);

        public static partial void bradotsharp_array_new_copy(out bradot_array r_dest, in bradot_array p_src);

        public static partial bradot_variant* bradotsharp_array_ptrw(ref bradot_array p_self);

        // dictionary.h

        public static partial void bradotsharp_dictionary_new(out bradot_dictionary r_dest);

        public static partial void bradotsharp_dictionary_new_copy(out bradot_dictionary r_dest,
            in bradot_dictionary p_src);

        // destroy functions

        public static partial void bradotsharp_packed_byte_array_destroy(ref bradot_packed_byte_array p_self);

        public static partial void bradotsharp_packed_int32_array_destroy(ref bradot_packed_int32_array p_self);

        public static partial void bradotsharp_packed_int64_array_destroy(ref bradot_packed_int64_array p_self);

        public static partial void bradotsharp_packed_float32_array_destroy(ref bradot_packed_float32_array p_self);

        public static partial void bradotsharp_packed_float64_array_destroy(ref bradot_packed_float64_array p_self);

        public static partial void bradotsharp_packed_string_array_destroy(ref bradot_packed_string_array p_self);

        public static partial void bradotsharp_packed_vector2_array_destroy(ref bradot_packed_vector2_array p_self);

        public static partial void bradotsharp_packed_vector3_array_destroy(ref bradot_packed_vector3_array p_self);

        public static partial void bradotsharp_packed_vector4_array_destroy(ref bradot_packed_vector4_array p_self);

        public static partial void bradotsharp_packed_color_array_destroy(ref bradot_packed_color_array p_self);

        public static partial void bradotsharp_variant_destroy(ref bradot_variant p_self);

        public static partial void bradotsharp_string_destroy(ref bradot_string p_self);

        public static partial void bradotsharp_string_name_destroy(ref bradot_string_name p_self);

        public static partial void bradotsharp_node_path_destroy(ref bradot_node_path p_self);

        public static partial void bradotsharp_signal_destroy(ref bradot_signal p_self);

        public static partial void bradotsharp_callable_destroy(ref bradot_callable p_self);

        public static partial void bradotsharp_array_destroy(ref bradot_array p_self);

        public static partial void bradotsharp_dictionary_destroy(ref bradot_dictionary p_self);

        // Array

        public static partial int bradotsharp_array_add(ref bradot_array p_self, in bradot_variant p_item);

        public static partial int bradotsharp_array_add_range(ref bradot_array p_self, in bradot_array p_collection);

        public static partial int bradotsharp_array_binary_search(ref bradot_array p_self, int p_index, int p_count, in bradot_variant p_value);

        public static partial void
            bradotsharp_array_duplicate(ref bradot_array p_self, bradot_bool p_deep, out bradot_array r_dest);

        public static partial void bradotsharp_array_fill(ref bradot_array p_self, in bradot_variant p_value);

        public static partial int bradotsharp_array_index_of(ref bradot_array p_self, in bradot_variant p_item, int p_index = 0);

        public static partial void bradotsharp_array_insert(ref bradot_array p_self, int p_index, in bradot_variant p_item);

        public static partial int bradotsharp_array_last_index_of(ref bradot_array p_self, in bradot_variant p_item, int p_index);

        public static partial void bradotsharp_array_make_read_only(ref bradot_array p_self);

        public static partial void bradotsharp_array_max(ref bradot_array p_self, out bradot_variant r_value);

        public static partial void bradotsharp_array_min(ref bradot_array p_self, out bradot_variant r_value);

        public static partial void bradotsharp_array_pick_random(ref bradot_array p_self, out bradot_variant r_value);

        public static partial bradot_bool bradotsharp_array_recursive_equal(ref bradot_array p_self, in bradot_array p_other);

        public static partial void bradotsharp_array_remove_at(ref bradot_array p_self, int p_index);

        public static partial Error bradotsharp_array_resize(ref bradot_array p_self, int p_new_size);

        public static partial void bradotsharp_array_reverse(ref bradot_array p_self);

        public static partial void bradotsharp_array_shuffle(ref bradot_array p_self);

        public static partial void bradotsharp_array_slice(ref bradot_array p_self, int p_start, int p_end,
            int p_step, bradot_bool p_deep, out bradot_array r_dest);

        public static partial void bradotsharp_array_sort(ref bradot_array p_self);

        public static partial void bradotsharp_array_to_string(ref bradot_array p_self, out bradot_string r_str);

        // Dictionary

        public static partial bradot_bool bradotsharp_dictionary_try_get_value(ref bradot_dictionary p_self,
            in bradot_variant p_key,
            out bradot_variant r_value);

        public static partial void bradotsharp_dictionary_set_value(ref bradot_dictionary p_self, in bradot_variant p_key,
            in bradot_variant p_value);

        public static partial void bradotsharp_dictionary_keys(ref bradot_dictionary p_self, out bradot_array r_dest);

        public static partial void bradotsharp_dictionary_values(ref bradot_dictionary p_self, out bradot_array r_dest);

        public static partial int bradotsharp_dictionary_count(ref bradot_dictionary p_self);

        public static partial void bradotsharp_dictionary_key_value_pair_at(ref bradot_dictionary p_self, int p_index,
            out bradot_variant r_key, out bradot_variant r_value);

        public static partial void bradotsharp_dictionary_add(ref bradot_dictionary p_self, in bradot_variant p_key,
            in bradot_variant p_value);

        public static partial void bradotsharp_dictionary_clear(ref bradot_dictionary p_self);

        public static partial bradot_bool bradotsharp_dictionary_contains_key(ref bradot_dictionary p_self,
            in bradot_variant p_key);

        public static partial void bradotsharp_dictionary_duplicate(ref bradot_dictionary p_self, bradot_bool p_deep,
            out bradot_dictionary r_dest);

        public static partial void bradotsharp_dictionary_merge(ref bradot_dictionary p_self, in bradot_dictionary p_dictionary, bradot_bool p_overwrite);

        public static partial bradot_bool bradotsharp_dictionary_recursive_equal(ref bradot_dictionary p_self, in bradot_dictionary p_other);

        public static partial bradot_bool bradotsharp_dictionary_remove_key(ref bradot_dictionary p_self,
            in bradot_variant p_key);

        public static partial void bradotsharp_dictionary_make_read_only(ref bradot_dictionary p_self);

        public static partial void bradotsharp_dictionary_to_string(ref bradot_dictionary p_self, out bradot_string r_str);

        // StringExtensions

        public static partial void bradotsharp_string_simplify_path(in bradot_string p_self,
            out bradot_string r_simplified_path);

        public static partial void bradotsharp_string_to_camel_case(in bradot_string p_self,
            out bradot_string r_camel_case);

        public static partial void bradotsharp_string_to_pascal_case(in bradot_string p_self,
            out bradot_string r_pascal_case);

        public static partial void bradotsharp_string_to_snake_case(in bradot_string p_self,
            out bradot_string r_snake_case);

        // NodePath

        public static partial void bradotsharp_node_path_get_as_property_path(in bradot_node_path p_self,
            ref bradot_node_path r_dest);

        public static partial void bradotsharp_node_path_get_concatenated_names(in bradot_node_path p_self,
            out bradot_string r_names);

        public static partial void bradotsharp_node_path_get_concatenated_subnames(in bradot_node_path p_self,
            out bradot_string r_subnames);

        public static partial void bradotsharp_node_path_get_name(in bradot_node_path p_self, int p_idx,
            out bradot_string r_name);

        public static partial int bradotsharp_node_path_get_name_count(in bradot_node_path p_self);

        public static partial void bradotsharp_node_path_get_subname(in bradot_node_path p_self, int p_idx,
            out bradot_string r_subname);

        public static partial int bradotsharp_node_path_get_subname_count(in bradot_node_path p_self);

        public static partial bradot_bool bradotsharp_node_path_is_absolute(in bradot_node_path p_self);

        public static partial bradot_bool bradotsharp_node_path_equals(in bradot_node_path p_self, in bradot_node_path p_other);

        public static partial int bradotsharp_node_path_hash(in bradot_node_path p_self);

        // BR, etc

        internal static partial void bradotsharp_bytes_to_var(in bradot_packed_byte_array p_bytes,
            bradot_bool p_allow_objects,
            out bradot_variant r_ret);

        internal static partial void bradotsharp_convert(in bradot_variant p_what, int p_type,
            out bradot_variant r_ret);

        internal static partial int bradotsharp_hash(in bradot_variant p_var);

        internal static partial IntPtr bradotsharp_instance_from_id(ulong p_instance_id);

        internal static partial void bradotsharp_print(in bradot_string p_what);

        public static partial void bradotsharp_print_rich(in bradot_string p_what);

        internal static partial void bradotsharp_printerr(in bradot_string p_what);

        internal static partial void bradotsharp_printraw(in bradot_string p_what);

        internal static partial void bradotsharp_prints(in bradot_string p_what);

        internal static partial void bradotsharp_printt(in bradot_string p_what);

        internal static partial float bradotsharp_randf();

        internal static partial uint bradotsharp_randi();

        internal static partial void bradotsharp_randomize();

        internal static partial double bradotsharp_randf_range(double from, double to);

        internal static partial double bradotsharp_randfn(double mean, double deviation);

        internal static partial int bradotsharp_randi_range(int from, int to);

        internal static partial uint bradotsharp_rand_from_seed(ulong seed, out ulong newSeed);

        internal static partial void bradotsharp_seed(ulong seed);

        internal static partial void bradotsharp_weakref(IntPtr p_obj, out bradot_ref r_weak_ref);

        internal static partial void bradotsharp_str_to_var(in bradot_string p_str, out bradot_variant r_ret);

        internal static partial void bradotsharp_var_to_bytes(in bradot_variant p_what, bradot_bool p_full_objects,
            out bradot_packed_byte_array r_bytes);

        internal static partial void bradotsharp_var_to_str(in bradot_variant p_var, out bradot_string r_ret);

        internal static partial void bradotsharp_err_print_error(in bradot_string p_function, in bradot_string p_file, int p_line, in bradot_string p_error, in bradot_string p_message = default, bradot_bool p_editor_notify = bradot_bool.False, bradot_error_handler_type p_type = bradot_error_handler_type.ERR_HANDLER_ERROR);

        // Object

        public static partial void bradotsharp_object_to_string(IntPtr ptr, out bradot_string r_str);
    }
}
