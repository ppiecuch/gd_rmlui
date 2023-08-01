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

#include "Godot_Renderer.h"
#include "RmlUi/Core.h"

#include "common/gd_core.h"
#include "scene/resources/mesh.h"
#include "servers/visual_server.h"

// Reference:
// ----------
//  1. https://godotengine.org/qa/675/how-to-clip-child-controls-to-parent-controls-rect-bounds

struct MeshWrapper
{
	Ref<ArrayMesh> mesh;
	Rml::TextureHandle texture;
	MeshWrapper() : texture(0) {}
};

struct TextureWrapper
{
	Ref<Texture> texture;
	TextureWrapper(const Ref<Texture> &texture) : texture(texture) {}
};

GodotRenderInterface::GodotRenderInterface()
{
	canvas_item = VisualServer::get_singleton()->canvas_item_create();
}

GodotRenderInterface::~GodotRenderInterface()
{
	VisualServer::get_singleton()->free(canvas_item);
}

// Called by RmlUi when it wants to render geometry that it does not wish to optimise.
void GodotRenderInterface::RenderGeometry(Rml::Vertex *vertices, int num_vertices, int *indices, int num_indices, const Rml::TextureHandle texture, const Rml::Vector2f &translation)
{
	PoolVector2Array v, t;
	PoolColorArray c;
	for(int i = 0; i < num_vertices; i++) {
		v.push_back(Vector2(vertices[i].position.x, vertices[i].position.y));
		t.push_back(Vector2(vertices[i].tex_coord.x, vertices[i].tex_coord.y));
		c.push_back(Color(vertices[i].colour.red/255.0, vertices[i].colour.green/255.0, vertices[i].colour.blue/255.f, vertices[i].colour.alpha/255.0));
	}

	PoolIntArray index;
	for(int i = 0; i < num_indices; i++) {
		index.push_back(indices[i]);
	}

	Array mesh_array;
	mesh_array.clear();
	mesh_array.resize(VS::ARRAY_MAX);
	mesh_array[VS::ARRAY_VERTEX] = v;
	mesh_array[VS::ARRAY_TEX_UV] = t;
	mesh_array[VS::ARRAY_COLOR] = c;
	mesh_array[VS::ARRAY_INDEX] = index;

	Ref<ArrayMesh> mesh = newref(ArrayMesh);
	mesh->add_surface_from_arrays(Mesh::PRIMITIVE_TRIANGLES, mesh_array, Array(), Mesh::ARRAY_FLAG_USE_2D_VERTICES);

	Transform2D transform;
	transform.translate(translation.x, translation.y);
	Color modulate(1,1,1,1);
	RID texture_rid = texture ? ((Texture*)texture)->get_rid() : RID();
	RID normal_map_rid;
	RID mask_rid;

	VisualServer::get_singleton()->canvas_item_add_mesh(canvas_item, mesh->get_rid(), transform, modulate, texture_rid, normal_map_rid, mask_rid);
}

// Called by RmlUi when it wants to compile geometry it believes will be static for the forseeable future.
Rml::CompiledGeometryHandle GodotRenderInterface::CompileGeometry(Rml::Vertex *vertices, int num_vertices, int *indices, int num_indices, const Rml::TextureHandle texture)
{
	MeshWrapper* wrapper = new MeshWrapper();
	wrapper->texture = texture;

	PoolVector2Array v, t;
	PoolColorArray c;
	for(int i = 0; i < num_vertices; i++) {
		v.push_back(Vector2(vertices[i].position.x, vertices[i].position.y));
		t.push_back(Vector2(vertices[i].tex_coord.x, vertices[i].tex_coord.y));
		c.push_back(Color(vertices[i].colour.red/255.0, vertices[i].colour.green/255.0, vertices[i].colour.blue/255.f, vertices[i].colour.alpha/255.0));
	}

	PoolIntArray index;
	for(int i = 0; i < num_indices; i++) {
		index.push_back(indices[i]);
	}

	Array mesh_array;
	mesh_array.resize(VS::ARRAY_MAX);
	mesh_array[VS::ARRAY_VERTEX] = v;
	mesh_array[VS::ARRAY_TEX_UV] = t;
	mesh_array[VS::ARRAY_COLOR] = c;
	mesh_array[VS::ARRAY_INDEX] = index;

	if (wrapper->mesh.is_null()) {
		wrapper->mesh = newref(ArrayMesh);
	}
	wrapper->mesh->add_surface_from_arrays(Mesh::PRIMITIVE_TRIANGLES, mesh_array, Array(), Mesh::ARRAY_FLAG_USE_2D_VERTICES);


	return (Rml::CompiledGeometryHandle)wrapper;
}

