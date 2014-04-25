/*
 * transactions.cpp
 *
 *  Created on: Sep 23, 2013
 *      Author: rayabhik
 */

#include "transactions.h"
#include "graphutils.h"
#include <map>
#include <ctime>
#include <algorithm>
#include <cstdlib>

using namespace std;
// Sutanay changing MAX_NBRS macro to variable definition
// #define MAX_NBRS 1
int MAX_NBRS = 1;

void setMaxNeighbors(int neighbor_count)
{
    MAX_NBRS = neighbor_count;
    return;
}

void printMaxNeighbors()
{
    cout << "MAX_NBRS: " << MAX_NBRS << endl;
}

//currently not being used
/*FSM::Graph* Transactions::extractSubgraph2(uint64_t s, uint64_t t, float freq_threshold)
{

  double rnd;

  //cout<<"Random number: "<<rnd<<endl;
  //cout<<"Extracting subgraph for "<<s<<"->"<<t<<endl;
  //Take one hop neighborhood from 's'
  map<uint64_t, FSM::Node*> nodes;
  map<uint64_t, FSM::Node*>::iterator n_it;
  FSM::Node *src = getNode(s);
  nodes[src->id] = src;
  //add 's' to nodes;
  map<string, FSM::Edge*> edges;//string contains canonical representation
  map<string, FSM::Edge*>::iterator e_it;
  //cout<<"Extracting 1 hop neighborhood for node "<<src->id<<endl;
  // uint64_t index = 0;
  //vector<uint64_t> rnd_index;
  //for (int i = 0; i < (src->adjacent_edges).size(); i++) rnd_index.push_back(i);
  //std::random_shuffle(rnd_index.begin(), rnd_index.end());

  //Calculate the frequency of the different edge labels in the neighborhood of 'src'
  map<string, uint64_t> nbr_vev_freq;
  for (int i = 0; i< (src->adjacent_edges).size(); i++)
        {
          if((src->adjacent_edges[i])->extracted ==false)
                {
                  FSM::Node *trg1 = getNode((src->adjacent_edges[i])->target);
                  string can_label = boost::lexical_cast<string>(src->type) + " " + \
                        boost::lexical_cast<string>((src->adjacent_edges)[i]->type) + " " + boost::lexical_cast<string>(trg1->type);
                  if (nbr_vev_freq.find(can_label) == nbr_vev_freq.end())
                        {
                          nbr_vev_freq[can_label] = 1;
                        }
                  else
                        {
                          nbr_vev_freq[can_label] += 1;
                        }
                }
        }
  map<string, uint64_t>::iterator nbr_it;
  map<string, uint64_t>::iterator prev_nbr_it;
  double sum = 0.0;
  map<double, string> nbr_rev_prb;
  //nbr_it = nbr_vev_freq.begin();
  //debugging nbr_vev_freq
  cout<<"Printing nbr_vev_freq for src"<<endl;
  for (nbr_it = nbr_vev_freq.begin(); nbr_it != nbr_vev_freq.end(); ++nbr_it)
        {
          cout<<nbr_it->first<<" => "<<nbr_it->second<<endl;
        }
  cout<<"Finished printing nbr_vev_freq."<<endl;
  //finished debugging


  for(int i = 0, count = 0; i<(src->adjacent_edges).size() && count<(int)(node_type_degree[src->type]/vertex_type_freq[src->type]); i++)
    {
          rnd = (double)(rand())/(double)(RAND_MAX);
          string vev_lab;
          //find the label with this probability 'rnd'
          for(nbr_it = nbr_vev_freq.begin(); nbr_it != nbr_vev_freq.end(); ++nbr_it)
                {
                  if(rnd <= (double)(nbr_it->second)/(double)((src->adjacent_edges).size()))
                        {
                          vev_lab = nbr_it->first;
                          break;
                        }
                  rnd -= (double)(nbr_it->second)/(double)((src->adjacent_edges).size());
                  //nbr_rev_prb[sum] = nbr_it->first;
                }

          cout<<"Extract subgraph: Selected canonical label: "<<vev_lab<<" with probability "<<rnd<<endl;
          //extract first label with label=vev_label and extracted=false
          for(int j = 0; j < (src->adjacent_edges).size(); j++)
                {
                  FSM::Node *trg1 = getNode((src->adjacent_edges[j])->target);
                  string can_label = boost::lexical_cast<string>(src->type) + " " + \
                        boost::lexical_cast<string>((src->adjacent_edges)[j]->type) + " " + boost::lexical_cast<string>(trg1->type);

                  if ((can_label == vev_lab)&&((src->adjacent_edges)[j]->extracted == false))
                        {
                          cout<<"Extract subgraph: Adding trg->edge "<<can_label<<"with probability "<<rnd<<endl;

                          //add the target node for all of the edges to 'nodes'
                          nodes[(src->adjacent_edges)[j]->target] = getNode((src->adjacent_edges)[j]->target);
                          //add the edges to 'edges'
                          string canonical = (boost::lexical_cast<string>((src->adjacent_edges)[j]->source) + "," + boost::lexical_cast<string>((src->adjacent_edges)[j]->target));
                          //cout<<"Src canonical: "<<canonical<<endl;
                          if(edges.find(canonical)==edges.end())
                                {
                                  edges[canonical] = (src->adjacent_edges)[j];
                                }
                          (src->adjacent_edges)[j]->extracted = true;
                          count++;
                          //}
                        }
                }
    }
  nbr_vev_freq.clear();//clear the map

  //take one hop neighborhood from 't'



  FSM::Node *trg = getNode(t);
  nodes[trg->id] = trg;
  //Calculate the frequency of the different edge labels in the neighborhood of 'trg'
  for (int i = 0; i< (trg->adjacent_edges).size(); i++)
        {
          if((trg->adjacent_edges[i])->extracted == false)
                {
                  FSM::Node *trg1 = getNode((trg->adjacent_edges[i])->target);
                  string can_label = boost::lexical_cast<string>(trg->type) + " " + \
                        boost::lexical_cast<string>((trg->adjacent_edges)[i]->type) + " " + boost::lexical_cast<string>(trg1->type);
                  if (nbr_vev_freq.find(can_label) == nbr_vev_freq.end())
                        {
                          nbr_vev_freq[can_label] = 1;
                        }
                  else
                        {
                          nbr_vev_freq[can_label] += 1;
                        }
                }
        }
  //index = 0;
  //debugging nbr_vev_freq//
  cout<<"Printing nbr_vev_freq for trg"<<endl;
  for (nbr_it = nbr_vev_freq.begin(); nbr_it != nbr_vev_freq.end(); ++nbr_it)
        {
          cout<<nbr_it->first<<" => "<<nbr_it->second<<endl;
        }
  cout<<"Finished printing nbr_vev_freq."<<endl;
  //finished debugging//

  //cout<<"Extracting 1-hop neighborhood for node "<<trg->id<<endl;
  for(int i = 0, count = 0; i<(trg->adjacent_edges).size() && count < (int)(node_type_degree[trg->type]/vertex_type_freq[trg->type]); i++)
    {
          rnd = (double)(rand())/(double)(RAND_MAX);
          string vev_lab;
          //find the label with this probability 'rnd'
          for(nbr_it = nbr_vev_freq.begin(); nbr_it != nbr_vev_freq.end(); ++nbr_it)
                {
                  if(rnd <= (double)(nbr_it->second)/(double)((trg->adjacent_edges).size()))
                        {
                          vev_lab = nbr_it->first;
                          break;
                        }
                  rnd -= (double)(nbr_it->second)/(double)((trg->adjacent_edges).size());
                  //nbr_rev_prb[sum] = nbr_it->first;
                }

          cout<<"Extract subgraph: Selected canonical label: "<<vev_lab<<" with probability "<<rnd<<endl;

          //extract first label with label=vev_label and extracted=false
          for(int j = 0; j < (trg->adjacent_edges).size(); j++)
                {
                  FSM::Node *trg1 = getNode((trg->adjacent_edges[j])->target);
                  string can_label = boost::lexical_cast<string>(trg->type) + " " + \
                        boost::lexical_cast<string>((trg->adjacent_edges)[j]->type) + " " + boost::lexical_cast<string>(trg1->type);

                  if ((can_label==vev_lab)&&((trg->adjacent_edges)[j]->extracted == false))
                        {
                          cout<<"Extract subgraph: Adding trg->edge "<<can_label<<"with probability "<<rnd<<endl;
                          //add the target node for all of the edges to 'nodes'
                          nodes[(trg->adjacent_edges)[j]->target] = getNode((trg->adjacent_edges)[j]->target);
                          //add the edges to 'edges'
                          string canonical = boost::lexical_cast<string>((trg->adjacent_edges)[j]->source) + "," + boost::lexical_cast<string>((trg->adjacent_edges)[j]->target);
                          //cout<<"Trg Canonical:"<<canonical<<endl;
                          if(edges.find(canonical)==edges.end())
                                {
                                  edges[canonical] = (trg->adjacent_edges)[j];
                                }
                          (trg->adjacent_edges)[j]->extracted = true;
                          count++;
                          //    }
                        }
                }  //add the target node for all of the edges to 'nodes'
    }
  //take all edges that are present between all nodes
  //in one hop neighborhood, ie, only edges that connect
  //2 nodes in this two-hop neighborhood.
  //iterate over all the nodes in "nodes"
  for (n_it=nodes.begin(); n_it != nodes.end(); n_it++)
    {
      FSM::Node *n = (*n_it).second;
      vector<FSM::Edge *>::iterator adj_it;
      for(adj_it = (n->adjacent_edges).begin(); adj_it != (n->adjacent_edges).end(); adj_it++)
                {
                  FSM::Edge *e_temp = (*adj_it);         //e_temp is a synonym of adj_it and is used interchangeably
                  if((nodes.find(e_temp->source)!= nodes.end())&&(nodes.find(e_temp->target)!= nodes.end()))
                        {
                          string e_temp_can = boost::lexical_cast<string>(e_temp->source) + "," + boost::lexical_cast<string>((e_temp->target));
                          //cout<<"e_temp_can:"<<e_temp_can<<endl;
                          if((edges.find(e_temp_can) == edges.end())&&(e_temp->extracted == false))
                                {
                                  edges[e_temp_can] = (*adj_it);
                                  (*adj_it)->extracted = true;
                                }
                        }
                }
        }
  //create graph extract by adding all nodes and edges IN "nodes" and "edges" resp.
  FSM::Graph *extract = new FSM::Graph();
  map<uint64_t, FSM::Node*>::iterator n_it1;
  map<uint64_t, uint64_t> new_id_nodelist;
  uint64_t node_id = 0;
  if (edges.size() > 0)
        {
          for (n_it1 = nodes.begin(); n_it1 != nodes.end(); n_it1++)
                {
                  //DONT ADD THE SAME NODE. CREATE A NEW NODE WITH SAME PROPERTIES AS THE NODE IN "nodes" EXCEPT THE ADJACENCY LIST.
                  FSM::Node *n_temp = new FSM::Node();
                  n_temp->id = node_id;//((*n_it1).second)->id;
                  n_temp->type = ((*n_it1).second)->type;
                  n_temp->label = ((*n_it1).second)->label;
                  n_temp->timestamp = ((*n_it1).second)->timestamp;
                  new_id_nodelist[((*n_it1).second)->id] = node_id;
                  //n_temp->id = node_id;
                  node_id++;
          //id = n->id;
          //type = n->type;
          //label = n->label;
          //timestamp = n->timestamp;
                  extract->addNode(n_temp);
                }
          //cout<<"nodes.size(): "<<nodes.size()<<" edges.size(): "<<edges.size()<<endl;
          map<string, FSM::Edge*>::iterator e_it1;
          for(e_it1 = edges.begin(); e_it1 != edges.end(); e_it1++)
                {
                  FSM::Edge *temp_edge = new FSM::Edge();
                  //((*e_it1).second)->source
                  temp_edge->source = new_id_nodelist[((*e_it1).second)->source];
                  //((*e_it1).second)->target
                  temp_edge->target = new_id_nodelist[((*e_it1).second)->target];
                  temp_edge->type = ((*e_it1).second)->type;
                  temp_edge->label = ((*e_it1).second)->label;
                  temp_edge->timestamp = ((*e_it1).second)->timestamp;
                  temp_edge->directed = ((*e_it1).second)->directed;
                  temp_edge->extracted = ((*e_it1).second)->extracted;
                  extract->addEdge(temp_edge);//(*e_it1).second);
                  //cout<<"adding edge: "<<temp_edge->source<<", "<<temp_edge->target<<" directed: "<<temp_edge->directed<<endl;

                  // cout<<"adding edge: "<<((*e_it1).second)->source<<", "<<((*e_it1).second)->target<<" directed: "<<((*e_it1).second)->directed<<endl;
                }
        }

  //cout<<"Order of extracted graph: "<<extract->node_order<<endl;
  //cout<<"Size of extracted graph: "<<extract->edge_size<<endl;
  //cout<<"printing extracted graph"<<endl;
  //extract->graphPrint();
  //cout<<endl;
  cout<<"End of extract"<<endl;
  if (extract->edge_size > 0)
        {
          return extract;
        }
  else
        {
          return NULL;
        }
}
*/

