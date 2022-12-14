/*
 * @Author: Hao Zhang sc22hz@leeds.ac.uk
 * @Date: 2022-11-05 15:47:27
 * @LastEditors: Hao Zhang sc22hz@leeds.ac.uk
 * @LastEditTime: 2022-11-06 16:16:58
 * @FilePath: /A1_manifold_test/manifold_test/face2faceindex.cpp
 * @Description: commandline util, convert .tri to .face file
 */
#include <iostream>
#include <fstream>
#include <string>
#include <dirent.h>

#include "FaceIndex.h"

// convert .tri file to .face file

int main(int argc, char *args[])
{
    if (argc >= 2) {
        char *file_path = args[1];
        // build FaceIndex onject from file
        FaceIndex f(file_path);
        if (f.isValid()) {
            std::cout << "Transform success with object: " << f.getName() << std::endl;
        } else {
            std::cout << "Transform failed with object: " << f.getName() << std::endl;
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
            cwd.append(f.getName());
            cwd.append(".face");
            saved_file_path = cwd;
        }
        // write to file
        f.saveTo(saved_file_path.c_str());
    } else {
        std::cout << "     Help info:" << std::endl;
        std::cout << "Usage           : ./face2faceindex input_file_path output_file_path" << std::endl;
        std::cout << "input_file_path : xxx.tri file path, required" << std::endl;
        std::cout << "ouput_file_path : xxx.face file path, optional" << std::endl;
        std::cout << "                  default is $(current path)/$(input_obj_name).face" << std::endl;
    }
    return 0;
}