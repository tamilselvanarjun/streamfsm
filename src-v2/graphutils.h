/*
 * graphutils.h
 *
 *  Created on: Sep 14, 2013
 *      Author: rayabhik
 */

#ifndef GRAPHUTILS_H_
#define GRAPHUTILS_H_

#include <iostream>
#include <vector>
#include <map>
#include <fstream>

#include "loader.h"

//using namespace std;

class graphutils
{
public:
  graphutils();
  //graph graphutils::create_GTL_graph(const char *update_file);
  static std::vector<std::vector<FSM::Edge *> > connected_components(const char *update_file_path);

  virtual
  ~graphutils();


};

#endif /* GRAPHUTILS_H_ */
