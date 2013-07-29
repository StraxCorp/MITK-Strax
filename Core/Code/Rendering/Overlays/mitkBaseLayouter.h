/*===================================================================
 *
The Medical Imaging Interaction Toolkit (MITK)

Copyright (c) German Cancer Research Center,
Division of Medical and Biological Informatics.
All rights reserved.

This software is distributed WITHOUT ANY WARRANTY; without
even the implied warranty of MERCHANTABILITY or FITNESS FOR
A PARTICULAR PURPOSE.

See LICENSE.txt or http://www.mitk.org for details.

===================================================================*/

#ifndef BASELAYOUTER_H
#define BASELAYOUTER_H

#include <MitkExports.h>
#include <itkObject.h>
#include <mitkCommon.h>
#include "mitkBaseRenderer.h"
#include "mitkOverlay.h"

namespace mitk {



/** @brief Baseclass of Overlay layouters */
/**
 *A BaseLayouter can be implemented to control a set of Overlays by means of position and size.
 *BaseLayouter::PrepareLayout() should be implemented with a routine to set the position of the internal m_ManagedOverlays List.
 *A layouter is always connected to one BaseRenderer, so there is one instance of the layouter for each BaseRenderer.
 *One type of layouter should always have a unique identifier.
 *@ingroup Overlays
*/
class MITK_CORE_EXPORT BaseLayouter : public itk::Object {
public:

  mitkClassMacro(BaseLayouter, itk::Object);

  void SetBaseRenderer(BaseRenderer* renderer);
  BaseRenderer *GetBaseRenderer();

  /** \brief Adds an Overlay to the internal list of managed Overlays.*/
  /** By calling this, the previous Layouter of the passed Overlays is called to remove this overlay from its internal list.*/
  void AddOverlay(Overlay *Overlay);

  /** \brief Removes the passed Overlay from the m_ManagedOverlays List */
  void RemoveOverlay(Overlay *Overlay);

  /** \brief Returns a unique identifier for one specific kind of layouter.*/
  std::string GetIdentifier();

  /** \brief Sets the positions of each managed overlay according to the layouter role*/
  /** This has to be implemented in order to provide a layouting procedure for the list of managed Overlays.
  *   The method has to provide a layouting for each identifier.*/
  virtual void PrepareLayout() = 0;


protected:

  /** \brief explicit constructor which disallows implicit conversions */
  explicit BaseLayouter();

  /** \brief virtual destructor in order to derive from this class */
  virtual ~BaseLayouter();

  /** \brief returns a list of the overlays that is managed by this Layouter. */
  std::list<mitk::Overlay*> GetManagedOverlays();

  /** \brief A unique identifier for one specific kind of layouter.*/
  /** If the implementation of the layouter can manage the overlay positions in different ways, each instance has to have
its own unique identifier.*/
  std::string m_Identifier;

private:

  /** \brief The baserenderer on which this layouter is active. */
  mitk::BaseRenderer* m_BaseRenderer;

  /** \brief List of the overlays managed by this layouter. */
  std::list<mitk::Overlay*> m_ManagedOverlays;

  /** \brief copy constructor */
  BaseLayouter( const BaseLayouter &);

  /** \brief assignment operator */
  BaseLayouter &operator=(const BaseLayouter &);
};

} // namespace mitk
#endif // BASELAYOUTER_H