// Called by RmlUi when it wants to render application-compiled geometry.
void GodotRenderInterface::RenderCompiledGeometry(Rml::CompiledGeometryHandle geometry, const Rml::Vector2f &translation)
{
	MeshWrapper* wrapper = (MeshWrapper*)geometry;

	ERR_FAIL_NULL(wrapper);
	ERR_FAIL_COND(wrapper->mesh.is_null());

	Transform2D transform;
	Color modulate(1,1,1,1);
	Texture *texture = (Texture*)wrapper->texture;
	RID texture_rid = texture ? texture->get_rid() : RID();
	RID normal_map_rid;
	RID mask_rid;

	VisualServer::get_singleton()->canvas_item_add_mesh(canvas_item, wrapper->mesh->get_rid(), transform, modulate, texture_rid, normal_map_rid, mask_rid);
}

// Called by RmlUi when it wants to release application-compiled geometry.
void GodotRenderInterface::ReleaseCompiledGeometry(Rml::CompiledGeometryHandle geometry)
{
	((MeshWrapper*)geometry)->mesh->clear_mesh();
}

// Called by RmlUi when it wants to enable or disable scissoring to clip content.
void GodotRenderInterface::EnableScissorRegion(bool enable)
{
	VisualServer::get_singleton()->canvas_item_set_clip(canvas_item, enable);
}

// Called by RmlUi when it wants to change the scissor region.
void GodotRenderInterface::SetScissorRegion(int x, int y, int width, int height)
{
	VisualServer::get_singleton()->canvas_item_set_custom_rect(canvas_item, true, Rect2(x, y, width, height));
}

// Called by RmlUi when a texture is required by the library.
bool GodotRenderInterface::LoadTexture(Rml::TextureHandle &texture_handle, Rml::Vector2i &texture_dimensions, const Rml::String &source)
{
	if (RES texture = ResourceLoader::load(source.c_str(), "Texture")) {
		TextureWrapper *wrapper = memnew(TextureWrapper(texture));
		texture_handle = (Rml::TextureHandle)wrapper;
		Rml::Log::Message(Rml::Log::LT_INFO, "Texture loaded from %s.", source.c_str());

		return true;
	}
	return false;
}

// Called by RmlUi when a texture is required to be built from an internally-generated sequence of pixels.
bool GodotRenderInterface::GenerateTexture(Rml::TextureHandle &texture_handle, const Rml::byte *source, const Rml::Vector2i &source_dimensions)
{
	const int source_size = source_dimensions.x * source_dimensions.y * 4;
	PoolByteArray source_data;
	source_data.resize(source_size); // RGBA only
	memcpy(source_data.write().ptr(), source, source_size);
	Image* img = memnew(Image);
	img->create(source_dimensions.x, source_dimensions.y, false, Image::FORMAT_RGBA8, source_data);
	Ref<ImageTexture> texture = memnew(ImageTexture);
	texture->create_from_image(img);
	TextureWrapper *wrapper = memnew(TextureWrapper(texture));
	texture_handle = (Rml::TextureHandle)wrapper;
	return true;
}

// Called by RmlUi when a loaded texture is no longer required.
void GodotRenderInterface::ReleaseTexture(Rml::TextureHandle texture_handle)
{
	memdelete((TextureWrapper*)texture_handle);
}
