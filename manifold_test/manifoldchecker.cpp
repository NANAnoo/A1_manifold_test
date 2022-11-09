/*
 * @Author: Hao Zhang sc22hz@leeds.ac.uk
 * @Date: 2022-11-06 20:51:28
 * @LastEditors: Hao Zhang sc22hz@leeds.ac.uk
 * @LastEditTime: 2022-11-09 16:15:01
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
 * @param {DirectedEdge} &obj: The object that is represented by directed-edge
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
 * @description: search all vertices from a vertex along connected edges by DFS
 * @param {DirectedEdge} &obj: The object that is represented by directed-edge
 * @param {unsigned int} current_vertex: the vertex should be checked
 * @param {unordered_set<unsigned int>} &searched_vertices: vertices that are searched
 */
void DFS_searchVerticesFrom(DirectedEdge &obj, unsigned int current_vertex, std::unordered_set<unsigned int> &searched_vertices)
{
    if (searched_vertices.find(current_vertex) == searched_vertices.end()) {
        // find a new vertex
        searched_vertices.insert(current_vertex);
        // iterate vertices on ring-0 of the vertex:
        DirectedEdge::HalfEdgeRef first_edge = obj.firstDirectedHalfEdgeOnVertex(current_vertex);
        DirectedEdge::HalfEdgeRef next_edge = first_edge;
        do {
            // find the face of this edge
            DirectedEdge::Face f = obj.getFaceAt(obj.faceIndexOfHalfEdge(next_edge));
            // find out the next vertex in CCW sequency
            unsigned int next_vertex_on_f = 0;
            if (f.vertex_index[0] == current_vertex) {
                next_vertex_on_f = f.vertex_index[1];
            } else if (f.vertex_index[1] == current_vertex) {
                next_vertex_on_f = f.vertex_index[2];
            } else if (f.vertex_index[2] == current_vertex) {
                next_vertex_on_f = f.vertex_index[0];
            }
            // try to search from next_vertex_on_f
            DFS_searchVerticesFrom(obj, next_vertex_on_f, searched_vertices);
            // go to next face
            next_edge = obj.nextHalfEdge(obj.otherHalfEdge(next_edge));
        } while (next_edge != first_edge);
    }
}

