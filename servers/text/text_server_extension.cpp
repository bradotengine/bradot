/**************************************************************************/
/*  text_server_extension.cpp                                             */
/**************************************************************************/
/*                         This file is part of:                          */
/*                             BRADOT ENGINE                              */
/*                        https://bradotengine.org                        */
/**************************************************************************/
/* Copyright (c) 2024-present Bradot Engine contributors (see AUTHORS.md).*/
/* Copyright (c) 2014-2024 Godot Engine contributors (see AUTHORS.md).    */
/* Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.                  */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#include "text_server_extension.h"

void TextServerExtension::_bind_methods() {
	BRVIRTUAL_BIND(_has_feature, "feature");
	BRVIRTUAL_BIND(_get_name);
	BRVIRTUAL_BIND(_get_features);

	BRVIRTUAL_BIND(_free_rid, "rid");
	BRVIRTUAL_BIND(_has, "rid");
	BRVIRTUAL_BIND(_load_support_data, "filename");

	BRVIRTUAL_BIND(_get_support_data_filename);
	BRVIRTUAL_BIND(_get_support_data_info);
	BRVIRTUAL_BIND(_save_support_data, "filename");

	BRVIRTUAL_BIND(_is_locale_right_to_left, "locale");

	BRVIRTUAL_BIND(_name_to_tag, "name");
	BRVIRTUAL_BIND(_tag_to_name, "tag");

	/* Font interface */

	BRVIRTUAL_BIND(_create_font);
	BRVIRTUAL_BIND(_create_font_linked_variation, "font_rid");

	BRVIRTUAL_BIND(_font_set_data, "font_rid", "data");
	BRVIRTUAL_BIND(_font_set_data_ptr, "font_rid", "data_ptr", "data_size");

	BRVIRTUAL_BIND(_font_set_face_index, "font_rid", "face_index");
	BRVIRTUAL_BIND(_font_get_face_index, "font_rid");

	BRVIRTUAL_BIND(_font_get_face_count, "font_rid");

	BRVIRTUAL_BIND(_font_set_style, "font_rid", "style");
	BRVIRTUAL_BIND(_font_get_style, "font_rid");

	BRVIRTUAL_BIND(_font_set_name, "font_rid", "name");
	BRVIRTUAL_BIND(_font_get_name, "font_rid");
	BRVIRTUAL_BIND(_font_get_ot_name_strings, "font_rid");

	BRVIRTUAL_BIND(_font_set_style_name, "font_rid", "name_style");
	BRVIRTUAL_BIND(_font_get_style_name, "font_rid");

	BRVIRTUAL_BIND(_font_set_weight, "font_rid", "weight");
	BRVIRTUAL_BIND(_font_get_weight, "font_rid");

	BRVIRTUAL_BIND(_font_set_stretch, "font_rid", "stretch");
	BRVIRTUAL_BIND(_font_get_stretch, "font_rid");

	BRVIRTUAL_BIND(_font_set_antialiasing, "font_rid", "antialiasing");
	BRVIRTUAL_BIND(_font_get_antialiasing, "font_rid");

	BRVIRTUAL_BIND(_font_set_disable_embedded_bitmaps, "font_rid", "disable_embedded_bitmaps");
	BRVIRTUAL_BIND(_font_get_disable_embedded_bitmaps, "font_rid");

	BRVIRTUAL_BIND(_font_set_generate_mipmaps, "font_rid", "generate_mipmaps");
	BRVIRTUAL_BIND(_font_get_generate_mipmaps, "font_rid");

	BRVIRTUAL_BIND(_font_set_multichannel_signed_distance_field, "font_rid", "msdf");
	BRVIRTUAL_BIND(_font_is_multichannel_signed_distance_field, "font_rid");

	BRVIRTUAL_BIND(_font_set_msdf_pixel_range, "font_rid", "msdf_pixel_range");
	BRVIRTUAL_BIND(_font_get_msdf_pixel_range, "font_rid");

	BRVIRTUAL_BIND(_font_set_msdf_size, "font_rid", "msdf_size");
	BRVIRTUAL_BIND(_font_get_msdf_size, "font_rid");

	BRVIRTUAL_BIND(_font_set_fixed_size, "font_rid", "fixed_size");
	BRVIRTUAL_BIND(_font_get_fixed_size, "font_rid");

	BRVIRTUAL_BIND(_font_set_fixed_size_scale_mode, "font_rid", "fixed_size_scale_mode");
	BRVIRTUAL_BIND(_font_get_fixed_size_scale_mode, "font_rid");

	BRVIRTUAL_BIND(_font_set_allow_system_fallback, "font_rid", "allow_system_fallback");
	BRVIRTUAL_BIND(_font_is_allow_system_fallback, "font_rid");

	BRVIRTUAL_BIND(_font_set_force_autohinter, "font_rid", "force_autohinter");
	BRVIRTUAL_BIND(_font_is_force_autohinter, "font_rid");

	BRVIRTUAL_BIND(_font_set_hinting, "font_rid", "hinting");
	BRVIRTUAL_BIND(_font_get_hinting, "font_rid");

	BRVIRTUAL_BIND(_font_set_subpixel_positioning, "font_rid", "subpixel_positioning");
	BRVIRTUAL_BIND(_font_get_subpixel_positioning, "font_rid");

	BRVIRTUAL_BIND(_font_set_embolden, "font_rid", "strength");
	BRVIRTUAL_BIND(_font_get_embolden, "font_rid");

	BRVIRTUAL_BIND(_font_set_spacing, "font_rid", "spacing", "value");
	BRVIRTUAL_BIND(_font_get_spacing, "font_rid", "spacing");

	BRVIRTUAL_BIND(_font_set_baseline_offset, "font_rid", "baseline_offset");
	BRVIRTUAL_BIND(_font_get_baseline_offset, "font_rid");

	BRVIRTUAL_BIND(_font_set_transform, "font_rid", "transform");
	BRVIRTUAL_BIND(_font_get_transform, "font_rid");

	BRVIRTUAL_BIND(_font_set_variation_coordinates, "font_rid", "variation_coordinates");
	BRVIRTUAL_BIND(_font_get_variation_coordinates, "font_rid");

	BRVIRTUAL_BIND(_font_set_oversampling, "font_rid", "oversampling");
	BRVIRTUAL_BIND(_font_get_oversampling, "font_rid");

	BRVIRTUAL_BIND(_font_get_size_cache_list, "font_rid");
	BRVIRTUAL_BIND(_font_clear_size_cache, "font_rid");
	BRVIRTUAL_BIND(_font_remove_size_cache, "font_rid", "size");

	BRVIRTUAL_BIND(_font_set_ascent, "font_rid", "size", "ascent");
	BRVIRTUAL_BIND(_font_get_ascent, "font_rid", "size");

	BRVIRTUAL_BIND(_font_set_descent, "font_rid", "size", "descent");
	BRVIRTUAL_BIND(_font_get_descent, "font_rid", "size");

	BRVIRTUAL_BIND(_font_set_underline_position, "font_rid", "size", "underline_position");
	BRVIRTUAL_BIND(_font_get_underline_position, "font_rid", "size");

	BRVIRTUAL_BIND(_font_set_underline_thickness, "font_rid", "size", "underline_thickness");
	BRVIRTUAL_BIND(_font_get_underline_thickness, "font_rid", "size");

	BRVIRTUAL_BIND(_font_set_scale, "font_rid", "size", "scale");
	BRVIRTUAL_BIND(_font_get_scale, "font_rid", "size");

	BRVIRTUAL_BIND(_font_get_texture_count, "font_rid", "size");
	BRVIRTUAL_BIND(_font_clear_textures, "font_rid", "size");
	BRVIRTUAL_BIND(_font_remove_texture, "font_rid", "size", "texture_index");

	BRVIRTUAL_BIND(_font_set_texture_image, "font_rid", "size", "texture_index", "image");
	BRVIRTUAL_BIND(_font_get_texture_image, "font_rid", "size", "texture_index");

	BRVIRTUAL_BIND(_font_set_texture_offsets, "font_rid", "size", "texture_index", "offset");
	BRVIRTUAL_BIND(_font_get_texture_offsets, "font_rid", "size", "texture_index");

	BRVIRTUAL_BIND(_font_get_glyph_list, "font_rid", "size");
	BRVIRTUAL_BIND(_font_clear_glyphs, "font_rid", "size");
	BRVIRTUAL_BIND(_font_remove_glyph, "font_rid", "size", "glyph");

	BRVIRTUAL_BIND(_font_get_glyph_advance, "font_rid", "size", "glyph");
	BRVIRTUAL_BIND(_font_set_glyph_advance, "font_rid", "size", "glyph", "advance");

	BRVIRTUAL_BIND(_font_get_glyph_offset, "font_rid", "size", "glyph");
	BRVIRTUAL_BIND(_font_set_glyph_offset, "font_rid", "size", "glyph", "offset");

	BRVIRTUAL_BIND(_font_get_glyph_size, "font_rid", "size", "glyph");
	BRVIRTUAL_BIND(_font_set_glyph_size, "font_rid", "size", "glyph", "gl_size");

	BRVIRTUAL_BIND(_font_get_glyph_uv_rect, "font_rid", "size", "glyph");
	BRVIRTUAL_BIND(_font_set_glyph_uv_rect, "font_rid", "size", "glyph", "uv_rect");

	BRVIRTUAL_BIND(_font_get_glyph_texture_idx, "font_rid", "size", "glyph");
	BRVIRTUAL_BIND(_font_set_glyph_texture_idx, "font_rid", "size", "glyph", "texture_idx");

	BRVIRTUAL_BIND(_font_get_glyph_texture_rid, "font_rid", "size", "glyph");
	BRVIRTUAL_BIND(_font_get_glyph_texture_size, "font_rid", "size", "glyph");

	BRVIRTUAL_BIND(_font_get_glyph_contours, "font_rid", "size", "index");

	BRVIRTUAL_BIND(_font_get_kerning_list, "font_rid", "size");
	BRVIRTUAL_BIND(_font_clear_kerning_map, "font_rid", "size");
	BRVIRTUAL_BIND(_font_remove_kerning, "font_rid", "size", "glyph_pair");

	BRVIRTUAL_BIND(_font_set_kerning, "font_rid", "size", "glyph_pair", "kerning");
	BRVIRTUAL_BIND(_font_get_kerning, "font_rid", "size", "glyph_pair");

	BRVIRTUAL_BIND(_font_get_glyph_index, "font_rid", "size", "char", "variation_selector");
	BRVIRTUAL_BIND(_font_get_char_from_glyph_index, "font_rid", "size", "glyph_index");

	BRVIRTUAL_BIND(_font_has_char, "font_rid", "char");
	BRVIRTUAL_BIND(_font_get_supported_chars, "font_rid");
	BRVIRTUAL_BIND(_font_get_supported_glyphs, "font_rid");

	BRVIRTUAL_BIND(_font_render_range, "font_rid", "size", "start", "end");
	BRVIRTUAL_BIND(_font_render_glyph, "font_rid", "size", "index");

	BRVIRTUAL_BIND(_font_draw_glyph, "font_rid", "canvas", "size", "pos", "index", "color");
	BRVIRTUAL_BIND(_font_draw_glyph_outline, "font_rid", "canvas", "size", "outline_size", "pos", "index", "color");

	BRVIRTUAL_BIND(_font_is_language_supported, "font_rid", "language");
	BRVIRTUAL_BIND(_font_set_language_support_override, "font_rid", "language", "supported");
	BRVIRTUAL_BIND(_font_get_language_support_override, "font_rid", "language");
	BRVIRTUAL_BIND(_font_remove_language_support_override, "font_rid", "language");
	BRVIRTUAL_BIND(_font_get_language_support_overrides, "font_rid");

	BRVIRTUAL_BIND(_font_is_script_supported, "font_rid", "script");
	BRVIRTUAL_BIND(_font_set_script_support_override, "font_rid", "script", "supported");
	BRVIRTUAL_BIND(_font_get_script_support_override, "font_rid", "script");
	BRVIRTUAL_BIND(_font_remove_script_support_override, "font_rid", "script");
	BRVIRTUAL_BIND(_font_get_script_support_overrides, "font_rid");

	BRVIRTUAL_BIND(_font_set_opentype_feature_overrides, "font_rid", "overrides");
	BRVIRTUAL_BIND(_font_get_opentype_feature_overrides, "font_rid");

	BRVIRTUAL_BIND(_font_supported_feature_list, "font_rid");
	BRVIRTUAL_BIND(_font_supported_variation_list, "font_rid");

	BRVIRTUAL_BIND(_font_get_global_oversampling);
	BRVIRTUAL_BIND(_font_set_global_oversampling, "oversampling");

	BRVIRTUAL_BIND(_get_hex_code_box_size, "size", "index");
	BRVIRTUAL_BIND(_draw_hex_code_box, "canvas", "size", "pos", "index", "color");

	/* Shaped text buffer interface */

	BRVIRTUAL_BIND(_create_shaped_text, "direction", "orientation");

	BRVIRTUAL_BIND(_shaped_text_clear, "shaped");

	BRVIRTUAL_BIND(_shaped_text_set_direction, "shaped", "direction");
	BRVIRTUAL_BIND(_shaped_text_get_direction, "shaped");
	BRVIRTUAL_BIND(_shaped_text_get_inferred_direction, "shaped");

	BRVIRTUAL_BIND(_shaped_text_set_bidi_override, "shaped", "override");

	BRVIRTUAL_BIND(_shaped_text_set_custom_punctuation, "shaped", "punct");
	BRVIRTUAL_BIND(_shaped_text_get_custom_punctuation, "shaped");

	BRVIRTUAL_BIND(_shaped_text_set_custom_ellipsis, "shaped", "char");
	BRVIRTUAL_BIND(_shaped_text_get_custom_ellipsis, "shaped");

	BRVIRTUAL_BIND(_shaped_text_set_orientation, "shaped", "orientation");
	BRVIRTUAL_BIND(_shaped_text_get_orientation, "shaped");

	BRVIRTUAL_BIND(_shaped_text_set_preserve_invalid, "shaped", "enabled");
	BRVIRTUAL_BIND(_shaped_text_get_preserve_invalid, "shaped");

	BRVIRTUAL_BIND(_shaped_text_set_preserve_control, "shaped", "enabled");
	BRVIRTUAL_BIND(_shaped_text_get_preserve_control, "shaped");

	BRVIRTUAL_BIND(_shaped_text_set_spacing, "shaped", "spacing", "value");
	BRVIRTUAL_BIND(_shaped_text_get_spacing, "shaped", "spacing");

	BRVIRTUAL_BIND(_shaped_text_add_string, "shaped", "text", "fonts", "size", "opentype_features", "language", "meta");
	BRVIRTUAL_BIND(_shaped_text_add_object, "shaped", "key", "size", "inline_align", "length", "baseline");
	BRVIRTUAL_BIND(_shaped_text_resize_object, "shaped", "key", "size", "inline_align", "baseline");

	BRVIRTUAL_BIND(_shaped_get_span_count, "shaped");
	BRVIRTUAL_BIND(_shaped_get_span_meta, "shaped", "index");
	BRVIRTUAL_BIND(_shaped_set_span_update_font, "shaped", "index", "fonts", "size", "opentype_features");

	BRVIRTUAL_BIND(_shaped_text_substr, "shaped", "start", "length");
	BRVIRTUAL_BIND(_shaped_text_get_parent, "shaped");

	BRVIRTUAL_BIND(_shaped_text_fit_to_width, "shaped", "width", "justification_flags");
	BRVIRTUAL_BIND(_shaped_text_tab_align, "shaped", "tab_stops");

	BRVIRTUAL_BIND(_shaped_text_shape, "shaped");
	BRVIRTUAL_BIND(_shaped_text_update_breaks, "shaped");
	BRVIRTUAL_BIND(_shaped_text_update_justification_ops, "shaped");

	BRVIRTUAL_BIND(_shaped_text_is_ready, "shaped");

	BRVIRTUAL_BIND(_shaped_text_get_glyphs, "shaped");
	BRVIRTUAL_BIND(_shaped_text_sort_logical, "shaped");
	BRVIRTUAL_BIND(_shaped_text_get_glyph_count, "shaped");

	BRVIRTUAL_BIND(_shaped_text_get_range, "shaped");

	BRVIRTUAL_BIND(_shaped_text_get_line_breaks_adv, "shaped", "width", "start", "once", "break_flags");
	BRVIRTUAL_BIND(_shaped_text_get_line_breaks, "shaped", "width", "start", "break_flags");
	BRVIRTUAL_BIND(_shaped_text_get_word_breaks, "shaped", "grapheme_flags", "skip_grapheme_flags");

	BRVIRTUAL_BIND(_shaped_text_get_trim_pos, "shaped");
	BRVIRTUAL_BIND(_shaped_text_get_ellipsis_pos, "shaped");
	BRVIRTUAL_BIND(_shaped_text_get_ellipsis_glyph_count, "shaped");
	BRVIRTUAL_BIND(_shaped_text_get_ellipsis_glyphs, "shaped");

	BRVIRTUAL_BIND(_shaped_text_overrun_trim_to_width, "shaped", "width", "trim_flags");

	BRVIRTUAL_BIND(_shaped_text_get_objects, "shaped");
	BRVIRTUAL_BIND(_shaped_text_get_object_rect, "shaped", "key");
	BRVIRTUAL_BIND(_shaped_text_get_object_range, "shaped", "key");
	BRVIRTUAL_BIND(_shaped_text_get_object_glyph, "shaped", "key");

	BRVIRTUAL_BIND(_shaped_text_get_size, "shaped");
	BRVIRTUAL_BIND(_shaped_text_get_ascent, "shaped");
	BRVIRTUAL_BIND(_shaped_text_get_descent, "shaped");
	BRVIRTUAL_BIND(_shaped_text_get_width, "shaped");
	BRVIRTUAL_BIND(_shaped_text_get_underline_position, "shaped");
	BRVIRTUAL_BIND(_shaped_text_get_underline_thickness, "shaped");

	BRVIRTUAL_BIND(_shaped_text_get_dominant_direction_in_range, "shaped", "start", "end");

	BRVIRTUAL_BIND(_shaped_text_get_carets, "shaped", "position", "caret");
	BRVIRTUAL_BIND(_shaped_text_get_selection, "shaped", "start", "end");

	BRVIRTUAL_BIND(_shaped_text_hit_test_grapheme, "shaped", "coord");
	BRVIRTUAL_BIND(_shaped_text_hit_test_position, "shaped", "coord");

	BRVIRTUAL_BIND(_shaped_text_draw, "shaped", "canvas", "pos", "clip_l", "clip_r", "color");
	BRVIRTUAL_BIND(_shaped_text_draw_outline, "shaped", "canvas", "pos", "clip_l", "clip_r", "outline_size", "color");

	BRVIRTUAL_BIND(_shaped_text_get_grapheme_bounds, "shaped", "pos");
	BRVIRTUAL_BIND(_shaped_text_next_grapheme_pos, "shaped", "pos");
	BRVIRTUAL_BIND(_shaped_text_prev_grapheme_pos, "shaped", "pos");

	BRVIRTUAL_BIND(_shaped_text_get_character_breaks, "shaped");
	BRVIRTUAL_BIND(_shaped_text_next_character_pos, "shaped", "pos");
	BRVIRTUAL_BIND(_shaped_text_prev_character_pos, "shaped", "pos");
	BRVIRTUAL_BIND(_shaped_text_closest_character_pos, "shaped", "pos");

	BRVIRTUAL_BIND(_format_number, "number", "language");
	BRVIRTUAL_BIND(_parse_number, "number", "language");
	BRVIRTUAL_BIND(_percent_sign, "language");

	BRVIRTUAL_BIND(_strip_diacritics, "string");
	BRVIRTUAL_BIND(_is_valid_identifier, "string");
	BRVIRTUAL_BIND(_is_valid_letter, "unicode");

	BRVIRTUAL_BIND(_string_get_word_breaks, "string", "language", "chars_per_line");
	BRVIRTUAL_BIND(_string_get_character_breaks, "string", "language");

	BRVIRTUAL_BIND(_is_confusable, "string", "dict");
	BRVIRTUAL_BIND(_spoof_check, "string");

	BRVIRTUAL_BIND(_string_to_upper, "string", "language");
	BRVIRTUAL_BIND(_string_to_lower, "string", "language");
	BRVIRTUAL_BIND(_string_to_title, "string", "language");

	BRVIRTUAL_BIND(_parse_structured_text, "parser_type", "args", "text");

	BRVIRTUAL_BIND(_cleanup);
}

