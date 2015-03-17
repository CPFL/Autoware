/*
 *  Copyright (c) 2015, Nagoya University
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *
 *  * Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 *  * Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 *  * Neither the name of Autoware nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 *  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 *  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 *  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 *  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <libgen.h>
#include <fstream>
#include "ros/ros.h"
#include <visualization_msgs/Marker.h>

#define INTERVAL_SEC	1
int swap_x_y = 1;


typedef std::vector<std::vector<std::string>> Tbl;

Tbl read_csv(const char* filename, int* max_id)
{
  std::ifstream ifs(filename);
  std::string line;
  std::getline(ifs, line); // Remove first line

  Tbl tbl;

  *max_id = -1;
  while (std::getline(ifs, line)) {
    std::istringstream ss(line);

    std::vector<std::string> columns;
    std::string column;
    while (std::getline(ss, column, ',')) {
      columns.push_back(column);
    }
    tbl.push_back(columns);

    int id = std::stoi(columns[0]);
    if (id > *max_id) *max_id = id;
  }
  return tbl;
}

/* for roadedge.csv */
struct RoadEdge {
  int id;
  int lid;
  int linkid;
};

std::vector<RoadEdge> read_roadedge(const char* filename)
{
  int max_id;
  Tbl tbl = read_csv(filename, &max_id);
  size_t i, n = tbl.size();
  std::vector<RoadEdge> ret(max_id + 1);
  for (i=0; i<n; i++) {
    int id = std::stoi(tbl[i][0]);
    ret[id].id = id;
    ret[id].lid = std::stoi(tbl[i][1]);
    ret[id].linkid = std::stoi(tbl[i][2]);
  }
  return ret;
}

/* for gutter.csv, guardrail.csv */
struct Gutter {
  int id;
  int aid;
  int type;
  int linkid;
};

std::vector<Gutter> read_gutter(const char* filename)
{
  int max_id;
  Tbl tbl = read_csv(filename, &max_id);
  size_t i, n = tbl.size();
  std::vector<Gutter> ret(max_id + 1);
  for (i=0; i<n; i++) {
    int id = std::stoi(tbl[i][0]);
    ret[id].id = id;
    ret[id].aid = std::stoi(tbl[i][1]);
    ret[id].type = std::stoi(tbl[i][2]);
    ret[id].linkid = std::stoi(tbl[i][3]);
  }
  return ret;
}

/* for curb.csv */
struct Curb {
  int id;
  int lid;
  double height;
  double width;
  int dir;
  int linkid;
};

std::vector<Curb> read_curb(const char* filename)
{
  int max_id;
  Tbl tbl = read_csv(filename, &max_id);
  size_t i, n = tbl.size();
  std::vector<Curb> ret(max_id + 1);
  for (i=0; i<n; i++) {
    int id = std::stoi(tbl[i][0]);
    ret[id].id = id;
    ret[id].lid = std::stoi(tbl[i][1]);
    ret[id].height = std::stod(tbl[i][2]);
    ret[id].width = std::stod(tbl[i][3]);
    ret[id].dir = std::stoi(tbl[i][4]);
    ret[id].linkid = std::stoi(tbl[i][5]);
  }
  return ret;
}

/* for whiteline.csv */
struct WhiteLine {
	int id;
	int lid;
	double width;
	char color;
	int type;
	int linkid;
};

std::vector<WhiteLine> read_whiteline(const char* filename)
{
  int max_id;
  Tbl tbl = read_csv(filename, &max_id);
  size_t i, n = tbl.size();
  std::vector<WhiteLine> ret(max_id + 1);
  for (i=0; i<n; i++) {
    int id = std::stoi(tbl[i][0]);
    ret[id].id = id;
    ret[id].lid = std::stoi(tbl[i][1]);
    ret[id].width = std::stod(tbl[i][2]);
    ret[id].color = tbl[i][3].c_str()[0];
    ret[id].type = std::stod(tbl[i][4]);
    ret[id].linkid = std::stoi(tbl[i][5]);
  }
  return ret;
}

/* for stopline.csv */
struct StopLine {
	int id;
	int lid;
	int tlid;
	int signid;
	int linkid;
};

std::vector<StopLine> read_stopline(const char* filename)
{
  int max_id;
  Tbl tbl = read_csv(filename, &max_id);
  size_t i, n = tbl.size();
  std::vector<StopLine> ret(max_id + 1);
  for (i=0; i<n; i++) {
    int id = std::stoi(tbl[i][0]);
    ret[id].id = id;
    ret[id].lid = std::stoi(tbl[i][1]);
    ret[id].tlid = std::stoi(tbl[i][2]);
    ret[id].signid = std::stoi(tbl[i][3]);
    ret[id].linkid = std::stoi(tbl[i][4]);
  }
  return ret;
}

