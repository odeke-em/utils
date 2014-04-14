// Author: Emmanuel Odeke <odeke@ualberta.ca>
#include <stdio.h>

#include <map>
#include <vector>
#include <iostream>

using namespace std;

inline int intCmp(const int &a, const int &b) {
    if (a != b) return a < b ? -1 : 1;
    return 0;
}

vector<int> *bySequence(int iIndex, const int &end) {
    if (iIndex < end) {
        vector<int> *seq = new vector<int>();
        while (iIndex < end) {
            seq->push_back(iIndex++);
        }
        return seq;
    } else {
        return NULL;
    }
}

template<typename T> void printVector(vector<T> *v) {
    cout << "[";
    if (v != NULL && v->size()) {
        int i=0, endIndex=v->size();
        while (1) {
            printf("%d", v->at(i));
            if (++i < endIndex) cout << " ";
            else break;
        }
    }
    cout << "]" << endl;
}

template<typename T> int bSearch(
    const vector<T> &sortedV, const T &query, int (*cmp)(const T &, const T &)
) {
    if (cmp != NULL) {
        int low=0, high=sortedV.size(), mid;
        while (low <= high) {
            mid = (low + high) >> 1;
            int comparison = cmp(sortedV[mid], query);
            if (comparison == 0) return mid;
            else if (comparison == -1) low = mid + 1;
            else high = mid - 1;
        }
    }

    return -1;
}

vector<int> *digitCancellable(const vector<int> iVect, const int numerator) {
    int shave = numerator/10, rem=numerator%10;
#ifdef DEBUG
    printf("shave: %d rem: %d\n", shave, rem);
#endif
    int start = bSearch(iVect, numerator, intCmp);
    vector<int> *results = NULL;
    if (start >= 0) {
        results = new vector<int>();
        for (int i=start; i < iVect.size(); ++i) {
            int val = iVect.at(i);
            if (val && val > numerator) {
                int lsd = val % 10,  msd = val / 10;
            #ifdef DEBUG
                printf("val: %d msd: %d lsd: %d\n", val, msd, lsd);
            #endif
                if ((lsd != 0) && (msd == rem) && ((float)(numerator)/val) == ((float)(shave)/lsd)) {
                    results->push_back(val);
                }
            }
        }
    }

    return results;
}

int main() {
    vector <int> *iVect = bySequence(10, 100);
    map<int, vector<int>* > v;

    for (int i=10; i < 100; ++i) {
        vector<int> *tmpV = digitCancellable(*iVect, i);
        if (tmpV->size() > 0) {
            v[i] = tmpV;
        } else {
            delete tmpV;
        }
    }

    map<int, vector<int> * >::iterator it=v.begin(), end=v.end();
    while (it != end) {
        printf("%d ", it->first);
        printVector(it->second);
        delete it->second;
        ++it;
    }

    delete iVect;
    return 0;
}
