/*
 * @Author: Hao Zhang sc22hz@leeds.ac.uk
 * @Date: 2022-11-05 16:11:54
 * @LastEditors: Hao Zhang sc22hz@leeds.ac.uk
 * @LastEditTime: 2022-11-08 14:35:10
 * @FilePath: /A1_manifold_test/manifold_test/PolygenSoup.cpp
 * @Description: 
 *      implementation of PolygenSoup
 */
#include "PolygenSoup.h"

#include <fstream>
#include <string>
#include <cstring>
#include <iostream>

#define BUFFER_LENGTH 1024

using namespace std;

PolygenSoup::PolygenSoup(const char *file_name)
{
    // initialize properties
    this->face_count = 0;
    this->face_arr = nullptr;
    this->is_valid = false;

    // file_name check
    if (file_name == nullptr)
        return;
    
    // suffix check
    string _file_name(file_name);
    string suffixStr = _file_name.substr(_file_name.find_last_of('.') + 1);
    if (suffixStr != "tri") {
        cout << "Sorry, the file is not a .tri file." << endl;
        return;
    }
    _file_name = _file_name.substr(_file_name.find_last_of('/') + 1);
    name = _file_name.substr(0, _file_name.find_first_of('.')).c_str();
    
    // prepare 
    ifstream file;
    char line[BUFFER_LENGTH];
    char *token;
    char separater[] = " :,\t";
    unsigned int recorded_face_count = 0;
    unsigned int recorded_vertex_index = 0;

    // try to open file
    file.open(file_name, ios::in);
    if (file.bad()) {
        cout << "File open failed" << endl;
        return;
    }
    // read face count
    file >> this->face_count;
    // alloc memory
    this->face_arr = new Face[this->face_count];
    // read vertex data
    for (unsigned int face_index = 0; face_index < this->face_count; face_index ++) {
        // read in three vertices in the face, 9 numbers in total
        file >> this->face_arr[face_index].vertices[0].x
             >> this->face_arr[face_index].vertices[0].y
             >> this->face_arr[face_index].vertices[0].z;
        file >> this->face_arr[face_index].vertices[1].x
             >> this->face_arr[face_index].vertices[1].y
             >> this->face_arr[face_index].vertices[1].z;
        file >> this->face_arr[face_index].vertices[2].x
             >> this->face_arr[face_index].vertices[2].y
             >> this->face_arr[face_index].vertices[2].z;
    }
    is_valid = true;
    file.close();
}

PolygenSoup::~PolygenSoup()
{
    if (this->face_arr != nullptr && is_valid) {
        if (this->face_arr != nullptr) {
            delete this->face_arr;
            this->face_arr = nullptr;
        }
    }
}