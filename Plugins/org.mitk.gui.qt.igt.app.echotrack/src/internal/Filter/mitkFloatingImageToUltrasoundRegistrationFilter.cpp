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

#include "mitkFloatingImageToUltrasoundRegistrationFilter.h"
#include <itkTransform.h>
#include <itkScalableAffineTransform.h>
#include <itkScalableAffineTransform.hxx>


mitk::FloatingImageToUltrasoundRegistrationFilter::FloatingImageToUltrasoundRegistrationFilter()
: mitk::NavigationDataPassThroughFilter(),
  m_Segmentation(nullptr),
  m_TransformSensorCSToMarkerCS(mitk::AffineTransform3D::New()),
  m_TransformMarkerCSToFloatingImageCS(mitk::AffineTransform3D::New()),
  m_TransformUSimageCSToTrackingCS(mitk::AffineTransform3D::New()),
  m_TransformCTimageIndexToWorld(mitk::AffineTransform3D::New()),
  m_TrackedUltrasoundActive(false)
{
  this->InitializeTransformationSensorCSToMarkerCS();
}


mitk::FloatingImageToUltrasoundRegistrationFilter::~FloatingImageToUltrasoundRegistrationFilter()
{
}

void mitk::FloatingImageToUltrasoundRegistrationFilter::InitializeTransformationSensorCSToMarkerCS()
{
  //The following calculations are related to the 3mm | 15mm fiducial configuration

  if (m_TransformSensorCSToMarkerCS.IsNull())
  {
    m_TransformSensorCSToMarkerCS = mitk::AffineTransform3D::New();
  }

  mitk::Vector3D translation;
  translation[0] = -18.175;
  translation[1] = 15.000;
  translation[2] = 8.001; // considering a distance from the base plate of 0.315 inch and not 0.313 inch

  m_TransformSensorCSToMarkerCS->SetOffset(translation);

  // Quaternion (x, y, z, r) --> n = (1,0,0) --> q(sin(90�),0,0,cos(90�))
  mitk::Quaternion q1(1, 0, 0, 0); // corresponding to a rotation of 180� around the normal x-axis.
  // .transpose() is needed for changing the rows and the columns of the returned rotation_matrix_transpose
  vnl_matrix_fixed<double, 3, 3> vnl_rotation = q1.rotation_matrix_transpose().transpose(); // :-)
  mitk::Matrix3D rotationMatrix;

  for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < 3; ++j) {
      rotationMatrix[i][j] = vnl_rotation[i][j];
    }
  }

  m_TransformSensorCSToMarkerCS->SetMatrix(rotationMatrix);
  //The transformation from the sensor-CS to the marker-CS is calculated now.
  MITK_INFO << "TransformSensorCSToMarkerCS = " << m_TransformSensorCSToMarkerCS;
  MITK_INFO << "TransformSensorCSToMarkerCS_Inverse = " << this->GetInverseTransform(m_TransformSensorCSToMarkerCS);
  /*mitk::NavigationData::Pointer navData = mitk::NavigationData::New();
  navData->SetOrientation(q1);
  navData->SetPosition(translation);
  navData->SetHasOrientation(true);
  navData->SetHasPosition(true);*/
}

void mitk::FloatingImageToUltrasoundRegistrationFilter::SetSegmentation(mitk::DataNode::Pointer segmentationNode)
{
  m_Segmentation = segmentationNode;
  m_TransformCTimageIndexToWorld = m_Segmentation->GetData()->GetGeometry()->GetIndexToWorldTransform();
}

void mitk::FloatingImageToUltrasoundRegistrationFilter::SetSurface(mitk::DataNode::Pointer surfaceNode)
{
  m_Surface = surfaceNode;
  mitk::AffineTransform3D::Pointer transform = m_Surface->GetData()->GetGeometry()->GetIndexToWorldTransform();
  MITK_WARN << "Surface-Transform = " << transform;
}

