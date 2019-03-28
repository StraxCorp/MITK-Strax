/*===================================================================

The Medical Imaging Interaction Toolkit (MITK)

Copyright (c) German Cancer Research Center,
Division of Medical and Biological Informatics.
All rights reserved.

This software is distributed WITHOUT ANY WARRANTY; without
even the implied warranty of MERCHANTABILITY or FITNESS FOR
A PARTICULAR PURPOSE.

See LICENSE.txt or http://www.mitk.org for details.

===================================================================*/

#ifndef QMITKABSTRACTDATANODEACTION_H
#define QMITKABSTRACTDATANODEACTION_H

#include <org_mitk_gui_qt_application_Export.h>

// mitk core
#include <mitkBaseRenderer.h>
#include <mitkDataStorage.h>
#include <mitkWeakPointer.h>

// berry
#include <berryIWorkbenchPartSite.h>

// qt
#include <QList>

class MITK_QT_APP QmitkAbstractDataNodeAction
{

public:

  QmitkAbstractDataNodeAction(berry::IWorkbenchPartSite::Pointer workbenchPartSite);
  QmitkAbstractDataNodeAction(berry::IWorkbenchPartSite* workbenchPartSite);

  virtual ~QmitkAbstractDataNodeAction();

  void SetDataStorage(mitk::DataStorage* dataStorage);
  void SetBaseRenderer(mitk::BaseRenderer* baseRenderer);

  virtual void InitializeWithDataNode(const mitk::DataNode*) { }

protected:

  virtual void InitializeAction() = 0;

  /**
  * @brief Grants access to the base renderer stored for the action.
  *        Will return nullptr if renderer was never set or has become invalid
  */
  mitk::BaseRenderer::Pointer GetBaseRenderer();

  mitk::DataNode::Pointer GetSelectedNode();
  QList<mitk::DataNode::Pointer> GetSelectedNodes();

  berry::IWorkbenchPartSite::WeakPtr m_WorkbenchPartSite;
  mitk::WeakPointer<mitk::DataStorage> m_DataStorage;

private:

  mitk::WeakPointer<mitk::BaseRenderer> m_BaseRenderer;

};

#endif // QMITKABSTRACTDATANODEACTION_H
