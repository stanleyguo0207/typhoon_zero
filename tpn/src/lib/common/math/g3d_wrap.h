//
//           ┌┬┐┬ ┬┌─┐┬ ┬┌─┐┌─┐┌┐┌
//            │ └┬┘├─┘├─┤│ ││ ││││
//            ┴  ┴ ┴  ┴ ┴└─┘└─┘┘└┘
//
// This file is part of the typhoon Project.
// Copyright (C) 2021 stanley0207@163.com
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#ifndef TYPHOON_ZERO_TPN_SRC_LIB_COMMON_MATH_G3D_WRAP_H_
#define TYPHOON_ZERO_TPN_SRC_LIB_COMMON_MATH_G3D_WRAP_H_

#include <memory>

#include "G3D/g3dmath.h"
#include "G3D/Vector2.h"
#include "G3D/Vector3.h"
#include "G3D/Vector4.h"
#include "G3D/Matrix3.h"
#include "G3D/Quat.h"

namespace tpn {

/// 空间坐标向量
using Position3D     = G3D::Vector3;
using Position3DSptr = std::shared_ptr<Position3D>;

/// 空间方向
/// Roll    翻滚(围绕X轴旋转)
///         Rotate on camera's direction vector
/// Pitch   俯仰(围绕Y轴旋转)
///         Roatate on the cross product of the other camera's up and direction vectors
/// Yaw     偏航(围绕Z轴旋转)
///         Rotate on camera's up vector
struct Direction3D {
  Direction3D() {}
  Direction3D(float r, float p, float y) : dir(r, p, y) {}
  Direction3D(const G3D::Vector3 &v) : dir(v) {}
  Direction3D(const Direction3D &other) : dir(other.dir) {}
  Direction3D &operator=(const Direction3D &other) {
    if (this != &other) {
      this->dir = other.dir;
    }
    return *this;
  }

  /// 获取 X轴旋转值
  float GetRoll const { return dir.x; }
  /// 设置 X轴旋转值
  void SetRoll(float r) { dir.x = r; }

  /// 获取 Y轴旋转值
  float GetPitch const { return dir.y; }
  /// 设置 Y轴旋转值
  void SetPitch(float p) { dir.y = p; }

  /// 获取 Z轴旋转值
  float GetYaw const { return dir.z; }
  /// 设置 Z轴旋转值
  void SetYaw(float y) { dir.z = y; }

  G3D::Vector3 dir;  ///< 空间方向向量
};

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_SRC_LIB_COMMON_MATH_G3D_WRAP_H_
