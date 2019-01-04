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


#ifndef MITKFLOATINGIMAGETOULTRASOUNDREGISTRATIONFILTER_H_HEADER_INCLUDED_
#define MITKFLOATINGIMAGETOULTRASOUNDREGISTRATIONFILTER_H_HEADER_INCLUDED_

#include <mitkNavigationDataPassThroughFilter.h>

namespace mitk
{
  /**Documentation
  * \brief This filter transforms a given floating image into the ultrasound coordinate system.
  *
  *
  * \ingroup US
  */
  class FloatingImageToUltrasoundRegistrationFilter : public NavigationDataPassThroughFilter
  {
  public:
    mitkClassMacro(FloatingImageToUltrasoundRegistrationFilter, NavigationDataPassThroughFilter);
    itkFactorylessNewMacro(Self)
    itkCloneMacro(Self)

    void InitializeTransformationSensorCSToMarkerCS();

    void SetSegmentation(mitk::DataNode::Pointer segmentationNode);
    void SetSurface(mitk::DataNode::Pointer surfaceNode);
    void SetTransformMarkerCSToFloatingImageCS( mitk::AffineTransform3D::Pointer transform );
    void SetTransformUSimageCSToTrackingCS(mitk::AffineTransform3D::Pointer transform);

  protected:
    FloatingImageToUltrasoundRegistrationFilter();
    ~FloatingImageToUltrasoundRegistrationFilter() override;

    /**Documentation
    * \brief filter execute method
    *
    *
    */
    void GenerateData() override;

    mitk::AffineTransform3D::Pointer GetInverseTransform(mitk::AffineTransform3D::Pointer transform);

  private:
    mitk::DataNode::Pointer m_Segmentation;
    mitk::DataNode::Pointer m_Surface;
    mitk::AffineTransform3D::Pointer m_TransformSensorCSToMarkerCS;
    mitk::AffineTransform3D::Pointer m_TransformMarkerCSToFloatingImageCS;
    mitk::AffineTransform3D::Pointer m_TransformUSimageCSToTrackingCS;
    mitk::AffineTransform3D::Pointer m_TransformCTimageIndexToWorld;

    bool m_TrackedUltrasoundActive;

  };
} // namespace mitk
#endif // MITKFLOATINGIMAGETOULTRASOUNDREGISTRATIONFILTER_H_HEADER_INCLUDED_
