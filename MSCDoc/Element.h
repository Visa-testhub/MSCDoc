#pragma once

#include <memory>
#include <stdexcept>
#include <wx/xml/xml.h>
#include <wx/graphics.h>


namespace XmlNodes
{
    constexpr const char* ObjectNodeName    = "Object";
    constexpr const char* PathNodeType      = "Path";
    constexpr const char* RectNodeType      = "Rect";
    constexpr const char* CircleNodeType    = "Circle";
    constexpr const char* TextNodeType      = "Text";

    constexpr const char* CenterElementNodeName = "Center";
    constexpr const char* RectElementNodeName   = "Rect";
    constexpr const char* PointElementNodeName  = "Point";

    constexpr const char* ColourAttribute   = "colour";
    constexpr const char* RadiusAttribute   = "radius";
    constexpr const char* XAttribute        = "x";
    constexpr const char* YAttribute        = "y";
    constexpr const char* WidthAttribute    = "width";
    constexpr const char* HeightAttribute   = "height";
    constexpr const char* TypeAttribute     = "type";

    constexpr const char* TransformNodeName = "Transform";
    constexpr const char* RotAttribute      = "rot";
    constexpr const char* ScaleXAttribute   = "scaleX";
    constexpr const char* ScaleYAttribute   = "scaleY";
    constexpr const char* TransXAttribute   = "transformX";
    constexpr const char* TransYAttribute   = "transformY";

    constexpr const char* DocumentNodeName  = "MSCDocument";
    constexpr const char* VersionAttribute  = "version";
    constexpr const char* VersionValue      = "1.0";
};


// Transform handles the Elements rotation, size and transform in the canvas
struct Transform
{
	double transX{ 0.0 };
	double transY{ 0.0 };
	double rotAngle{ 0.0 };
	double scaleX{ 0.0 };
	double scaleY{ 0.0 };
};


/*
** Yay, some runtime polymorphism added to the mix. Not the best case to use as the memory footprint doubles (if vs inline)
** with the searching for the correct function in the VT. Especially as these objects will be plentiful in the project.
** the cost is still after all so small that no need to think about optimization at this point at least and likely never.
**
** Main point why it's done this way is that good fellow polymorphism allows for easier extention of the element types and
** I don't like the variant and visit. Might change these to functional programming and use old C-style pointers to functions
** if this becomes too heavy.
*/

class Element
{
public:
	virtual ~Element() = default;

	virtual wxXmlNode*              Serialize() const = 0;
	static std::unique_ptr<Element> Deserialize(const wxXmlNode* node);
    
    //Cannot be static because we access the transform inside. 
    wxXmlNode*                      SerializeTransform();

    Transform transform;
};

class Circle : public Element
{
public:
    
    wxPoint2DDouble center;
    double			radius;
    wxColour		colour;
	

	wxXmlNode* Serialize()const override;
	static Circle Deserialize(const wxXmlNode* node);
};

class Rect : public Element
{
public:

    wxRect2DDouble	rect;
    wxColour	colour;

	wxXmlNode* Serialize()const override;
	static Rect Deserialize(const wxXmlNode* node);
};

class Path : public Element
{
public:

	std::vector<wxPoint2DDouble>	points;
	int								width;
	wxColour						colour;

	wxXmlNode* Serialize()const override;
	static Path Deserialize(const wxXmlNode* node);
};