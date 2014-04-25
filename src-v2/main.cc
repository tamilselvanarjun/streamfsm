/*
 * main.cpp
 *
 *  Created on: Sep 23, 2013
 *      Author: rayabhik
 */

#include "workingset.h"
#include "transactions.h"
// Sutanay - adding <string>
#include <string>
#include <sys/stat.h>
using namespace std;

// Added by Sutanay
struct StreamFSMConfig {
    StreamFSMConfig(string source, float f, bool flag, int max_nbrs = 1, float g = 0.1):
        data_source(source), freq_percent(f), 
        is_graph_directed(flag), max_neighbors(max_nbrs), gspan_percent(g) {}
    string  data_source;
    float   freq_percent;
    bool    is_graph_directed;
    int     max_neighbors;
    float gspan_percent;
};

bool Exists(const string& path)
{
    struct stat fstat;  
    int result = stat(path.c_str(), &fstat);
    return (result != -1); 
}

int getMaxBatchCount()
{
    int max_batches = 0;
    bool file_exists = false;
    char filepath[512];
    while(1) {
        sprintf(filepath, "../data/%d.g", max_batches+1);
        file_exists = Exists(filepath);
        if (!file_exists) {
            break;
        }
        max_batches++;
    } 
    return max_batches;
}

// Added by Sutanay

string getLogPath(const StreamFSMConfig& config)
{
/*
    size_t pos = config.data_graph_path.find_last_of("/");
    size_t file_name_offset = (pos == string::npos) ? 0:pos;
    size_t file_prefix_end = config.data_graph_path.find(".", 
            file_name+offset);
    string file_prefix = "TEST";
    if (file_prefix_end != string::npos) {
        file_prefix = 
                config.data_graph_path.substr(file_name_offset,  
                        file_prefix_end - file_name_offset);
    }
*/
    string data_source_tag = config.data_source; 
    char timestamp[64];
    time_t now = time(NULL);
    struct tm* timeinfo = localtime(&now);
    strftime(timestamp, 1024, "%F", timeinfo);
    
    char buf[1024];
    sprintf(buf, "stream_fsm_%s_%s.csv", data_source_tag.c_str(),
            timestamp);
    return string(buf);
}

// Added by Sutanay
void logResults(const StreamFSMConfig& config, long run_time)
{
    string log_path = getLogPath(config);
    ofstream ofs(log_path.c_str(), ios_base::app);
    if (ofs.good()) {
        cout << "INFO Appending stats to: " << log_path << endl;
        ofs << config.freq_percent << "," 
            << config.max_neighbors << "," << run_time << endl;
    }
    else {
        cout << "ERROR Faild to open: " << log_path << endl;
    }
    ofs.close();
    return;
}

using namespace std;

