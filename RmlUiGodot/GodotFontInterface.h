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

class GodotFontInterface : public Rml::FontEngineInterface
{
public:
	GodotFontInterface();
	virtual ~GodotFontInterface();

	/// Called by RmlUi when it wants to load a font face from file.
	virtual bool LoadFontFace(const Rml::String& file_name, bool fallback_face);

	/// Called by RmlUi when it wants to load a font face from memory, registered using the provided family, style, and weight.
	virtual bool LoadFontFace(const Rml::byte* data, int data_size, const Rml::String& family, Rml::Style::FontStyle style, Rml::Style::FontWeight weight, bool fallback_face);

	/// Called by RmlUi when a font configuration is resolved for an element. Should return a handle that
	/// can later be used to resolve properties of the face, and generate string geometry to be rendered.
	virtual Rml::FontFaceHandle GetFontFaceHandle(const Rml::String& family, Rml::Style::FontStyle style, Rml::Style::FontWeight weight, int size);

	/// Called by RmlUi when a list of font effects is resolved for an element with a given font face.
	virtual Rml::FontEffectsHandle PrepareFontEffects(Rml::FontFaceHandle handle, const Rml::FontEffectList &font_effects);

	/// Should return the point size of this font face.
	virtual int GetSize(Rml::FontFaceHandle handle);
	/// Should return the pixel height of a lower-case x in this font face.
	virtual int GetXHeight(Rml::FontFaceHandle handle);
	/// Should return the default height between this font face's baselines.
	virtual int GetLineHeight(Rml::FontFaceHandle handle);

	/// Should return the font's baseline, as a pixel offset from the bottom of the font.
	virtual int GetBaseline(Rml::FontFaceHandle handle);

	/// Should return the font's underline, as a pixel offset from the bottom of the font.
	virtual float GetUnderline(Rml::FontFaceHandle handle, float &thickness);

	/// Called by RmlUi when it wants to retrieve the width of a string when rendered with this handle.
	virtual int GetStringWidth(Rml::FontFaceHandle handle, const Rml::String& string, Rml::Character prior_character = Rml::Character::Null);

	/// Called by RmlUi when it wants to retrieve the geometry required to render a single line of text.
	virtual int GenerateString(Rml::FontFaceHandle face_handle, Rml::FontEffectsHandle font_effects_handle, const Rml::String& string, const Rml::Vector2f& position,
		const Rml::Colourb& colour, float opacity, Rml::GeometryList& geometry);

	/// Called by RmlUi to determine if the text geometry is required to be re-generated. Whenever the returned version
	/// is changed, all geometry belonging to the given face handle will be re-generated.
	virtual int GetVersion(Rml::FontFaceHandle handle);
};

#endif // RMLUI_GODOT_FONTENGINEINTERFACE_H
