/*
 * @Author: Hao Zhang sc22hz@leeds.ac.uk
 * @Date: 2022-11-05 15:45:55
 * @LastEditors: Hao Zhang sc22hz@leeds.ac.uk
 * @LastEditTime: 2022-11-09 01:20:08
 * @FilePath: /A1_manifold_test/manifold_test/DirectedEdge.cpp
 * @Description: 
 *      Half-Edge representation with directed-edge structure
 */
#include <iostream>
#include <fstream>
#include <cstring>
#include "DirectedEdge.h"
#include "FaceIndex.h"

using namespace std;

DirectedEdge::DirectedEdge(const char *file_path)
{
    // read .face file first
    FaceIndex model(file_path);
    if (model.isValid()) {
        // build this from the FaceIndex model
        new (this)DirectedEdge(&model);
    } else {
        is_valid = false;
        cout << "Directed Edge Build Error, the input file may be broken" << endl;
    }
}

// this process is O(n), which steps 1 to 3 are O(1), and steps 4, 5 are O(n)
DirectedEdge::DirectedEdge(FaceIndex *face_index_model)
{
    // init properties
    char *name = new char[strlen(face_index_model->getName())];
    strcpy(name, face_index_model->getName());
    this->obj_name = name;

    // copy raw data from face_index_model:
    // 1. copy vertex, O(1)
    unsigned int size = 3 * face_index_model->vertexCount();
    void *buffer = new float[size];
    memcpy(buffer, face_index_model->getVertexRawData(), sizeof(float) * size);
    this->vertices = (DirectedEdge::Vertex *)buffer;

    // 2. copy faces, O(1)
    size = 3 * face_index_model->faceCount();
    buffer = new unsigned int[size];
    memcpy(buffer, face_index_model->getFaceIndexRawData(), sizeof(unsigned int) * size);
    this->faces = (DirectedEdge::Face *)buffer;

    // 3. init other data, O(1)
    this->is_valid = true;
    this->face_count = face_index_model->faceCount();
    this->vertex_count = face_index_model->vertexCount();
    size = this->vertex_count;
    this->first_directed_edge_of_vertex = new unsigned int[size];
    this->face_count_of_vertex = new unsigned int[size];
    size = 3 * this->face_count;
    this->other_harf_of_edge = new unsigned int[size];

    // 4. assign initial value, O(n)
    for (unsigned int i = 0; i < this->vertex_count; i ++) {
        this->first_directed_edge_of_vertex[i] = UNKNOWN_HALF_EDGE;
        this->face_count_of_vertex[i] = 0;
    }
    for (unsigned int i = 0; i < 3 * this->face_count; i ++) {
        this->other_harf_of_edge[i] = UNKNOWN_HALF_EDGE;
    }

    // 5. find out first directed edge and half edge
    // iterate over all faces
    // use hash_map to acceleratre, this loop is O(n)
    unordered_map<HalfEdge, HalfEdgeRef, _halfedge_hashfunc, _halfedge_eqfunc> edge2edgeindex_map;
    for (unsigned int face_index = 0; face_index < this->face_count; face_index ++) {
        Face face = getFaceAt(face_index);
        // iterate over all three vertices on this face, this loop is O(3)
        for (unsigned int i = 0; i < 3; i ++) {
            // get vertex index and to_edge index, O(1)
            unsigned int vertex_index = face.vertex_index[i];
            HalfEdgeRef to_edge_ref = 3 * face_index + (i + 1) % 3;

            // update face count of the vertex and the to_edge, O(1)
            this->face_count_of_vertex[vertex_index] ++;

            // update first directed edge, O(1)
            if (first_directed_edge_of_vertex[vertex_index] == UNKNOWN_HALF_EDGE) {     
                first_directed_edge_of_vertex[vertex_index] = to_edge_ref;
            }
            // init edge as key, O(1)
            HalfEdge to_edge({vertex_index, face.vertex_index[(i + 1) % 3]});
            HalfEdge other_edge({to_edge.vertex_to, to_edge.vertex_from});
            
            // find key from hash_map, O(1)
            auto p = edge2edgeindex_map.find(to_edge);

            // check if the half edge is correct or not, O(1)
            if ( p != edge2edgeindex_map.end()) {
                // error, each half_edge should be unique
                // so this is a pinch edge, let's add it into group
                if (pinch_edges_groups.find(to_edge) == pinch_edges_groups.end())
                    pinch_edges_groups[to_edge].push_back(p->second);
                pinch_edges_groups[to_edge].push_back(to_edge_ref);
            } else {
                // update map, O(1)
                edge2edgeindex_map[to_edge] = to_edge_ref;
            }

            // try to find out the other_edge, O(1)
            p = edge2edgeindex_map.find(other_edge);
            if (p != edge2edgeindex_map.end()) {
                // find the half-edge pair, just update directly , O(1)
                other_harf_of_edge[to_edge_ref] = p->second;
                other_harf_of_edge[p->second] = to_edge_ref;
            }
        }
    }
}

// save to file
void DirectedEdge::saveTo(const char *file_path) 
{
    // file_name check
    if (file_path == nullptr) {
        cout << "DirectedEdge::save failed, the input file_path is empty" << endl;
        return;
    }
    
    // suffix check
    string path(file_path);
    string suffixStr = path.substr(path.find_last_of('.') + 1);
    if (suffixStr != "diredge") {
        if (path[path.size() - 1] == '/') {
            // is a dir, add name
             path.append(obj_name);
        }
        // add a .diredge suffix
        path.append(".diredge");
        file_path = path.c_str();
    }

    // try to open file
    ofstream file;
    file.open(file_path, ios::out);
    if (!file.is_open()) {
        cout << "DirectedEdge::save failed, create file failed" << endl;
        return;
    }
    print(file);
    file.close();
}

// print
void DirectedEdge::print(std::ostream &stream)
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

    // Third Block, fisrt directed edge
    for (int i = 0; i < vertex_count; i ++)
    {
        // FirstDirectedEdge $(index) $(FirstDirectedEdge)
        stream.setf(ios::showbase);
        stream << "FirstDirectedEdge " << i;
        stream << " " << first_directed_edge_of_vertex[i]
               << endl;
    }

    // Fourth Block, Faces
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

    // Last block, other half edge
    for (int i = 0; i < face_count * 3; i ++)
    {
        // OtherHalf $(index) $(OtherHalf)
        stream.setf(ios::showbase);
        stream << "OtherHalf " << i;
        stream << " " << (int)((other_harf_of_edge[i] == UNKNOWN_HALF_EDGE) ? -1 : other_harf_of_edge[i])
               << endl;
    }
}

DirectedEdge::~DirectedEdge()
{
    if (!this->is_valid)
        return;
    delete this->face_count_of_vertex;
    delete this->obj_name;
    delete this->vertices;
    delete this->faces;
    delete this->first_directed_edge_of_vertex;
    delete this->other_harf_of_edge;
}