//currently being used
FSM::Graph* Transactions::extractSubgraph(FSM::Graph *g, uint64_t s, uint64_t t, float freq_threshold)
{
  //cout<<"Extracting subgraph for "<<s<<"->"<<t<<endl;//DEBUG
  //Take one hop neighborhood from 's'
  map<uint64_t, FSM::Node*> nodes;
  map<uint64_t, FSM::Node*>::iterator n_it;
  FSM::Node *src = g->getNode(s);
  nodes[src->id] = src;
  //add 's' to nodes;
  map<string, FSM::Edge*> edges;//string contains canonical representation
  map<string, FSM::Edge*>::iterator e_it;
  //cout<<"Extracting 1 hop neighborhood for node "<<src->id<<endl;//DEBUG
  for(int i = 0, count = 0; i<(src->adjacent_edges).size() && count < MAX_NBRS; i++)
    {
          if (((src->adjacent_edges)[i]->extracted == false) && 1)
                {
                  FSM::Node *trg1 = g->getNode((src->adjacent_edges[i])->target);
                  // string can_label = boost::lexical_cast<string>(src->type) + " " + \
                  //    boost::lexical_cast<string>((src->adjacent_edges)[i]->type) + " " + boost::lexical_cast<string>(trg1->type);
                  //if (float(vev_type_freq[can_label]/total_vev_freq) >= freq_threshold)//construct the canonical label for the edge and check if it is frequent
                  //    {
                  //add the target node for all of the edges to 'nodes'
                  nodes[(src->adjacent_edges)[i]->target] = g->getNode((src->adjacent_edges)[i]->target);
                  //add the edges to 'edges'
                  string canonical = (boost::lexical_cast<string>((src->adjacent_edges)[i]->source) + "," + boost::lexical_cast<string>((src->adjacent_edges)[i]->target));
                  //cout<<"Src canonical: "<<canonical<<endl;//DEBUG
                  if(edges.find(canonical)==edges.end())
                        {
                          edges[canonical] = (src->adjacent_edges)[i];
                        }
                  (src->adjacent_edges)[i]->extracted = true;
                  count++;
                  //    }
                }
    }
  //take one hop neighborhood from 't'
  FSM::Node *trg = g->getNode(t);
  nodes[trg->id] = trg;
  //cout<<"Extracting 1-hop neighborhood for node "<<trg->id<<endl;//DEBUG
  for(int i = 0, count = 0; i<(trg->adjacent_edges).size() && count < MAX_NBRS; i++)
    {
      //add the target node for all of the edges to 'nodes'
          if ((trg->adjacent_edges)[i]->extracted == false)
                {
                  FSM::Node *trg1 = g->getNode((trg->adjacent_edges[i])->target);
                  // string can_label = boost::lexical_cast<string>(trg->type) + " " + \
                  //    boost::lexical_cast<string>((trg->adjacent_edges)[i]->type) + " " + boost::lexical_cast<string>(trg1->type);
                  //if (float(vev_type_freq[can_label]/total_vev_freq) >= freq_threshold)//construct the canonical label for the edge and check if it is frequent
                  //{

                  nodes[(trg->adjacent_edges)[i]->target] = g->getNode((trg->adjacent_edges)[i]->target);
                  //add the edges to 'edges'
                  string canonical = boost::lexical_cast<string>((trg->adjacent_edges)[i]->source) + "," + boost::lexical_cast<string>((trg->adjacent_edges)[i]->target);
                  //cout<<"Trg Canonical:"<<canonical<<endl;//DEBUG
                  if(edges.find(canonical)==edges.end())
                        {
                          edges[canonical] = (trg->adjacent_edges)[i];
                        }
                  (trg->adjacent_edges)[i]->extracted = true;
                  count++;
                  //}
                }
    }
  //take all edges that are present between all nodes
  //in one hop neighborhood, ie, only edges that connect
  //2 nodes in this two-hop neighborhood.
  //iterate over all the nodes in "nodes"
  for (n_it=nodes.begin(); n_it != nodes.end(); n_it++)
    {
      FSM::Node *n = (*n_it).second;
      vector<FSM::Edge *>::iterator adj_it;
      for(adj_it = (n->adjacent_edges).begin(); adj_it != (n->adjacent_edges).end(); adj_it++)
                {
                  FSM::Edge *e_temp = (*adj_it);         //e_temp is a synonym of adj_it and is used interchangeably
                  if((nodes.find(e_temp->source)!= nodes.end())&&(nodes.find(e_temp->target)!= nodes.end()))
                        {
                          string e_temp_can = boost::lexical_cast<string>(e_temp->source) + "," + boost::lexical_cast<string>((e_temp->target));
                          //cout<<"e_temp_can:"<<e_temp_can<<endl;//DEBUG
                          if((edges.find(e_temp_can) == edges.end())&&(e_temp->extracted == false))
                                {
                                  edges[e_temp_can] = (*adj_it);
                                  (*adj_it)->extracted = true;
                                }
                        }
                }
        }
  //create graph extract by adding all nodes and edges IN "nodes" and "edges" resp.
  FSM::Graph *extract = new FSM::Graph();
  extract->directed = g->directed;
  map<uint64_t, FSM::Node*>::iterator n_it1;
  map<uint64_t, uint64_t> new_id_nodelist;
  uint64_t node_id = 0;
  if (edges.size() > 0)
        {
          for (n_it1 = nodes.begin(); n_it1 != nodes.end(); n_it1++)
                {
                  //DONT ADD THE SAME NODE. CREATE A NEW NODE WITH SAME PROPERTIES AS THE NODE IN "nodes" EXCEPT THE ADJACENCY LIST.
                  FSM::Node *n_temp = new FSM::Node();
                  n_temp->id = node_id;//((*n_it1).second)->id;
                  n_temp->type = ((*n_it1).second)->type;
                  n_temp->label = ((*n_it1).second)->label;
                  n_temp->timestamp = ((*n_it1).second)->timestamp;
                  new_id_nodelist[((*n_it1).second)->id] = node_id;
                  //n_temp->id = node_id;
                  node_id++;
          //id = n->id;
          //type = n->type;
          //label = n->label;
          //timestamp = n->timestamp;
                  extract->addNode(n_temp);
                }
          //cout<<"nodes.size(): "<<nodes.size()<<" edges.size(): "<<edges.size()<<endl;//DEBUG
          map<string, FSM::Edge*>::iterator e_it1;
          for(e_it1 = edges.begin(); e_it1 != edges.end(); e_it1++)
                {
                  FSM::Edge *temp_edge = new FSM::Edge();
                  /*((*e_it1).second)->source*/temp_edge->source = new_id_nodelist[((*e_it1).second)->source];
                  /*((*e_it1).second)->target*/temp_edge->target = new_id_nodelist[((*e_it1).second)->target];
                  temp_edge->type = ((*e_it1).second)->type;
                  temp_edge->label = ((*e_it1).second)->label;
                  temp_edge->timestamp = ((*e_it1).second)->timestamp;
                  temp_edge->directed = ((*e_it1).second)->directed;
                  temp_edge->extracted = ((*e_it1).second)->extracted;
                  extract->addEdge(temp_edge);//(*e_it1).second);
                  //cout<<"adding edge: "<<temp_edge->source<<", "<<temp_edge->target<<" directed: "<<temp_edge->directed<<endl;//DEBUG

                  // cout<<"adding edge: "<<((*e_it1).second)->source<<", "<<((*e_it1).second)->target<<" directed: "<<((*e_it1).second)->directed<<endl;//DEBUG
                }
        }

  //cout<<"Order of extracted graph: "<<extract->node_order<<endl;//DEBUG
  //cout<<"Size of extracted graph: "<<extract->edge_size<<endl;//DEBUG
  //cout<<"printing extracted graph"<<endl;//DEBUG
  //extract->graphPrint();//DEBUG
  //cout<<endl;//DEBUG
  if (extract->edge_size > 0)
        {
          return extract;
        }
  else
        {
          return NULL;
        }
}

