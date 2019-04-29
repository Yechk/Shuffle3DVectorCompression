//
//Alex Eager's Vector 3 Compression Algorithm
//
//Compresses the data of a 3 float vector to the space of a 2 float vector
//This is a lossy method of compression and more of an experiment than anything
//The Average margin for error after decompression is fairly low, but this of course depends on the application
//
//My reasoning for this was to create a way to compress model data in GPU RAM more efficiently.
//I have not dived into testing that implementation yet
//
//Feel free to use/change this software for any purpose
//
//
//
//

//all built in headers

#include <iostream>
#include <cstring>

using namespace std;

//These are the structures for the compression algorithm
//
//vec3 holds three float values (totalling 12 bytes)
//a union is used to hold a float or int value for bitset manipulation

union FtRep
{
  float input;
  int output;

  FtRep(float i) : input{i} {};
};

struct vec3
{
  //unions for floats
  FtRep x;
  FtRep y;
  FtRep z;

  //basic constructors
  vec3(float _x, float _y, float _z) : x{_x}, y{_y}, z{_z} {};
  vec3() : x{0.0f}, y{0.0f}, z{0.0f} {};

};

//this struct handles all the compression/decompression
struct vec3Compressed
{
  //holds the compressed 3 floats
  _int64 data;

  //call to compress a vec3
  inline void Compress(const vec3& old)
  {
    cout << "---Compressing Vec3---" << endl;

    //first store the integer values into bitsets
    _int64 stx(old.x.output);
    _int64 sty(old.y.output);
    _int64 stz(old.z.output);

    //these masks are used to separate the data
    bitset<64> headMask(0xFFFF0000);
    bitset<64> tailMask(0x0000FFFF);

    //mask off the lower bits
    bitset<64> tailx = stx & tailMask;
    bitset<64> taily = sty & tailMask;
    bitset<64> tailz = stz & tailMask;

    //average together the lower 16 bits of each float
    unsigned long average = tailx.to_ulong() + taily.to_ulong() + tailz.to_ulong();
    average /= 3;

    //convert average to bitset
    bitset<64> bitAverage(average);
    bitAverage = bitAverage << 48;
    cout << "average: " << bitAverage << endl << endl;

    cout << "x value: " << stx << endl;
    cout << "y value: " << sty << endl;
    cout << "z value: " << stz << endl << endl;

    //the upper 16 bits are masked
    stx &= headMask;
    sty &= headMask;
    stz &= headMask;

    //reorder the data packed in 8 bytes
    data |= stz >> 16;
    data |= stx << 16;
    data |= sty;
    data |= bitAverage;


    cout << "data: " << endl << data << endl << endl;
  }

  inline void Decompress(vec3& ret)
  {
    //used to mask floats
    bitset<64> dataMask(0x00000000FFFF0000);

    bitset<64> tail = data >> 48;

    //next three sections create three float values by shifting and memcpy to convert to float type
    bitset<64> newVal = dataMask & (data >> 16) | tail;
    unsigned long nn = newVal.to_ulong();
    memcpy(&ret.x.input, &nn, 4);

    newVal = dataMask & data | tail;
    nn = newVal.to_ulong();
    memcpy(&ret.y.input, &nn, 4);

    newVal = dataMask & (data << 16) | tail;
    nn = newVal.to_ulong();
    memcpy(&ret.z.input, &nn, 4);

    //print results
    cout << endl;
    cout << "--decompressed value---" << endl;
    cout << fixed << ret.x.input << endl;
    cout << fixed << ret.y.input << endl;
    cout << fixed << ret.z.input << endl;

  }
};
