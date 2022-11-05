/*
 * @Author: Hao Zhang sc22hz@leeds.ac.uk
 * @Date: 2022-11-05 15:45:31
 * @LastEditors: Hao Zhang sc22hz@leeds.ac.uk
 * @LastEditTime: 2022-11-05 17:46:10
 * @FilePath: /A1_manifold_test/manifold_test/FaceIndex.cpp
 * @Description: convert polygen soup into face-index stucture
 */
#include "FaceIndex.h"
#include "PolygenSoup.h"
#include <iostream>

using namespace std;

FaceIndex::FaceIndex(char *file_name)
{
    PolygenSoup *polygen_soup = new PolygenSoup(file_name);
    new (this)FaceIndex(polygen_soup);
}

FaceIndex::FaceIndex(PolygenSoup *polygen)
{
    // check
    if (!polygen->isValid()) {
        cout << "face index init failed" << endl;
        return;
    }
}


FaceIndex::~FaceIndex()
{
}