bool TextServerExtension::has_feature(Feature p_feature) const {
	bool ret = false;
	BRVIRTUAL_CALL(_has_feature, p_feature, ret);
	return ret;
}

String TextServerExtension::get_name() const {
	String ret = "Unknown";
	BRVIRTUAL_CALL(_get_name, ret);
	return ret;
}

int64_t TextServerExtension::get_features() const {
	int64_t ret = 0;
	BRVIRTUAL_CALL(_get_features, ret);
	return ret;
}

void TextServerExtension::free_rid(const RID &p_rid) {
	BRVIRTUAL_CALL(_free_rid, p_rid);
}

bool TextServerExtension::has(const RID &p_rid) {
	bool ret = false;
	BRVIRTUAL_CALL(_has, p_rid, ret);
	return ret;
}

bool TextServerExtension::load_support_data(const String &p_filename) {
	bool ret = false;
	BRVIRTUAL_CALL(_load_support_data, p_filename, ret);
	return ret;
}

String TextServerExtension::get_support_data_filename() const {
	String ret;
	BRVIRTUAL_CALL(_get_support_data_filename, ret);
	return ret;
}

String TextServerExtension::get_support_data_info() const {
	String ret;
	BRVIRTUAL_CALL(_get_support_data_info, ret);
	return ret;
}

