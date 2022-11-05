/*
 * @Author: Hao Zhang sc22hz@leeds.ac.uk
 * @Date: 2022-11-05 16:11:37
 * @LastEditors: Hao Zhang sc22hz@leeds.ac.uk
 * @LastEditTime: 2022-11-05 17:52:08
 * @FilePath: /A1_manifold_test/manifold_test/PolygenSoup.h
 * @Description: 
 *      Polygen Soup struture, read in .tri file and 
 *      generate an object which contains basic face informations
 */
#ifndef PLOYGENSOUP_H
#define PLOYGENSOUP_H

class PolygenSoup
{
public:
    // vertex stucture in polygen soup
    struct Vertex
    {
        float x;
        float y;
        float z;
    };
    // face stucture in polygen soup, just store the index of vertex in CCW
    struct Face
    {
        Vertex vertexes[3];
    };
    
    /**
     * @description: constructor, create PolygenSoup from .tri file
     * @param {char} *file_name, .tri file
     */
    PolygenSoup(char *file_name);

    /**
     * @description: get the size of this PolygenSoup
     * @return the face count of this PolygenSoup
     */
    unsigned int size() { return face_count; }


    /**
     * @description: return if current polygen is workable
     * @return is_valid, if current polygen is workable
     */
    bool isValid() {return is_valid;}
    
    /**
     * @description: get the face at index
     * @return face stucture at index
     */
    const Face &operator[](unsigned int index) { return *face_arr[index]; }
    
    ~PolygenSoup();

private:
    /* face count in the mesh */
    unsigned int face_count;
    /* face data in the mesh, stored in array*/
    Face **face_arr;
    /* is valid or not*/
    bool is_valid;
};

#endif // PLOYGENSOUP_H