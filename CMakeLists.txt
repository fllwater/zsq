###Setting####################################################################
cmake_minimum_required(VERSION 2.8)
project(aro CXX)
add_definitions(-D_CRT_SECURE_NO_WARNINGS)
add_compile_options(-std=c++11 -fpermissive)
set(CMAKE_BUILD_TYPE Debug)
#set(CMAKE_INCLUDE_CURRENT_DIR ON) 
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/build)
set(CMAKE_SUPPRESS_REGENERATION TRUE)#对VS不生成ZERO_CHECK工程
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)






if(WIN32)
###WinLibrary#################################################################
message("Platform: Win32\n\nPlatform: Win32\n\nPlatform: Win32\n\nPlatform: Win32\n\nPlatform: Win32\n\nPlatform: Win32\n\n")
#1.1Qt5
include_directories(D:/app/qt/5.6/msvc2015_64/include)
file(GLOB Qt5_LIBS LIST_DIRECTORIES false D:/app/qt/5.6/msvc2015_64/lib/Qt5*.lib)#若全部包含则libGLESv2.lib或libGLESv2d.lib将影响pangolin显示

#1.2Boost
include_directories(D:/app/boost)
file(GLOB Boost_LIBS LIST_DIRECTORIES false D:/app/boost/lib64-msvc-14.0/*.lib)

#1.3Eigen3
include_directories(D:/app/eigen)

#1.4Sophus
include_directories(D:/app/sophus)

#1.5SuiteSparse(Lapack_Blas)
include_directories(D:/app/suitesparse/build/install/include)
file(GLOB SuiteSparese_LIBS  LIST_DIRECTORIES false D:/app/suitesparse/build/install/lib/*.lib)

#1.6Octomap(DynamicEDT3D)
include_directories(D:/app/octomap/build/install/include)
file(GLOB Octomap_LIBS LIST_DIRECTORIES false D:/app/octomap/build/install/lib/*.lib)

#2.1VTK
include_directories(D:/app/vtk/build/install/include/vtk-8.0)
file(GLOB VTK_LIBS LIST_DIRECTORIES false D:/app/vtk/build/install/lib/*.lib)

#2.2OpenCV
include_directories(D:/app/opencv/build/install/include)
file(GLOB OpenCV_LIBS LIST_DIRECTORIES false D:/app/opencv/build/install/x64/vc14/lib/*.lib)

#2.3Pangolin
include_directories(D:/app/pangolin/build/install/include)
file(GLOB Pangolin_LIBS LIST_DIRECTORIES false D:/app/pangolin/build/install/lib/*.lib)#conflict with Qt #find_package(Pangolin REQUIRED HINTS D:/app/pangolin/build/install/lib/cmake/Pangolin) set(Pangolin_LIBS ${Pangolin_LIBRARIES})
set(Pangolin_LIBS ${Pangolin_LIBS} glu32.lib opengl32.lib mf.lib mfplat.lib mfreadwrite.lib mfuuid.lib strmiids.lib)#额外库

#2.4Fbow

#3.1Gflags
include_directories(D:/app/gflags/build/install/include)
file(GLOB Gflags_LIBS LIST_DIRECTORIES false D:/app/gflags/build/install/lib/*.lib)

#3.2Glog
include_directories(D:/app/glog/build/install/include)
file(GLOB Glog_LIBS LIST_DIRECTORIES false D:/app/glog/build/install/lib/*.lib)

#3.3Ceres
include_directories(D:/app/ceres/build/install/include)
find_package(Ceres REQUIRED HINTS D:/app/ceres/build/install/CMake)#直接包含会缺少宏定义和shlwapi.lib

#3.4G2O
include_directories(D:/app/g2o/build/install/include D:/app/suitesparse/build/install/include/suitesparse)
file(GLOB G2O_LIBS  LIST_DIRECTORIES false D:/app/g2o/build/install/lib/*.lib)

#3.5Gtsam

#4.1Flann
include_directories(D:/app/flann/build/install/include)
file(GLOB Flann_LIBS LIST_DIRECTORIES false D:/app/flann/build/install/lib/*.lib)

#4.2Qhull
include_directories(D:/app/qhull/build/install/include)
file(GLOB Qhull_LIBS LIST_DIRECTORIES false D:/app/qhull/build/install/lib/*.lib)

#4.3PCL
include_directories(D:/app/pcl/build/install/include/pcl-1.8)
file(GLOB PCL_LIBS LIST_DIRECTORIES false D:/app/pcl/build/install/lib/*.lib)
find_package(PCL REQUIRED)#为增加宏定义
add_definitions(${PCL_DEFINITIONS})

set(ALL_LIBS
${Qt5_LIBS}
${Boost_LIBS}
#NoEigen
#NoSophus
${SuiteSparese_LIBS}
${Octomap_LIBS}
#################################
${VTK_LIBS}
${OpenCV_LIBS}
${Pangolin_LIBS}
#${Fbow_LIBS}
#################################
${Gflags_LIBS}
${Glog_LIBS}
${CERES_LIBRARIES}
${G2O_LIBS}
#{Gtsam_LIBS}
#################################
${Flann_LIBS}
${Qhull_LIBS}
${PCL_LIBS}
)
message("ALL_LIBS: ${ALL_LIBS}")
###WinFiles######################################################################
file(GLOB cmkdir LIST_DIRECTORIES false ${CMAKE_SOURCE_DIR}/*)
file(GLOB datdir LIST_DIRECTORIES false ${CMAKE_SOURCE_DIR}/data/*)
file(GLOB incdir LIST_DIRECTORIES false ${CMAKE_SOURCE_DIR}/include/*)
file(GLOB incbase LIST_DIRECTORIES false ${CMAKE_SOURCE_DIR}/include/base/*)
file(GLOB inccalib LIST_DIRECTORIES false ${CMAKE_SOURCE_DIR}/include/calib/*)
file(GLOB incstereo LIST_DIRECTORIES false ${CMAKE_SOURCE_DIR}/include/stereo/*)
file(GLOB incvslam LIST_DIRECTORIES false ${CMAKE_SOURCE_DIR}/include/vslam/*)
file(GLOB incwindow LIST_DIRECTORIES false ${CMAKE_SOURCE_DIR}/include/window/*)
file(GLOB srcdir LIST_DIRECTORIES false ${CMAKE_SOURCE_DIR}/source/*)
file(GLOB srcdemo LIST_DIRECTORIES false ${CMAKE_SOURCE_DIR}/source/demo/*)
source_group("data" FILES ${datdir})
source_group("include" FILES ${incdir})
source_group("include\\base" FILES ${incbase})
source_group("include\\calib" FILES ${inccalib})
source_group("include\\stereo" FILES ${incstereo})
source_group("include\\vslam" FILES ${incvslam})
source_group("include\\window" FILES ${incwindow})
source_group("source" FILES ${srcdir})
source_group("source\\demo" FILES ${srcdemo})
add_executable(${PROJECT_NAME} 
${cmkdir} 
${datdir} 
${incdir} ${incbase} ${inccalib} ${incmarker} ${incstereo} ${incvslam} ${incwindow}
${srcdir} ${srcdemo}
)
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(${PROJECT_NAME} ${ALL_LIBS})
endif(WIN32)






if(UNIX AND (${PROJECT_NAME} STREQUAL "aro"))
###UbuLibrary#################################################################
message("Platform: Linux\n\nPlatform: Linux\n\nPlatform: Linux\n\nPlatform: Linux\n\nPlatform: Linux\n\nPlatform: Linux\n\n")
#1.1Qt5
find_package(Qt5 REQUIRED COMPONENTS Core)
include_directories(/usr/include/x86_64-linux-gnu/qt5)
file(GLOB Qt5_LIBS_DZY /usr/lib/x86_64-linux-gnu/libQt5*.so)
message("Qt5_LIBS_DZY: ${Qt5_LIBS_DZY}")

#1.2Boost
find_package(Boost REQUIRED COMPONENTS atomic)
include_directories(usr/include)#/boost
file(GLOB Boost_LIBS_DZY /usr/lib/x86_64-linux-gnu/libboost_*.so)
message("Boost_LIBS_DZY: ${Boost_LIBS_DZY}")

#1.3Eigen3
include_directories(/usr/include/eigen3)

#1.4Sophus
include_directories(/usr/include/sophus-dzy)

#1.5SuiteSparse(Lapack_Blas)
include_directories(/usr/include/suitesparse)
set(SuiteSparese_LIBS_DZY
libgfortran.so libblas.so liblapack.so
libgfortran.a libblas.a liblapack.a
libamd.so libcamd.so libcolamd.so libccolamd.so libcxsparse.so libldl.so libklu.so libbtf.so libumfpack.so libcholmod.so libspqr.so libsuitesparseconfig.so
libamd.a libcamd.so libcolamd.a libccolamd.a libcxsparse.a libldl.a libklu.a libbtf.a libumfpack.a libcholmod.a libspqr.a libsuitesparseconfig.a)
message("SuiteSparese_LIBS_DZY: ${SuiteSparese_LIBS_DZY}")

#1.6Octomap(DynamicEDT3D)
find_package(octomap REQUIRED)
find_package(dynamicEDT3D REQUIRED)
include_directories(usr/include)#/octomap
include_directories(usr/include)#/dynamicEDT3D
message("OCTOMAP_LIBRARIES: ${OCTOMAP_LIBRARIES}")
message("DYNAMICEDT3D_LIBRARIES: ${DYNAMICEDT3D_LIBRARIES}")

#2.1VTK
find_package(VTK REQUIRED)
include_directories(usr/include)#/vtk-6.2
message("VTK_LIBRARIES: ${VTK_LIBRARIES}")

#2.2OpenCV
find_package(OpenCV 3.3.0 REQUIRED)
include_directories(usr/local/include)#/opencv /opencv2
message("OpenCV_LIBS: ${OpenCV_LIBS}")

#2.3Pangolin
find_package(Pangolin REQUIRED)
include_directories(usr/local/include)#/pangolin
message("Pangolin_LIBRARIES: ${Pangolin_LIBRARIES}")

#2.4Fbow

#3.1Gflags
include_directories(usr/include)#/gflags
set(Gflags_LIBS_DZY /usr/lib/x86_64-linux-gnu/libgflags.so)
message("Gflags_LIBS_DZY: ${GFLAGS_LIBRARIES}")

#3.2Glog
include_directories(usr/include)#/glog
set(Glog_LIBS_DZY /usr/lib/x86_64-linux-gnu/libglog.so)
message("Glog_LIBS_DZY: ${Glog_LIBS_DZY}")

#3.3Ceres
find_package(Ceres REQUIRED)
include_directories(usr/local/include)#/ceres
message("CERES_LIBRARIES: ${CERES_LIBRARIES}")

#3.4G2O
include_directories(usr/local/include)#/g2o
file(GLOB G2O_LIBS_DZY LIST_DIRECTORIES false /usr/local/lib/libg2o_*.so)
include_directories(/usr/include/suitesparse/cxsparse)

#3.5Gtsam

#4.1Flann
include_directories(usr/include)#/flann
set(Flann_LIBS_DZY /usr/lib/x86_64-linux-gnu/libflann.so)
message("Flann_LIBS_DZY: ${Flann_LIBS_DZY}")

#4.2Qhull
include_directories(usr/include)#/qhull
set(Qhull_LIBS_DZY /usr/lib/x86_64-linux-gnu/libqhull.so)
message("Qhull_LIBS_DZY: ${Qhull_LIBS_DZY}")

#4.3PCL
find_package(PCL REQUIRED)
include_directories( ${PCL_INCLUDE_DIRS} )
add_definitions( ${PCL_DEFINITIONS} )
message("PCL_INCLUDE_DIRS: ${PCL_INCLUDE_DIRS}")

set(ALL_LIBS
${Qt5_LIBS_DZY}
${Boost_LIBS_DZY}
#NoEigen
#NoSophus
${SuiteSparse_LIBS_DZY}
${OCTOMAP_LIBRARIES}
${DYNAMICEDT3D_LIBRARIES}
#################################
${VTK_LIBRARIES}
${OpenCV_LIBS}
${Pangolin_LIBRARIES}
#${Fbow_LIBS}
#################################
${Gflags_LIBS_DZY}
${Glog_LIBS_DZY}
${CERES_LIBRARIES}
${G2O_LIBS_DZY}
#{Gtsam_LIBS}
#################################
${Flann_LIBS_DZY}
${Qhull_LIBS_DZY}
${PCL_LIBRARIES}
)
list(REMOVE_ITEM ALL_LIBS "vtkproj4")
message("ALL_LIBS: ${ALL_LIBS}")
###UbuFiles######################################################################
add_executable(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/source/main.cpp)
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(${PROJECT_NAME} ${ALL_LIBS})
endif(UNIX AND (${PROJECT_NAME} STREQUAL "aro"))






if(UNIX AND (${PROJECT_NAME} STREQUAL "aros"))
###ROSLibrary#################################################################
message("Platform: ROS\n\nPlatform: ROS\n\nPlatform: ROS\n\nPlatform: ROS\n\nPlatform: ROS\n\nPlatform: ROS\n\n")
add_definitions(-DROS)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../aro/include)
#1.1Qt5
find_package(Qt5 REQUIRED COMPONENTS Core)
include_directories(/usr/include/x86_64-linux-gnu/qt5)
file(GLOB Qt5_LIBS_DZY /usr/lib/x86_64-linux-gnu/libQt5*.so)
message("Qt5_LIBS_DZY: ${Qt5_LIBS_DZY}")

#1.2Boost
find_package(Boost REQUIRED COMPONENTS atomic)
include_directories(usr/include)#/boost
file(GLOB Boost_LIBS_DZY /usr/lib/x86_64-linux-gnu/libboost_*.so)
message("Boost_LIBS_DZY: ${Boost_LIBS_DZY}")

#1.3Eigen3
include_directories(/usr/include/eigen3)

#1.4Sophus
include_directories(/usr/include/sophus-dzy)

#1.5SuiteSparse(Lapack_Blas)
include_directories(/usr/include/suitesparse)
set(SuiteSparese_LIBS_DZY
libgfortran.so libblas.so liblapack.so
libgfortran.a libblas.a liblapack.a
libamd.so libcamd.so libcolamd.so libccolamd.so libcxsparse.so libldl.so libklu.so libbtf.so libumfpack.so libcholmod.so libspqr.so libsuitesparseconfig.so
libamd.a libcamd.so libcolamd.a libccolamd.a libcxsparse.a libldl.a libklu.a libbtf.a libumfpack.a libcholmod.a libspqr.a libsuitesparseconfig.a)
message("SuiteSparese_LIBS_DZY: ${SuiteSparese_LIBS_DZY}")

#1.6Octomap(DynamicEDT3D)
find_package(octomap REQUIRED)
find_package(dynamicEDT3D REQUIRED)
include_directories(usr/include)#/octomap
include_directories(usr/include)#/dynamicEDT3D
message("OCTOMAP_LIBRARIES: ${OCTOMAP_LIBRARIES}")
message("DYNAMICEDT3D_LIBRARIES: ${DYNAMICEDT3D_LIBRARIES}")

#2.1VTK
find_package(VTK REQUIRED)
include_directories(usr/include)#/vtk-6.2
message("VTK_LIBRARIES: ${VTK_LIBRARIES}")

#2.2OpenCV
find_package(OpenCV 3.3.0 REQUIRED)
include_directories(usr/local/include)#/opencv /opencv2
message("OpenCV_LIBS: ${OpenCV_LIBS}")

#2.3Pangolin
find_package(Pangolin REQUIRED)
include_directories(usr/local/include)#/pangolin
message("Pangolin_LIBRARIES: ${Pangolin_LIBRARIES}")

#2.4Fbow

#3.1Gflags
include_directories(usr/include)#/gflags
set(Gflags_LIBS_DZY /usr/lib/x86_64-linux-gnu/libgflags.so)
message("Gflags_LIBS_DZY: ${GFLAGS_LIBRARIES}")

#3.2Glog
include_directories(usr/include)#/glog
set(Glog_LIBS_DZY /usr/lib/x86_64-linux-gnu/libglog.so)
message("Glog_LIBS_DZY: ${Glog_LIBS_DZY}")

#3.3Ceres
find_package(Ceres REQUIRED)
include_directories(usr/local/include)#/ceres
message("CERES_LIBRARIES: ${CERES_LIBRARIES}")

#3.4G2O
include_directories(usr/local/include)#/g2o
file(GLOB G2O_LIBS_DZY LIST_DIRECTORIES false /usr/local/lib/libg2o_*.so)
include_directories(/usr/include/suitesparse/cxsparse)

#3.5Gtsam

#4.1Flann
include_directories(usr/include)#/flann
set(Flann_LIBS_DZY /usr/lib/x86_64-linux-gnu/libflann.so)
message("Flann_LIBS_DZY: ${Flann_LIBS_DZY}")

#4.2Qhull
include_directories(usr/include)#/qhull
set(Qhull_LIBS_DZY /usr/lib/x86_64-linux-gnu/libqhull.so)
message("Qhull_LIBS_DZY: ${Qhull_LIBS_DZY}")

#4.3PCL
find_package(PCL REQUIRED)
include_directories( ${PCL_INCLUDE_DIRS} )
add_definitions( ${PCL_DEFINITIONS} )
message("PCL_INCLUDE_DIRS: ${PCL_INCLUDE_DIRS}")

#4.4ROS
set(CMAKE_PREFIX_PATH /opt/ros/kinetic)
find_package(catkin REQUIRED COMPONENTS 
  roscpp 
  roslib 
  rostime
  rosconsole
  roscpp_serialization
  std_msgs 
  std_srvs
  geometry_msgs
  message_generation)
include_directories(/opt/ros/kinetic/include)
message(catkin_LIBRARIES: ${catkin_LIBRARIES})

set(ALL_LIBS
${Qt5_LIBS_DZY}
${Boost_LIBS_DZY}
#NoEigen
#NoSophus
${SuiteSparse_LIBS_DZY}
${OCTOMAP_LIBRARIES}
${DYNAMICEDT3D_LIBRARIES}
#################################
${VTK_LIBRARIES}
${OpenCV_LIBS}
${Pangolin_LIBRARIES}
#${Fbow_LIBS}
#################################
${Gflags_LIBS_DZY}
${Glog_LIBS_DZY}
${CERES_LIBRARIES}
${G2O_LIBS_DZY}
#{Gtsam_LIBS}
#################################
${Flann_LIBS_DZY}
${Qhull_LIBS_DZY}
${PCL_LIBRARIES}
#################################
${catkin_LIBRARIES}
)

###ROSMerits######################################################################
add_message_files(DIRECTORY include/turtle/struct FILES
  TurtleState.msg)

add_service_files(DIRECTORY include/turtle/struct FILES
  TeleopTurtle.srv
  SpawnTurtle.srv
  DeleteTurtle.srv)

generate_messages(DEPENDENCIES #Generate added messages and services with any dependencies listed here
  std_msgs 
  std_srvs
  geometry_msgs)

catkin_package(
  #INCLUDE_DIRS include
  #DEPENDS system_lib
  #CATKIN_DEPENDS genmsg roscpp rospy std_msgs
  CATKIN_DEPENDS std_msgs std_srvs geometry_msgs message_runtime)

###ROSFiles######################################################################
file(GLOB_RECURSE this_for_msg_srv_action ${CMAKE_CURRENT_SOURCE_DIR}/include/*)
add_executable(turtleMain  source/turtle/turtleMain.cpp ${this_for_msg_srv_action})
add_executable(turtleTeleop source/turtle/turtleTeleop.cpp)
add_executable(turtleSquare source/turtle/turtleSquare.cpp)
add_executable(turtleMimic source/turtle/turtleMimic.cpp)

set_target_properties(turtleMain PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(turtleTeleop PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(turtleSquare PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(turtleMimic PROPERTIES LINKER_LANGUAGE CXX)

add_dependencies(turtleMain ${PROJECT_NAME}_gencpp)
add_dependencies(turtleTeleop ${PROJECT_NAME}_gencpp)
add_dependencies(turtleSquare ${PROJECT_NAME}_gencpp)
add_dependencies(turtleMimic ${PROJECT_NAME}_gencpp)

target_link_libraries(turtleMain  ${ALL_LIBS})
target_link_libraries(turtleTeleop ${ALL_LIBS})
target_link_libraries(turtleSquare ${ALL_LIBS})
target_link_libraries(turtleMimic ${ALL_LIBS})

endif(UNIX AND (${PROJECT_NAME} STREQUAL "aros"))