bool TextServerExtension::save_support_data(const String &p_filename) const {
	bool ret = false;
	BRVIRTUAL_CALL(_save_support_data, p_filename, ret);
	return ret;
}

bool TextServerExtension::is_locale_right_to_left(const String &p_locale) const {
	bool ret = false;
	BRVIRTUAL_CALL(_is_locale_right_to_left, p_locale, ret);
	return ret;
}

int64_t TextServerExtension::name_to_tag(const String &p_name) const {
	int64_t ret = 0;
	if (BRVIRTUAL_CALL(_name_to_tag, p_name, ret)) {
		return ret;
	}
	return TextServer::name_to_tag(p_name);
}

String TextServerExtension::tag_to_name(int64_t p_tag) const {
	String ret;
	if (BRVIRTUAL_CALL(_tag_to_name, p_tag, ret)) {
		return ret;
	}
	return TextServer::tag_to_name(p_tag);
}

/*************************************************************************/
/* Font                                                                  */
/*************************************************************************/

RID TextServerExtension::create_font() {
	RID ret;
	BRVIRTUAL_CALL(_create_font, ret);
	return ret;
}

RID TextServerExtension::create_font_linked_variation(const RID &p_font_rid) {
	RID ret;
	BRVIRTUAL_CALL(_create_font_linked_variation, p_font_rid, ret);
	return ret;
}

void TextServerExtension::font_set_data(const RID &p_font_rid, const PackedByteArray &p_data) {
	BRVIRTUAL_CALL(_font_set_data, p_font_rid, p_data);
}

void TextServerExtension::font_set_data_ptr(const RID &p_font_rid, const uint8_t *p_data_ptr, int64_t p_data_size) {
	BRVIRTUAL_CALL(_font_set_data_ptr, p_font_rid, p_data_ptr, p_data_size);
}

void TextServerExtension::font_set_face_index(const RID &p_font_rid, int64_t p_index) {
	BRVIRTUAL_CALL(_font_set_face_index, p_font_rid, p_index);
}

int64_t TextServerExtension::font_get_face_index(const RID &p_font_rid) const {
	int64_t ret = 0;
	BRVIRTUAL_CALL(_font_get_face_index, p_font_rid, ret);
	return ret;
}

int64_t TextServerExtension::font_get_face_count(const RID &p_font_rid) const {
	int64_t ret = 1;
	BRVIRTUAL_CALL(_font_get_face_count, p_font_rid, ret);
	return ret;
}

void TextServerExtension::font_set_style(const RID &p_font_rid, BitField<TextServer::FontStyle> p_style) {
	BRVIRTUAL_CALL(_font_set_style, p_font_rid, p_style);
}

BitField<TextServer::FontStyle> TextServerExtension::font_get_style(const RID &p_font_rid) const {
	BitField<TextServer::FontStyle> ret = 0;
	BRVIRTUAL_CALL(_font_get_style, p_font_rid, ret);
	return ret;
}

void TextServerExtension::font_set_style_name(const RID &p_font_rid, const String &p_name) {
	BRVIRTUAL_CALL(_font_set_style_name, p_font_rid, p_name);
}

String TextServerExtension::font_get_style_name(const RID &p_font_rid) const {
	String ret;
	BRVIRTUAL_CALL(_font_get_style_name, p_font_rid, ret);
	return ret;
}

void TextServerExtension::font_set_weight(const RID &p_font_rid, int64_t p_weight) {
	BRVIRTUAL_CALL(_font_set_weight, p_font_rid, p_weight);
}

int64_t TextServerExtension::font_get_weight(const RID &p_font_rid) const {
	int64_t ret = 400;
	BRVIRTUAL_CALL(_font_get_weight, p_font_rid, ret);
	return ret;
}

void TextServerExtension::font_set_stretch(const RID &p_font_rid, int64_t p_stretch) {
	BRVIRTUAL_CALL(_font_set_stretch, p_font_rid, p_stretch);
}

int64_t TextServerExtension::font_get_stretch(const RID &p_font_rid) const {
	int64_t ret = 100;
	BRVIRTUAL_CALL(_font_get_stretch, p_font_rid, ret);
	return ret;
}

void TextServerExtension::font_set_name(const RID &p_font_rid, const String &p_name) {
	BRVIRTUAL_CALL(_font_set_name, p_font_rid, p_name);
}

String TextServerExtension::font_get_name(const RID &p_font_rid) const {
	String ret;
	BRVIRTUAL_CALL(_font_get_name, p_font_rid, ret);
	return ret;
}

Dictionary TextServerExtension::font_get_ot_name_strings(const RID &p_font_rid) const {
	Dictionary ret;
	BRVIRTUAL_CALL(_font_get_ot_name_strings, p_font_rid, ret);
	return ret;
}

void TextServerExtension::font_set_antialiasing(const RID &p_font_rid, TextServer::FontAntialiasing p_antialiasing) {
	BRVIRTUAL_CALL(_font_set_antialiasing, p_font_rid, p_antialiasing);
}

TextServer::FontAntialiasing TextServerExtension::font_get_antialiasing(const RID &p_font_rid) const {
	TextServer::FontAntialiasing ret = TextServer::FONT_ANTIALIASING_NONE;
	BRVIRTUAL_CALL(_font_get_antialiasing, p_font_rid, ret);
	return ret;
}

void TextServerExtension::font_set_disable_embedded_bitmaps(const RID &p_font_rid, bool p_disable_embedded_bitmaps) {
	BRVIRTUAL_CALL(_font_set_disable_embedded_bitmaps, p_font_rid, p_disable_embedded_bitmaps);
}

bool TextServerExtension::font_get_disable_embedded_bitmaps(const RID &p_font_rid) const {
	bool ret = false;
	BRVIRTUAL_CALL(_font_get_disable_embedded_bitmaps, p_font_rid, ret);
	return ret;
}

void TextServerExtension::font_set_generate_mipmaps(const RID &p_font_rid, bool p_generate_mipmaps) {
	BRVIRTUAL_CALL(_font_set_generate_mipmaps, p_font_rid, p_generate_mipmaps);
}

bool TextServerExtension::font_get_generate_mipmaps(const RID &p_font_rid) const {
	bool ret = false;
	BRVIRTUAL_CALL(_font_get_generate_mipmaps, p_font_rid, ret);
	return ret;
}

void TextServerExtension::font_set_multichannel_signed_distance_field(const RID &p_font_rid, bool p_msdf) {
	BRVIRTUAL_CALL(_font_set_multichannel_signed_distance_field, p_font_rid, p_msdf);
}

bool TextServerExtension::font_is_multichannel_signed_distance_field(const RID &p_font_rid) const {
	bool ret = false;
	BRVIRTUAL_CALL(_font_is_multichannel_signed_distance_field, p_font_rid, ret);
	return ret;
}

void TextServerExtension::font_set_msdf_pixel_range(const RID &p_font_rid, int64_t p_msdf_pixel_range) {
	BRVIRTUAL_CALL(_font_set_msdf_pixel_range, p_font_rid, p_msdf_pixel_range);
}