/* for zebrazone.csv, sidewalk.csv, crossroads.csv */
struct ZebraZone {
	int id;
	int aid;
	int linkid;
};

std::vector<ZebraZone> read_zebrazone(const char* filename)
{
  int max_id;
  Tbl tbl = read_csv(filename, &max_id);
  size_t i, n = tbl.size();
  std::vector<ZebraZone> ret(max_id + 1);
  for (i=0; i<n; i++) {
    int id = std::stoi(tbl[i][0]);
    ret[id].id = id;
    ret[id].aid = std::stoi(tbl[i][1]);
    ret[id].linkid = std::stoi(tbl[i][2]);
  }
  return ret;
}

/* for crosswalk.csv */
struct CrossWalk {
	int id;
	int aid;
	int type;
	int bdid;
	int linkid;
};

std::vector<CrossWalk> read_crosswalk(const char* filename)
{
  int max_id;
  Tbl tbl = read_csv(filename, &max_id);
  size_t i, n = tbl.size();
  std::vector<CrossWalk> ret(max_id + 1);
  for (i=0; i<n; i++) {
    int id = std::stoi(tbl[i][0]);
    ret[id].id = id;
    ret[id].aid = std::stoi(tbl[i][1]);
    ret[id].type = std::stoi(tbl[i][2]);
    ret[id].bdid = std::stoi(tbl[i][3]);
    ret[id].linkid = std::stoi(tbl[i][4]);
  }
  return ret;
}

/* for road_surface_mark.csv */
struct RoadSurfaceMark {
  int id;
  int aid;
  std::string type;
  int linkid;
};

std::vector<RoadSurfaceMark> read_roadsurfacemark(const char* filename)
{
  int max_id;
  Tbl tbl = read_csv(filename, &max_id);
  size_t i, n = tbl.size();
  std::vector<RoadSurfaceMark> ret(max_id + 1);
  for (i=0; i<n; i++) {
    int id = std::stoi(tbl[i][0]);
    ret[id].id = id;
    ret[id].aid = std::stoi(tbl[i][1]);
    ret[id].type = tbl[i][2];
    ret[id].linkid = std::stoi(tbl[i][3]);
  }
  return ret;
}

/* for poledata.csv, utilitypole.csv */
struct PoleData {
  int id;
  int plid;
  int linkid;
};

std::vector<PoleData> read_poledata(const char* filename)
{
  int max_id;
  Tbl tbl = read_csv(filename, &max_id);
  size_t i, n = tbl.size();
  std::vector<PoleData> ret(max_id + 1);
  for (i=0; i<n; i++) {
    int id = std::stoi(tbl[i][0]);
    ret[id].id = id;
    ret[id].plid = std::stoi(tbl[i][1]);
    ret[id].linkid = std::stoi(tbl[i][2]);
  }
  return ret;
}

/* for signaldata.csv, roadsign.csv */
struct SignalData {
  int id;
  int vid;
  int plid;
  int type;
  int linkid;
};

std::vector<SignalData> read_signaldata(const char *filename)
{
  int max_id;
  Tbl tbl = read_csv(filename, &max_id);
  size_t i, n = tbl.size();
  std::vector<SignalData> ret(max_id + 1);
  for (i=0; i<n; i++) {
    int id = std::stoi(tbl[i][0]);
    ret[id].id = id;
    ret[id].vid = std::stoi(tbl[i][1]);
    ret[id].plid = std::stoi(tbl[i][2]);
    ret[id].type = std::stoi(tbl[i][3]);
    ret[id].linkid = std::stoi(tbl[i][4]);
  }
  return ret;
}

/* for streetlight.csv */
struct StreetLight {
  int id;
  int lid;
  int plid;
  int linkid;
};

std::vector<StreetLight> read_streetlight(const char *filename)
{
  int max_id;
  Tbl tbl = read_csv(filename, &max_id);
  size_t i, n = tbl.size();
  std::vector<StreetLight> ret(max_id + 1);
  for (i=0; i<n; i++) {
    int id = std::stoi(tbl[i][0]);
    ret[id].id = id;
    ret[id].lid = std::stoi(tbl[i][1]);
    ret[id].plid = std::stoi(tbl[i][2]);
    ret[id].linkid = std::stoi(tbl[i][3]);
  }
  return ret;
}


/* basic class */
/* for point.csv */
struct PointClass {
  int pid;
  double b;
  double l;
  double h;
  double bx;
  double ly;
  double ref;
  int mcode1;
  int mcode2;
  int mcode3;
};

