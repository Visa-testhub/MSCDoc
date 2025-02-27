#pragma once

#include <wx/geometry.h>
#include "Element.h"

class PaintableElement
{
public:
	PaintableElement(std::unique_ptr<Element> e, Transform& t);

	void Draw(wxGraphicsContext* gc) const;

	wxXmlNode* SerializeTransform();

	// With the element being unique ptr the its copy constructor is deleted.
	// this in mind we have to use the move semantics everywhere where the 
	// paintable element is being passed around. 
	// Learned this from 2 hours of searching for the compile error meaning.
	// Example in Documpent.cpp loadObject method.
	std::unique_ptr<Element>	element;
	Transform					transform;
	wxRect2DDouble				bounds;
};

