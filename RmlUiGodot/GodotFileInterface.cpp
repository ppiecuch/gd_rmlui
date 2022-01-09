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

#include "GodotFileInterface.h"

#include "core/os/file_access.h"

#define FAILURE (-1)

GodotFileInterface::GodotFileInterface(const Rml::String& root) : root(root)
{
}

GodotFileInterface::~GodotFileInterface()
{
}

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
