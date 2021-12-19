#ifndef __RNUMGEN_H__
#define __RNUMGEN_H__

#include <vector>

using namespace std;

class rnumgen {
public:
    rnumgen(int seedvalue, vector<int>& v);
    int rand() const;

private:
    vector<float> F;
};

#endif