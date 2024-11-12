#include<bits/stdc++.h>
#include "SimModel.h"
#include "SimUtil.h"
#include "SimDiscrete.h"

void writeVTK(const std::string &filename, const std::vector<std::array<double, 3>> &points, const std::vector<double> &fieldValues) {
  //Open the output file
  std::ofstream vtkFile(filename);
  if (!vtkFile.is_open()) {
    std::cerr << "Error opening file: " << filename << std::endl;
    return;
  }
  // VTK file header
  vtkFile << "# vtk DataFile Version 3.0\n";
  vtkFile << "Point data with scalar values\n";
  vtkFile << "ASCII\n";
  vtkFile << "DATASET POLYDATA\n";
  // Write the points
  vtkFile << "POINTS " << points.size() << " double\n";
  for (const auto &point : points) {
    vtkFile << point[0] << " " << point[1] << " " << point[2] << "\n";
  }
  // Write the point scalar values
  vtkFile << "POINT_DATA " << fieldValues.size() << "\n";
  vtkFile << "FIELD FieldData 1\n";  // FIELD section starts here
  vtkFile << "FieldValues 1 " << fieldValues.size() << " double\n";  // Define the field with name "FieldValues"
  for (const auto &value : fieldValues) {
    vtkFile << value << "\n";
  }
  // Close the file
  vtkFile.close();
  std::cout << "VTK file written: " << filename << std::endl;
}


void writetoTxt(const std::string& filename, const std::vector<std::array<double, 3>>& points, 
  const std::vector<double>& fields) {
  
  std::ofstream outfile(filename);
  if (!outfile) {
     std::cerr << "Error opening file: " << filename << std::endl;
     return;
  }
  outfile << "X Y Z Field\n";  // Optional header line
  for (size_t i = 0; i < points.size(); ++i) {
    const auto& point = points[i];
    double field = fields[i];
    outfile << point[0] << " " << point[1] << " " << point[2] << " " << field << "\n";
  }
  outfile.close();
  std::cout << "Data written to " << filename << std::endl;

}

int main(){
  printf("Compiled & running C++ code with Simmetrix Libs using CMake \n");
  
  SimModel_start();
  Sim_readLicenseFile(NULL);
  
  pNativeModel nm = NULL;
  pProgress p = NULL;
  pGModel g = GM_load("/users/nathd2/2BOX/Box_1_sim.smd", nm, p);
  pMesh sm =  M_load("/users/nathd2/2BOX/Box_1-case1.sms", g, p);

  //Box_dimensions
  double x_lower = -0.5;
  double x_upper = 0.5;
  double height = 0.25;

  //Random number
  int seed=100;
  double min_value=-0.35;
  double max_value=0.35;
  std::mt19937 generator(seed);
  std::uniform_real_distribution<double> distribution(min_value, max_value);
  
  int n_samples=1200;
  for (int smp=0; smp<n_samples; smp++) { double x_random = distribution(generator);
  
  std::cout<<"Sample: "<< smp <<"  X:"<<x_random<<std::endl; 
  double m2 = (height)/(x_upper-x_random);
  double m1 = (height)/(x_lower-x_random);

  //Array
  std::vector<std::array<double, 3>> points;
  std::vector<double> fields;

  //Loop over elements 
  RIter regions = M_regionIter(sm);
  pRegion rgn;
  int count = 0;
  while ((rgn = (pRegion) RIter_next(regions))) {
    
    double xyz[4][3];
    double cent[3];
    int x = EN_coord(rgn, xyz);
    EN_centroid	(rgn, cent);
    
    double field_local=0;
    if(cent[0]<x_random){
      double y1_upper = -0.5*height + m1*(cent[0]-x_random);
      if(cent[2]<y1_upper)
        field_local=1;
      else 
	field_local=1.2;
    }
    else{
      double y2_upper = -0.5*height + m2*(cent[0]-x_random);
      if(cent[2]<y2_upper)
        field_local=1.3;
      else 
        field_local=1.2;
    }
	
    points.push_back({cent[0], cent[1], cent[2]});
    fields.push_back(field_local);
    count ++;
  }
  RIter_delete(regions);
  //std::cout<<"Count regions: "<<count<<std::endl;

  /*
  //Loop over faces
  count=0;
  FIter faces = M_faceIter(sm);
  pFace thisFace;
  while ((thisFace = (pFace) FIter_next(faces))) {
    if(F_whatInType(thisFace) == 2){
      pGFace nonManFace = (pGFace) F_whatIn(thisFace);
      int faceTag = GEN_tag(nonManFace);
      //Temporary
      if(faceTag==6){
        double xyz[3][3];
        double cent[3];
        int x = EN_coord(thisFace, xyz);
        EN_centroid(thisFace, cent);
        //Fields on face centroids
        double field_local=0;
        if(cent[0]<x_random){
          double y1_upper = -0.5*height + m1*(cent[0]-x_random);
        if(cent[2]<y1_upper)
          field_local=1;
        else 
	  field_local=1.2;
        }
        else{
          double y2_upper = -0.5*height + m2*(cent[0]-x_random);
          if(cent[2]<y2_upper)
            field_local=1.3;
          else 
            field_local=1.2;
        }
        points.push_back({cent[0], cent[1], cent[2]});
        fields.push_back(field_local);
	
      }//tag
    }
    count++;
  }
  FIter_delete(faces);
  std::cout<<"Count Faces: "<<count<<std::endl;

  //Loop over vertices
  count=0;
  VIter vertices = M_vertexIter(sm);
  pVertex vtx;
  while ((vtx = (pVertex) VIter_next(vertices))) {
     double xyz[3];
     V_coord(vtx,xyz);
     count++;
  }
  VIter_delete(vertices);
  std::cout<<"Count Vertices: "<<count<<std::endl;
  // Till here
  */
  std::string file_name="centroid_field" + std::to_string(smp) + ".txt";
  writetoTxt(file_name, points, fields); }
  
  //writeVTK("dynamic_points_with_values.vtk", points, fields);

  M_release(sm);
  GM_release(g);

  SimModel_stop();
  std::cout<<"Done"<<std::endl;
  return 0;
}
