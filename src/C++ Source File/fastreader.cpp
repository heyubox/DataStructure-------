/*Descriptions: read data from bin file*/

#include "fastreader.h"
#include <QDebug>

FastReader::FastReader(std::string root)
{
    fptr = NULL;
    fptr = fopen(root.c_str(), "rb");
    assert(fptr);
}

bool FastReader::constructBox(double* lng, double* lat, int* id, int* time) {
    if (fread(id, sizeof(int), 1, fptr)) {
        fread(time, sizeof(int), 1, fptr);
        fread(lng, sizeof(double), 1, fptr);
        fread(lat, sizeof(double), 1, fptr);
    }
    else {
        return false;
    }
    if (fread(id + 1, sizeof(int), 1, fptr)) {
        fread(time + 1, sizeof(int), 1, fptr);
        fread(lng + 1, sizeof(double), 1, fptr);
        fread(lat + 1, sizeof(double), 1, fptr);
    }
    else {
        *(id + 1) = *id;
        *(time + 1) = *time;
        *(lng + 1) = *lng;
        *(lat + 1) = *lat;
    }
    return true;
}
