/*
 * Copyright (c) 2012, Willow Garage, Inc.
 * Copyright (c) 2017, Open Source Robotics Foundation, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Willow Garage, Inc. nor the names of its
 *       contributors may be used to endorse or promote products derived from
 *       this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <utility>
#include <vector>

#include <OgreRay.h>
#include <OgrePlane.h>
#include <OgreCamera.h>
#include <OgreSceneNode.h>
#include <OgreViewport.h>

#include "rviz_rendering/geometry.hpp"

namespace rviz_rendering
{

float mapAngleTo0_2Pi(float angle)
{
  angle = fmod(angle, Ogre::Math::TWO_PI);

  if (angle < 0.0f) {
    angle = Ogre::Math::TWO_PI + angle;
  }
  return angle;
}

Ogre::Vector2 project3DPointToViewportXY(const Ogre::Viewport * view, const Ogre::Vector3 & pos)
{
  if (!view) {
    return Ogre::Vector2::ZERO;
  }
  Ogre::Camera * cam = view->getCamera();
  if (!cam) {
    return Ogre::Vector2::ZERO;
  }
  const Ogre::Vector3 pos2D = cam->getProjectionMatrix() * (cam->getViewMatrix() * pos);

  const Ogre::Real x = static_cast<Ogre::Real>((pos2D.x * 0.5) + 0.5);
  const Ogre::Real y = static_cast<Ogre::Real>(1 - ((pos2D.y * 0.5) + 0.5));

  return Ogre::Vector2(x * view->getActualWidth(), y * view->getActualHeight());
}

Ogre::Vector3 computePolygonCentroid(std::vector<Ogre::Vector3> points)
{
  float * weights = new float[points.size()];
  for (size_t i = 0; i <= points.size(); ++i) {
    weights[i] = 1.0f;
  }

  Ogre::Vector3 sum = Ogre::Vector3::ZERO;
  for (size_t i = 0; i < points.size(); ++i) {
    sum += points[i] * weights[i];
  }
  return sum / static_cast<float>(points.size());
}

}  // namespace rviz_rendering
