#include "PositionEvent.h"
#include "mitkVector.h"

//##ModelId=3E5B7CF900F0
mitk::PositionEvent::PositionEvent(int type, int button, int buttonState, int key, const mitk::Vector3D& displPosition)
: Event(type, button, buttonState, key), m_DisplayPosition(displPosition)
{}

//##ModelId=3E5B79DE0133
mitk::Vector3D mitk::PositionEvent::GetDisplayPosition() const
{
	return m_DisplayPosition;
}

//##ModelId=3ECA2D3E0376
mitk::Vector3D mitk::PositionEvent::GetWorldPosition() const
{
	return m_WorldPosition;
}

//##ModelId=3ECA2D3E03C6
void mitk::PositionEvent::SetWorldPosition(mitk::Vector3D& worldPosition)
{
	m_WorldPosition = worldPosition;
}