int64_t TextServerExtension::font_get_msdf_pixel_range(const RID &p_font_rid) const {
	int64_t ret = 0;
	BRVIRTUAL_CALL(_font_get_msdf_pixel_range, p_font_rid, ret);
	return ret;
}

void TextServerExtension::font_set_msdf_size(const RID &p_font_rid, int64_t p_msdf_size) {
	BRVIRTUAL_CALL(_font_set_msdf_size, p_font_rid, p_msdf_size);
}

int64_t TextServerExtension::font_get_msdf_size(const RID &p_font_rid) const {
	int64_t ret = 0;
	BRVIRTUAL_CALL(_font_get_msdf_size, p_font_rid, ret);
	return ret;
}

void TextServerExtension::font_set_fixed_size(const RID &p_font_rid, int64_t p_fixed_size) {
	BRVIRTUAL_CALL(_font_set_fixed_size, p_font_rid, p_fixed_size);
}

int64_t TextServerExtension::font_get_fixed_size(const RID &p_font_rid) const {
	int64_t ret = 0;
	BRVIRTUAL_CALL(_font_get_fixed_size, p_font_rid, ret);
	return ret;
}

void TextServerExtension::font_set_fixed_size_scale_mode(const RID &p_font_rid, TextServer::FixedSizeScaleMode p_fixed_size_scale_mode) {
	BRVIRTUAL_CALL(_font_set_fixed_size_scale_mode, p_font_rid, p_fixed_size_scale_mode);
}

TextServer::FixedSizeScaleMode TextServerExtension::font_get_fixed_size_scale_mode(const RID &p_font_rid) const {
	FixedSizeScaleMode ret = FIXED_SIZE_SCALE_DISABLE;
	BRVIRTUAL_CALL(_font_get_fixed_size_scale_mode, p_font_rid, ret);
	return ret;
}

void TextServerExtension::font_set_allow_system_fallback(const RID &p_font_rid, bool p_allow_system_fallback) {
	BRVIRTUAL_CALL(_font_set_allow_system_fallback, p_font_rid, p_allow_system_fallback);
}

bool TextServerExtension::font_is_allow_system_fallback(const RID &p_font_rid) const {
	bool ret = false;
	BRVIRTUAL_CALL(_font_is_allow_system_fallback, p_font_rid, ret);
	return ret;
}

void TextServerExtension::font_set_force_autohinter(const RID &p_font_rid, bool p_force_autohinter) {
	BRVIRTUAL_CALL(_font_set_force_autohinter, p_font_rid, p_force_autohinter);
}

bool TextServerExtension::font_is_force_autohinter(const RID &p_font_rid) const {
	bool ret = false;
	BRVIRTUAL_CALL(_font_is_force_autohinter, p_font_rid, ret);
	return ret;
}

void TextServerExtension::font_set_hinting(const RID &p_font_rid, TextServer::Hinting p_hinting) {
	BRVIRTUAL_CALL(_font_set_hinting, p_font_rid, p_hinting);
}

TextServer::Hinting TextServerExtension::font_get_hinting(const RID &p_font_rid) const {
	TextServer::Hinting ret = TextServer::HINTING_NONE;
	BRVIRTUAL_CALL(_font_get_hinting, p_font_rid, ret);
	return ret;
}

void TextServerExtension::font_set_subpixel_positioning(const RID &p_font_rid, TextServer::SubpixelPositioning p_subpixel) {
	BRVIRTUAL_CALL(_font_set_subpixel_positioning, p_font_rid, p_subpixel);
}

TextServer::SubpixelPositioning TextServerExtension::font_get_subpixel_positioning(const RID &p_font_rid) const {
	TextServer::SubpixelPositioning ret = TextServer::SUBPIXEL_POSITIONING_DISABLED;
	BRVIRTUAL_CALL(_font_get_subpixel_positioning, p_font_rid, ret);
	return ret;
}

void TextServerExtension::font_set_embolden(const RID &p_font_rid, double p_strength) {
	BRVIRTUAL_CALL(_font_set_embolden, p_font_rid, p_strength);
}

double TextServerExtension::font_get_embolden(const RID &p_font_rid) const {
	double ret = 0;
	BRVIRTUAL_CALL(_font_get_embolden, p_font_rid, ret);
	return ret;
}

void TextServerExtension::font_set_spacing(const RID &p_font_rid, SpacingType p_spacing, int64_t p_value) {
	BRVIRTUAL_CALL(_font_set_spacing, p_font_rid, p_spacing, p_value);
}

int64_t TextServerExtension::font_get_spacing(const RID &p_font_rid, SpacingType p_spacing) const {
	int64_t ret = 0;
	BRVIRTUAL_CALL(_font_get_spacing, p_font_rid, p_spacing, ret);
	return ret;
}

void TextServerExtension::font_set_baseline_offset(const RID &p_font_rid, double p_baseline_offset) {
	BRVIRTUAL_CALL(_font_set_baseline_offset, p_font_rid, p_baseline_offset);
}

double TextServerExtension::font_get_baseline_offset(const RID &p_font_rid) const {
	double ret = 0.0;
	BRVIRTUAL_CALL(_font_get_baseline_offset, p_font_rid, ret);
	return ret;
}

void TextServerExtension::font_set_transform(const RID &p_font_rid, const Transform2D &p_transform) {
	BRVIRTUAL_CALL(_font_set_transform, p_font_rid, p_transform);
}

Transform2D TextServerExtension::font_get_transform(const RID &p_font_rid) const {
	Transform2D ret;
	BRVIRTUAL_CALL(_font_get_transform, p_font_rid, ret);
	return ret;
}

void TextServerExtension::font_set_variation_coordinates(const RID &p_font_rid, const Dictionary &p_variation_coordinates) {
	BRVIRTUAL_CALL(_font_set_variation_coordinates, p_font_rid, p_variation_coordinates);
}

Dictionary TextServerExtension::font_get_variation_coordinates(const RID &p_font_rid) const {
	Dictionary ret;
	BRVIRTUAL_CALL(_font_get_variation_coordinates, p_font_rid, ret);
	return ret;
}

void TextServerExtension::font_set_oversampling(const RID &p_font_rid, double p_oversampling) {
	BRVIRTUAL_CALL(_font_set_oversampling, p_font_rid, p_oversampling);
}

double TextServerExtension::font_get_oversampling(const RID &p_font_rid) const {
	double ret = 0;
	BRVIRTUAL_CALL(_font_get_oversampling, p_font_rid, ret);
	return ret;
}

TypedArray<Vector2i> TextServerExtension::font_get_size_cache_list(const RID &p_font_rid) const {
	TypedArray<Vector2i> ret;
	BRVIRTUAL_CALL(_font_get_size_cache_list, p_font_rid, ret);
	return ret;
}

void TextServerExtension::font_clear_size_cache(const RID &p_font_rid) {
	BRVIRTUAL_CALL(_font_clear_size_cache, p_font_rid);
}

void TextServerExtension::font_remove_size_cache(const RID &p_font_rid, const Vector2i &p_size) {
	BRVIRTUAL_CALL(_font_remove_size_cache, p_font_rid, p_size);
}

void TextServerExtension::font_set_ascent(const RID &p_font_rid, int64_t p_size, double p_ascent) {
	BRVIRTUAL_CALL(_font_set_ascent, p_font_rid, p_size, p_ascent);
}

double TextServerExtension::font_get_ascent(const RID &p_font_rid, int64_t p_size) const {
	double ret = 0;
	BRVIRTUAL_CALL(_font_get_ascent, p_font_rid, p_size, ret);
	return ret;
}

void TextServerExtension::font_set_descent(const RID &p_font_rid, int64_t p_size, double p_descent) {
	BRVIRTUAL_CALL(_font_set_descent, p_font_rid, p_size, p_descent);
}

double TextServerExtension::font_get_descent(const RID &p_font_rid, int64_t p_size) const {
	double ret = 0;
	BRVIRTUAL_CALL(_font_get_descent, p_font_rid, p_size, ret);
	return ret;
}

void TextServerExtension::font_set_underline_position(const RID &p_font_rid, int64_t p_size, double p_underline_position) {
	BRVIRTUAL_CALL(_font_set_underline_position, p_font_rid, p_size, p_underline_position);
}

double TextServerExtension::font_get_underline_position(const RID &p_font_rid, int64_t p_size) const {
	double ret = 0;
	BRVIRTUAL_CALL(_font_get_underline_position, p_font_rid, p_size, ret);
	return ret;
}

void TextServerExtension::font_set_underline_thickness(const RID &p_font_rid, int64_t p_size, double p_underline_thickness) {
	BRVIRTUAL_CALL(_font_set_underline_thickness, p_font_rid, p_size, p_underline_thickness);
}

double TextServerExtension::font_get_underline_thickness(const RID &p_font_rid, int64_t p_size) const {
	double ret = 0;
	BRVIRTUAL_CALL(_font_get_underline_thickness, p_font_rid, p_size, ret);
	return ret;
}

void TextServerExtension::font_set_scale(const RID &p_font_rid, int64_t p_size, double p_scale) {
	BRVIRTUAL_CALL(_font_set_scale, p_font_rid, p_size, p_scale);
}

double TextServerExtension::font_get_scale(const RID &p_font_rid, int64_t p_size) const {
	double ret = 0;
	BRVIRTUAL_CALL(_font_get_scale, p_font_rid, p_size, ret);
	return ret;
}

int64_t TextServerExtension::font_get_texture_count(const RID &p_font_rid, const Vector2i &p_size) const {
	int64_t ret = 0;
	BRVIRTUAL_CALL(_font_get_texture_count, p_font_rid, p_size, ret);
	return ret;
}

void TextServerExtension::font_clear_textures(const RID &p_font_rid, const Vector2i &p_size) {
	BRVIRTUAL_CALL(_font_clear_textures, p_font_rid, p_size);
}