/**
 * Function: extractSubgraphOriginalNodes
 * Purpose: Creates a graph transaction from a
 *
 */
FSM::Graph* Transactions::extractSubgraphOriginalNodes(FSM::Graph *g, uint64_t s, uint64_t t, float freq_threshold)
{
  //cout<<"Extracting subgraph for "<<s<<"->"<<t<<endl;
  //Take one hop neighborhood from 's'
  map<uint64_t, FSM::Node*> nodes;
  map<uint64_t, FSM::Node*>::iterator n_it;
  FSM::Node *src = g->getNode(s);
  nodes[src->id] = src;
  //add 's' to nodes;
  map<string, FSM::Edge*> edges;//string contains canonical representation
  map<string, FSM::Edge*>::iterator e_it;
  //cout<<"Extracting 1 hop neighborhood for node "<<src->id<<endl;//DEBUG
  for(int i = 0, count = 0; i<(src->adjacent_edges).size() && count < MAX_NBRS; i++)
    {
          if (((src->adjacent_edges)[i]->extracted == false) && 1)
                {
                  FSM::Node *trg1 = g->getNode((src->adjacent_edges[i])->target);
                  // string can_label = boost::lexical_cast<string>(src->type) + " " + \
                  //    boost::lexical_cast<string>((src->adjacent_edges)[i]->type) + " " + boost::lexical_cast<string>(trg1->type);
                  //if (float(vev_type_freq[can_label]/total_vev_freq) >= freq_threshold)//construct the canonical label for the edge and check if it is frequent
                  //    {
                  //add the target node for all of the edges to 'nodes'
                  nodes[(src->adjacent_edges)[i]->target] = g->getNode((src->adjacent_edges)[i]->target);
                  //add the edges to 'edges'
                  string canonical = (boost::lexical_cast<string>((src->adjacent_edges)[i]->source) + "," + boost::lexical_cast<string>((src->adjacent_edges)[i]->target));
                  //cout<<"Src canonical: "<<canonical<<endl;//DEBUG
                  if(edges.find(canonical)==edges.end())
                        {
                          edges[canonical] = (src->adjacent_edges)[i];
                        }
                  (src->adjacent_edges)[i]->extracted = true;
                  count++;
                  //    }
                }
    }
  //take one hop neighborhood from 't'
  FSM::Node *trg = g->getNode(t);
  nodes[trg->id] = trg;
  //cout<<"Extracting 1-hop neighborhood for node "<<trg->id<<endl;//DEBUG
  for(int i = 0, count = 0; i<(trg->adjacent_edges).size() && count < MAX_NBRS; i++)
    {
      //add the target node for all of the edges to 'nodes'
          if ((trg->adjacent_edges)[i]->extracted == false)
                {
                  FSM::Node *trg1 = g->getNode((trg->adjacent_edges[i])->target);
                  // string can_label = boost::lexical_cast<string>(trg->type) + " " + \
                  //    boost::lexical_cast<string>((trg->adjacent_edges)[i]->type) + " " + boost::lexical_cast<string>(trg1->type);
                  //if (float(vev_type_freq[can_label]/total_vev_freq) >= freq_threshold)//construct the canonical label for the edge and check if it is frequent
                  //{

                  nodes[(trg->adjacent_edges)[i]->target] = g->getNode((trg->adjacent_edges)[i]->target);
                  //add the edges to 'edges'
                  string canonical = boost::lexical_cast<string>((trg->adjacent_edges)[i]->source) + "," + boost::lexical_cast<string>((trg->adjacent_edges)[i]->target);
                  //cout<<"Trg Canonical:"<<canonical<<endl;//DEBUG
                  if(edges.find(canonical)==edges.end())
                        {
                          edges[canonical] = (trg->adjacent_edges)[i];
                        }
                  (trg->adjacent_edges)[i]->extracted = true;
                  count++;
                  //}
                }
    }
  //take all edges that are present between all nodes
  //in one hop neighborhood, ie, only edges that connect
  //2 nodes in this two-hop neighborhood.
  //iterate over all the nodes in "nodes"
  for (n_it=nodes.begin(); n_it != nodes.end(); n_it++)
    {
      FSM::Node *n = (*n_it).second;
      vector<FSM::Edge *>::iterator adj_it;
      for(adj_it = (n->adjacent_edges).begin(); adj_it != (n->adjacent_edges).end(); adj_it++)
                {
                  FSM::Edge *e_temp = (*adj_it);         //e_temp is a synonym of adj_it and is used interchangeably
                  if((nodes.find(e_temp->source)!= nodes.end())&&(nodes.find(e_temp->target)!= nodes.end()))
                        {
                          string e_temp_can = boost::lexical_cast<string>(e_temp->source) + "," + boost::lexical_cast<string>((e_temp->target));
                          //cout<<"e_temp_can:"<<e_temp_can<<endl;//DEBUG
                          if((edges.find(e_temp_can) == edges.end())&&(e_temp->extracted == false))
                                {
                                  edges[e_temp_can] = (*adj_it);
                                  (*adj_it)->extracted = true;
                                }
                        }
                }
        }

  /*//DEBUG
  //Print nodes
  map<uint64_t, FSM::Node*>::iterator n_it_debug;
  map<string, FSM::Edge*>::iterator e_it_debug;
  if (edges.size() > 0)
    {
      cout<<"Vertices in extract"<<endl;
      for(n_it_debug = nodes.begin(); n_it_debug != nodes.end(); n_it_debug++)
        {
          cout<<((*n_it_debug).second)->id<<endl;
        }
      cout<<"Edges in extract"<<endl;
      for(e_it_debug = edges.begin(); e_it_debug != edges.end(); e_it_debug++)
        {
          cout<<((*e_it_debug).second)->source<< " " << ((*e_it_debug).second)->target<< endl;
        }
    }
  //DEBUG END*/
  //create graph extract by adding all nodes and edges IN "nodes" and "edges" resp.
  FSM::Graph *extract = new FSM::Graph();
  extract->directed = g->directed;
  map<uint64_t, FSM::Node*>::iterator n_it1;
  map<uint64_t, uint64_t> new_id_nodelist;
  uint64_t node_id = 0;
  if (edges.size() > 0)
        {
          for (n_it1 = nodes.begin(); n_it1 != nodes.end(); n_it1++)
                {
                  //DONT ADD THE SAME NODE. CREATE A NEW NODE WITH SAME PROPERTIES AS THE NODE IN "nodes" EXCEPT THE ADJACENCY LIST.
                  FSM::Node *n_temp = new FSM::Node();
                  n_temp->id = ((*n_it1).second)->id;
                  n_temp->type = ((*n_it1).second)->type;
                  n_temp->label = ((*n_it1).second)->label;
                  n_temp->timestamp = ((*n_it1).second)->timestamp;
                  n_temp->adjacent_edges.clear();
                  //new_id_nodelist[((*n_it1).second)->id] = node_id;
                  //n_temp->id = node_id;
                  //node_id++;
          //id = n->id;
          //type = n->type;
          //label = n->label;
          //timestamp = n->timestamp;
                  if(!(extract->addNode(n_temp)))
                    {
                      cout<<"AddNode "<<n_temp->id<<" to extract failed."<<endl;
                      exit(1);
                    }
                  /*else
                    {
                      cout<<"AddNode "<<n_temp->id<<" to extract successful."<<endl;
                    }*/
                }
          //cout<<"nodes.size(): "<<nodes.size()<<" edges.size(): "<<edges.size()<<endl;//DEBUG
          map<string, FSM::Edge*>::iterator e_it1;
          for(e_it1 = edges.begin(); e_it1 != edges.end(); e_it1++)
                {
                  FSM::Edge *temp_edge = new FSM::Edge();
                  /*((*e_it1).second)->source*/temp_edge->source = ((*e_it1).second)->source;
                  /*((*e_it1).second)->target*/temp_edge->target = ((*e_it1).second)->target;
                  temp_edge->type = ((*e_it1).second)->type;
                  temp_edge->label = ((*e_it1).second)->label;
                  temp_edge->timestamp = ((*e_it1).second)->timestamp;
                  temp_edge->directed = ((*e_it1).second)->directed;
                  temp_edge->extracted = ((*e_it1).second)->extracted;
                  extract->addEdge(temp_edge);//(*e_it1).second);
                  //cout<<"adding edge: "<<temp_edge->source<<", "<<temp_edge->target<<" directed: "<<temp_edge->directed<<endl;//DEBUG

                  // cout<<"adding edge: "<<((*e_it1).second)->source<<", "<<((*e_it1).second)->target<<" directed: "<<((*e_it1).second)->directed<<endl;//DEBUG
                }
        }

  //cout<<"Order of extracted graph: "<<extract->node_order<<endl;//DEBUG
  //cout<<"Size of extracted graph: "<<extract->edge_size<<endl;//DEBUG
  //cout<<"printing extracted graph"<<endl;//DEBUG
  //extract->graphPrint();//DEBUG
  //cout<<endl;//DEBUG
  if (extract->edge_size > 0)
        {
          return extract;
        }
  else
        {
          return NULL;
        }
}

