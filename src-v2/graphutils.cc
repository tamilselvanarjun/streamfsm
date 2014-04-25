/*
 * graphutils.cpp
 *
 *  Created on: Sep 14, 2013
 *      Author: rayabhik
 */

#include <utility>
#include "graphutils.h"

//#include <GTL/graph.h>
//#include <GTL/components.h>
//#include <snap-core/Snap.h>
//#include <list>
#include <boost/graph/undirected_graph.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/graph/connected_components.hpp>


using namespace std;

graphutils::graphutils()
{
  // TODO Auto-generated constructor stub

}

graphutils::~graphutils()
{
  // TODO Auto-generated destructor stub
}



/**
 * Uses GTL's connected components to find the connected components
 */
/*vector<vector<FSM::Edge*> > graphutils::connected_components(const char *update_file_path)
{
  GTL::graph * G = new GTL::graph();
  vector<GTL::node> nodelist;
  map<uint64_t,GTL::node> nodemap;
  map<uint64_t, FSM::Node*> nodestore;
  map<node,uint64_t> revnodemap;
  map<string,FSM::Edge *> edgemap;
  vector<vector<FSM::Edge *> > component_list;
  //uint64_t nodecount = 1;
  string line;
  ifstream graphfile (update_file_path, ifstream::in);
  if(graphfile.is_open())
      {
        while(graphfile.good())
        {
            getline(graphfile, line);
            //tokenize the line
            vector<string> tokens;
            boost::split(tokens, line, boost::is_any_of(" "));
            //if first word is 'v' addNode
            if(tokens[0] == "v")
              {
                  GTL::node n = G->new_node();
                  FSM::Node *tempv = new FSM::Node();
                  nodelist.push_back(n);
                  tempv->id = uint64_t(atoi(tokens[1].c_str()));
                  tempv->type = uint64_t(atoi(tokens[2].c_str()));
                  tempv->label = tokens[3];
                  tempv->timestamp = 0;
                  nodemap[tempv->id] = n;
                  revnodemap[n] = tempv->id;
                  nodestore[tempv->id]=tempv;

              }
            //if first word is 'e' getNeighborhood(source)
            if(tokens[0] == "e")
              {
                FSM::Edge *tempe = new FSM::Edge();
                tempe->source = uint64_t(atoi(tokens[1].c_str()));
                tempe->target = uint64_t(atoi(tokens[2].c_str()));
                tempe->type = uint64_t(atoi(tokens[3].c_str()));
                tempe->label = tokens[4];
                tempe->timestamp = 0;
                tempe->directed = true;
                tempe->extracted = false;
                GTL::node n1 = nodemap[tempe->source];
                GTL::node n2 = nodemap[tempe->target];
                GTL::edge GTL_edge = G->new_edge(n1,n2);
                string edge = boost::lexical_cast<string>(tempe->source) + " " + boost::lexical_cast<string>(tempe->target);
                edgemap[edge] = tempe;
              }
        }
      }
  GTL::components *graph_comp = new components();
  if(graph_comp->run(*G)==GTL::algorithm::GTL_OK)
    {
      list<pair<list<node>, list<edge> > >::iterator comp_iter;
      for(comp_iter=graph_comp->components_begin();comp_iter!=graph_comp->components_end();++comp_iter)
        {
          pair<list<node>,list<edge> > component = *comp_iter;
          list<edge>::iterator edge_iter;
          list<edge> edgelist = component.second;//std::get<1>(component);
          vector<FSM::Edge *> component_edges;
          for(edge_iter = edgelist.begin(); edge_iter != edgelist.end(); ++edge_iter)
            {
              node source = (*edge_iter).source();
              node target = (*edge_iter).target();
              uint64_t source_id = revnodemap[source];
              uint64_t target_id = revnodemap[target];
              string edge = boost::lexical_cast<string>(source_id) + " " + boost::lexical_cast<string>(target_id);
              FSM::Edge *tempe = edgemap[edge];
              component_edges.push_back(tempe);
            }
          component_list.push_back(component_edges);
        }

    }



  return component_list;
}*/


/**
 * Uses GTL's connected components to find the connected components
 */
