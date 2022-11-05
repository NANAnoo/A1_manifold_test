/*
 * @Author: Hao Zhang sc22hz@leeds.ac.uk
 * @Date: 2022-11-05 16:11:54
 * @LastEditors: Hao Zhang sc22hz@leeds.ac.uk
 * @LastEditTime: 2022-11-05 22:46:26
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

PolygenSoup::PolygenSoup(char *file_name)
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
    Face *current_face = nullptr;

    // try to open file
    file.open(file_name, ios::in);
    if (!file.is_open()) {
        cout << "File open failed" << endl;
        return;
    }

    // read file
    while (!file.eof())
    {
        // read line
        file.getline(line, BUFFER_LENGTH);
        token = strtok(line, separater);
        if (token == nullptr)
            continue;
        if (this->face_count == 0) {
            // read face count
            this->face_count = std::atoi(token);
            if (this->face_count <= 0) 
                goto read_file_failed;
            // alloc memory
            this->face_arr = new Face*[this->face_count];
        } else {
            // read vertex data and insert vertex into current_face
            if (current_face == nullptr) {
                // create a new face if last face is inserted
                current_face = new Face();
            }
            current_face->vertices[recorded_vertex_index].x = std::atof(token);
            token = strtok(nullptr, separater);
            if (token == nullptr)
                goto read_file_failed;
            current_face->vertices[recorded_vertex_index].y = std::atof(token);
            token = strtok(nullptr, separater);
            if (token == nullptr)
                goto read_file_failed;
            current_face->vertices[recorded_vertex_index].z = std::atof(token);
            // check face is ready or not
            if (++ recorded_vertex_index == 3) {
                // insert current face
                this->face_arr[recorded_face_count ++] = current_face;
                current_face = nullptr;
                if (recorded_face_count > this->face_count) 
                    goto read_file_failed;
                // reset vertex index
                recorded_vertex_index = 0;
            }
        }
    }
    this->is_valid = true;
    file.close();
    return;

    read_file_failed:
    cout << "Sorry, the file format is wrong." << endl;
    // release memory if needed
    if (recorded_face_count > 0) {
        for (unsigned int i = 0; i < recorded_face_count; i ++) {
            delete this->face_arr[i];
            this->face_arr[i] = nullptr;
        }
    }
    if (this->face_count > 0) {
        delete this->face_arr;
         this->face_arr = nullptr;
    }
    if (current_face) {
        delete current_face;
    }
    file.close();
}

PolygenSoup::~PolygenSoup()
{
    if (this->face_count > 0 && is_valid) {
        for (unsigned int i = 0; i < this->face_count; i ++) {
            delete this->face_arr[i];
            this->face_arr[i] = nullptr;
        }
        delete this->face_arr;
        this->face_arr = nullptr;
    }
}