/**
 * parse the update file and find out any connected components present
 * for each component:
 *      add all the nodes in that component to Vextract
 *      for each node in the component find an adjacent edge that has not been extracted in this present update
 *      and add its target vertex to Vextract
 *      Create a transaction with all edges that are present between the nodes in Vextract (that
 *      have not been extracted yet.)
 *
 */
unsigned Transactions::createTransactionsSubgraph(FSM::Graph *g, const char *update_file, const char *tr_file, float freq)
{
    graphutils gutils;
    vector<vector<FSM::Edge *> > components = gutils.connected_components(update_file);
    vector<vector<FSM::Edge *> >::iterator comp_iter;
    for(comp_iter = components.begin(); comp_iter != components.end();++comp_iter)
      {
        FSM::Graph *transaction = extractSubgraphComponent(g, *comp_iter,freq);
        if (transaction != NULL)
          {
            transactions.push_back(transaction);
          }
      }
    ofstream trfile(tr_file, ios::app);
    if(transactions.size() > 1)//no point in printing single transaction
          {
            //cout<<endl<<"WRITING TO "<< tr_file<<" FROM EXTRACT_SUBGRAPH"<<endl;//DEBUG
            for(int i = 0; i<transactions.size();i++)
                  {
                    //Open transaction file
                    //ofstream trfile(tr_file, ios::app);
                    //(transactions[i])->graphPrint();//DEBUG
                    (transactions[i])->graphFilePrint(trfile,i);
                  }
          }
    trfile.close();
    //6.Reset extracted bit
    for (int i = 0; i < g->node_list.size(); i++)
          {
            for (int j = 0; j < (g->node_list[i]->adjacent_edges).size(); j++)
                  {
                    ((g->node_list[i])->adjacent_edges)[j]->extracted = false;
                  }
          }
	

}

