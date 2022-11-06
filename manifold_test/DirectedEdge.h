/*
 * @Author: Hao Zhang sc22hz@leeds.ac.uk
 * @Date: 2022-11-05 15:46:06
 * @LastEditors: Hao Zhang sc22hz@leeds.ac.uk
 * @LastEditTime: 2022-11-06 22:11:29
 * @FilePath: /A1_manifold_test/manifold_test/DirectedEdge.h
 * @Description: Half-Edge representation with directed-edge structure
 */
#ifndef DIRECTEDEDGE_H
#define DIRECTEDEDGE_H

#include <limits>
#include <ostream>
#include <vector>

#define UNKNOWN_HALF_EDGE std::numeric_limits<unsigned int>::max()

class FaceIndex;

class DirectedEdge
{
public:
    // structures in directed edge, continuous memory friendly
    // same as that in FaceIndex, record xyz value
    typedef unsigned int HalfEdgeRef;
    struct Vertex
    {
        float x;
        float y;
        float z;
    };
    // same as that in FaceIndex, record vertices index in CCW
    struct Face
    {
        unsigned int vertex_index[3];
    };
    // constructors
    // init from .face file
    DirectedEdge(const char *file_path);

    // init from faceindex object
    DirectedEdge(FaceIndex *face_index_model);

    // save to file
    void saveTo(const char *file_path);

    // print
    void print(std::ostream &stream);

    // getters, all are O(1)
    bool isValid() { return is_valid; }
    const char* getName() { return obj_name; }
    unsigned int getFaceCount() { return face_count; }
    unsigned int getFaceCountOfVertex(unsigned int vertex_index) {return face_count_of_vertex[vertex_index];}
    unsigned int getVertexCount() { return vertex_count; }
    const DirectedEdge::Face& getFaceAt(unsigned int index) { return faces[index]; }
    const DirectedEdge::Vertex& getVertexAt(unsigned int index) { return vertices[index]; }

    // ------------------------------all methods below this line are O(1)------------------------------------------------------
    // get index of the face that the edge is on
    unsigned int faceIndexOfHalfEdge(HalfEdgeRef edge) { return edge / 3;}

    // get the from vertex index of the edge
    // edge : [from vertex] -> to vertex
    unsigned int fromVertexIndexOfHalfEdge(HalfEdgeRef edge) {
        return getFaceAt(faceIndexOfHalfEdge(edge)).vertex_index[(edge + 2) % 3];
    }

    // get the to vertex index of the edge
    // edge : from vertex -> [to vertex]
    unsigned int toVertexIndexOfHalfEdge(HalfEdgeRef edge) {
        return this->faces[faceIndexOfHalfEdge(edge)].vertex_index[edge % 3];
    }
    
    // first half edge of the vertex
    // first falf edge is the edge that follows the vertex
    // For example: 
    //          first_falf_edge(vertex_1) == half_edge_2
    //..........................................................
    //....[vertex_0]<-------(half_edge_0)-----------[vertex_2]..
    //.........\........................................->......
    //..........\....................................../........
    //.......(half_edge_1).........................(half_edge_2)
    //............\................................../..........
    //.............\------->.[vertex_1]-------------/...........
    //..........................................................
    HalfEdgeRef firstDirectedHalfEdgeOnVertex(unsigned int vertex_index) { return first_directed_edge_of_vertex[vertex_index];}
    HalfEdgeRef nextHalfEdge(HalfEdgeRef edge) { return (edge + 1) % 3 + 3 * faceIndexOfHalfEdge(edge); }
    HalfEdgeRef prevHalfEdge(HalfEdgeRef edge) { return (edge + 2) % 3 + 3 * faceIndexOfHalfEdge(edge); }
    HalfEdgeRef otherHalfEdge(HalfEdgeRef edge) { return other_harf_of_edge[edge]; }
    // ------------------------------all methods until this line are O(1)------------------------------------------------------

    // debug information, collect all pinched edges
    std::vector<std::vector<HalfEdgeRef>> pinched_edges_group;

    ~DirectedEdge();
private:
    // properties
    /* name of this object */
    const char *obj_name;

    /* store the vertex coords, every 3 is a vertex*/
    Vertex *vertices;

    /* store the vertex indexes, every 3 is a face */
    Face *faces;

    /* store the first directed edge*/
    HalfEdgeRef *first_directed_edge_of_vertex;

    /* store the opposite edge of each edge*/
    HalfEdgeRef *other_harf_of_edge;

    /* store how many faces adjcent to the vertex */
    unsigned int *face_count_of_vertex;

    /* face count */
    unsigned int face_count;

    /* vertex count */
    unsigned int vertex_count;

    /* is valid */
    bool is_valid;

    // temporary stucture, used only in hash map
    struct HalfEdge
    {
        unsigned int vertex_from;
        unsigned int vertex_to;
    };

    /* strcuture used to build hash map */
    struct _halfedge_hashfunc
    {
        size_t operator()(const HalfEdge e) const
        {
            return size_t(e.vertex_from*14514+e.vertex_to*19);
        }	
    };
    struct _halfedge_eqfunc
    {
        bool operator()(const HalfEdge e1, HalfEdge e2) const
        {
            return (e1.vertex_from == e2.vertex_from && e1.vertex_to == e2.vertex_to);
        }	
    };
};

#endif // DIRECTEDEDGE_H