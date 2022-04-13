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

//Find Closely Related Species sorted with quick sort
vector<pair<string,string>> findSiblings(string commonName);

//getters
pair<string,string> getParentName(string name);
};