std::vector<PointClass> read_pointclass(const char *filename)
{
  int max_id;
  Tbl tbl = read_csv(filename, &max_id);
  size_t i, n = tbl.size();
  std::vector<PointClass> ret(max_id + 1);
  for (i=0; i<n; i++) {
    int id = std::stoi(tbl[i][0]);
    ret[id].pid = id;
    ret[id].b = std::stod(tbl[i][1]);
    ret[id].l = std::stod(tbl[i][2]);
    ret[id].h = std::stod(tbl[i][3]);
    ret[id].bx = std::stod(tbl[i][4]);
    ret[id].ly = std::stod(tbl[i][5]);
    ret[id].ref = std::stod(tbl[i][6]);
    ret[id].mcode1 = std::stoi(tbl[i][7]);
    ret[id].mcode2 = std::stod(tbl[i][8]);
    ret[id].mcode3 = std::stod(tbl[i][9]);
  }
  return ret;
}

/* for vector.csv */
struct VectorClass {
  int vid;
  int pid;
  double hang;
  double vang;
};

std::vector<VectorClass> read_vectorclass(const char *filename)
{
  int max_id;
  Tbl tbl = read_csv(filename, &max_id);
  size_t i, n = tbl.size();
  std::vector<VectorClass> ret(max_id + 1);
  for (i=0; i<n; i++) {
    int id = std::stoi(tbl[i][0]);
    ret[id].vid = id;
    ret[id].pid = std::stoi(tbl[i][1]);
    ret[id].hang = std::stod(tbl[i][2]);
    ret[id].vang = std::stod(tbl[i][3]);
  }
  return ret;
}

/* for line.csv */
struct LineClass {
  int lid;
  int bpid;
  int fpid;
  int blid;
  int flid;
};

std::vector<LineClass> read_lineclass(const char *filename)
{
  int max_id;
  Tbl tbl = read_csv(filename, &max_id);
  size_t i, n = tbl.size();
  std::vector<LineClass> ret(max_id + 1);
  for (i=0; i<n; i++) {
    int id = std::stoi(tbl[i][0]);
    ret[id].lid = id;
    ret[id].bpid = std::stoi(tbl[i][1]);
    ret[id].fpid = std::stoi(tbl[i][2]);
    ret[id].blid = std::stoi(tbl[i][3]);
    ret[id].flid = std::stoi(tbl[i][4]);
  }
  return ret;
}

/* for area.csv */
struct AreaClass {
  int aid;
  int slid;
  int elid;
};

std::vector<AreaClass> read_areaclass(const char *filename)
{
  int max_id;
  Tbl tbl = read_csv(filename, &max_id);
  size_t i, n = tbl.size();
  std::vector<AreaClass> ret(max_id + 1);
  for (i=0; i<n; i++) {
    int id = std::stoi(tbl[i][0]);
    ret[id].aid = id;
    ret[id].slid = std::stod(tbl[i][1]);
    ret[id].elid = std::stod(tbl[i][2]);
  }
  return ret;
}

/* for pole.csv */
struct PoleClass {
  int plid;
  int vid;
  double length;
  double dim;
};

std::vector<PoleClass> read_poleclass(const char *filename)
{
  int max_id;
  Tbl tbl = read_csv(filename, &max_id);
  size_t i, n = tbl.size();
  std::vector<PoleClass> ret(max_id + 1);
  for (i=0; i<n; i++) {
    int id = std::stoi(tbl[i][0]);
    ret[id].plid = id;
    ret[id].vid = std::stoi(tbl[i][1]);
    ret[id].length = std::stod(tbl[i][2]);
    ret[id].dim = std::stod(tbl[i][3]);
  }
  return ret;
}

/* for box.csv */
struct BoxClass {
  int bid;
  int pid1;
  int pid2;
  int pid3;
  int pid4;
  double height;
};

std::vector<BoxClass> read_boxclass(const char *filename)
{
  int max_id;
  Tbl tbl = read_csv(filename, &max_id);
  size_t i, n = tbl.size();
  std::vector<BoxClass> ret(max_id + 1);
  for (i=0; i<n; i++) {
    int id = std::stoi(tbl[i][0]);
    ret[id].bid = id;
    ret[id].pid1 = std::stoi(tbl[i][1]);
    ret[id].pid2 = std::stoi(tbl[i][2]);
    ret[id].pid3 = std::stoi(tbl[i][3]);
    ret[id].pid4 = std::stoi(tbl[i][4]);
    ret[id].height = std::stod(tbl[i][5]);
  }
  return ret;
}


/* Road data */
/* for dtlane.csv */
struct DTLane {
  int did;
  double dist;
  int pid;
  double dir;
  int apara;
  double r;
  double slope;
  double cant;
  double lw;
  double rw;
};

