cmake_minimum_required(VERSION 3.0.0)
project(manifold_test VERSION 0.1.0)

add_executable(face2faceindex ./manifold_test/face2faceindex.cpp ./manifold_test/FaceIndex.cpp ./manifold_test/PolygenSoup.cpp)
add_executable(faceindex2directededge ./manifold_test/faceindex2directededge.cpp ./manifold_test/PolygenSoup.cpp ./manifold_test/FaceIndex.cpp ./manifold_test/DirectedEdge.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
