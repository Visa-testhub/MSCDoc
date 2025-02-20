#include "Element.h"

std::unique_ptr<Element> Element::Deserialize(const wxXmlNode* node)
{
	wxString type = node->GetAttribute(XmlNodes::TypeAttribute);

	if (type == XmlNodes::PathNodeType)
	{
		return std::make_unique<Path>(Path::Deserialize(node));
	}
	else if (type == XmlNodes::RectNodeType)
	{
		return std::make_unique<Rect>(Rect::Deserialize(node));
	}
	else if (type == XmlNodes::CircleNodeType)
	{
		return std::make_unique<Circle>(Circle::Deserialize(node));
	}

	throw std::runtime_error("Unfamiliar element type" + type);
}

wxXmlNode* Element::SerializeTransform()
{
	wxXmlNode* transNode = new wxXmlNode(wxXML_ELEMENT_NODE, XmlNodes::TransformNodeName);
	
	transNode->AddAttribute(XmlNodes::TransXAttribute, wxString::FromDouble(transform.transX));
	transNode->AddAttribute(XmlNodes::TransYAttribute, wxString::FromDouble(transform.transY));
	transNode->AddAttribute(XmlNodes::RotAttribute, wxString::FromDouble(transform.rotAngle));
	transNode->AddAttribute(XmlNodes::ScaleXAttribute, wxString::FromDouble(transform.scaleX));
	transNode->AddAttribute(XmlNodes::ScaleYAttribute, wxString::FromDouble(transform.scaleY));

	// Lazy reason to avoid error checking is that if we can't allocate memory for the transNode, we are in deep s*** already.
	return transNode;
}

wxXmlNode* Circle::Serialize() const
{
	// Make xmlnode. name is Object.
	wxXmlNode* objNode = new wxXmlNode(wxXML_ELEMENT_NODE, XmlNodes::ObjectNodeName);
	// Add the general attributes. 
	objNode->AddAttribute(XmlNodes::TypeAttribute, XmlNodes::CircleNodeType);
	objNode->AddAttribute(XmlNodes::ColourAttribute, this->colour.GetAsString(wxC2S_HTML_SYNTAX));
	objNode->AddAttribute(XmlNodes::RadiusAttribute, wxString::FromDouble(radius));

	wxXmlNode* circleNode = new wxXmlNode(wxXML_ELEMENT_NODE, XmlNodes::CenterElementNodeName);
	circleNode->AddAttribute(XmlNodes::XAttribute, wxString::FromDouble(center.m_x));
	circleNode->AddAttribute(XmlNodes::YAttribute, wxString::FromDouble(center.m_y));

	objNode->AddChild(circleNode);

	return objNode;
}
 
Circle Circle::Deserialize(const wxXmlNode* node) 
{
	Circle obj;
	obj.colour.Set(node->GetAttribute(XmlNodes::ColourAttribute));
	obj.radius = wxAtof(node->GetAttribute(XmlNodes::RadiusAttribute));

	const wxXmlNode* centerNode = node->GetChildren();
	obj.center.m_x = wxAtof(centerNode->GetAttribute(XmlNodes::XAttribute));
	obj.center.m_y = wxAtof(centerNode->GetAttribute(XmlNodes::YAttribute));

	return obj;
}

wxXmlNode* Rect::Serialize() const
{
	wxXmlNode* objNode = new wxXmlNode(wxXML_ELEMENT_NODE, XmlNodes::ObjectNodeName);
	objNode->AddAttribute(XmlNodes::TypeAttribute, XmlNodes::RectNodeType);
	objNode->AddAttribute(XmlNodes::ColourAttribute, this->colour.GetAsString(wxC2S_HTML_SYNTAX));

	wxXmlNode* rectNode = new wxXmlNode(wxXML_ELEMENT_NODE, XmlNodes::RectElementNodeName);
	rectNode->AddAttribute(XmlNodes::XAttribute, wxString::FromDouble(rect.m_x));
	rectNode->AddAttribute(XmlNodes::YAttribute, wxString::FromDouble(rect.m_y));
	rectNode->AddAttribute(XmlNodes::WidthAttribute, wxString::FromDouble(rect.m_width));
	rectNode->AddAttribute(XmlNodes::HeightAttribute, wxString::FromDouble(rect.m_height));

	objNode->AddChild(rectNode);

	return objNode;
}

Rect Rect::Deserialize(const wxXmlNode* node) 
{
	Rect obj;
	obj.colour.Set(node->GetAttribute(XmlNodes::ColourAttribute));

	const wxXmlNode* rectNode = node->GetChildren();
	obj.rect.m_x = wxAtof(rectNode->GetAttribute(XmlNodes::XAttribute));
	obj.rect.m_y = wxAtof(rectNode->GetAttribute(XmlNodes::YAttribute));
	obj.rect.m_width = wxAtof(rectNode->GetAttribute(XmlNodes::WidthAttribute));
	obj.rect.m_height = wxAtof(rectNode->GetAttribute(XmlNodes::HeightAttribute));

	return obj;
}

wxXmlNode* Path::Serialize() const {
	wxXmlNode* objNode = new wxXmlNode(wxXML_ELEMENT_NODE, XmlNodes::ObjectNodeName);
	objNode->AddAttribute(XmlNodes::TypeAttribute, XmlNodes::PathNodeType);
	objNode->AddAttribute(XmlNodes::ColourAttribute, colour.GetAsString(wxC2S_HTML_SYNTAX));
	objNode->AddAttribute(XmlNodes::WidthAttribute, wxString::FromDouble(width));

	for (const auto& point : points) {
		wxXmlNode* pointNode = new wxXmlNode(wxXML_ELEMENT_NODE, XmlNodes::PointElementNodeName);
		pointNode->AddAttribute(XmlNodes::XAttribute, wxString::FromDouble(point.m_x));
		pointNode->AddAttribute(XmlNodes::YAttribute, wxString::FromDouble(point.m_y));
		objNode->AddChild(pointNode);
	}

	return objNode;
}

Path Path::Deserialize(const wxXmlNode* node)
{
	Path obj;
	obj.colour.Set(node->GetAttribute(XmlNodes::ColourAttribute));
	obj.width = wxAtof(node->GetAttribute(XmlNodes::WidthAttribute));

	for (wxXmlNode* pointNode = node->GetChildren(); pointNode; pointNode = pointNode->GetNext())
	{
		if (pointNode->GetName() != XmlNodes::PointElementNodeName)
		{
			continue;
		}
		wxString xS = pointNode->GetAttribute(XmlNodes::XAttribute);
		wxString yS = pointNode->GetAttribute(XmlNodes::YAttribute);
		double x = wxAtof(xS);
		double y = wxAtof(yS);

		obj.points.push_back(wxPoint2DDouble(x,y));
	}

	return obj;
}
