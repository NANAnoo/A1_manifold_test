/*
 * @Author: Hao Zhang sc22hz@leeds.ac.uk
 * @Date: 2022-11-05 15:45:31
 * @LastEditors: Hao Zhang sc22hz@leeds.ac.uk
 * @LastEditTime: 2022-11-05 23:33:17
 * @FilePath: /A1_manifold_test/manifold_test/FaceIndex.cpp
 * @Description: convert polygen soup into face-index stucture
 */
#include "FaceIndex.h"
#include "PolygenSoup.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <unordered_map>

using namespace std;

FaceIndex::FaceIndex(char *file_name)
{
    PolygenSoup polygen_soup(file_name);
    // placement new
    new (this)FaceIndex(&polygen_soup);
}

FaceIndex::FaceIndex(PolygenSoup *polygen)
{
    // set all variables to default
    this->face_count = 0;
    this->vertex_count = 0;
    this->is_valid = false;
    this->vertices = nullptr;
    this->faces = nullptr;
    
    // check if the polygen is valid or not
    if (!polygen->isValid() || polygen->size() == 0) {
        cout << "face index init failed" << endl;
        return;
    }

    // start build face index structure, use hash map to optimize
    // store Vertex -> index of the Vertex
    unordered_map<Vertex, unsigned int, _vertex_hashfunc, _vertex_eqfunc> vertices_map;

    // initialize
    char *name = new char(strlen(polygen->getName()));
    strcpy(name, polygen->getName());
    this->obj_name = name;
    this->face_count = polygen->size();
    this->faces = new Face[this->face_count];
    unsigned int index = 0;
    
    // find out all vertices, this loop is O(n)
    for (index = 0; index < this->face_count; index ++)
    {
        // iterate all face
        for (unsigned int i = 0; i < 3; i ++) {
            // get all vertices in this face
            PolygenSoup::Vertex v = (*polygen)[index].vertices[i];
            Vertex new_v({v.x, v.y, v.z});
            // transfer into FaceIndex::Vertex
            // and insert into hashmaps which can remove the same vertices
            // O(1) to find
            if (vertices_map.find(new_v) == vertices_map.end())
            {
                // find a new vertex, insert into hash map
                vertices_map[new_v] = this->vertex_count ++;
            }
        }
    }
    // init all vertices
    this->vertices = new Vertex[this->vertex_count];
    
    // insert all vertices in hashset, this loop is O(n)
    index = 0;
    for (pair<Vertex, unsigned int> p : vertices_map)
    {
        this->vertices[p.second].x = p.first.x;
        this->vertices[p.second].y = p.first.y;
        this->vertices[p.second].z = p.first.z;
        index ++;
    }

    // init all faces, this loop is O(n)
    index = 0;
    for (index = 0; index < this->face_count; index ++)
    {
        // iterate all face
        for (unsigned int i = 0; i < 3; i ++) {
            // get all vertices in this face
            PolygenSoup::Vertex v = (*polygen)[index].vertices[i];
            
            // transfer into FaceIndex::Vertex
            // find the index of the vertex, O(1) to find
            this->faces[index].vertex_index[i] = vertices_map.find(Vertex({v.x, v.y, v.z}))->second;
        }
    }
    this->is_valid = true;
}


// save to file
void FaceIndex::saveTo(const char *file_name)
{
    // file_name check
    if (file_name == nullptr) {
        cout << "FaceIndex::save failed, the inpu the file_name is empty" << endl;
        return;
    }
    
    // suffix check
    string name(file_name);
    string suffixStr = name.substr(name.find_last_of('.') + 1);
    if (suffixStr != "face") {
        // add a .face suffix
        suffixStr.append(".face");
        file_name = suffixStr.c_str();
    }

    // try to open file
    ofstream file;
    file.open(file_name, ios::out);
    if (!file.is_open()) {
        cout << "FaceIndex::save failed, create file failed" << endl;
        return;
    }
    print(file);
    file.close();
}

// print from a stream
void FaceIndex::print(ostream &stream)
{
    // First Block, information
    stream << "# University of Leeds 2022-2023\n";
    stream << "# COMP 5812M Assignment 1\n";
    stream << "# Hao Zhang\n";
    stream << "# 201670059\n";
    stream << "# \n";
    stream << "# Object Name: " << obj_name << endl;
    stream << "# Vertices=" << vertex_count << " Faces=" << face_count << endl;
    stream << "# \n";

    // Second Block, Vertices
    stream.precision(6);
    for (int i = 0; i < vertex_count; i ++)
    {
        // Vertex $(index) $(x) $(y) $(z)
        stream.setf(ios::showbase);
        stream << "Vertex " << i;
        stream.setf(ios::showpoint);
        stream << " " << vertices[i].x
               << " " << vertices[i].y
               << " " << vertices[i].z
               << endl;
    }

    // Last Block, Faces
    stream.setf(ios::showbase);
    for (int i = 0; i < face_count; i ++)
    {
        // Face $(index) $(v0_index) $(v1_index) $(v2_index)
        stream << "Face " << i 
               << " " << faces[i].vertex_index[0]
               << " " << faces[i].vertex_index[1]
               << " " << faces[i].vertex_index[2]
               << endl;
    }
}


FaceIndex::~FaceIndex()
{
    if (this->is_valid)
    {
        if (this->faces != nullptr)
            delete this->faces;
        if (this->vertices != nullptr)
            delete this->vertices;
    }
}