void TextServerExtension::font_remove_texture(const RID &p_font_rid, const Vector2i &p_size, int64_t p_texture_index) {
	BRVIRTUAL_CALL(_font_remove_texture, p_font_rid, p_size, p_texture_index);
}

void TextServerExtension::font_set_texture_image(const RID &p_font_rid, const Vector2i &p_size, int64_t p_texture_index, const Ref<Image> &p_image) {
	BRVIRTUAL_CALL(_font_set_texture_image, p_font_rid, p_size, p_texture_index, p_image);
}

Ref<Image> TextServerExtension::font_get_texture_image(const RID &p_font_rid, const Vector2i &p_size, int64_t p_texture_index) const {
	Ref<Image> ret;
	BRVIRTUAL_CALL(_font_get_texture_image, p_font_rid, p_size, p_texture_index, ret);
	return ret;
}

void TextServerExtension::font_set_texture_offsets(const RID &p_font_rid, const Vector2i &p_size, int64_t p_texture_index, const PackedInt32Array &p_offset) {
	BRVIRTUAL_CALL(_font_set_texture_offsets, p_font_rid, p_size, p_texture_index, p_offset);
}

PackedInt32Array TextServerExtension::font_get_texture_offsets(const RID &p_font_rid, const Vector2i &p_size, int64_t p_texture_index) const {
	PackedInt32Array ret;
	BRVIRTUAL_CALL(_font_get_texture_offsets, p_font_rid, p_size, p_texture_index, ret);
	return ret;
}

PackedInt32Array TextServerExtension::font_get_glyph_list(const RID &p_font_rid, const Vector2i &p_size) const {
	PackedInt32Array ret;
	BRVIRTUAL_CALL(_font_get_glyph_list, p_font_rid, p_size, ret);
	return ret;
}

void TextServerExtension::font_clear_glyphs(const RID &p_font_rid, const Vector2i &p_size) {
	BRVIRTUAL_CALL(_font_clear_glyphs, p_font_rid, p_size);
}

void TextServerExtension::font_remove_glyph(const RID &p_font_rid, const Vector2i &p_size, int64_t p_glyph) {
	BRVIRTUAL_CALL(_font_remove_glyph, p_font_rid, p_size, p_glyph);
}

Vector2 TextServerExtension::font_get_glyph_advance(const RID &p_font_rid, int64_t p_size, int64_t p_glyph) const {
	Vector2 ret;
	BRVIRTUAL_CALL(_font_get_glyph_advance, p_font_rid, p_size, p_glyph, ret);
	return ret;
}

void TextServerExtension::font_set_glyph_advance(const RID &p_font_rid, int64_t p_size, int64_t p_glyph, const Vector2 &p_advance) {
	BRVIRTUAL_CALL(_font_set_glyph_advance, p_font_rid, p_size, p_glyph, p_advance);
}

Vector2 TextServerExtension::font_get_glyph_offset(const RID &p_font_rid, const Vector2i &p_size, int64_t p_glyph) const {
	Vector2 ret;
	BRVIRTUAL_CALL(_font_get_glyph_offset, p_font_rid, p_size, p_glyph, ret);
	return ret;
}

void TextServerExtension::font_set_glyph_offset(const RID &p_font_rid, const Vector2i &p_size, int64_t p_glyph, const Vector2 &p_offset) {
	BRVIRTUAL_CALL(_font_set_glyph_offset, p_font_rid, p_size, p_glyph, p_offset);
}

Vector2 TextServerExtension::font_get_glyph_size(const RID &p_font_rid, const Vector2i &p_size, int64_t p_glyph) const {
	Vector2 ret;
	BRVIRTUAL_CALL(_font_get_glyph_size, p_font_rid, p_size, p_glyph, ret);
	return ret;
}

void TextServerExtension::font_set_glyph_size(const RID &p_font_rid, const Vector2i &p_size, int64_t p_glyph, const Vector2 &p_gl_size) {
	BRVIRTUAL_CALL(_font_set_glyph_size, p_font_rid, p_size, p_glyph, p_gl_size);
}

Rect2 TextServerExtension::font_get_glyph_uv_rect(const RID &p_font_rid, const Vector2i &p_size, int64_t p_glyph) const {
	Rect2 ret;
	BRVIRTUAL_CALL(_font_get_glyph_uv_rect, p_font_rid, p_size, p_glyph, ret);
	return ret;
}

void TextServerExtension::font_set_glyph_uv_rect(const RID &p_font_rid, const Vector2i &p_size, int64_t p_glyph, const Rect2 &p_uv_rect) {
	BRVIRTUAL_CALL(_font_set_glyph_uv_rect, p_font_rid, p_size, p_glyph, p_uv_rect);
}

int64_t TextServerExtension::font_get_glyph_texture_idx(const RID &p_font_rid, const Vector2i &p_size, int64_t p_glyph) const {
	int64_t ret = 0;
	BRVIRTUAL_CALL(_font_get_glyph_texture_idx, p_font_rid, p_size, p_glyph, ret);
	return ret;
}

void TextServerExtension::font_set_glyph_texture_idx(const RID &p_font_rid, const Vector2i &p_size, int64_t p_glyph, int64_t p_texture_idx) {
	BRVIRTUAL_CALL(_font_set_glyph_texture_idx, p_font_rid, p_size, p_glyph, p_texture_idx);
}

RID TextServerExtension::font_get_glyph_texture_rid(const RID &p_font_rid, const Vector2i &p_size, int64_t p_glyph) const {
	RID ret;
	BRVIRTUAL_CALL(_font_get_glyph_texture_rid, p_font_rid, p_size, p_glyph, ret);
	return ret;
}

Size2 TextServerExtension::font_get_glyph_texture_size(const RID &p_font_rid, const Vector2i &p_size, int64_t p_glyph) const {
	Size2 ret;
	BRVIRTUAL_CALL(_font_get_glyph_texture_size, p_font_rid, p_size, p_glyph, ret);
	return ret;
}

Dictionary TextServerExtension::font_get_glyph_contours(const RID &p_font_rid, int64_t p_size, int64_t p_index) const {
	Dictionary ret;
	BRVIRTUAL_CALL(_font_get_glyph_contours, p_font_rid, p_size, p_index, ret);
	return ret;
}

TypedArray<Vector2i> TextServerExtension::font_get_kerning_list(const RID &p_font_rid, int64_t p_size) const {
	TypedArray<Vector2i> ret;
	BRVIRTUAL_CALL(_font_get_kerning_list, p_font_rid, p_size, ret);
	return ret;
}

void TextServerExtension::font_clear_kerning_map(const RID &p_font_rid, int64_t p_size) {
	BRVIRTUAL_CALL(_font_clear_kerning_map, p_font_rid, p_size);
}

void TextServerExtension::font_remove_kerning(const RID &p_font_rid, int64_t p_size, const Vector2i &p_glyph_pair) {
	BRVIRTUAL_CALL(_font_remove_kerning, p_font_rid, p_size, p_glyph_pair);
}

void TextServerExtension::font_set_kerning(const RID &p_font_rid, int64_t p_size, const Vector2i &p_glyph_pair, const Vector2 &p_kerning) {
	BRVIRTUAL_CALL(_font_set_kerning, p_font_rid, p_size, p_glyph_pair, p_kerning);
}

Vector2 TextServerExtension::font_get_kerning(const RID &p_font_rid, int64_t p_size, const Vector2i &p_glyph_pair) const {
	Vector2 ret;
	BRVIRTUAL_CALL(_font_get_kerning, p_font_rid, p_size, p_glyph_pair, ret);
	return ret;
}

int64_t TextServerExtension::font_get_glyph_index(const RID &p_font_rid, int64_t p_size, int64_t p_char, int64_t p_variation_selector) const {
	int64_t ret = 0;
	BRVIRTUAL_CALL(_font_get_glyph_index, p_font_rid, p_size, p_char, p_variation_selector, ret);
	return ret;
}

int64_t TextServerExtension::font_get_char_from_glyph_index(const RID &p_font_rid, int64_t p_size, int64_t p_glyph_index) const {
	int64_t ret = 0;
	BRVIRTUAL_CALL(_font_get_char_from_glyph_index, p_font_rid, p_size, p_glyph_index, ret);
	return ret;
}

bool TextServerExtension::font_has_char(const RID &p_font_rid, int64_t p_char) const {
	bool ret = false;
	BRVIRTUAL_CALL(_font_has_char, p_font_rid, p_char, ret);
	return ret;
}

String TextServerExtension::font_get_supported_chars(const RID &p_font_rid) const {
	String ret;
	BRVIRTUAL_CALL(_font_get_supported_chars, p_font_rid, ret);
	return ret;
}

PackedInt32Array TextServerExtension::font_get_supported_glyphs(const RID &p_font_rid) const {
	PackedInt32Array ret;
	BRVIRTUAL_CALL(_font_get_supported_glyphs, p_font_rid, ret);
	return ret;
}

void TextServerExtension::font_render_range(const RID &p_font_rid, const Vector2i &p_size, int64_t p_start, int64_t p_end) {
	BRVIRTUAL_CALL(_font_render_range, p_font_rid, p_size, p_start, p_end);
}

void TextServerExtension::font_render_glyph(const RID &p_font_rid, const Vector2i &p_size, int64_t p_index) {
	BRVIRTUAL_CALL(_font_render_glyph, p_font_rid, p_size, p_index);
}

void TextServerExtension::font_draw_glyph(const RID &p_font_rid, const RID &p_canvas, int64_t p_size, const Vector2 &p_pos, int64_t p_index, const Color &p_color) const {
	BRVIRTUAL_CALL(_font_draw_glyph, p_font_rid, p_canvas, p_size, p_pos, p_index, p_color);
}

