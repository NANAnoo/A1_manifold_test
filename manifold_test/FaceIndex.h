/*
 * @Author: Hao Zhang sc22hz@leeds.ac.uk
 * @Date: 2022-11-05 15:45:21
 * @LastEditors: Hao Zhang sc22hz@leeds.ac.uk
 * @LastEditTime: 2022-11-06 16:28:33
 * @FilePath: /A1_manifold_test/manifold_test/FaceIndex.h
 * @Description: 
 *      convert polygen soup into face-index stucture
 *      head file, declare the face and vertex stucture in FaceIndex
 */
#ifndef FACEINDEX_H
#define FACEINDEX_H

#include <ostream>

class PolygenSoup;

class FaceIndex
{
public:
    // stucture in face index, continuous memory friendly
    // vertex stucture in FaceIndex
    struct Vertex
    {
        float x;
        float y;
        float z;
    };
    // face stucture in FaceIndex, just store the index of vertex in CCW
    struct Face
    {
        unsigned int vertex_index[3];
    };

    // constructors
    FaceIndex(const char *file_name);
    FaceIndex(PolygenSoup *polygen);

    // save to file
    void saveTo(const char *file_name);

    // print
    void print(std::ostream &stream);

    // getters
    unsigned int faceCount() {return face_count;}
    unsigned int vertexCount() {return vertex_count;}

    Vertex getVertexAt(unsigned int index) {return vertices[index];}
    Face getFaceAt(unsigned int index) {return faces[index];}

    const float *getVertexRawData() {return (float *)(vertices);}
    unsigned int *getFaceIndexRawData() {return (unsigned int *)(faces);}

    const char *getName() {return obj_name;};

    bool isValid() {return is_valid;}

    ~FaceIndex();

private:
    /* build model from .face file*/
    void initFromFaceIndexFile(const char *file_path);
    /* name of this object */
    const char *obj_name;
    /* store the vertex coords, every 3 is a vertex*/
    Vertex *vertices;
    /* store the vertex indexes, every 3 is a face */
    Face *faces;
    /* face count */
    unsigned int face_count;
    /* vertex count */
    unsigned int vertex_count;
    /* is valid */
    bool is_valid;

    /* strcuture used to build hash map */
    struct _vertex_hashfunc
    {
        size_t operator()(const Vertex v) const
        {
            return size_t(v.x*14514+v.y*19+v.z);
        }	
    };
    struct _vertex_eqfunc
    {
        bool operator()(const Vertex v1, Vertex v2) const
        {
            return (v1.x==v2.x && v1.y==v2.y && v1.z==v2.z);
        }	
    };
};

#endif //FACEINDEX_H