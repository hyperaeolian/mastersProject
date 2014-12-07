/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#ifndef BINARYDATA_H_89815423_INCLUDED
#define BINARYDATA_H_89815423_INCLUDED

namespace BinaryData
{
    extern const char*   AudioApp_h;
    const int            AudioApp_hSize = 5994;

    extern const char*   AudioApp_cpp;
    const int            AudioApp_cppSize = 1819061;

    extern const char*   Design_cpp;
    const int            Design_cppSize = 10307;

    // Points to the start of a list of resource names.
    extern const char* namedResourceList[];

    // Number of elements in the namedResourceList array.
    const int namedResourceListSize = 3;

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding data and its size (or a null pointer if the name isn't found).
    const char* getNamedResource (const char* resourceNameUTF8, int& dataSizeInBytes) throw();
}

#endif
