/*============================================================================

The Medical Imaging Interaction Toolkit (MITK)

Copyright (c) German Cancer Research Center (DKFZ)
All rights reserved.

Use of this source code is governed by a 3-clause BSD license that can be
found in the LICENSE file.

============================================================================*/

#ifndef MITKDICOMIOHelper_H
#define MITKDICOMIOHelper_H

#include "mitkDICOMDatasetAccessingImageFrameInfo.h"
#include "mitkIDICOMTagsOfInterest.h"

#include <MitkDICOMReaderExports.h>

namespace mitk
{
  class BaseData;

  typedef std::vector<mitk::DICOMDatasetAccess::FindingsListType> FindingsListVectorType;
  typedef BaseData *BaseDataPointer;

  MITKDICOMREADER_EXPORT mitk::IDICOMTagsOfInterest *GetDicomTagsOfInterestService();

  MITKDICOMREADER_EXPORT FindingsListVectorType ExtractPathsOfInterest(const DICOMTagPathList &pathsOfInterest,
                                                const DICOMDatasetAccessingImageFrameList &frames);

  MITKDICOMREADER_EXPORT void SetProperties(BaseDataPointer image, const FindingsListVectorType &findings);
}

#endif
