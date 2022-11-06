/*
 * @Author: Hao Zhang sc22hz@leeds.ac.uk
 * @Date: 2022-11-05 15:47:49
 * @LastEditors: Hao Zhang sc22hz@leeds.ac.uk
 * @LastEditTime: 2022-11-06 19:05:19
 * @FilePath: /A1_manifold_test/manifold_test/faceindex2directededge.cpp
 * @Description: commandline util, convert .face to .diredge file
 */
#include <iostream>
#include <fstream>

#include "DirectedEdge.h"

// convert .tri file to .face file

int main(int argc, char *args[])
{
        if (argc >= 2) {
        char *file_path = args[1];
        // build FaceIndex onject from file
        DirectedEdge obj(file_path);
        if (obj.isValid()) {
            std::cout << "Transform success with object: " << obj.getName() << std::endl;
        } else {
            std::cout << "Transform failed with object: " << obj.getName() << std::endl;
            return 0;
        }
        std::string saved_file_path;
        if (argc >=3 )
        {
            // save file has been specificed
            saved_file_path = args[2];
        }
        if (saved_file_path == "")
        {
            // setup default path
            std::string cwd = args[0];
            cwd = cwd.substr(0, cwd.find_last_of('/') + 1);
            cwd.append(obj.getName());
            cwd.append(".diredge");
            saved_file_path = cwd;
        }
        // write to file
        obj.saveTo(saved_file_path.c_str());
    } else {
        std::cout << "     Help info:" << std::endl;
        std::cout << "Usage           : ./faceindex2directededge input_file_path output_file_path" << std::endl;
        std::cout << "input_file_path : xxx.face file path, required" << std::endl;
        std::cout << "ouput_file_path : xxx.diredge file path, optional" << std::endl;
        std::cout << "                  default is $(current path)/$(input_obj_name).face" << std::endl;
    }
    return 0;
}