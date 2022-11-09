/*
 * @Author: Hao Zhang sc22hz@leeds.ac.uk
 * @Date: 2022-11-05 15:45:31
 * @LastEditors: Hao Zhang sc22hz@leeds.ac.uk
 * @LastEditTime: 2022-11-09 01:18:51
 * @FilePath: /A1_manifold_test/manifold_test/FaceIndex.cpp
 * @Description: convert polygen soup into face-index stucture
 */
#include "FaceIndex.h"
#include "PolygenSoup.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <unordered_map>

#define BUFFER_LENGTH 1024

using namespace std;

FaceIndex::FaceIndex(const char *file_path)
{
    // set all variables to default
    this->face_count = 0;
    this->vertex_count = 0;
    this->is_valid = false;
    this->vertices = nullptr;
    this->faces = nullptr;

    // suffix check
    string _file_name(file_path);
    string suffixStr = _file_name.substr(_file_name.find_last_of('.') + 1);
    if (suffixStr == "face") {
        // build from .face file
        initFromFaceIndexFile(file_path);
        return;
    }
    // build from .tri file
    PolygenSoup polygen_soup(file_path);
    // placement new
    new (this)FaceIndex(&polygen_soup);
}

FaceIndex::FaceIndex(PolygenSoup *polygen)
{   
    // check if the polygen is valid or not
    if (!polygen->isValid() || polygen->size() == 0) {
        cout << "face index init failed" << endl;
        return;
    }

    // start build face index structure, use hash map to optimize, O(n)
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
        // iterate all face, O(3) loop
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
        // iterate all face, this loop is O(3)
        for (unsigned int i = 0; i < 3; i ++) {
            // get all vertices in this face
            PolygenSoup::Vertex v = (*polygen)[index].vertices[i];
            
            // transfer into FaceIndex::Vertex
            // find the index of the vertex, O(1)
            this->faces[index].vertex_index[i] = vertices_map.find(Vertex({v.x, v.y, v.z}))->second;
        }
    }
    this->is_valid = true;
}

void FaceIndex::initFromFaceIndexFile(const char *file_path)
{
    // prepare 
    ifstream file;
    char line[BUFFER_LENGTH];
    char *token;
    char separater[] = " :=#\t";
    bool read_file_succcess = true;

    // try to open file
    file.open(file_path, ios::in);
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
        
        // parse status
        if (strcmp(token, "Object") == 0) {
            // Name
            token = strtok(nullptr, separater);
            // $(obj_name)
            token = strtok(nullptr, separater);
            char *buffer = new char[strlen(token)];
            strcpy(buffer, token);
            obj_name = buffer;
            if (token == nullptr) {
                read_file_succcess = false;
                break;
            }
        } else if (strcmp(token, "Vertices") == 0) {
            // vertex count
            token = strtok(nullptr, separater);
            if (token != nullptr) {
                this->vertex_count = atoi(token);
            } else {
                read_file_succcess = false;
                break;
            }
            // face count
            token = strtok(nullptr, separater);
            if (token != nullptr && strcmp(token, "Faces") == 0) {
                token = strtok(nullptr, separater);
                if (token != nullptr) {
                    this->face_count = atoi(token);
                } else {
                    read_file_succcess = false;
                    break;
                }
            } else {
                read_file_succcess = false;
                break;
            }
            // alloc memory
            this->vertices = new Vertex[this->vertex_count];
            this->faces = new Face[this->face_count];
        } else if (strcmp(token, "Vertex") == 0) {
            // insert vertex data
            token = strtok(nullptr, separater);
            if (token == nullptr) {
                read_file_succcess = false;
                break;
            }
            unsigned int index = atoi(token);
            if (index < this->vertex_count) {
                // valid index, insert data
                this->vertices[index].x = atof(strtok(nullptr, separater));
                this->vertices[index].y = atof(strtok(nullptr, separater));
                this->vertices[index].z = atof(strtok(nullptr, separater));
            } else {
                read_file_succcess = false;
                break;
            }
        } else if (strcmp(token, "Face") == 0) {
            // insert face data
            token = strtok(nullptr, separater);
            if (token == nullptr) {
                read_file_succcess = false;
                break;
            }
            unsigned int index = atoi(token);
            if (index < this->face_count) {
                // valid index, insert data
                this->faces[index].vertex_index[0] = atoi(strtok(nullptr, separater));
                this->faces[index].vertex_index[1] = atoi(strtok(nullptr, separater));
                this->faces[index].vertex_index[2] = atoi(strtok(nullptr, separater));
            } else {
                read_file_succcess = false;
                break;
            }        
        }
    }
    if (!read_file_succcess) {
        // read failed
        cout << "Sorry, the file format is wrong." << endl;
        // try to release memory
        if (this->faces != nullptr)
            delete this->faces;
        if (this->vertices != nullptr)
            delete this->vertices;
    } else {
        // build success
        is_valid = true;
    }
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
    string path(file_name);
    string suffixStr = path.substr(path.find_last_of('.') + 1);
    if (suffixStr != "face") {
        if (path[path.size() - 1] == '/') {
            // is a dir, add name
             path.append(obj_name);
        }
        // add a .face suffix
        path.append(".face");
        file_name = path.c_str();
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