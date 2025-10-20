#ifndef CUSTOMMATRIX_H
#define CUSTOMMATRIX_H

#include <iostream>
#include <string>

using namespace std;

class CustomMatrix{
public:
    bool mat[3][3]={0};
    CustomMatrix(){

    }

    //alternate way to instantiate the class
    CustomMatrix(bool m[3][3]){
        const int m_size = sizeof(m) / sizeof(m[0]);
        for (int i=0;i< 3;i++) {
            for(int j=0; j <3 ;j++){
                //cout << i << " " << j << endl;
                mat[i][j] = m[i][j];
            }
        }
    }

    std::string toString() const {
        std::string key = "";
        for(int i=0; i<3; i++){
            for(int j=0; j<3; j++){
                key += (mat[i][j] ? '1' : '0');
            }
        }
        return key;
    }
    bool isCenterSet() const {
        return mat[1][1];
    }
    int getNeighborCount() const {
        int count = 0;
        for(int i=0; i<3; i++){
            for(int j=0; j<3; j++){
                if(i == 1 && j == 1) continue; // Lewati piksel tengah
                if(mat[i][j]) count++;
            }
        }
        return count;
    }




    void fillMatrix(bool m[3][3]){
        const int m_size = sizeof(m) / sizeof(m[0]);
        for (int i=0;i< m_size;i++) {
            for(int j=0; j < sizeof(m[0]);j++){
                //cout << i << " " << j << endl;
                mat[i][j] = m[i][j];
            }
        }
    }
};



#endif // CUSTOMMATRIX_H
