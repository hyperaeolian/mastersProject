/*
  ==============================================================================

    Matrix.h
    Created: 3 Aug 2014 3:25:21pm
    Author:  milrob

 *Mark Allen Weiss
  ==============================================================================
*/

#ifndef MATRIX_H_INCLUDED
#define MATRIX_H_INCLUDED

#include <vector>

template <typename T>
class Matrix {
public:
    Matrix(int rows, int cols) : array(rows){
        for (auto& row : array)
            row.resize(cols);
    }
    
    Matrix(std::vector<std::vector<T> >v) : array {v} {}
    Matrix(std::vector<std::vector<T> >&& v) : array {std::move(v)} {}
    
    // need operator[] to return a const & for from, but plain ref for to
    const std::vector<T>& operator[](int row) const { return array[row]; }
    std::vector<T>&       operator[](int row)       { return array[row]; }
    
    int numrows() const { return array.size(); }
    int numcols() const { return numrows() ? array[0].size() : 0; }
    void resize(int rows, int cols) { for (auto& row : array) row.resize(cols); }
    
private:
    std::vector<std::vector<T> > array;
};



#endif  // MATRIX_H_INCLUDED
