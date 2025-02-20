#include "Document.h"

//wxWidgets internally expects document and view classes to have dynamic class capabilities.
wxIMPLEMENT_DYNAMIC_CLASS(Document, wxDocument);


std::ostream& Document::SaveObject(std::ostream& stream)
{
    // First phase. Write the elements in xml
    
	wxXmlDocument doc;
    wxXmlNode* docNode = new wxXmlNode(wxXML_ELEMENT_NODE, XmlNodes::DocumentNodeName);

    docNode->AddAttribute(XmlNodes::VersionAttribute, XmlNodes::VersionValue);
    for (const auto& element : elements)
    {
        wxXmlNode* node = element->Serialize();
        docNode->AddChild(node);

        wxXmlNode* childNode = element->SerializeTransform();
        node->AddChild(childNode);
    }  
    doc.SetRoot(docNode);

    // Second phase. Compress the xml
    OSWrapper wrapper(stream);
    wxZipOutputStream zip(wrapper);
    zip.PutNextEntry("MSCDocument.xml");

    // Third phase. save and close the zip file.
    doc.Save(zip);
    zip.CloseEntry();
    zip.Close();

    return stream;
}

std::istream& Document::LoadObject(std::istream& stream)
{
    // First phase. Read the stream and unpack it from zip to xml.
    bool docPopulated = false;
    wxXmlDocument doc;
    ISWrapper wrapper = ISWrapper(stream);
    wxZipInputStream zipIn(wrapper);
    std::unique_ptr<wxZipEntry> entry(zipIn.GetNextEntry());

    while (entry)
    {
        wxString entryName = entry->GetName();

        if (entryName == "MSCDocument.xml" && zipIn.CanRead())
        {
            doc.Load(zipIn);
            zipIn.CloseEntry();
            docPopulated = true;
            break;
        }

        zipIn.CloseEntry();
        entry.reset(zipIn.GetNextEntry());
    }

    // Did we populate the wxXmlDoc?
    if (!docPopulated)
    {
        // TODO: more error handling
        return stream;
    }

    // Second phase. populate this documents elements array from the loaded xml
    elements.clear();
    for (wxXmlNode* node = doc.GetRoot()->GetChildren(); node; node = node->GetNext())
    {
        if (node->GetName() != XmlNodes::ObjectNodeName)
            continue;

        std::unique_ptr<Element> element = Element::Deserialize(node);
        if (element)
        {
            elements.push_back(std::move(element));
        }
    }


    return stream;
}
