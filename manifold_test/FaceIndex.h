#ifndef FACEINDEX_H
#define FACEINDEX_H

class PolygenSoup;

class FaceIndex
{
private:
    /* data */
public:
    FaceIndex(/* args */);
    FaceIndex(char *file_name);
    FaceIndex(PolygenSoup *polygen);
    ~FaceIndex();
};

#endif //FACEINDEX_H