void mitk::FloatingImageToUltrasoundRegistrationFilter::SetTransformMarkerCSToFloatingImageCS(mitk::AffineTransform3D::Pointer transform)
{
  m_TransformMarkerCSToFloatingImageCS = transform;
}

void mitk::FloatingImageToUltrasoundRegistrationFilter::SetTransformUSimageCSToTrackingCS(mitk::AffineTransform3D::Pointer transform)
{
  m_TransformUSimageCSToTrackingCS = transform;
}

void mitk::FloatingImageToUltrasoundRegistrationFilter::GenerateData()
{
  Superclass::GenerateData();

  if (m_TrackedUltrasoundActive)
  {
    MITK_WARN << "The CT-to-US-registration is not supported by tracked ultrasound, yet.";
    return;
  }

  //IMPORTANT --- Hard coded --- First device = EM-Sensor | eventually second device = needle
  MITK_INFO  << "GenerateData() FloatingImageToUltrasoundRegistration";
  mitk::NavigationData::Pointer transformSensorCSToTracking = this->GetOutput(0);

  // cancel, if the EM-sensor is currently not being tracked
  if (!transformSensorCSToTracking->IsDataValid())
  {
    return;
  }

  //We start the transformation with the (constant) indexToWorld-Transform of the segmentation image:
  mitk::AffineTransform3D::Pointer totalTransformation = m_TransformCTimageIndexToWorld->Clone();
  //MITK_WARN << "CTImageTransform = " << totalTransformation;
  //Compose it with the inverse transform of marker-CS to floating image-CS:
  totalTransformation->Compose(this->GetInverseTransform(m_TransformMarkerCSToFloatingImageCS));
  //MITK_WARN << "CTImage_MarkerTransform = " << totalTransformation;
  //Compose this with the inverse transform of EM-sensor-CS to marker-CS:
  totalTransformation->Compose(this->GetInverseTransform(m_TransformSensorCSToMarkerCS));
  //MITK_WARN << "CTImage_Marker_SensorTransform = " << totalTransformation;
  //Compose this with the transform of the sensor-CS to Tracking-CS:
  totalTransformation->Compose(transformSensorCSToTracking->GetAffineTransform3D());
  //MITK_WARN << "CTImage_Marker_Sensor_TrackingTransform = " << totalTransformation;
  //Compose this with the inverse transform  of USimage-CS to tracking-CS:
  totalTransformation->Compose(this->GetInverseTransform(m_TransformUSimageCSToTrackingCS));
  //MITK_WARN << "CTImage_Marker_Sensor_Tracking_USTransform = " << totalTransformation;
  //Finally, set the total transformation (from floatingImage-CS to US-image-CS
  // to the selected floatingImageSurface:

  /*m_Segmentation->GetData()->GetGeometry()
    ->SetIndexToWorldTransform(totalTransformation);
  m_Segmentation->Modified();*/
  m_Surface->GetData()->GetGeometry()
    ->SetIndexToWorldTransform(totalTransformation);
  m_Surface->Modified();
}

mitk::AffineTransform3D::Pointer mitk::FloatingImageToUltrasoundRegistrationFilter::GetInverseTransform(mitk::AffineTransform3D::Pointer transform)
{
  mitk::AffineTransform3D::Pointer inverseTransform = mitk::AffineTransform3D::New();
  mitk::AffineTransform3D::Pointer inverse = dynamic_cast<mitk::AffineTransform3D*>(transform->GetInverseTransform().GetPointer());

  //itk::SmartPointer<itk::ScalableAffineTransform<mitk::ScalarType, 3U>> inverse = dynamic_cast<itk::ScalableAffineTransform<mitk::ScalarType, 3U>*> (transform->GetInverseTransform().GetPointer()); //itkScalableAffineTransform_hxx

  if (inverse.IsNull())
  {
    MITK_WARN << "Could not get inverse transform of mitk::AffineTransform3D. Returning nullptr";
    return nullptr;
  }

  inverseTransform->SetOffset(inverse->GetOffset());
  inverseTransform->SetMatrix(inverse->GetMatrix());

  return inverseTransform;
}
