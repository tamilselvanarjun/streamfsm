/********************************************
 * Large Graph Stream Miner Project
 * Filename: loader.h
 * Purpose: Declares Edge, Node and Graph 
 *          classes. 
 * 
 * Author: Abhik Ray
 ********************************************/

#ifndef LOADER_H_
#define LOADER_H_

#include<iostream>
#include<vector>
#include<map>
#include<fstream>
#include<boost/algorithm/string.hpp>
#include<boost/algorithm/string/classification.hpp>
#include<boost/lexical_cast.hpp>



namespace FSM{

typedef unsigned long int uint64_t;
//map<uint64_t, uint64_t> edge_type_freq;//stores frequency of edge labels
//map<uint64_t, uint64_t> vertex_type_freq;//stores frequency of vertex labels
//map<string, uint64_t> vev_type_freq;//stores frequency of source vertex, edge, target vertex label combination

class Edge
{
  public:
  uint64_t source;
  uint64_t target;
  unsigned type;
  std::string label;
  time_t timestamp;
  bool directed;
  bool extracted;
  Edge(){}
  Edge(Edge *e);
    /* {
      source = e->source;
      target = e->target;
      type = e->type;
      label = e->label;
      timestamp = e->timestamp;
    }*/
  Edge(uint64_t, uint64_t, unsigned, std::string, time_t, bool);
};

class Node
{
  public:
  uint64_t id;
  unsigned type;
  std::string label;
  time_t timestamp;
  std::vector<Edge *> adjacent_edges;
  Node(){}
  Node(Node *);
  bool push_edge(Edge *e)
  {
    try
      {
	adjacent_edges.push_back(e);
      }
    catch(std::bad_alloc&)
      {
	std::cout<<"Error pushing edge"<<std::endl;
	return false;
      }
    return true;    
  }
  bool operator ==(const Node *other)
    {
    return (this->id == other->id);
    }
};

//map<string, uint64_t> dfs_freq_map;

class Graph
{
 public:
  uint64_t edge_size;
  uint64_t node_order;
  bool directed;
 public:
  Graph()
	{
	  edge_size = 0;
	  node_order = 0;
	  node_list.clear();
	  //transactions.clear();
	}
  Graph(Graph *);
  Graph(std::vector<Node>, std::vector<Edge>);
  std::vector<Node *> node_list;//Maybe use Map or Set to keep unique copies
  std::vector<uint64_t> node_index; // only stores Node Ids

  //vector<Edge> edge_list;
  Node * getNode(uint64_t);
  std::vector<Node *>::iterator getVertices();
  bool addNode(Node *);
  bool addEdge(Edge *);
  bool addUndirectedEdge(Edge *);
  std::vector<Edge>::iterator getNeighborhood(uint64_t);
  uint64_t getEdgeSize(){return edge_size;}
  uint64_t getNodeSize(){return node_order;}
  bool loadGraph(const char *);//Load graph from file
  void graphPrint();
  void graphFilePrint(std::ofstream &tfile, int i);
  void graph_statistics(uint64_t);
  //uint64_t merge(WorkingSet *w);
  //long calc_rel_support(WorkingSet *);
};

};

#endif /* LOADER_H_ */