/*vector<vector<FSM::Edge*> > graphutils::connected_components(const char *update_file_path)
{
  TSnap::PNGraph G = TSnap::TNGraph::New();
  vector<uint64_t> nodelist;
  map<uint64_t, FSM::Node*> nodestore;
  map<string,FSM::Edge *> edgemap;
  vector<vector<FSM::Edge *> > component_list;
  map<uint64_t, vector<uint64_t> > neighbors;
  string line;
  ifstream graphfile (update_file_path, ifstream::in);
  if(graphfile.is_open())
      {
        while(graphfile.good())
        {
            getline(graphfile, line);
            //tokenize the line
            vector<string> tokens;
            boost::split(tokens, line, boost::is_any_of(" "));
            //if first word is 'v'
            if(tokens[0] == "v")
              {
                //DO NOTHING
              }
            //if first word is 'e'
            if(tokens[0] == "e")
              {
                FSM::Edge *tempe = new FSM::Edge();
                tempe->source = uint64_t(atoi(tokens[1].c_str()));
                tempe->target = uint64_t(atoi(tokens[2].c_str()));
                tempe->type = uint64_t(atoi(tokens[3].c_str()));
                tempe->label = tokens[4];
                tempe->timestamp = 0;
                tempe->directed = true;
                tempe->extracted = false;
                if (find(nodelist.begin(), nodelist.end(),tempe->source) == nodelist.end())
                  {
                    g->AddNode(tempe->source);
                    nodelist.push_back(tempe->source);

                  }
                if (find(nodelist.begin(), nodelist.end(),tempe->target) == nodelist.end())
                  {
                    g->AddNode(tempe->target);
                    nodelist.push_back(tempe->target);
                    neighbors[tempe->source].push_back(tempe->target);
                  }
                G->AddEdge(tempe->source, tempe->target);
                string edge = boost::lexical_cast<string>(tempe->source) + " " + boost::lexical_cast<string>(tempe->target);
                edgemap[edge] = tempe;
              }
        }
      }
  // Get weakly connected components
  TSnap::TCnComV WCnComV;
  TSnap::GetWccs(G, WCnComV);
  for (int i = 0; i < WCnComV.Len(); i++)
    {
      vector<FSM::Edge *> comp_edges;
      TSnap::TIntV comp_nodes = WCnComV[i].NIdV;
      for(int i = 0; i < comp_nodes.Len(); i++)
        {
          uint64_t v = comp_nodes[i];
          vector<uint64_t> nbrlist = neighbors[v];
          vector<uint64_t>::iterator nbr_iter;
          for (nbr_iter = nbrlist.begin(); nbr_iter != nbrlist.end(); ++nbr_iter)
            {
              string edge_canon = boost::lexical_cast<string>(v) + " " + boost::lexical_cast<string>(*nbr_iter);
              if (edgemap.find(edge_canon) != edgemap.end())
                {
                  comp_edges.push_back(edgemap[edge_canon]);
                }
            }
        }
      component_list.push_back(comp_edges);
    }
  return component_list;
}*/