std::vector<DTLane> read_dtlane(const char *filename)
{
  int max_id;
  Tbl tbl = read_csv(filename, &max_id);
  size_t i, n = tbl.size();
  std::vector<DTLane> ret(max_id + 1);
  for (i=0; i<n; i++) {
    int id = std::stoi(tbl[i][0]);
    ret[id].did = id;
    ret[id].dist = std::stod(tbl[i][1]);
    ret[id].pid = std::stoi(tbl[i][2]);
    ret[id].dir = std::stod(tbl[i][3]);
    ret[id].apara = std::stoi(tbl[i][4]);
    ret[id].r = std::stod(tbl[i][5]);
    ret[id].slope = std::stod(tbl[i][6]);
    ret[id].cant = std::stod(tbl[i][7]);
    ret[id].lw = std::stod(tbl[i][8]);
    ret[id].rw = std::stod(tbl[i][9]);
  }
  return ret;
}

/* for node.csv */
struct NodeData {
	int nid;
	int pid;
};

std::vector<NodeData> read_nodedata(const char *filename)
{
  int max_id;
  Tbl tbl = read_csv(filename, &max_id);
  size_t i, n = tbl.size();
  std::vector<NodeData> ret(max_id + 1);
  for (i=0; i<n; i++) {
    int id = std::stoi(tbl[i][0]);
    ret[id].nid = id;
    ret[id].pid = std::stoi(tbl[i][1]);
  }
  return ret;
}

/* for lane.csv */
struct LaneData {
  int lnid;
  int did;
  int blid;
  int flid;
  int bnid;
  int fnid;
  int jct;
  int blid2;
  int blid3;
  int blid4;
  int flid2;
  int flid3;
  int flid4;
  int clossid;
  double span;
  int lcnt;
  int lno;
};

std::vector<LaneData> read_lanedata(const char *filename)
{
  int max_id;
  Tbl tbl = read_csv(filename, &max_id);
  size_t i, n = tbl.size();
  std::vector<LaneData> ret(max_id + 1);
  for (i=0; i<n; i++) {
    int id = std::stoi(tbl[i][0]);
    ret[id].lnid = id;
    ret[id].did = std::stoi(tbl[i][1]);
    ret[id].blid = std::stoi(tbl[i][2]);
    ret[id].flid = std::stoi(tbl[i][3]);
    ret[id].bnid = std::stoi(tbl[i][4]);
    ret[id].fnid = std::stoi(tbl[i][5]);
    ret[id].jct = std::stoi(tbl[i][6]);
    ret[id].blid2 = std::stoi(tbl[i][7]);
    ret[id].blid3 = std::stoi(tbl[i][8]);
    ret[id].blid4 = std::stoi(tbl[i][9]);
    ret[id].flid2 = std::stoi(tbl[i][10]);
    ret[id].flid3 = std::stoi(tbl[i][11]);
    ret[id].flid4 = std::stoi(tbl[i][12]);
    ret[id].clossid = std::stoi(tbl[i][13]);
    ret[id].span = std::stod(tbl[i][14]);
    ret[id].lcnt = std::stoi(tbl[i][15]);
    ret[id].lno = std::stoi(tbl[i][16]);
  }
  return ret;
}


void calc_ang_to_xyzw(double vang, double hang, double* x, double* y, double* z, double *w)
{
  *x = *y = *z = 0;
  *w = 1;

  if (vang == 90) {
    double rad = M_PI * (-hang) / 180;
    *x = 0;
    *y = 0;
    *z = sin(rad/2);
    *w = cos(rad/2);
  }
}

void set_marker_data(visualization_msgs::Marker* marker,
		    double px, double py, double pz, double ox, double oy, double oz, double ow,
		    double sx, double sy, double sz, double r, double g, double b, double a)
{
  if(swap_x_y) {
    marker->pose.position.x = py;
    marker->pose.position.y = px;
    marker->pose.orientation.x = oy;
    marker->pose.orientation.y = ox;
  } else {
    marker->pose.position.x = px;
    marker->pose.position.y = py;
    marker->pose.orientation.x = ox;
    marker->pose.orientation.y = oy;
  }
  marker->pose.position.z = pz;

  marker->pose.orientation.z = oz;
  marker->pose.orientation.w = ow;

  marker->scale.x = sx;
  marker->scale.y = sy;
  marker->scale.z = sz;

  marker->color.r = r;
  marker->color.g = g;
  marker->color.b = b;
  marker->color.a = a;
}

void add_marker_points(visualization_msgs::Marker* marker, double x, double y, double z)
{
  geometry_msgs::Point p;
  p.x = x;
  p.y = y;
  p.z = z;
  marker->points.push_back(p);
}

