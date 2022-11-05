/*
 * @Author: Hao Zhang sc22hz@leeds.ac.uk
 * @Date: 2022-11-05 15:47:27
 * @LastEditors: Hao Zhang sc22hz@leeds.ac.uk
 * @LastEditTime: 2022-11-05 18:45:18
 * @FilePath: /A1_manifold_test/manifold_test/face2faceindex.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <iostream>
#include <fstream>

#include "FaceIndex.h"

// convert .tri file to .face file

int main(int argc, char *args[])
{
    if (argc == 2) {
        char *file_path = args[1];
        FaceIndex f(file_path);
    }
    return 0;
}