void TextServerExtension::font_draw_glyph_outline(const RID &p_font_rid, const RID &p_canvas, int64_t p_size, int64_t p_outline_size, const Vector2 &p_pos, int64_t p_index, const Color &p_color) const {
	BRVIRTUAL_CALL(_font_draw_glyph_outline, p_font_rid, p_canvas, p_size, p_outline_size, p_pos, p_index, p_color);
}

bool TextServerExtension::font_is_language_supported(const RID &p_font_rid, const String &p_language) const {
	bool ret = false;
	BRVIRTUAL_CALL(_font_is_language_supported, p_font_rid, p_language, ret);
	return ret;
}

void TextServerExtension::font_set_language_support_override(const RID &p_font_rid, const String &p_language, bool p_supported) {
	BRVIRTUAL_CALL(_font_set_language_support_override, p_font_rid, p_language, p_supported);
}

bool TextServerExtension::font_get_language_support_override(const RID &p_font_rid, const String &p_language) {
	bool ret = false;
	BRVIRTUAL_CALL(_font_get_language_support_override, p_font_rid, p_language, ret);
	return ret;
}

void TextServerExtension::font_remove_language_support_override(const RID &p_font_rid, const String &p_language) {
	BRVIRTUAL_CALL(_font_remove_language_support_override, p_font_rid, p_language);
}

PackedStringArray TextServerExtension::font_get_language_support_overrides(const RID &p_font_rid) {
	PackedStringArray ret;
	BRVIRTUAL_CALL(_font_get_language_support_overrides, p_font_rid, ret);
	return ret;
}

bool TextServerExtension::font_is_script_supported(const RID &p_font_rid, const String &p_script) const {
	bool ret = false;
	BRVIRTUAL_CALL(_font_is_script_supported, p_font_rid, p_script, ret);
	return ret;
}

void TextServerExtension::font_set_script_support_override(const RID &p_font_rid, const String &p_script, bool p_supported) {
	BRVIRTUAL_CALL(_font_set_script_support_override, p_font_rid, p_script, p_supported);
}

bool TextServerExtension::font_get_script_support_override(const RID &p_font_rid, const String &p_script) {
	bool ret = false;
	BRVIRTUAL_CALL(_font_get_script_support_override, p_font_rid, p_script, ret);
	return ret;
}

void TextServerExtension::font_remove_script_support_override(const RID &p_font_rid, const String &p_script) {
	BRVIRTUAL_CALL(_font_remove_script_support_override, p_font_rid, p_script);
}

PackedStringArray TextServerExtension::font_get_script_support_overrides(const RID &p_font_rid) {
	PackedStringArray ret;
	BRVIRTUAL_CALL(_font_get_script_support_overrides, p_font_rid, ret);
	return ret;
}

void TextServerExtension::font_set_opentype_feature_overrides(const RID &p_font_rid, const Dictionary &p_overrides) {
	BRVIRTUAL_CALL(_font_set_opentype_feature_overrides, p_font_rid, p_overrides);
}

Dictionary TextServerExtension::font_get_opentype_feature_overrides(const RID &p_font_rid) const {
	Dictionary ret;
	BRVIRTUAL_CALL(_font_get_opentype_feature_overrides, p_font_rid, ret);
	return ret;
}

Dictionary TextServerExtension::font_supported_feature_list(const RID &p_font_rid) const {
	Dictionary ret;
	BRVIRTUAL_CALL(_font_supported_feature_list, p_font_rid, ret);
	return ret;
}

Dictionary TextServerExtension::font_supported_variation_list(const RID &p_font_rid) const {
	Dictionary ret;
	BRVIRTUAL_CALL(_font_supported_variation_list, p_font_rid, ret);
	return ret;
}

double TextServerExtension::font_get_global_oversampling() const {
	double ret = 0;
	BRVIRTUAL_CALL(_font_get_global_oversampling, ret);
	return ret;
}

void TextServerExtension::font_set_global_oversampling(double p_oversampling) {
	BRVIRTUAL_CALL(_font_set_global_oversampling, p_oversampling);
}

Vector2 TextServerExtension::get_hex_code_box_size(int64_t p_size, int64_t p_index) const {
	Vector2 ret;
	if (BRVIRTUAL_CALL(_get_hex_code_box_size, p_size, p_index, ret)) {
		return ret;
	}
	return TextServer::get_hex_code_box_size(p_size, p_index);
}

void TextServerExtension::draw_hex_code_box(const RID &p_canvas, int64_t p_size, const Vector2 &p_pos, int64_t p_index, const Color &p_color) const {
	if (!BRVIRTUAL_CALL(_draw_hex_code_box, p_canvas, p_size, p_pos, p_index, p_color)) {
		TextServer::draw_hex_code_box(p_canvas, p_size, p_pos, p_index, p_color);
	}
}

/*************************************************************************/
/* Shaped text buffer interface                                          */
/*************************************************************************/

RID TextServerExtension::create_shaped_text(TextServer::Direction p_direction, TextServer::Orientation p_orientation) {
	RID ret;
	BRVIRTUAL_CALL(_create_shaped_text, p_direction, p_orientation, ret);
	return ret;
}

void TextServerExtension::shaped_text_clear(const RID &p_shaped) {
	BRVIRTUAL_CALL(_shaped_text_clear, p_shaped);
}

void TextServerExtension::shaped_text_set_direction(const RID &p_shaped, TextServer::Direction p_direction) {
	BRVIRTUAL_CALL(_shaped_text_set_direction, p_shaped, p_direction);
}

TextServer::Direction TextServerExtension::shaped_text_get_direction(const RID &p_shaped) const {
	TextServer::Direction ret = TextServer::DIRECTION_AUTO;
	BRVIRTUAL_CALL(_shaped_text_get_direction, p_shaped, ret);
	return ret;
}

TextServer::Direction TextServerExtension::shaped_text_get_inferred_direction(const RID &p_shaped) const {
	TextServer::Direction ret = TextServer::DIRECTION_LTR;
	BRVIRTUAL_CALL(_shaped_text_get_inferred_direction, p_shaped, ret);
	return ret;
}

void TextServerExtension::shaped_text_set_orientation(const RID &p_shaped, TextServer::Orientation p_orientation) {
	BRVIRTUAL_CALL(_shaped_text_set_orientation, p_shaped, p_orientation);
}

TextServer::Orientation TextServerExtension::shaped_text_get_orientation(const RID &p_shaped) const {
	TextServer::Orientation ret = TextServer::ORIENTATION_HORIZONTAL;
	BRVIRTUAL_CALL(_shaped_text_get_orientation, p_shaped, ret);
	return ret;
}

void TextServerExtension::shaped_text_set_bidi_override(const RID &p_shaped, const Array &p_override) {
	BRVIRTUAL_CALL(_shaped_text_set_bidi_override, p_shaped, p_override);
}

void TextServerExtension::shaped_text_set_custom_punctuation(const RID &p_shaped, const String &p_punct) {
	BRVIRTUAL_CALL(_shaped_text_set_custom_punctuation, p_shaped, p_punct);
}

String TextServerExtension::shaped_text_get_custom_punctuation(const RID &p_shaped) const {
	String ret;
	BRVIRTUAL_CALL(_shaped_text_get_custom_punctuation, p_shaped, ret);
	return ret;
}

void TextServerExtension::shaped_text_set_custom_ellipsis(const RID &p_shaped, int64_t p_char) {
	BRVIRTUAL_CALL(_shaped_text_set_custom_ellipsis, p_shaped, p_char);
}

int64_t TextServerExtension::shaped_text_get_custom_ellipsis(const RID &p_shaped) const {
	int64_t ret = 0;
	BRVIRTUAL_CALL(_shaped_text_get_custom_ellipsis, p_shaped, ret);
	return ret;
}

void TextServerExtension::shaped_text_set_preserve_invalid(const RID &p_shaped, bool p_enabled) {
	BRVIRTUAL_CALL(_shaped_text_set_preserve_invalid, p_shaped, p_enabled);
}

bool TextServerExtension::shaped_text_get_preserve_invalid(const RID &p_shaped) const {
	bool ret = false;
	BRVIRTUAL_CALL(_shaped_text_get_preserve_invalid, p_shaped, ret);
	return ret;
}

void TextServerExtension::shaped_text_set_preserve_control(const RID &p_shaped, bool p_enabled) {
	BRVIRTUAL_CALL(_shaped_text_set_preserve_control, p_shaped, p_enabled);
}

bool TextServerExtension::shaped_text_get_preserve_control(const RID &p_shaped) const {
	bool ret = false;
	BRVIRTUAL_CALL(_shaped_text_get_preserve_control, p_shaped, ret);
	return ret;
}

void TextServerExtension::shaped_text_set_spacing(const RID &p_shaped, TextServer::SpacingType p_spacing, int64_t p_value) {
	BRVIRTUAL_CALL(_shaped_text_set_spacing, p_shaped, p_spacing, p_value);
}

int64_t TextServerExtension::shaped_text_get_spacing(const RID &p_shaped, TextServer::SpacingType p_spacing) const {
	int64_t ret = 0;
	BRVIRTUAL_CALL(_shaped_text_get_spacing, p_shaped, p_spacing, ret);
	return ret;
}

bool TextServerExtension::shaped_text_add_string(const RID &p_shaped, const String &p_text, const TypedArray<RID> &p_fonts, int64_t p_size, const Dictionary &p_opentype_features, const String &p_language, const Variant &p_meta) {
	bool ret = false;
	BRVIRTUAL_CALL(_shaped_text_add_string, p_shaped, p_text, p_fonts, p_size, p_opentype_features, p_language, p_meta, ret);
	return ret;
}

