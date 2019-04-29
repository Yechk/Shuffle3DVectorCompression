//
//
//MAIN file to test algorithm and display some results.
//for more infomation read the vectorStructs.h file

#include "vectorStructs.h"
#include <iostream>
#include <bitset>
#include <cstring>

using namespace std;

//simple random floats
float RandFloat()
{
    double intVal = (float)(rand() % 100000);
    intVal /= 1000.0f;

    return (float)intVal;
}

int main()
{

  //for error checking
  float cumulativeError = 0.0f;

  //create and decompress 1000 vectors
  for(uint i = 0; i < 1000; i++)
  {

    cout << endl;

    //random value for vector
    vec3 value = vec3(RandFloat(),RandFloat(),RandFloat());

    //copy to second for comparison
    vec3 newVal = vec3(value.x.input, value.y.input, value.z.input);
    vec3Compressed vec;

    //run compression
    vec.Compress(newVal);

    cout << endl;
    cout << "--original value---" << endl;
    cout << fixed << value.x.input << endl;
    cout << fixed << value.y.input << endl;
    cout << fixed << value.z.input << endl;

    //decompress
    vec.Decompress(newVal);

    //calculate error for single vector
    float localError = abs(value.x.input - newVal.x.input) + abs(value.y.input - newVal.y.input) + abs(value.z.input - newVal.z.input);
    localError /= 3.0f;

    cumulativeError += localError;
  }

  //average all errors
  cumulativeError /= 1000.0f;

  cout << endl;
  cout << "average error for 1000 vectors: " << fixed << cumulativeError << endl;

  return 0;
}
