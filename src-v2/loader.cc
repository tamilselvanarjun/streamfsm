/********************************************
 * Large Graph Stream Miner Project
 * Filename: loader.cc
 * Purpose: Declares Edge, Node and Graph 
 *          classes. 
 * 
 * Author: Abhik Ray
 ********************************************/
//#include</home/rayabhik/research/libraries/boost_1_51_0/boost/algorithm/string.hpp>
//#include "loader.h"
#include "loader.h"
//#include "graphutils.h"
#include <ctime>
#include <algorithm>
#include <cstdlib>


using namespace std;

map<uint64_t, uint64_t> edge_type_freq;//edge label freq
map<uint64_t, uint64_t> vertex_type_freq;//vertex label freq
map<string, uint64_t> vev_type_freq;//source, edge, dest freq
map<uint64_t, uint64_t> node_type_degree;//total degree by node type


long total_vev_freq = 0;
long total_edge_freq = 0;

FSM::Edge::Edge(Edge *e)
{
  source = e->source;
  target = e->target;
  type = e->type;
  label = e->label;
  timestamp = e->timestamp;
  directed = e->directed;
}

FSM::Edge::Edge(uint64_t s, uint64_t t, unsigned type_id,	\
     string label, time_t tstamp, bool d)
{
  source = s;
  target = t;
  type = type_id;
  this->label = label;
  timestamp = tstamp;
  directed = d;
  //might have to add in the change to the 'extracted' data member
}

//Very useful for copying existing node to new node without replicating the adjacency list
FSM::Node::Node(Node *n)
{
  id = n->id;
  type = n->type;
  label = n->label;
  timestamp = n->timestamp;
}

FSM::Node * FSM::Graph::getNode(uint64_t node_id)
{
  //cout<<"Node list size: "<<node_list.size()<<endl;//DEBUG
  for (int i = 0; i < node_list.size(); i++)
	{
	  //cout<<"node id: "<<node_list[i]->id<<endl;//DEBUG
	  if(node_list[i]->id == node_id)
		{
		  return (node_list[i]);
		}
	}
  cout<<"Node with ID "<<node_id<<" not found!!"<<endl;
  return NULL;

}

vector<FSM::Node *>::iterator  FSM::Graph::getVertices()//to return pointer/vector or not to
{
  return node_list.begin();  
}

/*bool FSM::Graph::addNode(Node *n)
{
  bool node_added = false;
  try
	{
        if(find(node_index.begin(),node_index.end(),n->id) != node_index.end())
          {
            node_list.push_back(n);
            node_added = true;
          }
	}
  catch(bad_alloc&)
	{
	  cout<<"Error pushing node"<<endl;
	  exit(1);
	}
  if (node_added == true)
    {
      node_order++;
      node_index.push_back(n->id);
      return true;
    }
  cout<<"Node "<< n->id <<" added!!"<<endl;
  return true;//This needs to be changed to return false and loadgraph needs to be updated as well as anywhere else addNode is used.
//Perform exception handling of push_back here
//return false;
}*/

bool FSM::Graph::addNode(FSM::Node *n)
{
  try
        {
        node_list.push_back(n);
        }
  catch(bad_alloc&)
        {
          cout<<"Error pushing node"<<endl;
          exit(1);
        }
  node_order++;
  //cout<<"Node "<< n->id <<" added!!"<<endl;//DEBUG
  return true;
//Perform exception handling of push_back here
//return false;
}


bool FSM::Graph::addEdge(Edge *e)
{
  FSM::Node *temp = getNode(e->source);
  //temp->push_edge(e);
  //cout<<"Adding edge: e "<<e->source<<" -> "<<e->target<<endl;//DEBUG
  if(!(temp->push_edge(e)))
	{
	  return false;
	}
  edge_size++;
  return true;
}

bool FSM::Graph::addUndirectedEdge(Edge *e)
{
  FSM::Node *s = getNode(e->source);
  FSM::Node *t = getNode(e->target);
  if(!(s->push_edge(e)))
    {
      return false;
    }
  Edge *e_temp = new Edge(e);
  e_temp->source = e->target;
  e_temp->target = e->source;
  if(!(t->push_edge(e_temp)))
    {
      return false;
    }
  edge_size++;
  return true;
}


