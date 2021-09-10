#
#           ┌┬┐┬ ┬┌─┐┬ ┬┌─┐┌─┐┌┐┌
#            │ └┬┘├─┘├─┤│ ││ ││││
#            ┴  ┴ ┴  ┴ ┴└─┘└─┘┘└┘
#
# This file is part of the typhoon Project.
# Copyright (C) 2021 stanley0207@163.com
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

set(G3DLIB_SOURCES_FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/source/AABox.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/source/Any.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/source/AnyTableReader.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/source/BinaryFormat.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/source/BinaryInput.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/source/BinaryOutput.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/source/Box.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/source/Capsule.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/source/CollisionDetection.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/source/CoordinateFrame.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/source/Crypto.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/source/Cylinder.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/source/debugAssert.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/source/FileSystem.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/source/fileutils.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/source/format.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/source/g3dfnmatch.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/source/g3dmath.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/source/GThread.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/source/Line.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/source/LineSegment.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/source/Log.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/source/Matrix3.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/source/Matrix4.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/source/MemoryManager.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/source/PhysicsFrame.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/source/Plane.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/source/prompt.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/source/Quat.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/source/Random.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/source/Ray.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/source/RegistryUtil.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/source/Sphere.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/source/stringutils.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/source/System.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/source/TextInput.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/source/TextOutput.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/source/Triangle.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/source/uint128.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/source/UprightFrame.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/source/Vector2.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/source/Vector3.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/source/Vector4.cpp
  )

add_library(g3dlib
  STATIC
    ${G3DLIB_SOURCES_FILES}
  )

target_include_directories(g3dlib
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
  )

target_link_libraries(g3dlib
  PRIVATE
    typhoon-dependency-interface
  PUBLIC
    zlib
    threads
  )

set_target_properties(g3dlib
  PROPERTIES
    FOLDER
      "third_party"
  )
