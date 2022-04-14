#pragma once
#include <vector>
#include <string>
#include <unordered_map>
using namespace std;

class TaxonomyGraph
{
private:
//Ancestor graph maps parentID to vector of childrenIDs
unordered_map<string, vector<string>> ancestorGraph;
//maps childID to its Parent used for finding complete ancestry of a single species
unordered_map<string,string> childToParentID;
//Maps common name to pair<scientificName,taxonID>
unordered_map<string, pair<string,string>> nameToID;
//Maps taxonID to pair<commonName,language>
unordered_map<string, vector<pair<string,string>>> idToNameLang;
//Maps taxonID to pair<commonName,scientificName>
unordered_map<string, pair<string,string>> idToName;
public:
//Parses the taxonomy data and inserts data into ancestorGraph, nameToID and idToName
void ReadTaxonomyIDs(string filename);
void ReadCommonNames(string filename);

//verification
bool NameExists(string commonName);

//algorithms
//returns the tree starting from the root biota down to the speciesName
vector<pair<string,string>> SpeciesAncestorTree(string speciesName);

//uses BFS and returns a vector containing both scientific and common names of
//the animals in the shortest common ancestral path between two species
vector<pair<string,string>> CommonAncestorPath(string commonName1, string commonName2);
pair<string,string> CommonAncestor(string commonName1, string commonName2);

//Quick Sort Implementation

// Merge Sort Implementation
/*template<typename T>
void merge(vector<T>& arr, int left, int mid, int right)
{
    int n1 = mid - left + 1;
    int n2 = right - mid;
    vector<T> X;
    vector<T> Y;
    for (int i = 0; i < n1; i++)
    {
        X.push_back(arr.at(left + i));
    }
    for (int j = 0; j < n2; j++)
    {
        Y.push_back(arr.at(mid + 1 + j));
    }
    int i, j, k;
    i = 0;
    j = 0;
    k = left;
    while (i < n1 && j < n2)
    {
        if (X.at(i) <= Y.at(j))
        {
            arr.at(k) = X.at(i);
            i++;
        }
        else
        {
            arr.at(k) = Y.at(j);
            j++;
        }
    k++;
}
    while (i < n1)
    {
        arr.at(k) = X.at(i);
        i++;
        k++;
    }
    while (j < n2)
    {
        arr.at(k) = Y.at(j);
        j++;
        k++;
    }
}

template<typename T>
void mergeSort(vector<T>& arr, int left, int right)
{
    if (left < right)
    {
        int mid = left + (right - left) / 2;
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}*/
  
//Find Closely Related Species sorted with quick sort
vector<pair<string,string>> findSiblings(string commonName);

//getters
pair<string,string> getParentName(string name);
};


