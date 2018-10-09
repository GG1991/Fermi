/*
  2016JAN27. BSC, BARCELONA, SPAIN 
  Migue Zavala 
  2016MAR25 
*/
#include <iostream> // cin, cout, endl, cerr
#include <vector>   // vector
#include <map>
#include <mpi.h>
#include <omp.h>
#include <cmath>
#include <assert.h>     /* assert */
#include <algorithm>    // std::fill
#include <sstream>      // std::stringstream
#include <iomanip>
#include <fstream>      // std::ofstream
#include <limits>       // 
//#include <boost/numeric/odeint.hpp>
//#include "spring.hpp"

//#include <>
//#include <>
#include "commdom.hpp"
#include "read_file.hpp"
using namespace std;

typedef std::numeric_limits< double > dbl;


void get_tetra_coords_j(double*  coords, int* vertices, int n_dist_j, int* elemts_i, double*      coords_j, double* tetracoords_j);
void        propi2propj(double* props_i, int* vertices, int n_dist_j, int* elemts_i, double* tetracoords_j, double*       props_j);

int n_node = 4; // tetras:4, tria:3 
int    DIM = 3;
int    DIM_PROPS; //MATMATMAT 

CommDom   CD = CommDom();

string IAM   = "PLEPP";
string NAMEj = "DIRIC"; 
string NAMEi = "NEUMA"; 