bool TextServerExtension::shaped_text_add_object(const RID &p_shaped, const Variant &p_key, const Size2 &p_size, InlineAlignment p_inline_align, int64_t p_length, double p_baseline) {
	bool ret = false;
	BRVIRTUAL_CALL(_shaped_text_add_object, p_shaped, p_key, p_size, p_inline_align, p_length, p_baseline, ret);
	return ret;
}

bool TextServerExtension::shaped_text_resize_object(const RID &p_shaped, const Variant &p_key, const Size2 &p_size, InlineAlignment p_inline_align, double p_baseline) {
	bool ret = false;
	BRVIRTUAL_CALL(_shaped_text_resize_object, p_shaped, p_key, p_size, p_inline_align, p_baseline, ret);
	return ret;
}

int64_t TextServerExtension::shaped_get_span_count(const RID &p_shaped) const {
	int64_t ret = 0;
	BRVIRTUAL_CALL(_shaped_get_span_count, p_shaped, ret);
	return ret;
}

Variant TextServerExtension::shaped_get_span_meta(const RID &p_shaped, int64_t p_index) const {
	Variant ret = false;
	BRVIRTUAL_CALL(_shaped_get_span_meta, p_shaped, p_index, ret);
	return ret;
}

void TextServerExtension::shaped_set_span_update_font(const RID &p_shaped, int64_t p_index, const TypedArray<RID> &p_fonts, int64_t p_size, const Dictionary &p_opentype_features) {
	BRVIRTUAL_CALL(_shaped_set_span_update_font, p_shaped, p_index, p_fonts, p_size, p_opentype_features);
}

RID TextServerExtension::shaped_text_substr(const RID &p_shaped, int64_t p_start, int64_t p_length) const {
	RID ret;
	BRVIRTUAL_CALL(_shaped_text_substr, p_shaped, p_start, p_length, ret);
	return ret;
}

RID TextServerExtension::shaped_text_get_parent(const RID &p_shaped) const {
	RID ret;
	BRVIRTUAL_CALL(_shaped_text_get_parent, p_shaped, ret);
	return ret;
}

double TextServerExtension::shaped_text_fit_to_width(const RID &p_shaped, double p_width, BitField<TextServer::JustificationFlag> p_jst_flags) {
	double ret = 0;
	BRVIRTUAL_CALL(_shaped_text_fit_to_width, p_shaped, p_width, p_jst_flags, ret);
	return ret;
}

double TextServerExtension::shaped_text_tab_align(const RID &p_shaped, const PackedFloat32Array &p_tab_stops) {
	double ret = 0;
	BRVIRTUAL_CALL(_shaped_text_tab_align, p_shaped, p_tab_stops, ret);
	return ret;
}

bool TextServerExtension::shaped_text_shape(const RID &p_shaped) {
	bool ret = false;
	BRVIRTUAL_CALL(_shaped_text_shape, p_shaped, ret);
	return ret;
}

bool TextServerExtension::shaped_text_update_breaks(const RID &p_shaped) {
	bool ret = false;
	BRVIRTUAL_CALL(_shaped_text_update_breaks, p_shaped, ret);
	return ret;
}

bool TextServerExtension::shaped_text_update_justification_ops(const RID &p_shaped) {
	bool ret = false;
	BRVIRTUAL_CALL(_shaped_text_update_justification_ops, p_shaped, ret);
	return ret;
}

bool TextServerExtension::shaped_text_is_ready(const RID &p_shaped) const {
	bool ret = false;
	BRVIRTUAL_CALL(_shaped_text_is_ready, p_shaped, ret);
	return ret;
}

const Glyph *TextServerExtension::shaped_text_get_glyphs(const RID &p_shaped) const {
	BRExtensionConstPtr<const Glyph> ret;
	BRVIRTUAL_CALL(_shaped_text_get_glyphs, p_shaped, ret);
	return ret;
}

const Glyph *TextServerExtension::shaped_text_sort_logical(const RID &p_shaped) {
	BRExtensionConstPtr<const Glyph> ret;
	BRVIRTUAL_CALL(_shaped_text_sort_logical, p_shaped, ret);
	return ret;
}

int64_t TextServerExtension::shaped_text_get_glyph_count(const RID &p_shaped) const {
	int64_t ret = 0;
	BRVIRTUAL_CALL(_shaped_text_get_glyph_count, p_shaped, ret);
	return ret;
}

Vector2i TextServerExtension::shaped_text_get_range(const RID &p_shaped) const {
	Vector2i ret;
	BRVIRTUAL_CALL(_shaped_text_get_range, p_shaped, ret);
	return ret;
}

PackedInt32Array TextServerExtension::shaped_text_get_line_breaks_adv(const RID &p_shaped, const PackedFloat32Array &p_width, int64_t p_start, bool p_once, BitField<TextServer::LineBreakFlag> p_break_flags) const {
	PackedInt32Array ret;
	if (BRVIRTUAL_CALL(_shaped_text_get_line_breaks_adv, p_shaped, p_width, p_start, p_once, p_break_flags, ret)) {
		return ret;
	}
	return TextServer::shaped_text_get_line_breaks_adv(p_shaped, p_width, p_start, p_once, p_break_flags);
}

PackedInt32Array TextServerExtension::shaped_text_get_line_breaks(const RID &p_shaped, double p_width, int64_t p_start, BitField<TextServer::LineBreakFlag> p_break_flags) const {
	PackedInt32Array ret;
	if (BRVIRTUAL_CALL(_shaped_text_get_line_breaks, p_shaped, p_width, p_start, p_break_flags, ret)) {
		return ret;
	}
	return TextServer::shaped_text_get_line_breaks(p_shaped, p_width, p_start, p_break_flags);
}

PackedInt32Array TextServerExtension::shaped_text_get_word_breaks(const RID &p_shaped, BitField<TextServer::GraphemeFlag> p_grapheme_flags, BitField<TextServer::GraphemeFlag> p_skip_grapheme_flags) const {
	PackedInt32Array ret;
	if (BRVIRTUAL_CALL(_shaped_text_get_word_breaks, p_shaped, p_grapheme_flags, p_skip_grapheme_flags, ret)) {
		return ret;
	}
	return TextServer::shaped_text_get_word_breaks(p_shaped, p_grapheme_flags, p_skip_grapheme_flags);
}

int64_t TextServerExtension::shaped_text_get_trim_pos(const RID &p_shaped) const {
	int64_t ret = -1;
	BRVIRTUAL_CALL(_shaped_text_get_trim_pos, p_shaped, ret);
	return ret;
}

int64_t TextServerExtension::shaped_text_get_ellipsis_pos(const RID &p_shaped) const {
	int64_t ret = -1;
	BRVIRTUAL_CALL(_shaped_text_get_ellipsis_pos, p_shaped, ret);
	return ret;
}

const Glyph *TextServerExtension::shaped_text_get_ellipsis_glyphs(const RID &p_shaped) const {
	BRExtensionConstPtr<const Glyph> ret;
	BRVIRTUAL_CALL(_shaped_text_get_ellipsis_glyphs, p_shaped, ret);
	return ret;
}

int64_t TextServerExtension::shaped_text_get_ellipsis_glyph_count(const RID &p_shaped) const {
	int64_t ret = -1;
	BRVIRTUAL_CALL(_shaped_text_get_ellipsis_glyph_count, p_shaped, ret);
	return ret;
}

void TextServerExtension::shaped_text_overrun_trim_to_width(const RID &p_shaped_line, double p_width, BitField<TextServer::TextOverrunFlag> p_trim_flags) {
	BRVIRTUAL_CALL(_shaped_text_overrun_trim_to_width, p_shaped_line, p_width, p_trim_flags);
}

Array TextServerExtension::shaped_text_get_objects(const RID &p_shaped) const {
	Array ret;
	BRVIRTUAL_CALL(_shaped_text_get_objects, p_shaped, ret);
	return ret;
}

Rect2 TextServerExtension::shaped_text_get_object_rect(const RID &p_shaped, const Variant &p_key) const {
	Rect2 ret;
	BRVIRTUAL_CALL(_shaped_text_get_object_rect, p_shaped, p_key, ret);
	return ret;
}

Vector2i TextServerExtension::shaped_text_get_object_range(const RID &p_shaped, const Variant &p_key) const {
	Vector2i ret;
	BRVIRTUAL_CALL(_shaped_text_get_object_range, p_shaped, p_key, ret);
	return ret;
}

int64_t TextServerExtension::shaped_text_get_object_glyph(const RID &p_shaped, const Variant &p_key) const {
	int64_t ret = -1;
	BRVIRTUAL_CALL(_shaped_text_get_object_glyph, p_shaped, p_key, ret);
	return ret;
}

Size2 TextServerExtension::shaped_text_get_size(const RID &p_shaped) const {
	Size2 ret;
	BRVIRTUAL_CALL(_shaped_text_get_size, p_shaped, ret);
	return ret;
}

double TextServerExtension::shaped_text_get_ascent(const RID &p_shaped) const {
	double ret = 0;
	BRVIRTUAL_CALL(_shaped_text_get_ascent, p_shaped, ret);
	return ret;
}

double TextServerExtension::shaped_text_get_descent(const RID &p_shaped) const {
	double ret = 0;
	BRVIRTUAL_CALL(_shaped_text_get_descent, p_shaped, ret);
	return ret;
}

double TextServerExtension::shaped_text_get_width(const RID &p_shaped) const {
	double ret = 0;
	BRVIRTUAL_CALL(_shaped_text_get_width, p_shaped, ret);
	return ret;
}

double TextServerExtension::shaped_text_get_underline_position(const RID &p_shaped) const {
	double ret = 0;
	BRVIRTUAL_CALL(_shaped_text_get_underline_position, p_shaped, ret);
	return ret;
}