int main(int argc, char * argv[])
{
  float freq_per = boost::lexical_cast<float>(argv[1]);
  string d_flag = boost::lexical_cast<string>(argv[2]);
  string data_source = "TEST";
  string gspan_percent = "0.1";
  int max_neighbors = 1;
  if (argc > 3) {
    max_neighbors = atoi(argv[3]);
  }
  if (argc > 4) {
    data_source = argv[4];
  }
  if (argc > 5){
      gspan_percent = boost::lexical_cast<string>(argv[5]);
  }
  bool directed_flag;
  if (d_flag=="d")
    {
      directed_flag = true;
    }
  else if(d_flag == "u")
    {
      directed_flag = false;
    }
  else
    {
      cout<<"Invalid parameter for directed flag"<<endl;
      exit(1);
    }
  // Added by Sutanay
  StreamFSMConfig test_config(data_source, freq_per, 
            directed_flag, max_neighbors, boost::lexical_cast<float>(gspan_percent));
  setMaxNeighbors(max_neighbors);

  int max_batches = getMaxBatchCount();
  cout << "INFO MAX_BATCHES = " << max_batches << endl;

  time_t strt = time(NULL);

  FSM::Graph *g = new FSM::Graph();
  g->directed = directed_flag;
  Transactions *trans1 = new Transactions();
  WorkingSet *ws1 = new WorkingSet();
  // WorkingSet *ws2 = new WorkingSet();
  map<string, FreqSubgraph *>::iterator dfs_freq_map_it;

  g->loadGraph("../data/1.g");
  //g->graphPrint(); //DEBUGGING
  string gname("../cur_exp/t.graph");
  trans1->createTransactions(g,"../data/1.g",gname.c_str(), freq_per);

  //Call gspan with graph structure output
  string gspan = "./gSpan -f " + gname + " -s " + gspan_percent/*0.1"+ boost::lexical_cast<string>(int(freq_per * g->transactions.size()))*/ +" -o";
  system(gspan.c_str());
  ws1->loadSet("../cur_exp/t.graph.fp");

  //Call gspan with DFS code output
  gspan = "./gSpan -f " + gname + " -s " + gspan_percent /*+ boost::lexical_cast<string>(int(freq_per * g->transactions.size()))*/ +" -o -d";
  system(gspan.c_str());
  ws1->loadSet2("../cur_exp/t.graph.fp");

  string trans_fname("../cur_exp/trans.graph");
  // cout<<"Enter the number of batches (>2):";
  // cin>>max_batches;
  //time_t batchstrt = time(NULL);//BATCH TIMING
  g->loadGraph("../data/2.g");//using load graph to add the updates to the graph

  //  cout<<"Graph after adding updates"<<endl;
  //graphPrint();
  cout<<endl;

  trans1->createTransactions(g,"../data/2.g", trans_fname.c_str(),freq_per);
  string freqfilename = "../cur_exp/freqfile.graph";// + boost::lexical_cast<string>(batch) + ".graph";
  ofstream freqfile(freqfilename.c_str(),ios::out);


  uint64_t count;
  if(trans1->transactions.size() > 1)
        {
          //system("./gSpan -f trans.graph -s 0.1 -o");
          (ws1->fs).clear();
          string gspan = "./gSpan -f " + trans_fname + " -s " + gspan_percent /*+ boost::lexical_cast<string>(int(freq_per * g->transactions.size()))*/ +" -o";
          cout<<gspan<<endl;
          system(gspan.c_str());
          ws1->loadSet("../cur_exp/trans.graph.fp");

          gspan = "./gSpan -f " + trans_fname + " -s " + gspan_percent /*+ boost::lexical_cast<string>(int(freq_per * g->transactions.size()))*/ +" -o -d";
          cout<<gspan<<endl;
          system(gspan.c_str());
          ws1->loadSet2("../cur_exp/trans.graph.fp");
          //time_t batchstop = time(NULL);//BATCH TIMING
          //freqfile<<"Batch 2"<<": Time taken: "<<(batchstop - batchstrt)<<endl;
          freqfile<<"Batch 2"<<endl;
	  count = 0;
          for (dfs_freq_map_it = (ws1->dfs_freq_map).begin(); dfs_freq_map_it != (ws1->dfs_freq_map).end(); ++dfs_freq_map_it)
                {
                  if((dfs_freq_map_it->second)->frequency >= freq_per)
                        {
                          freqfile<<dfs_freq_map_it->first<<" => "<<(dfs_freq_map_it->second)->frequency<<endl;
                          //graphfileprint the subgraph
                          ((dfs_freq_map_it->second)->subgraph)->graphFilePrint(freqfile, count++);
                        }
                }
        }
  for (int batch=3; batch<=max_batches; batch++)
        {
          string uname = "../data/" + boost::lexical_cast<string>(batch) + ".g";
          string trans_temp("../cur_exp/trans_temp.graph");
          //time_t batchstrt = time(NULL);//BATCH TIMING
          g->loadGraph(uname.c_str());
          trans1->transactions.clear();
          trans1->createTransactions(g, uname.c_str(), trans_temp.c_str(), freq_per);
          //uint64_t count = merge(ws,g);
          //cout<<count<<" graphs merged"<<endl;
          //cout<<"Printing merged graphs"<<endl;
          string upfilename = "../cur_exp/trans_up" + boost::lexical_cast<string>(batch) + ".graph";
          ofstream trfile1(upfilename.c_str(),ios::out);
          for (int i=0; i<(trans1->transactions).size();i++)
                {
                  (trans1->transactions[i])->graphFilePrint(trfile1, i);
                  //cout<<endl;
                }
          trfile1.close();

          string gspan = "./gSpan -f" + upfilename + " -s " + gspan_percent /*+ boost::lexical_cast<string>(int(freq_per * g->transactions.size()))*/ +" -o";
          //system("./gSpan -f trans_up.graph -s 0.5 -o");
          //cout<<gspan<<endl;
          system(gspan.c_str());
          string fpupfilename = "../cur_exp/trans_up" + boost::lexical_cast<string>(batch) + ".graph.fp";
          ws1->fs.clear();//clear it but keep a backup incase frq subs is empty. See ANN proj report Fig 1
          ws1->loadSet(fpupfilename.c_str());

          gspan = "./gSpan -f" + upfilename + " -s " + gspan_percent /*+ boost::lexical_cast<string>(int(freq_per * g->transactions.size()))*/ +" -o -d";
          //system("./gSpan -f trans_up.graph -s 0.5 -o");
          //cout<<gspan<<endl;
          system(gspan.c_str());

          //ws1->fs.clear();//clear it but keep a backup incase frq subs is empty. See ANN proj report Fig 1
          ws1->loadSet2(fpupfilename.c_str());
          //time_t batchstop = time(NULL);//BATCH TIMING
          //freqfile<<"\n****************\nBatch "<<batch<<": Time taken: "<<(batchstop - batchstrt)<<endl;
	  freqfile<<"\n*****************\nBatch "<<batch<<endl;
          for (dfs_freq_map_it = ws1->dfs_freq_map.begin(); dfs_freq_map_it != ws1->dfs_freq_map.end(); ++dfs_freq_map_it)
                {
                  if((dfs_freq_map_it->second)->frequency >= freq_per)
                        {
                          freqfile<<dfs_freq_map_it->first<<" => "<<dfs_freq_map_it->second->frequency<<endl;
                          //graphfileprint:
                          ((dfs_freq_map_it->second)->subgraph)->graphFilePrint(freqfile, count++);
                        }
                }

          //Here you do the relative support counting on ws2(TBD)
          /*string freqfilename = "./cur_exp/freqfile" + boost::lexical_cast<string>(batch) + ".graph";
          ofstream freqfile(freqfilename.c_str(),ios::out);
          int count = 0;
          for (int i = 0; i < ws1->fs.size(); i++)
                {
                  for (int j = 0; j<((ws1->fs)[i])->frequency; j++)
                        {
                          ((ws1->fs)[i])->subgraph->graphFilePrint(freqfile,count++);
                        }
                }
          for (int i = 0; i < ws2->fs.size(); i++)
                {
                  for (int j = 0; j<((ws2->fs)[i])->frequency; j++)
                        {
                          ((ws2->fs)[i])->subgraph->graphFilePrint(freqfile,count++);
                        }
                        }*/
          //at this point count has an extra increment
          //gspan = "./CloseGraph " + freqfilename + " -s10" /*+ boost::lexical_cast<string>(int(freq_per * --count))*/ + " -o";
          //cout <<gspan<<endl;
          //system(gspan.c_str());
          //string coresetfilename = freqfilename + ".fp";
          //ws1->fs.clear();
          //ws1->loadSet(coresetfilename.c_str());
          }
  //ws->checkIsomorphism(ws->fs, g->transactions);
  time_t stop = time(NULL);
  cout<<"#Nodes:"<<g->node_order<<" #Edges"<<g->edge_size<<endl;
  cout<<"Time taken: "<<(stop - strt)<<endl;

  //  Sutanay: Adding logging
  logResults(test_config, (stop-strt)); 
  /*map<string, uint64_t>::iterator vev_iter;
  for(vev_iter = vev_type_freq.begin(); vev_iter != vev_type_freq.end(); ++vev_iter)
        {
          cout<<vev_iter->first<<" -> "<<vev_iter->second<<" "<< (double)(vev_iter->second)/(double)(total_vev_freq)<<endl;
        }
  cout<<"Total_vev_freq: "<<total_vev_freq<<endl;*/
}
