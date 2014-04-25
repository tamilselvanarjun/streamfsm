/********************************************
 * Large Graph Stream Miner Project
 * Filename: loader.cc
 * Purpose: Declares the working set class 
 *          
 * 
 * Author: Abhik Ray
 ********************************************/
//#include</home/rayabhik/research/libraries/boost_1_51_0/boost/algorithm/string.hpp>
#include "workingset.h"

using namespace std;

//Used for loading gSpan output into Graph*
bool WorkingSet::loadSet(const char *path)
{
  cout<<"Loadset 1"<<endl;
  ifstream graphfile (path, ifstream::in);
  string line;
  
  if(graphfile.is_open())
    {
	  int index = 0;
      while(graphfile.good())
		{
		  getline(graphfile, line);
		  //tokenize the line
		  vector<string> tokens;
		  boost::split(tokens, line, boost::is_any_of(" "));
		  // for(int i=0; i<tokens.size();i++)
		  //	{
			  //if first word is 'v' addNode
		  //cout<<"Index: "<<index<<" tokens[0]:"<<tokens[0]<<"$"<<endl;//DEBUG
		  if(tokens[0] == "t")//new transaction
			{
			
			  FreqSubgraph *ftemp = new FreqSubgraph();
			  fs.push_back(ftemp);//if vector creates it's own memory then this would create a memory leak
			  //cout<<"Frequency from file "<<tokens[4].c_str()<<endl;//DEBUG
			  fs[index]->frequency = uint64_t(atoi(tokens[4].c_str()));
			  /*tempv->id = uint64_t(atoi(tokens[1].c_str()));
			  tempv->type = uint64_t(atoi(tokens[2].c_str()));
			  tempv->label = tokens[3];
			  tempv->timestamp = 0;//tokens[4];
			  if (!(subgraph->addNode(tempv)))
				{
				  cout<<"Add Node failed!!"<<endl;
				  exit(1);
				  }*/
			}
		  else if(tokens[0] == "v")
				{
				  FSM::Node *tempv = new FSM::Node;//if vector creates it's own memory then this would create a memory leak
				  tempv->id = uint64_t(atoi(tokens[1].c_str()));
				  tempv->type = uint64_t(atoi(tokens[2].c_str()));
				  tempv->label = "";//tokens[3];
				  tempv->timestamp = 0;//tokens[4];
				  if (!((fs[index]->subgraph)->addNode(tempv)))
					{
					  cout<<"Add Node failed!!"<<endl;
					  exit(1);
					}
				}
		  //if first word is 'e' getNeighborhood(source)
		  else if(tokens[0] == "e")
			{
			  FSM::Edge *tempe = new FSM::Edge;//if vector creates it's own memory then this would create a memory leak
			  tempe->source = uint64_t(atoi(tokens[1].c_str()));
			  tempe->target = uint64_t(atoi(tokens[2].c_str()));
			  tempe->type = uint64_t(atoi(tokens[3].c_str()));
			  tempe->label = "";//tokens[4];
			  tempe->timestamp = 0;//tokens[5];
			  tempe->directed = true;
			  tempe->extracted = false;
			  if(!((fs[index]->subgraph)->addEdge(tempe)))
				{
				  cout<<"Add Edge failed!!"<<endl;
				  exit(1);
				}
			}
		  else if(tokens[0] == "" || tokens[0] == "\n" || tokens[0] == "\t")
			{
			  //fs[index]->subgraph->graphPrint();
			  index++;
			  cout<<endl;
			  continue;
			}
		  else
			{
			  //index++;
			  //fs[index]->subgraph->graphPrint();
			  //cout<<endl;
			  //continue;
			  cout<<"Invalid Line in Frequent Set\n";
			  exit (1);
			}		  
		}
	}
  graphfile.close();
}

