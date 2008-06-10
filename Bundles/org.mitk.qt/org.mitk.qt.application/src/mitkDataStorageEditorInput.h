#ifndef MITKDATASTORAGEEDITORINPUT_H_
#define MITKDATASTORAGEEDITORINPUT_H_

#include <org.opencherry.ui/src/cherryIEditorInput.h>

#include <org.mitk.core.services/src/mitkIDataStorageReference.h>

class mitkDataStorageEditorInput : public cherry::IEditorInput
{
public:
  cherryClassMacro(mitkDataStorageEditorInput);
  
  bool Exists();
  std::string GetName();
  std::string GetToolTipText();
  
  mitk::IDataStorageReference::Pointer GetDataStorageReference();
  
private:
  
  mitk::IDataStorageReference::Pointer m_DataStorageRef;
};

#endif /*MITKDATASTORAGEEDITORINPUT_H_*/