struct vertex_property
{
  uint64_t Id;
};
//typedef boost::undirected_graph<VertexProperty, boost::no_property> BGraph;
typedef boost::adjacency_list<boost::setS, boost::vecS, boost::undirectedS, vertex_property, boost::no_property> BGraph;
vector<vector<FSM::Edge*> > graphutils::connected_components(const char *update_file_path)
{

  BGraph g;
  typedef BGraph::vertex_descriptor BVertex;
  typedef BGraph::edge_descriptor BEdge;
  //typedef typename Boost::
  //TSnap::PNGraph G = TSnap::TNGraph::New();
  vector<uint64_t> nodelist;
  map<uint64_t, BVertex> BVertexmap;
  map<uint64_t, FSM::Node*> nodestore;
  map<string,FSM::Edge *> edgemap;
  vector<vector<FSM::Edge *> > component_list;
  map<uint64_t, vector<uint64_t> > neighbors;
  string line;
  ifstream graphfile (update_file_path, ifstream::in);
  if(graphfile.is_open())
      {
        while(graphfile.good())
        {
            getline(graphfile, line);
            //tokenize the line
            vector<string> tokens;
            boost::split(tokens, line, boost::is_any_of(" "));
            //if first word is 'v'
            if(tokens[0] == "v")
              {
                //DO NOTHING
              }
            //if first word is 'e'
            if(tokens[0] == "e")
              {
                BVertex v0, v1;
                FSM::Edge *tempe = new FSM::Edge();
                tempe->source = uint64_t(atoi(tokens[1].c_str()));
                tempe->target = uint64_t(atoi(tokens[2].c_str()));
                tempe->type = uint64_t(atoi(tokens[3].c_str()));
                tempe->label = tokens[4];
                tempe->timestamp = 0;
                tempe->directed = true;
                tempe->extracted = false;
                //Add source vertex to boost adj list
                if (find(nodelist.begin(), nodelist.end(),tempe->source) == nodelist.end())
                  {
                    v0 = boost::add_vertex(g);
                    g[v0].Id = tempe->source;
                    nodelist.push_back(tempe->source);
                    BVertexmap[tempe->source] = v0;
                  }
                //Add target vertex to boost adj list
                if (find(nodelist.begin(), nodelist.end(),tempe->target) == nodelist.end())
                  {
                    //g->AddNode(tempe->target);
                    v1 = boost::add_vertex(g);
                    g[v1].Id = tempe->target;
                    nodelist.push_back(tempe->target);
                    BVertexmap[tempe->target] = v1;

                  }
                //G->AddEdge(tempe->source, tempe->target);
                //Add target to source's neighbor if not already present
                if (find(neighbors[tempe->source].begin(), neighbors[tempe->source].end(),tempe->target) == neighbors[tempe->source].end())
                  {
                    neighbors[tempe->source].push_back(tempe->target);
                  }
                //Add edge to boost graph only if edge not present. Multi-edges are not allowed
                string edge = boost::lexical_cast<string>(tempe->source) + " " + boost::lexical_cast<string>(tempe->target);
                if(edgemap.find(edge) == edgemap.end())
                  {
                    boost::add_edge(BVertexmap[tempe->source], BVertexmap[tempe->target],g);
                    edgemap[edge] = tempe;
                  }
              }
        }
      }
  graphfile.close();
  // Get weakly connected components
  vector<int> component(boost::num_vertices(g));
  int num = boost::connected_components(g, &component[0]);
  map <int, vector<uint64_t> > ccnum_vlist;
  pair<map<int, vector<uint64_t> >::iterator, bool > ret;
  vector<uint64_t> tempvertexvec;
  map <int, vector<uint64_t> >::iterator cc_iter;
  BGraph::vertex_iterator vi, vi_end;
  for(tie(vi, vi_end) = vertices(g); vi != vi_end; ++vi)
    {
      tempvertexvec.push_back(g[*vi].Id);
      ret = ccnum_vlist.insert(make_pair(component[*vi], tempvertexvec));
      if (!ret.second)
        {
          ret.first->second.push_back(g[*vi].Id);
        }
      tempvertexvec.clear();
    }

  for (cc_iter = ccnum_vlist.begin(); cc_iter != ccnum_vlist.end(); cc_iter++)
    {
      vector<FSM::Edge *> comp_edges;
      for (int i = 0; i < (*cc_iter).second.size(); i++)
        {
          uint64_t v = ((*cc_iter).second)[i];
          vector<uint64_t> nbrlist = neighbors[v];
          vector<uint64_t>::iterator nbr_iter;
          for(nbr_iter = nbrlist.begin(); nbr_iter != nbrlist.end(); ++nbr_iter)
            {
              string edge_canon = boost::lexical_cast<string>(v) + " " + boost::lexical_cast<string>(*nbr_iter);
              if (edgemap.find(edge_canon) != edgemap.end())
                {
                  comp_edges.push_back(edgemap[edge_canon]);
                }
            }
        }
      component_list.push_back(comp_edges);
    }
  /*TSnap::GetWccs(G, WCnComV);
  for (int i = 0; i < WCnComV.Len(); i++)
    {
      vector<FSM::Edge *> comp_edges;
      TSnap::TIntV comp_nodes = WCnComV[i].NIdV;
      for(int i = 0; i < comp_nodes.Len(); i++)
        {
          uint64_t v = comp_nodes[i];
          vector<uint64_t> nbrlist = neighbors[v];
          vector<uint64_t>::iterator nbr_iter;
          for (nbr_iter = nbrlist.begin(); nbr_iter != nbrlist.end(); ++nbr_iter)
            {
              string edge_canon = boost::lexical_cast<string>(v) + " " + boost::lexical_cast<string>(*nbr_iter);
              if (edgemap.find(edge_canon) != edgemap.end())
                {
                  comp_edges.push_back(edgemap[edge_canon]);
                }
            }
        }
      component_list.push_back(comp_edges);
    }*/
  return component_list;
}