bool FSM::Graph::loadGraph(const char *path)
{
  ifstream graphfile (path, ifstream::in) ;
  string line;
  string can_label;

  if(graphfile.is_open())
    {
      while(graphfile.good())
        {


		  getline(graphfile, line);
		  //cout<<line<<endl;
		  //tokenize the line
		  vector<string> tokens;
		  boost::split(tokens, line, boost::is_any_of(" "));
		  // for(int i=0; i<tokens.size();i++)
		  //	{
			  //if first word is 'v' addNode
			  if(tokens[0] == "v")
				{
				  FSM::Node *tempv = new FSM::Node;//if vector creates it's own memory then this would create a memory leak
				  tempv->id = uint64_t(atoi(tokens[1].c_str()));
				  tempv->type = uint64_t(atoi(tokens[2].c_str()));
				  tempv->label = tokens[3];
				  tempv->timestamp = 0;//tokens[4];
				  if (vertex_type_freq.find(tempv->type) == vertex_type_freq.end())
					{
					  vertex_type_freq[tempv->type] = 1;//init freq to 1
					}
				  else
					{
					  vertex_type_freq[tempv->type] = vertex_type_freq[tempv->type] + 1;//incr. freq
					}
				  if (!(addNode(tempv)))
					{
					  cout<<"Add Node failed!!"<<endl;
					  exit(1);
					}
				  /*else
				    {
				      cout<<"Added node "<<getNode(tempv->id)->id<<endl;
				    }*/
				  /*vector<Node *>::iterator nodelist_iter;
				  for(nodelist_iter = node_list.begin(); nodelist_iter != node_list.end(); ++nodelist_iter)
				    {
				      cout<<(*nodelist_iter)->id<<" ";
				    }
				  cout<<endl;*/
				}
			  //if first word is 'e' getNeighborhood(source)
			  if(tokens[0] == "e")
				{
				  FSM::Edge *tempe = new FSM::Edge;//if vector creates it's own memory then this would create a memory leak
				  tempe->source = uint64_t(atoi(tokens[1].c_str()));
				  tempe->target = uint64_t(atoi(tokens[2].c_str()));
				  tempe->type = uint64_t(atoi(tokens[3].c_str()));
				  tempe->label = tokens[4];
				  tempe->timestamp = 0;//tokens[5];
				  tempe->directed = true;
				  tempe->extracted = false;
				  if (edge_type_freq.find(tempe->type) == edge_type_freq.end())
					{
					  edge_type_freq[tempe->type] = 1;
					}
				  else
					{
					  edge_type_freq[tempe->type] = edge_type_freq[tempe->type] + 1;
					}
				  can_label = boost::lexical_cast<string>(getNode(tempe->source)->type) + " " + \
					boost::lexical_cast<string>(tempe->type) + " " + boost::lexical_cast<string>(getNode(tempe->target)->type);
				  if(vev_type_freq.find(can_label) == vev_type_freq.end())
					{
					  vev_type_freq[can_label] = 1;
					}
				  else
					{
					  vev_type_freq[can_label] = vev_type_freq[can_label] + 1;
					}
				  if(node_type_degree.find(getNode(tempe->source)->type) != node_type_degree.end())
					{
					  node_type_degree[getNode(tempe->source)->type] = 1;
					}
				  else
					{
					  node_type_degree[getNode(tempe->source)->type] += 1;
					}
				  if (directed == true)
				    {
				      if(!(addEdge(tempe)))
				        {
				          cout<<"Add Edge failed!!"<<endl;
				          exit(1);
				        }
				    }
				  else
				    {
				      if(!(addUndirectedEdge(tempe)))
				        {
				          cout<<"Add Undirected Edge failed!!"<<endl;
				          exit(1);
				        }
				    }
				}
			  //}
		  //If sourcenode not found, error message
		  //If found, Add edge to  neighborhood
		}
	  //sum up the total vev_types and store in total_vev_freq
	  std::map<string, uint64_t>::iterator vev_iter;
	  total_vev_freq = 0;
	  for (vev_iter = vev_type_freq.begin(); vev_iter != vev_type_freq.end();++vev_iter)
		{
		  total_vev_freq += vev_iter->second;
		}
	  map<uint64_t, uint64_t>::iterator e_it;
	  total_edge_freq = 0;
	  for(e_it = edge_type_freq.begin(); e_it != edge_type_freq.end(); ++e_it)
		{
		  total_edge_freq += e_it->second;
		}
	}
  graphfile.close();
  return true;
}

