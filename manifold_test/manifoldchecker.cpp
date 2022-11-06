/*
 * @Author: Hao Zhang sc22hz@leeds.ac.uk
 * @Date: 2022-11-06 20:51:28
 * @LastEditors: Hao Zhang sc22hz@leeds.ac.uk
 * @LastEditTime: 2022-11-06 23:19:17
 * @FilePath: /A1_manifold_test/manifold_test/manifoldchecker.cpp
 * @Description: 
 *      check if the object is manifold or not
 *      if this is a manifold, output the genus count
 *      otherwise, output the information that fails
 */


#include <iostream>
#include <fstream>
#include <vector>
#include <limits>
#include <unordered_set>

#include "DirectedEdge.h"

/**
 * @description: get count of the face around a vertex
 * @param {DirectedEdge} &obj: the mesh that is represented by directed-edge
 * @param {unsigned int} vertex_index: the vertex should be checked
 * @return count of the face around the vertex
 */
int faceCountAroundfVertexOnMesh(DirectedEdge &obj, unsigned int vertex_index)
{
    DirectedEdge::HalfEdgeRef first_edge = obj.firstDirectedHalfEdgeOnVertex(vertex_index);
    DirectedEdge::HalfEdgeRef next_edge = first_edge;
    int around_face_count = 0;
    do {
        next_edge = obj.nextHalfEdge(obj.otherHalfEdge(next_edge));
        around_face_count ++;
        if (around_face_count > obj.getFaceCount()) {
            // endless loop, error
            return -1;
        }
    } while (next_edge != first_edge);
    return around_face_count;
}

/**
 * @description: iterate all vertex from a vertex along evry edges by BFS, ring-0 -> .. -> ring-n
 * @param {DirectedEdge} &obj: the mesh that is represented by directed-edge
 * @param {unsigned int} vertex_index: the vertex should be checked
 * @param {unordered_set<unsigned int>} &passedby_vertices: vertices that are passed by
 */
void iterateAllVertexOnMesh(DirectedEdge &obj, unsigned int vertex_index, std::unordered_set<unsigned int> &passedby_vertices)
{
    if (passedby_vertices.find(vertex_index) == passedby_vertices.end()) {
        // find a new vertex
        passedby_vertices.insert(vertex_index);
        // iterate ring-0 of the vertex:
        DirectedEdge::HalfEdgeRef first_edge = obj.firstDirectedHalfEdgeOnVertex(vertex_index);
        DirectedEdge::HalfEdgeRef next_edge = first_edge;
        do {
            // find the face of this edge
            unsigned int face_index = obj.faceIndexOfHalfEdge(next_edge);
            DirectedEdge::Face f = obj.getFaceAt(face_index);
            for (unsigned int i = 0; i < 3; i ++) {
                // iterate vertex in the face
                unsigned int sub_vertex_index = f.vertex_index[i];
                if (sub_vertex_index != vertex_index) {
                    // try to iterate sub_vertex
                    iterateAllVertexOnMesh(obj, sub_vertex_index, passedby_vertices);
                }
            }
            // go to next face
            next_edge = obj.nextHalfEdge(obj.otherHalfEdge(next_edge));
        } while (next_edge != first_edge);
    }
}

int main(int argc, char *args[])
{
    if (argc >= 2) {
        char *file_path = args[1];
        // build DirectedEdge onject from file
        DirectedEdge obj(file_path);
        if (!obj.isValid()) {
            std::cout << "Sorry, the object build failed, please check the origin file." << std::endl;
            return 0;
        }
        bool is_manifold = true;
        int face_count = obj.getFaceCount();
        int vertices_count = obj.getVertexCount();
        int half_edges_count = 3 * face_count;
        // check if the obj is manifold or not:
        // First. Check the half edge is pinched or not
        if (obj.pinched_edges_group.size() > 0) {
            std::cout << "The mesh is not manifold, has pinch edges " << std::endl;
            is_manifold = false;
        }
        for (std::vector<DirectedEdge::HalfEdgeRef> edges : obj.pinched_edges_group)
        {
            std::cout << "Half Edges : ";
            for (DirectedEdge::HalfEdgeRef edge : edges) {
                std::cout << edge << " ";
            }
            std::cout << "are pinched toghther." << std::endl;
        }
        if (!is_manifold) return 0;

        // Second. Check if the face is continuous or not, is if each half edge has opposite edge
        std::vector<DirectedEdge::HalfEdgeRef> uncontinuous_edges;
        for (unsigned int index = 0; index < half_edges_count; index ++)
        {
            if (obj.otherHalfEdge(index) >= half_edges_count) {
                uncontinuous_edges.push_back(index);
            }
        }
        if (uncontinuous_edges.size() > 0) {
            std::cout << "The mesh is not manifold, some faces are uncontinuous " << std::endl;
            std::cout << "Half Edges : ";
        }
        for (DirectedEdge::HalfEdgeRef edge : uncontinuous_edges) {
            std::cout << edge << " ";
        }
        if (uncontinuous_edges.size() > 0) {
            std::cout << "don't have opposite edge." << std::endl;
            is_manifold = false;
        }
        if (!is_manifold) return 0;

        // Third. Check if there is any pinch vertex
        std::vector<DirectedEdge::HalfEdgeRef> pinched_vertices;
        for (unsigned int index = 0; index < vertices_count; index ++)
        {
            int around_face_count = faceCountAroundfVertexOnMesh(obj, index);
            if (around_face_count < obj.getFaceCountOfVertex(index)) {
                pinched_vertices.push_back(index);
            }
        }
        if (pinched_vertices.size() > 0) {
            std::cout << "The mesh is not manifold, has pinch points " << std::endl;
            std::cout << "pinched points : ";
            is_manifold = false;
        }
        for (unsigned int index : pinched_vertices) {
            std::cout << index << " ";
        }
        std::cout << std::endl;
        if (!is_manifold) return 0;

        // Last. Check if the mesh is continuous or not,
        // iterate over all vertex along the edge, check if all vertices can be passed by
        std::unordered_set<unsigned int> passedby_vertices;
        iterateAllVertexOnMesh(obj, 0, passedby_vertices);
        if (passedby_vertices.size() < obj.getVertexCount()) {
            // can't pass by all vertices, the manifold is not continous
            is_manifold = false;
            std::cout << "The mesh is not manifold, mesh is uncontinuous " << std::endl;
        }
        if (!is_manifold) return 0;
        
        // if is manifold, calculate genus via Euler Formula
        // Euler Formula: vertices_count + faces_count - edges_count = 2 - 2 * genus
        // genus = 1 + (edges_count - vertices_count - faces_count) / 2
        // edges_count = half_edges_count / 2
        if (is_manifold) {
            int genus = 1 + (half_edges_count / 2 - vertices_count - face_count) / 2;
            std::cout << "The mesh is manifold !" << std::endl;
            std::cout << "Genus of the mesh is " << genus << std::endl;
        }
    } else {
        std::cout << "     Help info:" << std::endl;
        std::cout << "Usage           : ./manifoldchecker input_file_path" << std::endl;
        std::cout << "input_file_path : xxx.face or xxx.tri file path" << std::endl << std::endl;
        std::cout << "Description     : check if the object is manifold or not" << std::endl;
        std::cout << "                  if this is a manifold, output the genus count" << std::endl;
        std::cout << "                  otherwise, output the information that fails" << std::endl;
    }
    return 0;
}