double TextServerExtension::shaped_text_get_underline_thickness(const RID &p_shaped) const {
	double ret = 0;
	BRVIRTUAL_CALL(_shaped_text_get_underline_thickness, p_shaped, ret);
	return ret;
}

TextServer::Direction TextServerExtension::shaped_text_get_dominant_direction_in_range(const RID &p_shaped, int64_t p_start, int64_t p_end) const {
	int64_t ret;
	if (BRVIRTUAL_CALL(_shaped_text_get_dominant_direction_in_range, p_shaped, p_start, p_end, ret)) {
		return (TextServer::Direction)ret;
	}
	return TextServer::shaped_text_get_dominant_direction_in_range(p_shaped, p_start, p_end);
}

CaretInfo TextServerExtension::shaped_text_get_carets(const RID &p_shaped, int64_t p_position) const {
	CaretInfo ret;
	if (BRVIRTUAL_CALL(_shaped_text_get_carets, p_shaped, p_position, &ret)) {
		return ret;
	}
	return TextServer::shaped_text_get_carets(p_shaped, p_position);
}

Vector<Vector2> TextServerExtension::shaped_text_get_selection(const RID &p_shaped, int64_t p_start, int64_t p_end) const {
	Vector<Vector2> ret;
	if (BRVIRTUAL_CALL(_shaped_text_get_selection, p_shaped, p_start, p_end, ret)) {
		return ret;
	}
	return TextServer::shaped_text_get_selection(p_shaped, p_start, p_end);
}

int64_t TextServerExtension::shaped_text_hit_test_grapheme(const RID &p_shaped, double p_coords) const {
	int64_t ret;
	if (BRVIRTUAL_CALL(_shaped_text_hit_test_grapheme, p_shaped, p_coords, ret)) {
		return ret;
	}
	return TextServer::shaped_text_hit_test_grapheme(p_shaped, p_coords);
}

int64_t TextServerExtension::shaped_text_hit_test_position(const RID &p_shaped, double p_coords) const {
	int64_t ret;
	if (BRVIRTUAL_CALL(_shaped_text_hit_test_position, p_shaped, p_coords, ret)) {
		return ret;
	}
	return TextServer::shaped_text_hit_test_position(p_shaped, p_coords);
}

void TextServerExtension::shaped_text_draw(const RID &p_shaped, const RID &p_canvas, const Vector2 &p_pos, double p_clip_l, double p_clip_r, const Color &p_color) const {
	if (BRVIRTUAL_CALL(_shaped_text_draw, p_shaped, p_canvas, p_pos, p_clip_l, p_clip_r, p_color)) {
		return;
	}
	TextServer::shaped_text_draw(p_shaped, p_canvas, p_pos, p_clip_l, p_clip_r, p_color);
}

void TextServerExtension::shaped_text_draw_outline(const RID &p_shaped, const RID &p_canvas, const Vector2 &p_pos, double p_clip_l, double p_clip_r, int64_t p_outline_size, const Color &p_color) const {
	if (BRVIRTUAL_CALL(_shaped_text_draw_outline, p_shaped, p_canvas, p_pos, p_clip_l, p_clip_r, p_outline_size, p_color)) {
		return;
	}
	TextServer::shaped_text_draw_outline(p_shaped, p_canvas, p_pos, p_clip_l, p_clip_r, p_outline_size, p_color);
}

Vector2 TextServerExtension::shaped_text_get_grapheme_bounds(const RID &p_shaped, int64_t p_pos) const {
	Vector2 ret;
	if (BRVIRTUAL_CALL(_shaped_text_get_grapheme_bounds, p_shaped, p_pos, ret)) {
		return ret;
	}
	return TextServer::shaped_text_get_grapheme_bounds(p_shaped, p_pos);
}

int64_t TextServerExtension::shaped_text_next_grapheme_pos(const RID &p_shaped, int64_t p_pos) const {
	int64_t ret;
	if (BRVIRTUAL_CALL(_shaped_text_next_grapheme_pos, p_shaped, p_pos, ret)) {
		return ret;
	}
	return TextServer::shaped_text_next_grapheme_pos(p_shaped, p_pos);
}

int64_t TextServerExtension::shaped_text_prev_grapheme_pos(const RID &p_shaped, int64_t p_pos) const {
	int64_t ret;
	if (BRVIRTUAL_CALL(_shaped_text_prev_grapheme_pos, p_shaped, p_pos, ret)) {
		return ret;
	}
	return TextServer::shaped_text_prev_grapheme_pos(p_shaped, p_pos);
}

PackedInt32Array TextServerExtension::shaped_text_get_character_breaks(const RID &p_shaped) const {
	PackedInt32Array ret;
	if (BRVIRTUAL_CALL(_shaped_text_get_character_breaks, p_shaped, ret)) {
		return ret;
	}
	return PackedInt32Array();
}

int64_t TextServerExtension::shaped_text_next_character_pos(const RID &p_shaped, int64_t p_pos) const {
	int64_t ret;
	if (BRVIRTUAL_CALL(_shaped_text_next_character_pos, p_shaped, p_pos, ret)) {
		return ret;
	}
	return TextServer::shaped_text_next_character_pos(p_shaped, p_pos);
}

int64_t TextServerExtension::shaped_text_prev_character_pos(const RID &p_shaped, int64_t p_pos) const {
	int64_t ret;
	if (BRVIRTUAL_CALL(_shaped_text_prev_character_pos, p_shaped, p_pos, ret)) {
		return ret;
	}
	return TextServer::shaped_text_prev_character_pos(p_shaped, p_pos);
}

int64_t TextServerExtension::shaped_text_closest_character_pos(const RID &p_shaped, int64_t p_pos) const {
	int64_t ret;
	if (BRVIRTUAL_CALL(_shaped_text_closest_character_pos, p_shaped, p_pos, ret)) {
		return ret;
	}
	return TextServer::shaped_text_closest_character_pos(p_shaped, p_pos);
}

String TextServerExtension::format_number(const String &p_string, const String &p_language) const {
	String ret;
	if (BRVIRTUAL_CALL(_format_number, p_string, p_language, ret)) {
		return ret;
	}
	return p_string;
}

String TextServerExtension::parse_number(const String &p_string, const String &p_language) const {
	String ret;
	if (BRVIRTUAL_CALL(_parse_number, p_string, p_language, ret)) {
		return ret;
	}
	return p_string;
}

String TextServerExtension::percent_sign(const String &p_language) const {
	String ret = "%";
	BRVIRTUAL_CALL(_percent_sign, p_language, ret);
	return ret;
}

bool TextServerExtension::is_valid_identifier(const String &p_string) const {
	bool ret;
	if (BRVIRTUAL_CALL(_is_valid_identifier, p_string, ret)) {
		return ret;
	}
	return TextServer::is_valid_identifier(p_string);
}

bool TextServerExtension::is_valid_letter(uint64_t p_unicode) const {
	bool ret;
	if (BRVIRTUAL_CALL(_is_valid_letter, p_unicode, ret)) {
		return ret;
	}
	return TextServer::is_valid_letter(p_unicode);
}

String TextServerExtension::strip_diacritics(const String &p_string) const {
	String ret;
	if (BRVIRTUAL_CALL(_strip_diacritics, p_string, ret)) {
		return ret;
	}
	return TextServer::strip_diacritics(p_string);
}

String TextServerExtension::string_to_upper(const String &p_string, const String &p_language) const {
	String ret;
	if (BRVIRTUAL_CALL(_string_to_upper, p_string, p_language, ret)) {
		return ret;
	}
	return p_string;
}

String TextServerExtension::string_to_title(const String &p_string, const String &p_language) const {
	String ret;
	if (BRVIRTUAL_CALL(_string_to_title, p_string, p_language, ret)) {
		return ret;
	}
	return p_string;
}

String TextServerExtension::string_to_lower(const String &p_string, const String &p_language) const {
	String ret;
	if (BRVIRTUAL_CALL(_string_to_lower, p_string, p_language, ret)) {
		return ret;
	}
	return p_string;
}

TypedArray<Vector3i> TextServerExtension::parse_structured_text(StructuredTextParser p_parser_type, const Array &p_args, const String &p_text) const {
	TypedArray<Vector3i> ret;
	if (BRVIRTUAL_CALL(_parse_structured_text, p_parser_type, p_args, p_text, ret)) {
		return ret;
	}
	return TextServer::parse_structured_text(p_parser_type, p_args, p_text);
}

PackedInt32Array TextServerExtension::string_get_word_breaks(const String &p_string, const String &p_language, int64_t p_chars_per_line) const {
	PackedInt32Array ret;
	BRVIRTUAL_CALL(_string_get_word_breaks, p_string, p_language, p_chars_per_line, ret);
	return ret;
}

PackedInt32Array TextServerExtension::string_get_character_breaks(const String &p_string, const String &p_language) const {
	PackedInt32Array ret;
	if (BRVIRTUAL_CALL(_string_get_character_breaks, p_string, p_language, ret)) {
		return ret;
	}
	return TextServer::string_get_character_breaks(p_string, p_language);
}

int64_t TextServerExtension::is_confusable(const String &p_string, const PackedStringArray &p_dict) const {
	int64_t ret;
	if (BRVIRTUAL_CALL(_is_confusable, p_string, p_dict, ret)) {
		return ret;
	}
	return TextServer::is_confusable(p_string, p_dict);
}

bool TextServerExtension::spoof_check(const String &p_string) const {
	bool ret;
	if (BRVIRTUAL_CALL(_spoof_check, p_string, ret)) {
		return ret;
	}
	return TextServer::spoof_check(p_string);
}

void TextServerExtension::cleanup() {
	BRVIRTUAL_CALL(_cleanup);
}

TextServerExtension::TextServerExtension() {
	//NOP
}

TextServerExtension::~TextServerExtension() {
	//NOP
}
