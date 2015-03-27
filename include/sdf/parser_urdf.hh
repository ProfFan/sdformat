/*
 * Copyright 2012 Open Source Robotics Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
*/
#ifndef URDF2SDF_HH
#define URDF2SDF_HH

#include <tinyxml.h>
#include <vector>
#include <string>
#include <map>

#include "sdf/Types.hh"
#include "sdf/Console.hh"
#include "sdf/system_util.hh"

namespace sdf
{
  /// \internal
  /// \brief Private data for SDFExtension
  class SDFExtensionPrivate
  {
    public: SDFExtensionPrivate() {} 

    // for reducing fixed joints and removing links
    public: std::string oldLinkName;
    public: sdf::Pose reductionTransform;

    // visual
    public: std::string material;
    public: std::vector<boost::shared_ptr<TiXmlElement> > visual_blobs;

    // body, default off
    public: bool setStaticFlag;
    public: bool gravity;
    public: bool isDampingFactor;
    public: double dampingFactor;
    public: bool isMaxContacts;
    public: int maxContacts;
    public: bool isMaxVel;
    public: double maxVel;
    public: bool isMinDepth;
    public: double minDepth;
    public: bool selfCollide;

    // geom, contact dynamics
    public: bool isMu1, isMu2, isKp, isKd;
    public: double mu1, mu2, kp, kd;
    public: std::string fdir1;
    public: bool isLaserRetro;
    public: double laserRetro;

    // joint, joint limit dynamics
    public: bool isStopCfm, isStopErp, isInitialJointPosition, isFudgeFactor;
    public: double stopCfm, stopErp, initialJointPosition, fudgeFactor;
    public: bool isProvideFeedback;
    public: bool provideFeedback;
    public: bool isImplicitSpringDamper;
    public: bool implicitSpringDamper;
    public: bool isStopKp, isStopKd;
    public: double stopKp, stopKd;

    // blobs into body or robot
    public: std::vector<boost::shared_ptr<TiXmlElement> > blobs;

  };

  /// \addtogroup sdf
  /// \{

  /// \brief A class for holding sdf extension elements in urdf
  class SDFORMAT_VISIBLE SDFExtension
  {
    public: SDFExtension()
      : dataPtr(new SDFExtensionPrivate)
    {
     this->dataPtr->material.clear();
     this->dataPtr->visual_blobs.clear();
     this->dataPtr->setStaticFlag = false;
     this->dataPtr->gravity = true;
     this->dataPtr->isDampingFactor = false;
     this->dataPtr->isMaxContacts = false;
     this->dataPtr->isMaxVel = false;
     this->dataPtr->isMinDepth = false;
     this->dataPtr->fdir1.clear();
     this->dataPtr->isMu1 = false;
     this->dataPtr->isMu2 = false;
     this->dataPtr->isKp = false;
     this->dataPtr->isKd = false;
     this->dataPtr->selfCollide = false;
     this->dataPtr->isLaserRetro = false;
     this->dataPtr->isStopCfm = false;
     this->dataPtr->isStopErp = false;
     this->dataPtr->isStopKp = false;
     this->dataPtr->isStopKd = false;
     this->dataPtr->isInitialJointPosition = false;
     this->dataPtr->isFudgeFactor = false;
     this->dataPtr->isProvideFeedback = false;
     this->dataPtr->isImplicitSpringDamper = false;
     this->dataPtr->blobs.clear();

     this->dataPtr->dampingFactor = 0;
     this->dataPtr->maxContacts = 0;
     this->dataPtr->maxVel = 0;
     this->dataPtr->minDepth = 0;
     this->dataPtr->mu1 = 0;
     this->dataPtr->mu2 = 0;
     this->dataPtr->kp = 100000000;
     this->dataPtr->kd = 1;
     this->dataPtr->laserRetro = 101;
     this->dataPtr->stopCfm = 0;
     this->dataPtr->stopErp = 0.1;
     this->dataPtr->stopKp = 100000000;
     this->dataPtr->stopKd = 1;
     this->dataPtr->initialJointPosition = 0;
     this->dataPtr->fudgeFactor = 1;

     this->dataPtr->provideFeedback = false;
     this->dataPtr->implicitSpringDamper = false;
    }

