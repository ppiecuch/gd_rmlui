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

#include "Godot_Platform.h"

#include "core/variant.h"
#include "core/os/os.h"
#include "core/os/file_access.h"
#include "scene/resources/font.h"
#include "scene/resources/dynamic_font.h"

#include <string>
#include <map>

#include "default_bitmap.gen.h"

static std::map<std::tuple<std::string, int, int, int>, Ref<Font>> _font_cache;

#define FAILURE (-1)

/// SystemInterface

// Get the number of seconds elapsed since the start of the application
double GodotSystemInterface::GetElapsedTime()
{
	return static_cast<float>(OS::get_singleton()->get_ticks_msec()/1000.);
}


/// FileInterface

GodotFileInterface::GodotFileInterface(const Rml::String& root) : root(root) { }
GodotFileInterface::~GodotFileInterface() { }

// Opens a file.
Rml::FileHandle GodotFileInterface::Open(const Rml::String& path)
{
	// Attempt to open the file relative to the application's root.
	if (FileAccess *fa = FileAccess::open((root + path).c_str(), FileAccess::READ))
	{
		return (Rml::FileHandle) fa;
	}

	// Attempt to open the file relative to the current working directory.
	if (FileAccess *fa = FileAccess::open(path.c_str(), FileAccess::READ))
	{
		return (Rml::FileHandle) fa;
	}

	return 0;
}

// Closes a previously opened file.
void GodotFileInterface::Close(Rml::FileHandle file)
{
	if (FileAccess *fa = reinterpret_cast<FileAccess*>(file))
	{
		fa->close();
		memdelete(fa);
	}
}

// Reads data from a previously opened file.
size_t GodotFileInterface::Read(void* buffer, size_t size, Rml::FileHandle file)
{
	if (FileAccess *fa = reinterpret_cast<FileAccess*>(file))
	{
		return fa->get_buffer(static_cast<uint8_t*>(buffer), size);
	}
	return 0;
}

// Seeks to a point in a previously opened file.
bool GodotFileInterface::Seek(Rml::FileHandle file, long offset, int origin)
{
	if (FileAccess *fa = reinterpret_cast<FileAccess*>(file))
	{
		switch (origin)
		{
			case SEEK_SET: fa->seek(offset); break;
			case SEEK_CUR: fa->seek(fa->get_position() + offset); break;
			case SEEK_END: fa->seek_end(offset); break;
			default: return FAILURE;
		}
	}
	return FAILURE;
}

// Returns the current position of the file pointer.
size_t GodotFileInterface::Tell(Rml::FileHandle file)
{
	if (FileAccess *fa = reinterpret_cast<FileAccess*>(file))
	{
		return fa->get_position();
	}
	return FAILURE;
}


/// FontInterface

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

bool GodotFontInterface::LoadFontFace(const Rml::String& /*file_name*/, bool /*fallback_face*/, Rml::Style::FontWeight /*weight*/)
{
	return false;
}

bool GodotFontInterface::LoadFontFace(const Rml::byte* /*data*/, int /*data_size*/, const Rml::String& /*font_family*/, Rml::Style::FontStyle /*style*/,
	Rml::Style::FontWeight /*weight*/, bool /*fallback_face*/)
{
	return false;
}

Rml::FontFaceHandle GodotFontInterface::GetFontFaceHandle(const Rml::String& /*family*/, Rml::Style::FontStyle /*style*/, Rml::Style::FontWeight /*weight*/,
	int /*size*/)
{
	return 0;
}

Rml::FontEffectsHandle GodotFontInterface::PrepareFontEffects(Rml::FontFaceHandle /*handle*/, const Rml::FontEffectList& /*font_effects*/)
{
	return 0;
}

int GodotFontInterface::GetSize(Rml::FontFaceHandle /*handle*/)
{
	return 0;
}

int GodotFontInterface::GetXHeight(Rml::FontFaceHandle /*handle*/)
{
	return 0;
}

int GodotFontInterface::GetLineHeight(Rml::FontFaceHandle /*handle*/)
{
	return 0;
}

int GodotFontInterface::GetBaseline(Rml::FontFaceHandle /*handle*/)
{
	return 0;
}

float GodotFontInterface::GetUnderline(Rml::FontFaceHandle /*handle*/, float& /*thickness*/)
{
	return 0;
}

int GodotFontInterface::GetStringWidth(Rml::FontFaceHandle /*handle*/, const Rml::String& /*string*/, Rml::Character /*prior_character*/)
{
	return 0;
}

int GodotFontInterface::GenerateString(Rml::FontFaceHandle /*face_handle*/, Rml::FontEffectsHandle /*font_effects_handle*/, const Rml::String& /*string*/,
	const Rml::Vector2f& /*position*/, const Rml::Colourb& /*colour*/, float /*opacity*/, Rml::GeometryList& /*geometry*/)
{
	return 0;
}

int GodotFontInterface::GetVersion(Rml::FontFaceHandle /*handle*/)
{
	return 0;
}