void add_marker_points_pointclass(visualization_msgs::Marker* marker, PointClass& pclass)
{
  if(swap_x_y) {
    add_marker_points(marker,
		      pclass.ly,
		      pclass.bx,
		      pclass.h);
  } else {
    add_marker_points(marker,
		      pclass.bx,
		      pclass.ly,
		      pclass.h);
  }
}

void publish_marker(visualization_msgs::Marker* marker,
		    ros::Publisher& pub,
		    ros::Rate& rate)
{
    ros::ok();
    pub.publish(*marker);
    rate.sleep();
    marker->id++;
}

void publish_poleclass(PoleClass& poleclass,
		       double r, double g, double b, double a,
		       std::vector<VectorClass> vectorclasses,
		       std::vector<PointClass> pointclasses,
		       visualization_msgs::Marker* marker,
		       ros::Publisher& pub,
		       ros::Rate& rate)
{
  int vid = poleclass.vid;
  int pid = vectorclasses[vid].pid;

  marker->type = visualization_msgs::Marker::CYLINDER;

  double ox, oy, oz, ow;
  calc_ang_to_xyzw(vectorclasses[vid].vang, vectorclasses[vid].hang, 
		   &ox, &oy, &oz, &ow);

  set_marker_data(marker,
		  pointclasses[pid].bx, 
		  pointclasses[pid].ly, 
		  pointclasses[pid].h + poleclass.length / 2,
		  ox, oy, oz, ow,
		  poleclass.dim, 
		  poleclass.dim, 
		  poleclass.length,
		  r, g, b, a);

  publish_marker(marker, pub, rate);
}		  

