import jet02 as mesh 
obj = mesh.Negociente

import sys
file_name = sys.argv[0]
sys.path.append("../Salome2Alya")


#------------------------------------------------------------| Salome2dat |---# 
from salome2dat import *

S = Salome2dat()
S.get_data(obj)
#S.save_data(file_name)

R = S.get_points()
modR = np.sqrt( (R**2).sum(axis=1) )
S.VTK.set_points_prop(modR, "|R|")


#-----------------------------------------------------| Initial Conditions |---# 
Etot = np.zeros(R.shape[0])
Rho  = np.zeros(R.shape[0])
Vel  = np.zeros(R.shape)

Rho0 = 1.2 
T0   = 300.0 
V0   = 200.0 
Etot[:]  = T0
Rho[:]   = Rho0
Vel[:]   = 0.0
Vel[:,0] = 0.0
Vel[:,1] = 0.0

OK  = np.sqrt( (R[:,0]**2 + R[:,1]**2) )<2.9 
OK *= R[:,2]==0.0  

Vel[OK,2] = V0
Etot[OK]  = 3*T0

OK = R[:,2]>49.9
Rho[OK] = Rho0


#------------------------------------------------------------------| alya |---# 
S.Alya.NUMBER_OF_STEPS = 250
S.Alya.set_prop_vals([0.0,0.0,0.0], "VELOCITY")
S.Alya.set_prop_vals(T0, "TEMPERATURE")

S.Alya.set_prop_vec(Vel, file_name, "VELOCITY") 
S.Alya.set_prop(Rho, file_name, "DENSITY") 
S.Alya.set_prop(Etot, file_name, "TEMPERATURE") 
#S.Alya.set_prop(T, file_name, "ENERGY") 
#S.Alya.set_prop(Press, file_name, "PRESSURE") 

S.to_alya(file_name)


init_keys = {}
init_keys["Inlet"] = ["V", "T"] 
init_keys["Top"]   = ["R"]
init_keys["Walls"] = ["V", "T"]

init_vals = {}
init_vals["Inlet"] = [[0.0,0.0,V0],  3*T0]
init_vals["Top"]   =  [Rho0]
init_vals["Walls"] = [[0.0,0.0,0.0], 1*T0]

del S.groups_names["EdgesWalls"] 
S.Alya.set_initial_conditions_codes(S.groups_names, init_keys, init_vals)


#-----------------------------------------------------------------------------# 
print "OK! \n"