int main(int argc, char *args[])
{
    if (argc >= 2) {
        char *file_path = args[1];
        // build DirectedEdge object from file
        DirectedEdge obj(file_path);
        if (!obj.isValid()) {
            std::cout << "Sorry, the object build failed, please check the origin file." << std::endl;
            return 0;
        }
        bool is_manifold = true;
        int face_count = obj.getFaceCount();
        int vertices_count = obj.getVertexCount();
        int half_edges_count = 3 * face_count;

        // CHECK IF the obj is manifold or not
        // Four steps in total, O(n) complexity

        // ---------------------------------------------------------------------------------------------------------------------
        // First. Check if every face is continuous or not, each half edge should have an opposite edge, O(n)
        std::vector<DirectedEdge::HalfEdgeRef> uncontinuous_edges;
        // a simple loop that iterates the array: "obj.other_harf_of_edge", O(n)
        for (unsigned int edge = 0; edge < half_edges_count; edge ++)
        {   
            if (obj.otherHalfEdge(edge) >= half_edges_count) {
                // this doesn't have a opposite edge, :(
                uncontinuous_edges.push_back(edge);
            }
        }
        // output information
        if (uncontinuous_edges.size() > 0) {
            std::cout << std::endl << "[Uncontinous face error] The object has some faces that are uncontinuous, " << std::endl;
            std::cout << "Half Edges : " << std::endl;
        }
        for (DirectedEdge::HalfEdgeRef edge : uncontinuous_edges) {
            std::cout << edge << " ";
        }
        if (uncontinuous_edges.size() > 0) {
            std::cout << std::endl << "don't have opposite edge." << std::endl;
            is_manifold = false;
        }
        // Actually, the program can return now if is_manifold == false!
        // But let's output the pinch edges!
        // if (!is_manifold) {
        //     std::cout << "The object is not a manifold :(" << std::endl;
        //     return;
        // }

        // ---------------------------------------------------------------------------------------------------------------------
        // Second. Check if there are pinch edges, O(n)
        if (obj.pinch_edges_groups.size() > 0) {
            std::cout << std::endl << "[Pinch edges error] The object has pinch edges :" << std::endl;
            is_manifold = false;
        }
        // A simple loop that iterates the hash_map: "obj.pinch_edges_groups", O(n)
        for (auto item : obj.pinch_edges_groups)
        {
            std::cout << "Half-Edges : " << std::endl;
            for (DirectedEdge::HalfEdgeRef edge : item.second) {
                std::cout << edge << " ";
            }
            std::cout << std::endl << "are duplicated that represent the same half edge:" << std::endl;
            DirectedEdge::HalfEdge edge = item.first;
            std::cout << "vertex[" << edge.vertex_from << "]-->vertex[" << edge.vertex_to <<"]" << std::endl << std::endl; 
        }
        if (!is_manifold) {
            std::cout << std::endl << "The object is not a manifold :(" << std::endl;
            // if there are error edges, no need to continue to check pinch points, happy to retuen
            return 0;
        }

        // ---------------------------------------------------------------------------------------------------------------------
        // Third. Check if there is any pinch vertex, O(n) = O(2 * E), E is the number of edges
        std::vector<DirectedEdge::HalfEdgeRef> pinched_vertices;
        // Complexity of the loop is :
        //   O(degree(v0)) + O(degree(v1)) + ... + O(degree(vn)) = O(2 * E)
        //   because, degree(v0) + degree(v1) + ... + degree(vn) = 2 * E, according to Handshaking Theorem
        for (unsigned int index = 0; index < vertices_count; index ++)
        {
            // >>>> Pinch vertex has only one circle <<<<
            // So if I travel from the vertex along a circle
            // The number of faces that I passed by, should be the number of faces that attach to the vertex
            // 
            // Complexity of faceCountAroundfVertexOnMesh():
            // And the number of the faces that attach to a vertex is equal to the number of edges that attach to the vertex
            // That number is also called the degree of the vertex 
            // So the complexity of faceCountAroundfVertexOnMesh is O(d), where d is the degree of the vertex
            int circle_face_count = faceCountAroundfVertexOnMesh(obj, index);
            if (circle_face_count < obj.getFaceCountOfVertex(index)) {
                // So lucky that I have already recorded how many faces that attach to a vertex
                // So this step is O(n)
                pinched_vertices.push_back(index);
            }
        }
        if (pinched_vertices.size() > 0) {
            std::cout << std::endl << "[Pinch points error] The object has pinch points " << std::endl;
            std::cout << "pinched points : ";
            is_manifold = false;
        }
        for (unsigned int index : pinched_vertices) {
            std::cout << index << " ";
        }
        std::cout << std::endl;
        if (!is_manifold) {
            std::cout << std::endl << "The object is not a manifold :(" << std::endl;
            return 0;
        }

        // ---------------------------------------------------------------------------------------------------------------------
        // Last. Check if The mesh is continuous or not, O(n) = O(E + V)
        // Start from an arbitary vertex and search it's neighbour vertices and neighbour neighbour vertices and so on
        //
        // And by this way, DFS (Depth-First-Search), we can get a set of vertices 
        // that each of them can be accessed from the initial vertex along a batch of connected edges
        // 
        // After that, check if all vertices in The object can be searched from the initial vertex
        // use DFS (Depth-First-Search) whose complexity is O(E + V), E = number of edges, V = number of vertices
        std::unordered_set<unsigned int> searched_vertices;
        DFS_searchVerticesFrom(obj, 0, searched_vertices);
        if (searched_vertices.size() < obj.getVertexCount()) {
            // can't pass by all vertices, the manifold is not continous
            is_manifold = false;
            std::cout << "The object is uncontinuous, so it is not manifold :(" << std::endl;
        }
        if (!is_manifold) return 0;
        
        // if object is a manifold, calculate genus via Euler Formula
        // Euler Formula: vertices_count + faces_count - edges_count = 2 - 2 * genus
        // genus = 1 + (edges_count - vertices_count - faces_count) / 2
        // edges_count = half_edges_count / 2
        if (is_manifold) {
            int genus = 1 + (half_edges_count / 2 - vertices_count - face_count) / 2;
            std::cout << "The object is manifold :)" << std::endl;
            std::cout << "Genus of The object is " << genus << std::endl;
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