int main(int argc, char** argv)
{
  //--------------------------------------------------------------------||---//
  int idx = -1;
  int init_col=-1;

  string  name_argv = "";
  if(argc==2) name_argv = argv[1];

  string         namei  = ":(";
  string         namej  = ":(";
  string         dicti  = ":(";

  map<string, vector<int>    >            ArrayI;
  map<string, vector<double> >            ArrayD;

  map<string,string>                      Files;
  typedef map<string,string>::iterator    FilesIt;

  map<string,MPI_Comm>                    Commij;
  typedef map<string,MPI_Comm>::iterator  It;

//string PATH = "/home/bsc21/bsc21704/z2016/REPOSITORY/COMMDOM/PLE_2016MAR24/LIBPLEPP/Wrappers/Cpp/"; 
  string PATH = "./"; 
  if( name_argv == NAMEi )
  {
    namei         = name_argv; 
    namej         = NAMEj; 
    Commij[NAMEj] = MPI_COMM_NULL;
    // 
    //PATH += "/Cerfacs02";
    PATH += "/IZQ";
    Files["types"]        = PATH +"_TYPES.dat";       // 136896 
    Files["connectivity"] = PATH +"_ELEMENTS.dat";
    Files["points"]       = PATH +"_COORDINATES.dat"; // 69055  
    init_col = 1; 
  }

  if( name_argv == NAMEj )
  {
    namei         = name_argv; 
    namej         = NAMEi; 
    Commij[NAMEi] = MPI_COMM_NULL;
    // 
    //PATH += "/A";
    PATH += "/DER";
    Files["types"]        = PATH +"_TYPES.dat";       // 10370 
    Files["connectivity"] = PATH +"_ELEMENTS.dat";
    Files["points"]       = PATH +"_COORDINATES.dat"; // 5441 
  //Files["metis"]        = PATH +"Mesh02_ELEMENTS.metis.epart.3";
    init_col = 1;                                                             // <-- NOTE: 1==ALYA TYPE 
  }

  vector<read_log_file> DATA( Files.size() );

  //--------------------------------------------------------------| PLEPP |---//
  MPI_Init(NULL, NULL);
  MPI_Comm PLEPP_COMM_WORLD = MPI_COMM_WORLD;

  int  app_id = -1;
  int  n_apps = -1;
  MPI_Comm  local_comm;

  // CommDom
//CommDom  CD = CommDom();
  CD.init();
  CD.set_app_type(IAM);
  CD.set_world_comm(PLEPP_COMM_WORLD);
  CD.set_app_name(namei); 
  CD.name_to_id(&app_id, &n_apps, &local_comm);
  CD.__create_interaction__();
  CD.create_commij(local_comm);
  
  int local_rank = -1;
  int local_size = -1;
  MPI_Comm_rank(local_comm, &local_rank);
  MPI_Comm_size(local_comm, &local_size);

  //---------------------------------------------------------------------||---//
//MPI_Barrier(PLEPP_COMM_WORLD);

  int n_cells = -1; 
  for(FilesIt it=Files.begin(); it!=Files.end(); it++)
  {
    string name = it->first; 
    string path = it->second;   

    read_log_file DATA; 
    DATA.set_name( path  );
    DATA.run();
    
    vector< vector<double> > vdata( DATA.get_vdata() );

    if(name=="points")
    {
      ArrayD[name] = vector<double>(0);
      for(int i=0,k=0; i<vdata.size(); i++)
      {
      //for(int j=init_col; j<vdata[i].size(); j++) ArrayD[name].push_back( vdata[i][j] );
        for(int j=init_col,l=0; j<DIM+init_col; j++) ArrayD[name].push_back( vdata[i][j] );  
      }
    }
    else if(name=="metis")
    {
      ArrayI[name] = vector<int>(0);
      for(int i=0,k=0; i<vdata.size(); i++)
      {
        for(int j=0; j<vdata[i].size(); j++) ArrayI[name].push_back( (int)vdata[i][j] );
      }
    }
    else
    {
      ArrayI[name] = vector<int>(0);  
      for(int i=0,k=0; i<vdata.size(); i++) 
      {
        for(int j=init_col; j<vdata[i].size(); j++) ArrayI[name].push_back( (int)vdata[i][j] ); 
      }
    }

    DATA.end();
  }

  vector<int>        types( ArrayI.find("types")->second );
  vector<int> connectivity;
  vector<double>    points( ArrayD.find("points")->second );

  //---------------------------------------------------------------------||---//
  if(false)
  {
    read_log_file DATA;
    DATA.set_name( Files.find("connectivity")->second );
    DATA.run();
    DATA.end();
    vector< vector<double> > aux( DATA.get_vdata() );

    for(int i=0; i<aux.size(); i++)
    {
      for(int j=init_col; j<aux[i].size(); j++)
      {
        connectivity.push_back( (int)aux[i][j]  );
      }
    }
    aux.clear(); 

  }

  //---------------------------------------------------------------------||---//
  if(ArrayI.find("metis") != ArrayI.end())
  {
    vector<int> gpartition( ArrayI.find("metis")->second );

    map<int, vector<int> >            parts;   
    map<int, vector<int> >::iterator  it;

    for(int i=0; i<gpartition.size(); i++)
    { 
      int key = gpartition[i]; 
      it = parts.find(key);
      if(it != parts.end())  parts[key].push_back(i);   
      else                   parts[key] = vector<int>(1,i);  
    } 

    vector<int>    lpartition( parts[local_rank] ); 
    cout<<"local_rank:"<< local_rank <<" parts:"<< lpartition.size() <<" "<< lpartition[0] ;
    cout<<"\n";

    read_log_file DATA;
    DATA.set_name( Files.find("connectivity")->second );
    DATA.run();
    DATA.end();
    vector< vector<double> > aux( DATA.get_vdata() );

    vector<int> _types( lpartition.size(), -1);
    vector<int> _connectivity;
    for(int i=0; i<lpartition.size(); i++)
    {
      int key   = lpartition[i];
      _types[i] = types[key];  

      for(int j=init_col; j<aux[key].size(); j++)
      {
        _connectivity.push_back( (int)aux[key][j]  );
      }
    }
    aux.clear();

    types.clear(); 
    types = _types; 
   _types.clear(); 

    connectivity.clear(); 
    connectivity = _connectivity; 
   _connectivity.clear(); 

  }
  else 
  {
    read_log_file DATA;
    DATA.set_name( Files.find("connectivity")->second );
    DATA.run();
    DATA.end();
    vector< vector<double> > aux( DATA.get_vdata() );

    for(int i=0; i<aux.size(); i++)
    {
      for(int j=init_col; j<aux[i].size(); j++)
      {
        connectivity.push_back( (int)aux[i][j]  );
      }
    }

    cout<<"["<< namei <<"] n_pts:"<<  points.size()/DIM <<"\n";
    cout<<"["<< namei <<"] n_cells:"<<  aux.size()  <<"\n";  

    aux.clear();

  }
 
  //-----------------------------------------------------------| LOCATION |---//
//  MPI_Barrier(PLEPP_COMM_WORLD);

  MPI_Comm  commij = MPI_COMM_NULL;
  if( (CD.__get_app_name__() == namei)&&(CD.__get_friends__(namej) == 1) ) commij = CD.get_mpi_commij(namej);
  if( (CD.__get_app_name__() == namej)&&(CD.__get_friends__(namei) == 1) ) commij = CD.get_mpi_commij(namei); 

  int            n_vertices_i = 0;
  double* vertex_coords_i_ptr = NULL;

  int            n_vertices_j = 0;
  double* vertex_coords_j_ptr = NULL; 
  double* vertex_props_j_ptr = NULL; //MATMATMAT 

  int       n_elements_i = 0;
  int*  vertex_num_i_ptr = NULL;
  int* vertex_type_i_ptr = NULL;

  if(commij != MPI_COMM_NULL)
  {
  // My2ALYA
  char *aux_char01 = new char[ namei.size() ];
  char *aux_char02 = new char[ namei.size() ];

  memcpy( aux_char01, namei.c_str(), namei.size() );
  memcpy( aux_char02, namei.c_str(), namei.size() );
  
  ofstream propsfile;
  if( name_argv == NAMEi ) propsfile.open ("props_DIRIC.dat"); 
  if( name_argv == NAMEj ) propsfile.open ("props_NEUMA.dat"); 
  
  DIM_PROPS = 4; //MATMATMAT 
  int num_nodes = points.size()/DIM;
  vector<double>    propspropsi(DIM_PROPS*num_nodes); //MATMATMAT
  for(int i=0; i<propspropsi.size()/DIM_PROPS; i++)
  {
    //propspropsi[i] = i+1;
    //propsfile << i+1 << " " << name_argv << " " << propspropsi[i] << "\n";
    //
    //propspropsi[2*i] = points[2*i]*50000;
    //propspropsi[2*i+1] = points[2*i+1]*50000;
    //propspropsi[2*i] = i+1;
    //propspropsi[2*i+1] = i+1;
    //propsfile << i+1 << " " << name_argv << " " << propspropsi[2*i] << " " << propspropsi[2*i+1]  << "\n";
    //
    //propspropsi[3*i] = points[2*i]*50000;
    //propspropsi[3*i+1] = points[2*i+1]*50000;
    //propspropsi[3*i+2] = i+1;
    //propspropsi[3*i] = i+1;
    //propspropsi[3*i+1] = i+1;
    //propspropsi[3*i+2] = i+1;
    //propsfile << i+1 << " " << name_argv << " " << propspropsi[3*i] << " " << propspropsi[3*i+1]  << " " << propspropsi[3*i+2] << "\n";
    //
    propspropsi[4*i] = points[3*i];
    propspropsi[4*i+1] = points[3*i+1];
    propspropsi[4*i+2] = points[3*i+2];
    propspropsi[4*i+3] = (i+1);
    //propspropsi[4*i] = i+1;
    //propspropsi[4*i+1] = i+1;
    //propspropsi[4*i+2] = i+1;
    //propspropsi[4*i+3] = i+1;
    propsfile << i+1 << " " << name_argv << " " << propspropsi[4*i] << " " << propspropsi[4*i+1]  << " " << propspropsi[4*i+2] << " " << propspropsi[4*i+3] << "\n";
  }
  //exit(0);
  cout << "\n SIZE DE PROPSPROPSI " << name_argv << " " << propspropsi.size() << "\n";
  propsfile.close();

  if(0)
  CD.__mpi_sendrecv_char__(aux_char01, namei.size(),
                           aux_char02, namei.size(),
                           local_comm, commij);


             n_vertices_i = points.size()/DIM; 
      vertex_coords_i_ptr = points.data(); 

             n_elements_i = types.size();   
         vertex_num_i_ptr = connectivity.data(); 
        vertex_type_i_ptr = types.data(); 

      if(local_rank==0)
      {
               n_vertices_j = points.size()/DIM;
        vertex_coords_j_ptr = points.data();
        vertex_props_j_ptr = propspropsi.data(); //MATMATMAT
        //vertex_props_j_ptr = points.data(); //MATMATMAT
      }

      CD.locator_create2(local_comm, commij, 1e-3);
      CD.locator_set_cs_mesh(n_vertices_i,
                             n_elements_i,
                             vertex_coords_i_ptr,
                             vertex_num_i_ptr,
                             vertex_type_i_ptr, 
                             n_vertices_j,
                             vertex_coords_j_ptr, DIM, vertex_props_j_ptr, DIM_PROPS); //MATMATMAT
  
      CD.save_dist_coords(0, local_comm);

  //Spring S = Spring(); 
  //S.init(0.0, 0.0, 1.0, 1.0, 1.0, 0.0); // x, v, k, c, m, t 

    double time = 0.0;   
    //for(int itime=0; itime<1e6; itime++)
    ofstream distpropsfile;
    if( name_argv == NAMEi ) distpropsfile.open ("distprops_DIRIC.dat"); 
    if( name_argv == NAMEj ) distpropsfile.open ("distprops_NEUMA.dat"); 
    
    for(int itime=0; itime<1; itime++)
    {

      double dt = 0.0;  
      // Time step 
      {
        double send[1]={1.0e-2}, recv[1]={1.0/HUGE_VAL};
        int n_send=1, n_recv=1; 
        CD.__mpi_sendrecv_real__( send, n_send, recv, n_recv, local_comm, commij);
        CD.__mpi_bcast_real__(                  recv, n_recv, local_comm, commij);  

        time += 1.0/recv[0]; 
        dt    = 1.0/recv[0];  
        cout<<"["<< namei <<"] itime="<< itime <<" recv:"<< 1.0/recv[0] 
                                    //<<" "<< time 
                                      <<" \n";
      }

      // Local properties (to be sent)  
      vector<double>    props_i(n_vertices_j*DIM, HUGE_VAL);
      vector<double>  disp(2); 
      disp[0] =  0.0; 
      disp[1] =  0.0; //-1.0e-5; 
     

      for(int i=0,j=0; i<DIM; i++)
      {
        for(int k=0; k<n_vertices_j; k++) props_i[j++] = disp[i]; //-(i+1);  
      } 
      
      // Data to recv
      int n_recv = CD.get_n_interior();
      vector<double> var_ji(n_recv*DIM, HUGE_VAL);
      // Doing something with the data recieved 
      vector<int> interior_list_j(n_recv, -1); 
      CD.__locator_get_interior_list__( interior_list_j.data() ); 
      //for(int i=0; i<n_recv; i++): Props[ interior_list_j[i]-1 ] = var_ji[i]

      // Data to send (interpolation) 
      int n_send = CD.get_n_dist_points();

      vector<int>    dist_locations_i(n_send    , -1);
      vector<double>    dist_coords_j(n_send*DIM, HUGE_VAL);
      //vector<double>    dist_props_j (n_send*DIM, HUGE_VAL); //MATIAS
      vector<double>    dist_props_j (n_send*DIM_PROPS, HUGE_VAL); //MATMATMAT

      CD.__locator_get_dist_locations__( dist_locations_i.data() );
      CD.__locator_get_dist_coords__(       dist_coords_j.data() );
      CD.__locator_get_dist_props__(       dist_props_j.data() ); //MATIAS

      for (int i=0; i<dist_props_j.size()/DIM_PROPS; i++){
       //distpropsfile << i+1 << " " << name_argv << " " << dist_props_j[i] << " " << interior_list_j[i] << "\n"; // las props que recibo vs mis nodos
       //distpropsfile << i+1 << " " << name_argv << " " << dist_props_j[2*i] << " " << dist_props_j[2*i+1]  << " " << interior_list_j[i] << "\n";
       //distpropsfile << i+1 << " " << name_argv << " " << dist_props_j[3*i] << " " << dist_props_j[3*i+1]  << " " << dist_props_j[3*i+2] << " " << interior_list_j[i] << "\n";
       distpropsfile << i+1 << " " << name_argv << " " << dist_props_j[4*i] << " " << dist_props_j[4*i+1]  << " " << dist_props_j[4*i+2] << " " << dist_props_j[4*i+3] << " " << interior_list_j[i] << "\n";
      }//MATIAS
 
      cout << "\n SIZE DE DIST_PROPS_J " << name_argv << " " << dist_props_j.size() << "\n";
 

      vector<double>   shapef_j( n_node*n_send, HUGE_VAL);
      get_tetra_coords_j(vertex_coords_i_ptr, vertex_num_i_ptr,
                         n_send, dist_locations_i.data(), dist_coords_j.data(), shapef_j.data() );

      vector<double> var_ij(n_send*DIM, HUGE_VAL);
      for(int i=0; i<DIM; i++)
      {
        propi2propj(props_i.data()+n_vertices_j*i, vertex_num_i_ptr, n_send, dist_locations_i.data(), shapef_j.data(), var_ij.data()+n_send*i); 
      } 

      
      vector<double> aux(var_ij); 
      for(int i=0,j=0; i<n_send; i++)
      {
        for(int k=0; k<DIM; k++, j++) var_ij[j] = aux[k*n_send + i]; 
      }
      aux.clear(); 

      // Exchange  
      CD.__locator_exchange_double_scalar__(var_ij.data(), var_ji.data(), DIM); 
 
      for(int i=0; i<DIM; i++) 
      {
        vector<double> aux; //(n_recv*i + var_ji.begin(), n_recv*(i+1) + var_ji.begin());  
        for(int j=0;j<n_recv; j++) aux.push_back( var_ji[j*DIM + i] );  

        double average = 0.0;  
        for(int k=0; k<n_recv; k++) average += aux[k];
      //average /= n_recv; 

      //S.run( average,dt ); 

        cout<<"["<< namei <<"] itime="<< itime <<
                            " range[ "<< *std::min_element( aux.begin(), aux.end() ) <<", "
                                      << average <<", "
                                      << *std::max_element( aux.begin(), aux.end() ) <<" "
                                      <<"] \n";
      } 


      std::string        filename("fsi");
      std::stringstream  srut;
      srut<< std::setfill('0') << std::setw(6) << itime;
      filename += "_"+srut.str();
      filename += ".vtk";

      ofstream myfile;
      myfile.open ( filename.c_str() );

      for(int i=0,j=0;i<n_recv; i++)
      {
        int idx = interior_list_j[i]-1; 
        myfile<< idx  <<" "; 
        myfile<< scientific /*fixed*/ << showpoint;
        myfile<< setprecision( 10 ); //std::numeric_limits<double>::max_digits10 );
        myfile<< time <<" ";
        for(int k=0;k<DIM; k++) myfile<< points[idx*DIM   + k] <<" ";   
        for(int k=0;k<DIM; k++) myfile<< var_ji[  i*DIM   + k] <<" ";  // [x1 y1] [x2 y2] ... [xn yn] 
      //for(int k=0;k<DIM; k++) myfile<< var_ji[ k*n_recv + i] <<" ";  // [x1 x2 ... xn] [y1 y2 ... yn]    
        myfile<<"\n"; 
      }

      myfile<<"\n";
      myfile.close();

    }
    distpropsfile.close();

    CD.locator_destroy();

  } 

  //--------------------------------------------------------------------||---//
//  MPI_Barrier(PLEPP_COMM_WORLD);
//  MPI_Abort(commij, -1);        
  MPI_Finalize();

  return 0;
} // main
//=======================================================================||===//
//=======================================================================||===//

