#include <iomanip>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <queue>
#include <set>
#include <algorithm>
#include "TaxonomyGraph.h"
using namespace std;

void TaxonomyGraph::ReadTaxonomyIDs(string filename)
{
  ifstream file(filename);
  string currentData;
  string column[4]; 
  //skip first line
  getline(file,currentData);

  //parse each line with tab delimiter
  int i = 0;
  while(getline(file,currentData,'\t'))
  {
    switch(i)
    {
      case 0:
      //taxonID
        column[0] = currentData;
        break;
      case 2:
      //parentID
        column[1] = currentData;
        break;
      case 5:
      //scientific name
        column[2] = currentData;
        break;
      case 9:
      //generalName
        column[3] = currentData;
        break;
    }

    //if the end of the line is reached
    if(currentData.find('\n') != string::npos)
    {
      //assign map values
      idToName[column[0]] = make_pair(column[3],column[2]);

      //assign graph values
      ancestorGraph[column[1]].push_back(column[0]);
      ancestorGraph[column[0]].push_back(column[1]);
      //assign childToParentID
      childToParentID[column[0]] = column[1];
      //if(ancestorGraph.find(column[0]) == ancestorGraph.end())
          //ancestorGraph[column[0]].push_back(column[1]);
      
      //get rid of \n in fron of next taxonomyID
      currentData = currentData.substr(currentData.find('\n')+1);
      column[0] = currentData;
      i = 1;
    }
    else
      i++;
  }
  
  file.close();
}

void TaxonomyGraph::ReadCommonNames(string filename)
{
  ifstream file(filename);
  string currentData;
  string column[3]; 
  //skip first line
  getline(file,currentData);

  //parse each line with tab delimiter
  int i = 0;
  while(getline(file,currentData,'\t'))
  {
    switch(i)
    {
      case 0:
      //taxonID
        column[0] = currentData;
        break;
      case 2:
      //commonName
        column[1] = currentData;
        break;
      case 4:
      //language
        column[2] = currentData;
        break;
    }

    //if the end of the line is reached
    if(currentData.find('\n') != string::npos)
    {
      //assign map values taxonID => <commonName, language>
      idToNameLang[column[0]].push_back(make_pair(column[1],column[2]));

      //get rid of \n in fron of next taxonomyID
      currentData = currentData.substr(currentData.find('\n')+1);
      column[0] = currentData;
      i = 1;
    }
    else
      i++;
  }
  
  file.close();

  //process idToNameLang into english preferred idToName
  for(auto member: idToNameLang)
  {
    for(unsigned int i = 0; i < member.second.size(); i++)
    {
      if(member.second[i].second == "eng")
      {
        string temp = idToName[member.first].second;
        idToName[member.first] = make_pair(member.second[i].first,temp);
        break;
      }
      else if(i == member.second.size() - 1)
      {
        string temp = idToName[member.first].second;
        idToName[member.first] = make_pair(member.second[i].first,temp);
      }
    }
  }

  //create nameToID
  for(auto member: idToName)
  {
    nameToID[member.second.first] = make_pair(member.second.second,member.first);
  }
}

vector<pair<string,string>> TaxonomyGraph::CommonAncestorPath(string commonName1, string commonName2)
{
  //do BFS to find ancestral path
  vector<pair<string,string>> ancestorPath;
  set<string> visited;
  queue<string> q;

  //insert commonName taxonID
  visited.insert(nameToID[commonName1].second);
  //push first element to the queue
  q.push(nameToID[commonName1].second);

  while(!q.empty())
  {
    string parentID = q.front();
    ancestorPath.push_back(make_pair(idToName[parentID].first, idToName[parentID].second));
    q.pop();

    for(unsigned int i = 0; i < ancestorGraph[parentID].size(); i++)
    {
      string childID = ancestorGraph[parentID][i];
      
        if(idToName[childID].first == commonName2)
        {
          ancestorPath.push_back(make_pair(idToName[childID].first, idToName[childID].second));
          return ancestorPath;
        }
        if(visited.find(childID) == visited.end())
        {
          visited.insert(childID);
          q.push(childID);
        }
      
    }
  }
  return ancestorPath;
}

pair<string,string> TaxonomyGraph::CommonAncestor(string commonName1, string commonName2)
{
    vector<pair<string,string>> path1 = SpeciesAncestorTree(commonName1);
    vector<pair<string,string>> path2 = SpeciesAncestorTree(commonName2);

    //find the smaller path
    int size = 0;
    if(path1.size() < path2.size())
        size = path1.size();
    else
        size = path2.size();
    //compare paths up to first difference then the previous element is the
    //common ancestor
    for(int i = 0; i < size; i++)
    {
        if(path1[i] != path2[i])
            return path1[i-1];
    }
    //if equivalent up return end of smaller path - 1
    if(path1.size() < path2.size())
        return path1[path1.size()-2];
    else
        return path2[path2.size()-2];
}

vector<pair<string,string>> TaxonomyGraph::SpeciesAncestorTree(string speciesName)
{
  vector<pair<string,string>> ancestorTree;
  string childID = nameToID[speciesName].second;

  //check if there is an ID
  if (NameExists(speciesName) == false)
      return {};

  for(unsigned int i = 0; i < childToParentID.size(); i++)
  {
    ancestorTree.push_back(make_pair(idToName[childID].first, idToName[childID].second));
    if(childID == "5T6MX")
    {
      reverse(ancestorTree.begin(),ancestorTree.end());
      return ancestorTree;
    }

    //traverse up the tree
    childID = childToParentID[childID];
  }
  //path not found
  return {};
}

vector<pair<string,string>> TaxonomyGraph::findSiblings(string commonName)
{
   string childID = nameToID[commonName].second;
   string parentID = childToParentID[childID];
   vector<string> siblingsCommon;

   //determine siblings
   for(unsigned int i = 0; i < ancestorGraph[parentID].size(); i++)
   {
       if(ancestorGraph[parentID][i] == childID || ancestorGraph[parentID][i] == childToParentID[parentID])
           continue;
       else
       {
           string siblingCommonName = idToName[ancestorGraph[parentID][i]].first;
           if(siblingCommonName.size() > 0 && count(siblingsCommon.begin(),siblingsCommon.end(),siblingCommonName) == 0)
               siblingsCommon.push_back(siblingCommonName);
       }
   }

   //do quick sort here
   quickSort<string>(siblingsCommon,0,siblingsCommon.size()-1);

   //recombine with scientific names
   vector<pair<string,string>> siblings;
   for(unsigned int i = 0; i < siblingsCommon.size(); i++)
   {
       string scientficName = nameToID[siblingsCommon[i]].first;
       siblings.push_back(make_pair(siblingsCommon[i],scientficName));
   }

   return siblings;
}

pair<string,string> TaxonomyGraph::getParentName(string name)
{
   string childID = nameToID[name].second;
   string parentID = childToParentID[childID];
   return idToName[parentID];
}


bool TaxonomyGraph::NameExists(string commonName)
{
  if(idToName[nameToID[commonName].second].first.size() > 0)
    return true;
  else
    return false;
}