int main(int argc, char **argv)
{

/*

#!/bin/sh
rosrun sample_data sample_vector_map poledata.csv signaldata.csv pole.csv vector.csv point.csv roadsign.csv dtlane.csv node.csv lane.csv whiteline.csv zebrazone.csv area.csv crosswalk.csv line.csv roadedge.csv road_surface_mark.csv

# EOF

*/

  ros::init(argc, argv, "sample_vector_map");
  ros::NodeHandle n;
  ros::Publisher pub = n.advertise<visualization_msgs::Marker>("/vector_map", 10, true);


  std::vector<PointClass> pointclasses;
  std::vector<PoleClass> poleclasses;
  std::vector<VectorClass> vectorclasses;
  std::vector<AreaClass> areaclasses;
  std::vector<LineClass> lines;

  std::vector<PoleData> poledatas;
  std::vector<SignalData> signaldatas;
  std::vector<SignalData> roadsigns;
  std::vector<DTLane> dtlanes;
  std::vector<NodeData> nodes;
  std::vector<LaneData> lanes;
  std::vector<WhiteLine> whitelines;
  std::vector<ZebraZone> zebrazones;
  std::vector<CrossWalk> crosswalks;
  std::vector<RoadEdge> roadedges;
  std::vector<Gutter> roadmarks;

  std::cerr << "Load csv files" << std::endl;

  argc--;
  argv++;
  while(argc > 0) {
    std::string name(basename(argv[0]));

    if(name == "point.csv") {
      pointclasses = read_pointclass(argv[0]);
      std::cerr << "  load " << argv[0] 
		<< ", pointclasses.size()=" <<  pointclasses.size() << std::endl;
    } else if(name == "pole.csv") {
      poleclasses = read_poleclass(argv[0]);
      std::cerr << "  load " << argv[0]
		<< ", poleclasses.size()=" <<  poleclasses.size() << std::endl;
    } else if(name == "vector.csv") {
      vectorclasses = read_vectorclass(argv[0]);
      std::cerr << "  load " << argv[0] 
		<< ", vectorclasses.size()" << vectorclasses.size() << std::endl;

    } else if(name == "area.csv") {
      areaclasses = read_areaclass(argv[0]);
      std::cerr << "  load " << argv[0] 
		<< ", areaclasses.size()=" << areaclasses.size() << std::endl;
    } else if(name == "line.csv") {
      lines = read_lineclass(argv[0]);
      std::cerr << "  load " << argv[0] 
		<< ", lines.size()=" << lines.size() << std::endl;
    } else if(name == "poledata.csv") {
      poledatas = read_poledata(argv[0]);
      std::cerr << "  load " << argv[0] 
		<< ", poledatas.size()=" << poledatas.size() << std::endl;
    } else if(name == "signaldata.csv") {
      signaldatas = read_signaldata(argv[0]);
      std::cerr << "  load " << argv[0] 
		<< ", signaldatas.size()=" << signaldatas.size() << std::endl;
    } else if(name == "roadsign.csv") {
      roadsigns = read_signaldata(argv[0]);
      std::cerr << "  load " << argv[0] 
		<< ", roadsigns.size()=" << roadsigns.size() << std::endl;
    } else if(name == "dtlane.csv") {
      dtlanes = read_dtlane(argv[0]);
      std::cerr << "  load " << argv[0] 
		<< ", dtlanes.size()=" << dtlanes.size() << std::endl;
    } else if(name == "node.csv") {
      nodes = read_nodedata(argv[0]);
      std::cerr << "  load " << argv[0] 
		<< ", nodes.size()=" << nodes.size() << std::endl;
    } else if(name == "lane.csv") {
      lanes = read_lanedata(argv[0]);
      std::cerr << "  load " << argv[0] 
		<< ", lanes.size()=" << lanes.size() << std::endl;
    } else if(name == "whiteline.csv") {
      whitelines = read_whiteline(argv[0]);
      std::cerr << "  load " << argv[0] 
		<< ", whitelines.size()=" << whitelines.size() << std::endl;
    } else if(name == "zebrazone.csv") {
      zebrazones = read_zebrazone(argv[0]);
      std::cerr << "  load " << argv[0] 
		<< ", zebrazones.size()=" << zebrazones.size() << std::endl;
    } else if(name == "crosswalk.csv") {
      crosswalks = read_crosswalk(argv[0]);
      std::cerr << "  load " << argv[0] 
		<< ", crosswalks.size()=" << crosswalks.size() << std::endl;
    } else if(name == "roadedge.csv") {
      roadedges = read_roadedge(argv[0]);
      std::cerr << "  load " << argv[0] 
		<< ", roadedges.size()=" << roadedges.size() << std::endl;
    } else if(name == "road_surface_mark.csv") {
      roadmarks = read_gutter(argv[0]);
      std::cerr << "  load " << argv[0] 
		<< ", roadmarks.size()=" << roadmarks.size() << std::endl;
    }

    argc--;
    argv++;
  }

  std::cerr << "finished load files" << std::endl;

  if(pointclasses.size() <= 0) {
    std::cerr << "Usage: vector_map_loader csv_file\n"
	      << "\tpoint.csv is not loaded"
              << std::endl;
    std::exit(1);
  }


  visualization_msgs::Marker marker;
  marker.header.frame_id = "/map";
  marker.header.stamp = ros::Time::now();

  marker.ns = "vector_map";
  marker.action = visualization_msgs::Marker::ADD;
  marker.lifetime = ros::Duration();

  marker.type = visualization_msgs::Marker::CYLINDER;

  ros::Rate rate(1000);
  size_t i;

  std::cerr << "start publish vector map" << std::endl;

  while(ros::ok()) {
    marker.id = 0;

    // poledata
    if(poledatas.size() > 0 && (poleclasses.size() <= 0 || vectorclasses.size() <= 0)) {
      std::cerr << "error: pole.csv or vector.csv is not loaded \n"
		<< "\tpoledata.csv needs pole.csv and vector.csv "
		<< std::endl;
      std::exit(1);
    }
    for (i=0; i<poledatas.size(); i++) {
      if (poledatas[i].id <= 0) {
	continue;
      }
      int plid = poledatas[i].plid;
      publish_poleclass(poleclasses[plid], 
			0, 1, 0, 1,
			vectorclasses,
			pointclasses,
			&marker, pub, rate);
    }
    rate.sleep();

    // signaldata
    if(signaldatas.size() > 0 && (poleclasses.size() <= 0 || vectorclasses.size() <= 0)) {
      std::cerr << "error: pole.csv or vector.csv is not loaded.\n"
		<< "\tsignaldata.csv needs pole.csv and vector.csv "
		<< std::endl;
      std::exit(1);
    }
    for (i=0; i<signaldatas.size(); i++) {
      if (signaldatas[i].id <= 0) {
	continue;
      }
      int vid = signaldatas[i].vid;
      int pid = vectorclasses[vid].pid;

      rate.sleep();
      marker.type = visualization_msgs::Marker::SPHERE;

      double ox, oy, oz, ow;
      calc_ang_to_xyzw(vectorclasses[vid].vang, vectorclasses[vid].hang,
		       &ox, &oy, &oz, &ow);

      double r = 0, g = 0, b = 0, a = 1;
      switch (signaldatas[i].type) {
      case 1:
	r = 1;
	break;
      case 2:
	b = 1;
	break;
      case 3:
	r = 1;
	g = 1;
	break;
      case 4:
	marker.type = visualization_msgs::Marker::CUBE;
	r = 1;
	break;
      case 5:
	marker.type = visualization_msgs::Marker::CUBE;
	b = 1;
	break;
      default:
	break;
      }

      set_marker_data(&marker,
		      pointclasses[pid].bx, 
		      pointclasses[pid].ly, 
		      pointclasses[pid].h,
		      ox, oy, oz, ow,
		      0.5, 0.5, 0.5,
		      r, g, b, a);
      publish_marker(&marker, pub, rate);
      rate.sleep();
      rate.sleep();

      // signal pole
      if (signaldatas[i].type == 2) { // blue
	int plid = signaldatas[i].plid;
	rate.sleep();
	if (plid > 0) {
	  publish_poleclass(poleclasses[plid],
			    0.5, 0.5, 0.5, 1,
			    vectorclasses,
			    pointclasses,
			    &marker, pub, rate);
	  rate.sleep();
	  rate.sleep();
	}
      }
    }

    // roadsigns
    if(roadsigns.size() > 0 && (poleclasses.size() <= 0 || vectorclasses.size() <= 0)) {
      std::cerr << "error: pole.csv or vector.csv is not loaded\n"
		<< "\troadsign.csv needs pole.csv and vector.csv "
		<< std::endl;
      std::exit(1);
    }
    for (i=0; i<roadsigns.size(); i++) {
      if (roadsigns[i].id <= 0) {
	continue;
      }
      int vid = roadsigns[i].vid;
      int pid = vectorclasses[vid].pid;

      marker.type = visualization_msgs::Marker::SPHERE;

      double ox, oy, oz, ow;
      calc_ang_to_xyzw(vectorclasses[vid].vang, vectorclasses[vid].hang,
		       &ox, &oy, &oz, &ow);

      set_marker_data(&marker,
		      pointclasses[pid].bx, 
		      pointclasses[pid].ly, 
		      pointclasses[pid].h,
		      ox, oy, oz, ow,
		      1.0, 0.1, 1.0,
		      1, 1, 1, 1);
      publish_marker(&marker, pub, rate);

      // road sign pole
      int plid = roadsigns[i].plid;
      publish_poleclass(poleclasses[plid],
			1, 1, 1, 1,
			vectorclasses,
			pointclasses,
			&marker, pub, rate);
    }

    // cross walk
    if(crosswalks.size() > 0 && (areaclasses.size() <= 0 || lines.size() <= 0)) {
      std::cerr << "error: area.csv or line.csv is not loaded.\n"
		<< "\tcrosswalk.csv needs area.csv and line.csv"
		<< std::endl;
      std::exit(1);
    }
    for (i=0; i<crosswalks.size(); i++) {
      if (crosswalks[i].id <= 0) {
	continue;
      }
      int aid = crosswalks[i].aid;
      int slid = areaclasses[aid].slid;
      int elid = areaclasses[aid].elid;

      marker.type = visualization_msgs::Marker::LINE_STRIP;

      set_marker_data(&marker,
		      0, 0, 0,
		      0, 0, 0, 1,
		      0.1, 0, 0,
		      1, 1, 1, 1);
    
      marker.points.clear();
      for(int lid = slid; ; lid = lines[lid].flid) {
	int pid = lines[lid].bpid;
	add_marker_points_pointclass(&marker, pointclasses[pid]);
	if (lid == elid) {
	  pid = lines[lid].fpid;
	  add_marker_points_pointclass(&marker, pointclasses[pid]);
	  break;
	}
      }

      publish_marker(&marker, pub, rate);
    }

    // road data
    if(lanes.size() > 0 && dtlanes.size() <= 0) {
      std::cerr << "error: dtlane.csv is not loaded.\n"
		<< "\tlane.csv needs dtlane.csv"
		<< std::endl;
      std::exit(1);
    }
    for (i=0; i<lanes.size(); i++) {
      if (lanes[i].lnid <= 0) {
	continue;
      }
      int did = lanes[i].did;
      int pid = dtlanes[did].pid;
      double ox, oy, oz, ow;

      marker.type = visualization_msgs::Marker::CUBE;

      ox = 0.0;
      oy = 0.0;
      oz = sin(dtlanes[did].dir / 2);
      ow = cos(dtlanes[did].dir / 2);

      set_marker_data(&marker,
		      pointclasses[pid].bx - lanes[i].span/2, pointclasses[pid].ly - (dtlanes[did].lw + dtlanes[did].rw)/2, pointclasses[pid].h,
		      ox, oy, oz, ow,
		      lanes[i].span, dtlanes[did].lw + dtlanes[did].rw, 0.1,
		      0.5, 0, 0, 0.3);
    
      publish_marker(&marker, pub, rate);
    }

    // zebrazone ?????
    if(zebrazones.size() > 0 && (areaclasses.size() <= 0 || lines.size() <= 0)) {
      std::cerr << "error: area.csv or line.csv is not loaded.\n"
		<< "\tzebrazone.csv needs area.csv and line.csv."
		<< std::endl;
      std::exit(1);
    }
    for (i=0; i<zebrazones.size(); i++) {
      if (zebrazones[i].id <= 0) {
	continue;
      }
      int aid = zebrazones[i].aid;
      int slid = areaclasses[aid].slid;
      int elid = areaclasses[aid].elid;
      marker.type = visualization_msgs::Marker::LINE_STRIP;

      set_marker_data(&marker,
		      0, 0, 0,
		      0, 0, 0, 1,
		      0.1, 0, 0,
		      1, 1, 1, 1);
      for(int lid = slid; ; lid = lines[lid].flid) {
	int pid = lines[lid].bpid;
	add_marker_points_pointclass(&marker, pointclasses[pid]);
	if (lid == elid) {
	  pid = lines[lid].fpid;
	  add_marker_points_pointclass(&marker, pointclasses[pid]);
	  break;
	}
      }

      publish_marker(&marker, pub, rate);
    }

    // white line
    if(whitelines.size() > 0 && lines.size() <= 0) {
      std::cerr << "err: line.csv is not loaded.\n"
		<< "\twhiteline.csv needs line.csv"
		<< std::endl;
      std::exit(1);
    }
    marker.type = visualization_msgs::Marker::LINE_STRIP;
    for (i=0; i<whitelines.size(); i++) {
      if (whitelines[i].id <= 0) {
	continue;
      }
      int lid = whitelines[i].lid;
      if(lines[lid].blid == 0) {
	;
	double r = 1.0, g = 1.0, b = 1.0;
	if(whitelines[i].color == 'Y') {
	  g = 0.5;
	  b = 0.0;
	}
	set_marker_data(&marker,
			0, 0, 0,
			0, 0, 0, 1,
			whitelines[i].width, 0, 0,
			r, g, b, 1);
    
	marker.points.clear();
      }

      int pid = lines[lid].bpid;
      add_marker_points_pointclass(&marker, pointclasses[pid]);
      if(lines[lid].flid == 0) {
	pid = lines[lid].fpid;
	add_marker_points_pointclass(&marker, pointclasses[pid]);
      }

      publish_marker(&marker, pub, rate);
    }


    // roadedge
    if(roadedges.size() > 0 && (lines.size() <= 0)) {
      std::cerr << "error: line.csv is not loaded.\n"
		<< "\troadedge.csv needs line.csv"
		<< std::endl;
      std::exit(1);
    }
    marker.type = visualization_msgs::Marker::LINE_STRIP;
    for (i=0; i<roadedges.size(); i++) {
      if (roadedges[i].id <= 0) {
	continue;
      }
      int lid = roadedges[i].lid;
      if(lines[lid].blid == 0) {
	;
	set_marker_data(&marker,
			0, 0, 0,
			0, 0, 0, 1,
			0.1, 0, 0,
			0.5, 0.5, 0.5, 1);		// grey @@@@
    
	marker.points.clear();
      }

      int pid = lines[lid].bpid;
      add_marker_points_pointclass(&marker, pointclasses[pid]);
      if(lines[lid].flid == 0) {
	pid = lines[lid].fpid;
	add_marker_points_pointclass(&marker, pointclasses[pid]);
      }

      publish_marker(&marker, pub, rate);
    }


    // road_surface_mark
    if(roadmarks.size() > 0 && (areaclasses.size() <= 0 || lines.size() <= 0)) {
      std::cerr << "error: area.csv or line.csv is not loaded.\n"
		<< "\troad_surface_mark.csv needs area.csv and line.csv"
		<< std::endl;
      std::exit(1);
    }
    for (i=0; i<roadmarks.size(); i++) {
      if (roadmarks[i].id <= 0) {
	continue;
      }
      int aid = roadmarks[i].aid;
      int slid = areaclasses[aid].slid;
      int elid = areaclasses[aid].elid;
      double w = 0.2;

      marker.type = visualization_msgs::Marker::LINE_STRIP;
      set_marker_data(&marker,
		      0, 0, 0,
		      0, 0, 0, 1,
		      w, 0, 0,
		      1, 1, 1, 1);
      marker.points.clear();
      for(int lid = slid; ; lid = lines[lid].flid) {
	int pid = lines[lid].bpid;
	add_marker_points_pointclass(&marker, pointclasses[pid]);
	if (lid == elid) {
	  pid = lines[lid].fpid;
	  add_marker_points_pointclass(&marker, pointclasses[pid]);
	  break;
	}
      }

      publish_marker(&marker, pub, rate);
    }

#ifdef DEBUG_PRINT
    std::cerr << "publish end" << std::endl;
#endif
    sleep(INTERVAL_SEC);
  }

  return 0;
}