void
propi2propj(double* props_i, int* vertices, int n_dist_j, int* elemts_i, double* tetracoords_j, double* props_j )
{
  int    ii, ielem;
  int      vertices_i[n_node];
  double vol_coords_j[n_node];
  double       prop_j[n_node];

  if(n_dist_j > 0)
  {
    for(int ii=0; ii<n_dist_j; ii++ )
    {
      ielem = elemts_i[ii]-1; //  <- Fortran style ?? 
      for(int jj=0; jj<n_node; jj++)   vertices_i[jj]  =      vertices[n_node*ielem + jj]-1; // <- Fortran style -> C style !!  
      for(int jj=0; jj<n_node; jj++) vol_coords_j[jj]  = tetracoords_j[n_node*ii    + jj];
      for(int jj=0; jj<n_node; jj++)       prop_j[jj]  =       props_i[ vertices_i[jj]  ];

      props_j[ii] = 0.0;
      for(int jj=0; jj<n_node; jj++)      props_j[ii] += prop_j[jj] * vol_coords_j[jj];
    }
  }
}


void
get_tetra_coords_j(double* coords, int* vertices, int n_dist_j, int* elemts_i, double* coords_j, double* tetracoords_j)
{
  int    ii, ielem;
  int      vertices_i[n_node];
  double vol_coords_j[n_node];
  double      point_j[DIM];

  if(n_dist_j > 0)
  {
    for(int ii=0; ii<n_dist_j; ii++ )
    {
      for(int jj=0; jj<DIM;     jj++)   point_j[jj] = coords_j[DIM*ii + jj];

      ielem = elemts_i[ii]-1; //  <- Fortran style ?? 
      for(int jj=0; jj<n_node; jj++) vertices_i[jj] = vertices[n_node*ielem + jj]; // <- Fortran style, yes!! (ple rest one) 

      CD.__simple_interpolation__( coords, vertices_i, point_j, vol_coords_j );

      for(int jj=0; jj<n_node; jj++) tetracoords_j[n_node*ii + jj] = vol_coords_j[jj];
    }
  }
}

//=======================================================================||===//
//=======================================================================||===//
