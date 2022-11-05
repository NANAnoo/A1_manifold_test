/*
 * @Author: Hao Zhang sc22hz@leeds.ac.uk
 * @Date: 2022-11-05 15:45:21
 * @LastEditors: Hao Zhang sc22hz@leeds.ac.uk
 * @LastEditTime: 2022-11-05 17:42:58
 * @FilePath: /A1_manifold_test/manifold_test/FaceIndex.h
 * @Description: 
 *      convert polygen soup into face-index stucture
 *      head file, declare the face and vertex stucture in FaceIndex
 */
#ifndef FACEINDEX_H
#define FACEINDEX_H

class PolygenSoup;

class FaceIndex
{
private:
    /* data */
public:
    FaceIndex(char *file_name);
    FaceIndex(PolygenSoup *polygen);
    ~FaceIndex();
};

#endif //FACEINDEX_H