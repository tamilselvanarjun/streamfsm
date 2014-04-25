/*
 * Transactions.h
 *
 *  Created on: Sep 23, 2013
 *      Author: rayabhik
 */

#ifndef TRANSACTIONS_H_
#define TRANSACTIONS_H_

#include "loader.h"
//using namespace std;

// extern int MAX_NBRS;
void printMaxNeighbors();
void setMaxNeighbors(int max);

class Transactions
{
public:
  std::vector<FSM::Graph*> transactions;//list of transactions, SHOULD BE A SEPARATE CLASS

  Transactions()
  {
    transactions.clear();
  }
  FSM::Graph * extractSubgraph2(uint64_t s, uint64_t t, float freq_threshold);
  FSM::Graph * extractSubgraph(FSM::Graph *, uint64_t s, uint64_t t, float freq_threshold);
  FSM::Graph * extractSubgraphComponent(FSM::Graph *, std::vector<FSM::Edge *>, float);
  FSM::Graph* extractSubgraphOriginalNodes(FSM::Graph *, uint64_t s, uint64_t t, float freq_threshold);
  FSM::Graph * graphIntersection(FSM::Graph *, FSM::Graph *);
  FSM::Graph * graphUnion(FSM::Graph *, FSM::Graph *);
  unsigned createTransactions(FSM::Graph *, const char *update_file_path, const char *tr_file, float freq);
  unsigned createTransactionsSubgraph(FSM::Graph *, const char * update_file_path, const char *tr_file, float freq);
};


#endif /* TRANSACTIONS_H_ */
