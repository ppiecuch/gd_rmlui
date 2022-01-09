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

#ifndef RMLUI_GODOT_FILEINTERFACE_H
#define RMLUI_GODOT_FILEINTERFACE_H

#include <RmlUi/Core/StringUtilities.h>
#include <RmlUi/Core/FileInterface.h>

/**
	RmlUi file interface for the Godot Engine.
	@author Pawel Piecuch
 */

class GodotFileInterface : public Rml::FileInterface
{
public:
	GodotFileInterface(const Rml::String& root = "");
	virtual ~GodotFileInterface();

	/// Opens a file.		
	virtual Rml::FileHandle Open(const Rml::String& path);

	/// Closes a previously opened file.		
	virtual void Close(Rml::FileHandle file);

	/// Reads data from a previously opened file.		
	virtual size_t Read(void* buffer, size_t size, Rml::FileHandle file);

	/// Seeks to a point in a previously opened file.		
	virtual bool Seek(Rml::FileHandle file, long offset, int origin);

	/// Returns the current position of the file pointer.		
	virtual size_t Tell(Rml::FileHandle file);

private:
	Rml::String root;
};

#endif // RMLUI_GODOT_FILEINTERFACE_H