FSM::Graph * Transactions::extractSubgraphComponent(FSM::Graph *g, vector<FSM::Edge *> edge_list, float freq)
{
  vector<FSM::Edge *>::iterator edge_iter;
  FSM::Graph *union_graph = new FSM::Graph();
  map<uint64_t, uint64_t> nodemap;
  map<uint64_t, uint64_t> revnodemap;
  uint64_t nodecount = 0;
  for (edge_iter = edge_list.begin(); edge_iter != edge_list.end(); ++edge_iter)
    {
       FSM::Graph *g_temp = extractSubgraphOriginalNodes(g, (*edge_iter)->source, (*edge_iter)->target,freq);
       if(g_temp != NULL)
         {
           //g_temp->graphPrint();//DEBUG
           //cout<<endl;//DEBUG
           vector<FSM::Node *>::iterator temp_node_iter;
           for(temp_node_iter = (g_temp->node_list).begin(); temp_node_iter != (g_temp->node_list).end(); ++temp_node_iter)
             {
               FSM::Node *n = new FSM::Node();
               n->id = nodecount;//
               n->label = (*temp_node_iter)->label;
               n->timestamp = (*temp_node_iter)->timestamp;
               n->type = (*temp_node_iter)->type;
               nodemap[nodecount] = (*temp_node_iter)->id;
               revnodemap[(*temp_node_iter)->id] = nodecount;
               union_graph->addNode(n);
               nodecount++;
             }
           for(temp_node_iter = g_temp->node_list.begin(); temp_node_iter != g_temp->node_list.end(); ++temp_node_iter)
             {
               vector<FSM::Edge *>::iterator temp_edge_iter;
               for(temp_edge_iter = (*temp_node_iter)->adjacent_edges.begin(); temp_edge_iter != (*temp_node_iter)->adjacent_edges.end(); ++temp_edge_iter)
                 {
                   FSM::Edge *e = new FSM::Edge();
                   e->source = revnodemap[(*temp_edge_iter)->source];
                   e->target = revnodemap[(*temp_edge_iter)->target];
                   e->type = (*temp_edge_iter)->type;
                   e->label = (*temp_edge_iter)->label;
                   e->directed = (*temp_edge_iter)->directed;
                   e->extracted = (*temp_edge_iter)->extracted;
                   e->timestamp = (*temp_edge_iter)->timestamp;
                   union_graph->addEdge(e);
                 }
             }
         }
    }
  //union_graph->graphPrint();//DEBUG
  //cout<<endl;//DeBUG
  if(union_graph->edge_size > 0)
    {
      return union_graph;
    }
  else
    {
      return NULL;
    }
}

