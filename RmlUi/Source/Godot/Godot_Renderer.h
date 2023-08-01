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

#ifndef RMLUI_GODOT_RENDERER_H
#define RMLUI_GODOT_RENDERER_H

#include <RmlUi/Core/RenderInterface.h>

#include "core/rid.h"

/// Low level Godot Engine render interface for RmlUi
/// @author Pawel Piecuch
class GodotRenderInterface : public Rml::RenderInterface
{
	RID canvas_item;
	int m_width;
	int m_height;

public:
	GodotRenderInterface();
	~GodotRenderInterface();

    void SetViewport(int width, int height);

	// Called by RmlUi when it wants to render geometry that it does not wish to optimise.
	virtual void RenderGeometry(Rml::Vertex* vertices, int num_vertices, int* indices, int num_indices, Rml::TextureHandle texture, const Rml::Vector2f& translation);

	// Called by RmlUi when it wants to compile geometry it believes will be static for the forseeable future.
	virtual Rml::CompiledGeometryHandle CompileGeometry(Rml::Vertex* vertices, int num_vertices, int* indices, int num_indices, Rml::TextureHandle texture);

	// Called by RmlUi when it wants to render application-compiled geometry.
	virtual void RenderCompiledGeometry(Rml::CompiledGeometryHandle geometry, const Rml::Vector2f& translation);
	// Called by RmlUi when it wants to release application-compiled geometry.
	virtual void ReleaseCompiledGeometry(Rml::CompiledGeometryHandle geometry);

	// Called by RmlUi when it wants to enable or disable scissoring to clip content.
	virtual void EnableScissorRegion(bool enable);
	// Called by RmlUi when it wants to change the scissor region.
	virtual void SetScissorRegion(int x, int y, int width, int height);

	// Called by RmlUi when a texture is required by the library.
	virtual bool LoadTexture(Rml::TextureHandle& texture_handle, Rml::Vector2i& texture_dimensions, const Rml::String& source);
	// Called by RmlUi when a texture is required to be built from an internally-generated sequence of pixels.
	virtual bool GenerateTexture(Rml::TextureHandle& texture_handle, const Rml::byte* source, const Rml::Vector2i& source_dimensions);
	// Called by RmlUi when a loaded texture is no longer required.
	virtual void ReleaseTexture(Rml::TextureHandle texture_handle);

	int getWidth() const { return m_width; }
	int getHeight() const { return m_height; }
};

#endif // RMLUI_GODOT_RENDERER_H
