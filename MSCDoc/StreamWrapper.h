#pragma once

#include <wx/stream.h>
#include <iostream>

// Allows us to write data to standard C++ streams with wxWidgets streams.
struct OSWrapper : public wxOutputStream
{
	OSWrapper(std::ostream& stream) : m_stream(stream) {}

	size_t OnSysWrite(const void* buffer, size_t size) override
	{
		m_stream.write((const char*)buffer, size);
		return size;
	}

	std::ostream& m_stream;
};

// Allows us to read data from standard C++ streams with wxWidgets streams.
struct ISWrapper : public wxInputStream
{
	ISWrapper(std::istream& stream) : m_stream(stream) {}

	size_t OnSysRead(void* buffer, size_t size) override
	{
		m_stream.read((char*)buffer, size);
		return size;
	}

	std::istream& m_stream;
};