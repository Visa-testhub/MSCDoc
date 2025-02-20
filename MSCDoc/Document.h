#pragma once

#include <wx/docview.h>
#include <wx/stream.h>
//#include <wx/fs_zip.h>
#include <wx/zipstrm.h>
#include <iostream>

#include "Element.h"
#include "StreamWrapper.h"

class Document : public wxDocument
{
public:
	std::ostream& SaveObject(std::ostream& stream) override;
	std::istream& LoadObject(std::istream& stream) override;

	
	std::vector<std::unique_ptr<Element>> elements;

	//TODO: Find a place to construct a handler for the filesystem
	// wxFileSystem::AddHandler(new wxZipFSHandler);
	// Maybe here??

	//wxWidgets internally expects document and view classes to have dynamic class capabilities.
	wxDECLARE_DYNAMIC_CLASS(Document);
};