/**
 * Calculates graph statistics:
 * a) Max_Degree
 * b) Min_degree
 * c) Avg_degree
 */
void FSM::Graph::graph_statistics(uint64_t batch_num)
{
  vector<FSM::Node *>::iterator vertex_iter;
  uint64_t max_degree = 0;
  uint64_t min_degree = 2000;
  uint64_t total_degree = 0;
  for(vertex_iter = node_list.begin(); vertex_iter != node_list.end(); ++vertex_iter)
    {
      uint64_t temp_degree = (*vertex_iter)->adjacent_edges.size();
      if(temp_degree > max_degree)
        {
          max_degree = temp_degree;
        }
      else if(temp_degree < min_degree)
        {
          min_degree = temp_degree;
        }
      total_degree += temp_degree;
    }
  cout<<"BATCH :"<<batch_num<<endl;
  cout<<"Max_degree: "<<max_degree<<endl;
  cout<<"Min_degree: "<<max_degree<<endl;

}
vector<FSM::Edge>::iterator FSM::Graph::getNeighborhood(uint64_t)
{
  //currently getNode works the same
}



void FSM::Graph::graphPrint()
{
  cout<<"Printing graph:"<<endl;
  cout<<"Order: "<<node_list.size()<<endl;
  for(int i = 0; i < node_list.size(); i++)
	{
	  cout<<"v "<<node_list[i]->id<<" "<<node_list[i]->type<<" "
		<<node_list[i]->label<<" "<<node_list[i]->timestamp<<endl;
	}
  for(int i = 0;i < node_list.size();i++)
	{
	  for(int j = 0; j < node_list[i]->adjacent_edges.size(); j++)
		{
		  cout<<"e "<<node_list[i]->adjacent_edges[j]->source<<" "
			  <<node_list[i]->adjacent_edges[j]->target<<" "<<node_list[i]->adjacent_edges[j]->type<<endl;
		}
	}
  cout<<endl;
}

void FSM::Graph::graphFilePrint(ofstream &tfile, int t)
{
  //UNDIRECTED GRAPH; CHANGE MULTIEDGE TO SINGLE
  vector<string> edge_canonical_vector;

  tfile<<"t # "<< t <<endl;
  for(int i = 0; i < node_list.size(); i++)
	{
	  //tfile<<"v "<<node_list[i]->id<<" "<<node_list[i]->type<<" "
	  //  <<node_list[i]->label<<" "<<node_list[i]->timestamp<<endl;
	  tfile<<"v "<<(node_list[i]->id)<<" "<</*node_list[i]->type<<*/node_list[i]->type<<endl;
	}
  for(int i = 0;i < node_list.size();i++)
	{
	  for(int j = 0; j < node_list[i]->adjacent_edges.size(); j++)
		{
		  /*tfile<<"e "<<node_list[i]->adjacent_edges[j]->source<<" "
			   <<node_list[i]->adjacent_edges[j]->target<<" "
			   <<node_list[i]->adjacent_edges[j]->type<<" "
			   <<node_list[i]->adjacent_edges[j]->label<<" "
			   <<node_list[i]->adjacent_edges[j]->timestamp<<endl;*/
	          string edge_canonical;
	          uint64_t source = node_list[i]->adjacent_edges[j]->source;
	          uint64_t target = node_list[i]->adjacent_edges[j]->target;
	          unsigned edge_type = node_list[i]->adjacent_edges[j]->type;
	          if (source < target)
	            {
	              edge_canonical = boost::lexical_cast<string>(source) + "," + boost::lexical_cast<string>(edge_type) + "," + boost::lexical_cast<string>(target);
	            }
	          else
	            {
	              edge_canonical = boost::lexical_cast<string>(target) + "," + boost::lexical_cast<string>(edge_type) + "," + boost::lexical_cast<string>(source);
	            }
	          if(find(edge_canonical_vector.begin(), edge_canonical_vector.end(), edge_canonical) == edge_canonical_vector.end())
	            {
	              edge_canonical_vector.push_back(edge_canonical);
	              tfile<<"e "<<boost::lexical_cast<string>(node_list[i]->adjacent_edges[j]->source)<<" "
			   <<boost::lexical_cast<string>(node_list[i]->adjacent_edges[j]->target)<<" "
			   <<boost::lexical_cast<string>(node_list[i]->adjacent_edges[j]->type)<<endl;
	            }
		}
	}
  //tfile<<endl;
}
/*
void Graph::graphFilePrint()
{
  for(int i = 0; i < node_list.size(); i++)
	{
	  string line = "v " + boost::lexical_cast<string>(node_list[i]->id) + \
		" " + boost::lexical_cast<string>(node_list[i]->type) + \
		" " + node_list[i]->label + " " + \
		boost::lexical_cast<string>(node_list[i]->timestamp) + "\n";
	  
	  
	}
  for(int i = 0;i < node_list.size();i++)
	{
	  for(int j = 0; j < node_list[i]->adjacent_edges.size(); j++)
		{
		  cout<<"e "<<node_list[i]->adjacent_edges[j]->source<<" "
			  <<node_list[i]->adjacent_edges[j]->target<<endl;
		}
	}
	}*/

