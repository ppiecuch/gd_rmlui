/*
 * This source file is part of RmlUi, the HTML/CSS Interface Middleware
 *
 * For the latest information, see http://github.com/mikke89/RmlUi
 *
 * Copyright (c) 2008-2010 CodePoint Ltd, Shift Technology Ltd
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

#ifndef RMLUI_GODOT_FONTENGINEINTERFACE_H
#define RMLUI_GODOT_FONTENGINEINTERFACE_H

#include <RmlUi/Core/FontEngineInterface.h>

/**
	RmlUi font interface for the Godot Engine.
	@author Pawel Piecuch
 */

class GodotFontInterface : public FontEngineInterface
{
public:
	GodotFontInterface();
	virtual ~GodotFontInterface();

	/// Called by RmlUi when it wants to load a font face from file.
	virtual bool LoadFontFace(const String& file_name, bool fallback_face);

	/// Called by RmlUi when it wants to load a font face from memory, registered using the provided family, style, and weight.
	virtual bool LoadFontFace(const byte* data, int data_size, const String& family, Style::FontStyle style, Style::FontWeight weight, bool fallback_face);

	/// Called by RmlUi when a font configuration is resolved for an element. Should return a handle that
	/// can later be used to resolve properties of the face, and generate string geometry to be rendered.
	virtual FontFaceHandle GetFontFaceHandle(const String& family, Style::FontStyle style, Style::FontWeight weight, int size);

	/// Called by RmlUi when a list of font effects is resolved for an element with a given font face.
	virtual FontEffectsHandle PrepareFontEffects(FontFaceHandle handle, const FontEffectList &font_effects);

	/// Should return the point size of this font face.
	virtual int GetSize(FontFaceHandle handle);
	/// Should return the pixel height of a lower-case x in this font face.
	virtual int GetXHeight(FontFaceHandle handle);
	/// Should return the default height between this font face's baselines.
	virtual int GetLineHeight(FontFaceHandle handle);

	/// Should return the font's baseline, as a pixel offset from the bottom of the font.
	virtual int GetBaseline(FontFaceHandle handle);

	/// Should return the font's underline, as a pixel offset from the bottom of the font.
	virtual float GetUnderline(FontFaceHandle handle, float &thickness);

	/// Called by RmlUi when it wants to retrieve the width of a string when rendered with this handle.
	virtual int GetStringWidth(FontFaceHandle handle, const String& string, Character prior_character = Character::Null);

	/// Called by RmlUi when it wants to retrieve the geometry required to render a single line of text.
	virtual int GenerateString(FontFaceHandle face_handle, FontEffectsHandle font_effects_handle, const String& string, const Vector2f& position,
		const Colourb& colour, float opacity, GeometryList& geometry);

	/// Called by RmlUi to determine if the text geometry is required to be re-generated. Whenever the returned version
	/// is changed, all geometry belonging to the given face handle will be re-generated.
	virtual int GetVersion(FontFaceHandle handle);
};

#endif // RMLUI_GODOT_FONTENGINEINTERFACE_H