//Used for loading gSpan output into Graph*
bool WorkingSet::loadSet2(const char *path)
{
  ifstream graphfile (path, ifstream::in);
  string line1, line2, line3;
  //cout<<"Loadset 2: "<<path<<endl;//DEBUG
  if(graphfile.is_open())
    {
	  int index = 0;
      while(graphfile.good())
		{
		  getline(graphfile, line1);
		  //getline(graphfile, line2);
		  //getline(graphfile, line3);
		  //tokenize the line
		  vector<string> tokens;
		  boost::split(tokens, line1, boost::is_any_of(" "));
		  // for(int i=0; i<tokens.size();i++)
		  //	{
			  //if first word is 'v' addNode
		  //cout<<"Index: "<<index<<" tokens[0]:"<<tokens[0]<<"$"<<endl;//DEBUG
		  if(tokens[0] == "t")//new transaction
			{
			  getline(graphfile, line2);
			  uint64_t freq = (uint64_t)(atoi(tokens[4].c_str()));
			  if (dfs_freq_map.find(line2)!= dfs_freq_map.end())
				{
				  //cout<<line2<<endl;//DEBUG
				  //(dfs_freq_map[line2]->subgraph)->graphPrint();//DEBUG
				  (dfs_freq_map[line2])->frequency += freq;				  
				}
			  else
				{
				  dfs_freq_map[line2] = fs[index];
				}
			  index++;
			  //FreqSubgraph *ftemp = new FreqSubgraph();
			  //fs.push_back(ftemp);//if vector creates it's own memory then this would create a memory leak
			  //fs[index]->frequency = uint64_t(atoi(tokens[4].c_str()));
			  /*tempv->id = uint64_t(atoi(tokens[1].c_str()));
			  tempv->type = uint64_t(atoi(tokens[2].c_str()));
			  tempv->label = tokens[3];
			  tempv->timestamp = 0;//tokens[4];
			  if (!(subgraph->addNode(tempv)))
				{
				  cout<<"Add Node failed!!"<<endl;
				  exit(1);
				  }*/
			}
		  /*else if(tokens[0] == "v")
				{
				  Node *tempv = new Node;//if vector creates it's own memory then this would create a memory leak
				  tempv->id = uint64_t(atoi(tokens[1].c_str()));
				  tempv->type = uint64_t(atoi(tokens[2].c_str()));
				  tempv->label = "";//tokens[3];
				  tempv->timestamp = 0;//tokens[4];
				  if (!((fs[index]->subgraph)->addNode(tempv)))
					{
					  cout<<"Add Node failed!!"<<endl;
					  exit(1);
					}
					}*/
		  //if first word is 'e' getNeighborhood(source)
		  /* else if(tokens[0] == "e")
			{
			  Edge *tempe = new Edge;//if vector creates it's own memory then this would create a memory leak
			  tempe->source = uint64_t(atoi(tokens[1].c_str()));
			  tempe->target = uint64_t(atoi(tokens[2].c_str()));
			  tempe->type = uint64_t(atoi(tokens[3].c_str()));
			  tempe->label = "";//tokens[4];
			  tempe->timestamp = 0;//tokens[5];
			  tempe->directed = true;
			  tempe->extracted = false;
			  if(!((fs[index]->subgraph)->addEdge(tempe)))
				{
				  cout<<"Add Edge failed!!"<<endl;
				  exit(1);
				}
				}*/
		  else if(tokens[0] == "" || tokens[0] == "\n" || tokens[0] == "\t")
			{
			  //fs[index]->subgraph->graphPrint();
			  //index++;
			  //cout<<endl;
			  continue;
			}
		  else
			{
			  //index++;
			  //fs[index]->subgraph->graphPrint();
			  //cout<<endl;
			  //continue;
			  cout<<"Invalid Line in Frequent Set\n";
			  exit (1);
			}	
		  
		}
	}
  graphfile.close();
}


//Uses VF2 to check the isomorphism counts for the working set
/*bool WorkingSet::checkIsomorphism(vector<FreqSubgraph *> freqsub, vector<FSM::graph *> transactions)
{
  ARGEdit ed1, ed2;
  map<int, int> nodemap;
  int i,j;
  for(i=0;i<freqsub.size();i++)
	{
	  for(j=0;j<transactions.size();j++)
		{
		  //Convert the frequent subgraph to VFlIb format
		  for(int n = 0; n < freqsub[i]->subgraph->node_order;n++)
			{
			  ed1.InsertNode(&(freqsub[i]->subgraph->node_list[n]->type));
			  nodemap[(((freqsub[i])->subgraph)->node_list[n])->id] = n;
			}
		  for(int n=0; n<freqsub[i]->subgraph->node_order;n++)
			{
			  for (int m=0; m < freqsub[i]->subgraph->node_list[n]->adjacent_edges.size();m++)
				{
				  FSM::Edge *e = freqsub[i]->subgraph->node_list[n]->adjacent_edges[m];
				  ed1.InsertEdge(nodemap[e->source], nodemap[e->target], &(e->type));
				}
			}
		  nodemap.clear();
		  //Convert the graph transaction to VFLib
		  cout<<"Graph Transaction:"<<endl;
		  transactions[j]->graphPrint();
		  for(int n = 0; n < transactions[j]->node_order;n++)
			{
			  ed2.InsertNode(&(transactions[j]->node_list[n]->type));
			  cout<<"Inserting node "<<((transactions[j])->node_list[n])->id<<endl;
			  nodemap[((transactions[j])->node_list[n])->id] = n;
			}
		  for(int n=0; n<transactions[j]->node_order;n++)
			{
			  for (int m=0; m < transactions[j]->node_list[n]->adjacent_edges.size();m++)
				{
				  FSM::Edge *e = transactions[j]->node_list[n]->adjacent_edges[m];
				  cout<<"Inserting edge: "<<nodemap[e->source]<<"->"<<nodemap[e->target]<<" "<<e->type<<endl;
				  ed2.InsertEdge(nodemap[e->source], nodemap[e->target], &(e->type));
				}
			}
		  nodemap.clear();
		  //Construct the two graphs
		  Graph fgraph(&ed1);//not the Graph that this program natively defines, has to be changed
		  Graph tgraph(&ed2);//same as above
		  
		  //Perform the sgiso. If true increment the frequency of the subgraph
		  VF2SubState s0(&fgraph, &tgraph);
		  int n;
		  node_id fn[freqsub[i]->subgraph->node_order], tn[freqsub[i]->subgraph->node_order];
		  if (match(&s0, &n,fn,tn))
			{
			  cout<<"Match found between"<<endl<<"Graph 1"<<endl;
			  freqsub[i]->subgraph->graphPrint();
			  cout<<"Graph 2"<<endl;
			  transactions[j]->graphPrint();
			  freqsub[i]->frequency++;//increment frequency if match found
			}		  
		}
	}

}*/