/*uint64_t merge(WorkingSet *w, FSM::Graph *g)
{
  uint64_t count=0;   
  uint64_t max = 0;
  uint64_t totfreq = 0;
  uint64_t meanfreq = 0;
  for(int i=0;i <(w->fs).size();i++)
    {
      totfreq += (w->fs[i])->frequency;
    }
  meanfreq = (totfreq+1) / ((w->fs).size()+1);
  for(int i=0; i<(w->fs).size();i++)
    {
      if((w->fs[i])->frequency < meanfreq)
		{
		  for(int j=0;j<(w->fs[i])->frequency * 0.1;j++)
			{
			  (g->transactions).push_back((w->fs[i])->subgraph);
			  count++;
			}
		}
    }
  return count;
}*/

/*bool sgiso(graph *a, graph *b)
{
  ARGEdit ed1, ed2;
  map<int, int> nodemap;
  int i,j;
  for (int n = 0; n<a->node_order; n++)
	{
	  ed1.InsertNode(&(a->node_list[n]->type));
	  nodemap[a->node_list[n]->id] = n;
	}
  for(int n = 0; n<a->node_order;n++)
	{
	  for(int m = 0; m < a->edge_size; m++)
		{
		  Edge *e = (a->node_list[n])->adjacent_edges[m];
		  ed1.InsertEdge(nodemap[e->source], nodemap[e->target],&(e->type));
		}
	}
  nodemap.clear();
  for(int n = 0; n < b->node_order; n++)
	{
	  ed2.InsertNode(&(b->node_list[n]->type));
	  nodemap[b->node_list[n]->id] = n;
	}
  for(int n = 0; n<b->node_order;n++)
	{
	  for(int m = 0; m < b->node_list[n]->adjacent_edges.size(); m++)
		{
		  Edge *e = b->node_list[n]->adjacent_edges[m];
		  ed2.InsertEdge(nodemap[e->source], nodemap[e->target],&(e->type));
		}
	}
  nodemap.clear();
  Graph fgraph(&ed1);
  Graph tgraph(&ed2);
  VF2SubState s0(&fgraph, &tgraph);
  int n;
  node_id fn[a->node_order], tn[b->node_order];
  return match(&s0, &n, fn, tn);
}*/

/*bool sort_subgraph_size(FreqSubgraph a, FreqSubgraph b)
{
  return (a.subgraph->edge_size > b.subgraph->edge_size);
}*/

/*void calc_rel_support(vector <FreqSubgraph *> &fs)
{
  //Sort the entire vector in terms of size
  //std::stable_sort(fs.begin(), fs.end(),sort_subgraph_size);
  // Compare every subgraph with every subgraph larger than it to check if it is a subgraph. If yes subtract the frequency of the larger from the smaller.
  for(int i = 0;i < fs.size();i++)
	{
	  for (int j = 1;j<fs.size();i++)
		{
		  if (sgiso(fs[i]->subgraph, fs[j]->subgraph))
			{
			  fs[i]->rel_freq -= fs[j]->frequency;
			}
		}
	}
  //That should do
}*/

