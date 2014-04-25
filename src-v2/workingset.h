/********************************************
 * Large Graph Stream Miner Project
 * Filename: loader.cc
 * Purpose: Declares the working set class 
 *          
 * 
 * Author: Abhik Ray
 ********************************************/
#ifndef WORKINGSET_H_
#define WORKINGSET_H_
//#include</home/rayabhik/research/libraries/boost_1_51_0/boost/algorithm/string.hpp>
//#include "loader.h"
#include "loader.h"
//#include "argraph.h"
//#include "argedit.h"
//#include "vf2_sub_state.h"
//#include "match.h"
//using namespace std;



typedef unsigned long uint64_t;

class FreqSubgraph
{
 public:
  FSM::Graph *subgraph;
  //vector<graph*> occurrences;//we can keep a vector as these occurrences will be disjoint as they come from disjoint graphs
  uint64_t frequency;
  long rel_freq;
  std::string DFScode;

  FreqSubgraph()
	{
	  subgraph = new FSM::Graph();
	  frequency = 0;
	}
  
  /*FreqSubgraph(FreqSubgraph *fs)
	{
	  subgraph = new graph(fs->subgraph);
	  //copy set of occurrences
	  for (int i=0; i<(fs->occurrences).size();i++)
		{
		  occurrences.push_back((fs->occurrences)[i]);
		}
	  frequency = fs->frequency;
	  //canonical.append(fs->canonical);	  
	  }  */
};

class WorkingSet
{
 public:
  std::vector<FreqSubgraph*> fs;//vector of frequent subgraphs
  std::map<std::string, FreqSubgraph*> dfs_freq_map;
  //vector<string> dfs_code_list;

  bool loadSet(const char *path);
  bool loadSet2(const char *path);
  //bool checkIsomorphism(vector<FreqSubgraph *> freqsub, vector<FSM::graph *> trans);
  void sort(std::vector<FSM::Graph *> freqsub);
};
//bool checkIsomorphism(vector<Graph *> freqsub, vector<Graph *> transactions);
class Vf2Lib
{
  //converter for a vector of graphs 

};

#endif /* WORKINGSET_H_ */
