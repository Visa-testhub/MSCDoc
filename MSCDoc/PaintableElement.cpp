#include "PaintableElement.h"

PaintableElement::PaintableElement(std::unique_ptr<Element> e, Transform& t) : element(std::move(e)), transform(t)
{
	bounds = element->GetBounds();
}

void PaintableElement::Draw(wxGraphicsContext* gc) const
{
	//Push the current state like trnasformation, clipping etc. to the stack.
	gc->PushState();

	wxAffineMatrix2D matrix;

	//Set the transform to correct place.
	matrix.Translate(transform.transX, transform.transY);

	//Rotate the element around its center.
	matrix.Translate(bounds.GetCentre().m_x, bounds.GetCentre().m_y);
	matrix.Rotate(transform.rotAngle);
	matrix.Translate(-bounds.GetCentre().m_x, -bounds.GetCentre().m_y);

	//Scale the element to correct size.
	matrix.Translate(bounds.GetCentre().m_x, bounds.GetCentre().m_y);
	matrix.Scale(transform.scaleX, transform.scaleY);
	matrix.Translate(bounds.GetCentre().m_x, bounds.GetCentre().m_y);

	gc->SetTransform(gc->CreateMatrix(matrix));
	element->Draw(gc);

	
	//Sets current stateof the context to the state saved by a preceding call to pushState() and removes that state from the stack.
	gc->PopState();
}

wxXmlNode* PaintableElement::SerializeTransform()
{
	return element->SerializeTransform(transform);
}
