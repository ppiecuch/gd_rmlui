/*
 * This source file is part of RmlUi, the HTML/CSS Interface Middleware
 *
 * For the latest information, see http://github.com/mikke89/RmlUi
 *
 * Copyright (c) 2008-2010 CodePoint Ltd, Shift Technology Ltd
 * Copyright (c) 2019 The RmlUi Team, and contributors
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#include "GodotFontInterface.h"

#include <string>
#include <map>

static std::map<std::tuple<std::string, int, int, int>, Ref<Font>> _font_cache;

#ifdef MODULE_FREETYPE_ENABLED
static Ref<Font> _get_dynamic_font(const std::string &path, int size, int outline_size, Color outline_color, int stretch) {
	std::tuple<std::string, int, int, int> key = std::make_tuple(path, size, stretch, outline_size);
	if (_font_cache.count(key)) {
		return _font_cache[key];
	}
	Ref<DynamicFont> font = memnew(DynamicFont);
	font->set_font_data(ResourceLoader::load(path.c_str()));
	font->set_size(size);
	if (stretch > 0 && stretch < 100) {
		font->set_stretch_scale(stretch);
	}
	if (outline_size > 0) {
		font->set_outline_size(outline_size);
		font->set_outline_color(outline_color);
	}
	_font_cache[key] = font;
	return font;
}

#endif // MODULE_FREETYPE_ENABLED

static Ref<BitmapFont> _get_default_bitmap_font() {
	static Ref<BitmapFont> bfont;
	if (bfont.is_null()) {
		bfont.instance();
		Ref<BitmapFont> _font = memnew(BitmapFont);
		if (_font->create_from_fnt_ptr(_default_bitmap_fnt, strlen(_default_bitmap_fnt), _default_bitmap_png) != OK) {
			// this should not happen, since all data are embedded
			// if so, we are rather running out of resources/crashing
			WARN_PRINT("Failed to load default bitmap font.");
		}
	}
	return bfont;
}


GodotFontInterface::GodotFontInterface() {}

GodotFontInterface::~GodotFontInterface() {}

bool GodotFontInterface::LoadFontFace(const String& /*file_name*/, bool /*fallback_face*/)
{
	return false;
}

bool GodotFontInterface::LoadFontFace(const byte* /*data*/, int /*data_size*/, const String& /*font_family*/, Style::FontStyle /*style*/,
	Style::FontWeight /*weight*/, bool /*fallback_face*/)
{
	return false;
}

FontFaceHandle GodotFontInterface::GetFontFaceHandle(const String& /*family*/, Style::FontStyle /*style*/, Style::FontWeight /*weight*/,
	int /*size*/)
{
	return 0;
}

FontEffectsHandle GodotFontInterface::PrepareFontEffects(FontFaceHandle /*handle*/, const FontEffectList& /*font_effects*/)
{
	return 0;
}

int GodotFontInterface::GetSize(FontFaceHandle /*handle*/)
{
	return 0;
}

int GodotFontInterface::GetXHeight(FontFaceHandle /*handle*/)
{
	return 0;
}

int GodotFontInterface::GetLineHeight(FontFaceHandle /*handle*/)
{
	return 0;
}

int GodotFontInterface::GetBaseline(FontFaceHandle /*handle*/)
{
	return 0;
}

float GodotFontInterface::GetUnderline(FontFaceHandle /*handle*/, float& /*thickness*/)
{
	return 0;
}

int GodotFontInterface::GetStringWidth(FontFaceHandle /*handle*/, const String& /*string*/, Character /*prior_character*/)
{
	return 0;
}

int GodotFontInterface::GenerateString(FontFaceHandle /*face_handle*/, FontEffectsHandle /*font_effects_handle*/, const String& /*string*/,
	const Vector2f& /*position*/, const Colourb& /*colour*/, float /*opacity*/, GeometryList& /*geometry*/)
{
	return 0;
}

int GodotFontInterface::GetVersion(FontFaceHandle /*handle*/)
{
	return 0;
}