unsigned Transactions::createTransactions(FSM::Graph *g,const char *update_file, const char *tr_file, float freq)
{
  //For every edge use extractSubgraph to extract a Graph * object
  //and create an array of graphs and print them out in a file
  //TBD later: Go through the list of graphs and if they have common edges
  //then remove them.
  //uint64_t trans_id = 0;
  //Repeat lines for every line in file

  //1.Parse line in file (line represents edge)
  ifstream updatefile (update_file, ifstream::in) ;
  string line;
  uint64_t s, t;
  srand(time(NULL));
  if(updatefile.is_open())
    {
      while(updatefile.good())
                {
                  getline(updatefile, line);
                  //cout<<endl<<line<<endl;//DEBUG
                  //tokenize the line
                  vector<string> tokens;
                  boost::split(tokens, line, boost::is_any_of(" "));
                  if(tokens[0] == "v")
                        {
                          //do nothing
                        }

                  else if(tokens[0] == "e")
                        {
                          s = uint64_t(atoi(tokens[1].c_str()));
                          t = uint64_t(atoi(tokens[2].c_str()));
                          FSM::Node *n = g->getNode(s);
                          bool extract_flag = false;//flag to check if current edge (s,t) is not extracted or if it is
                          for (int i = 0; i < (n->adjacent_edges).size();i++)
                                {
                                  if(((n->adjacent_edges)[i]->target == t) && ((n->adjacent_edges)[i]->extracted == false))//extract edge (s,t) only if it has not already been extracted
                                        {
                                          extract_flag = true;
                                        }
                                }
                          if(extract_flag == true)
                                {
                                  FSM::Graph *temp = extractSubgraph(g,s,t,freq);
                                  if (temp != NULL)
                                        {
                                          transactions.push_back(temp);
                                        }
                                }
                          else
                                {
                                  extract_flag = false;
                                }
                        }
                  else
                        {
                          cout<<"Invalid line in update file: "<<line<<endl;
                        }
                }
          updatefile.close();
        }

  ofstream trfile(tr_file, ios::app);
  if(transactions.size() > 1)//no point in printing single transaction
        {
          //cout<<endl<<"WRITING TO "<< tr_file<<" FROM EXTRACT_SUBGRAPH"<<endl;
          for(int i = 0; i<transactions.size();i++)
                {
                  //Open transaction file
                  //ofstream trfile(tr_file, ios::app);
                  (transactions[i])->graphFilePrint(trfile,i);
                }
        }
  trfile.close();
  //6.Reset extracted bit
  for (int i = 0; i < g->node_list.size(); i++)
        {
          for (int j = 0; j < (g->node_list[i]->adjacent_edges).size(); j++)
                {
                  ((g->node_list[i])->adjacent_edges)[j]->extracted = false;
                }
        }

}

