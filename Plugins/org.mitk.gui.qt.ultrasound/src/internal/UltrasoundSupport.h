/*=========================================================================

Program:   Medical Imaging & Interaction Toolkit
Language:  C++
Date:      $Date$
Version:   $Revision$ 
 
Copyright (c) German Cancer Research Center, Division of Medical and
Biological Informatics. All rights reserved.
See MITKCopyright.txt or http://www.mitk.org/copyright.html for details.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/


#ifndef UltrasoundSupport_h
#define UltrasoundSupport_h

#include <berryISelectionListener.h>

#include <QmitkAbstractView.h>

#include "ui_UltrasoundSupportControls.h"


/*!
  \brief UltrasoundSupport

  \warning  This class is not yet documented. Use "git blame" and ask the author to provide basic documentation.

  \sa QmitkFunctionality
  \ingroup ${plugin_target}_internal
*/
class UltrasoundSupport : public QmitkAbstractView
{  
  // this is needed for all Qt objects that should have a Qt meta-object
  // (everything that derives from QObject and wants to have signal/slots)
  Q_OBJECT
  
  public:  

    static const std::string VIEW_ID;

    virtual void CreateQtPartControl(QWidget *parent);

   signals:
     
    void DeviceServiceUpdated();

  public slots:
    void OnNewDeviceWidgetDone();
     
  protected slots:
  
    void OnClickedAddNewDevice();

    void OnClickedViewDevice();

    void DisplayImage();

  protected:
  
    virtual void SetFocus();

    QTimer *m_Timer;

    mitk::USDevice::Pointer m_Device;

    mitk::DataNode::Pointer m_Node;
   
    // Not necessary?
    //const QList<mitk::DataNode::Pointer>& nodes );

    Ui::UltrasoundSupportControls m_Controls;

};

#endif // UltrasoundSupport_h

