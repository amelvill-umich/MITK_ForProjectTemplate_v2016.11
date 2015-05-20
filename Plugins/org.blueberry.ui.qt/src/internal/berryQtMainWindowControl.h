/*===================================================================

BlueBerry Platform

Copyright (c) German Cancer Research Center,
Division of Medical and Biological Informatics.
All rights reserved.

This software is distributed WITHOUT ANY WARRANTY; without
even the implied warranty of MERCHANTABILITY or FITNESS FOR
A PARTICULAR PURPOSE.

See LICENSE.txt or http://www.mitk.org for details.

===================================================================*/


#ifndef BERRYQTMAINWINDOWCONTROL_H_
#define BERRYQTMAINWINDOWCONTROL_H_

#include <QMainWindow>

#include <berryShell.h>
#include <internal/berryQtWidgetController.h>

namespace berry {

class QtMainWindowControl : public QMainWindow
{

  Q_OBJECT

public:

  QtMainWindowControl(Shell* shell, QWidget* parent = nullptr, Qt::WindowFlags flags = nullptr);

protected:

  // used for shell listeners
  void changeEvent(QEvent* event) override;
  void closeEvent(QCloseEvent* closeEvent) override;

  // used for control listeners
  void moveEvent(QMoveEvent* event) override;
  void resizeEvent(QResizeEvent* event) override;
  void inFocusEvent(QFocusEvent* event);

private:

  QtWidgetController::Pointer controller;

};

}

#endif /* BERRYQTMAINWINDOWCONTROL_H_ */