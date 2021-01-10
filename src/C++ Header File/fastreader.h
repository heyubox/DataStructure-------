/*read the dataSet of dot in bin file*/


#ifndef FASTREADER_H
#define FASTREADER_H

#include<cstdio>
#include<string>

class FastReader
{
public:
    FastReader(std::string);
    bool constructBox(double* , double* , int *, int *);

private:
    FILE *fptr;
};

#endif // FASTREADER_H