    public: SDFExtension(const SDFExtension &ge)
      : dataPtr(new SDFExtensionPrivate)
    {
      this->dataPtr->material = ge.dataPtr->material;
      this->dataPtr->visual_blobs = ge.dataPtr->visual_blobs;
      this->dataPtr->setStaticFlag = ge.dataPtr->setStaticFlag;
      this->dataPtr->gravity = ge.dataPtr->gravity;
      this->dataPtr->isDampingFactor = ge.dataPtr->isDampingFactor;
      this->dataPtr->isMaxContacts = ge.dataPtr->isMaxContacts;
      this->dataPtr->isMaxVel = ge.dataPtr->isMaxVel;
      this->dataPtr->isMinDepth = ge.dataPtr->isMinDepth;
      this->dataPtr->fdir1 = ge.dataPtr->fdir1;
      this->dataPtr->isMu1 = ge.dataPtr->isMu1;
      this->dataPtr->isMu2 = ge.dataPtr->isMu2;
      this->dataPtr->isKp = ge.dataPtr->isKp;
      this->dataPtr->isKd = ge.dataPtr->isKd;
      this->dataPtr->selfCollide = ge.dataPtr->selfCollide;
      this->dataPtr->isLaserRetro = ge.dataPtr->isLaserRetro;
      this->dataPtr->isStopKp = ge.dataPtr->isStopKp;
      this->dataPtr->isStopKd = ge.dataPtr->isStopKd;
      this->dataPtr->isStopCfm = ge.dataPtr->isStopCfm;
      this->dataPtr->isStopErp = ge.dataPtr->isStopErp;
      this->dataPtr->isInitialJointPosition =
        ge.dataPtr->isInitialJointPosition;
      this->dataPtr->isFudgeFactor = ge.dataPtr->isFudgeFactor;
      this->dataPtr->isProvideFeedback = ge.dataPtr->isProvideFeedback;
      this->dataPtr->isImplicitSpringDamper =
        ge.dataPtr->isImplicitSpringDamper;
      this->dataPtr->provideFeedback = ge.dataPtr->provideFeedback;
      this->dataPtr->implicitSpringDamper = ge.dataPtr->implicitSpringDamper;
      this->dataPtr->oldLinkName = ge.dataPtr->oldLinkName;
      this->dataPtr->reductionTransform = ge.dataPtr->reductionTransform;
      this->dataPtr->blobs = ge.dataPtr->blobs;

      this->dataPtr->dampingFactor = ge.dataPtr->dampingFactor;
      this->dataPtr->maxContacts = ge.dataPtr->maxContacts;
      this->dataPtr->maxVel = ge.dataPtr->maxVel;
      this->dataPtr->minDepth = ge.dataPtr->minDepth;
      this->dataPtr->mu1 = ge.dataPtr->mu1;
      this->dataPtr->mu2 = ge.dataPtr->mu2;
      this->dataPtr->kp = ge.dataPtr->kp;
      this->dataPtr->kd = ge.dataPtr->kd;
      this->dataPtr->laserRetro = ge.dataPtr->laserRetro;
      this->dataPtr->stopKp = ge.dataPtr->stopKp;
      this->dataPtr->stopKd = ge.dataPtr->stopKd;
      this->dataPtr->stopCfm = ge.dataPtr->stopCfm;
      this->dataPtr->stopErp = ge.dataPtr->stopErp;
      this->dataPtr->initialJointPosition = ge.dataPtr->initialJointPosition;
      this->dataPtr->fudgeFactor = ge.dataPtr->fudgeFactor;
    }

    public: virtual ~SDFExtension()
    {
      delete this->dataPtr;
      this->dataPtr = NULL;
    }

    /// \brief Private data pointer
    public: SDFExtensionPrivate *dataPtr;

    friend class SDFORMAT_VISIBLE URDF2SDF;
  };


  /// \brief URDF to SDF converter
  class SDFORMAT_VISIBLE URDF2SDF
  {
    /// \brief constructor
    public: URDF2SDF();

    /// \brief destructor
    public: ~URDF2SDF();

    /// \brief convert urdf xml document string to sdf xml document
    /// \param[in] _xmlDoc a tinyxml document containing the urdf model
    /// \return a tinyxml document containing sdf of the model
    public: TiXmlDocument InitModelDoc(TiXmlDocument* _xmlDoc);

    /// \brief convert urdf file to sdf xml document
    /// \param[in] _urdfStr a string containing filename of the urdf model
    /// \return a tinyxml document containing sdf of the model
    public: TiXmlDocument InitModelFile(const std::string &_filename);

    /// \brief convert urdf string to sdf xml document, with option to enforce
    /// limits.
    /// \param[in] _urdfStr a string containing model urdf
    /// \param[in] _enforceLimits option to enforce joint limits
    /// \return a tinyxml document containing sdf of the model
    public: TiXmlDocument InitModelString(const std::string &_urdfStr,
                                          bool _enforceLimits = true);

    /// things that do not belong in urdf but should be mapped into sdf
    /// @todo: do this using sdf definitions, not hard coded stuff
    private: void ParseSDFExtension(TiXmlDocument &_urdfXml);

    /// list extensions for debugging
    private: void ListSDFExtensions();

    /// list extensions for debugging
    private: void ListSDFExtensions(const std::string &_reference);
  };
  /// \}
}

#endif
