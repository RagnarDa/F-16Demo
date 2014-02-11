#pragma once

/*/////////////////////////////////////////////////////////////////////////
Pointer to function of force source in body axis 
x,y,z			  - force components in body coordinate system
pos_x,pos_y,pos_z - position of force source in body coordinate system 

body coordinate system system is always X - positive forward ,
										Y - positive up,
										Z - positive to right 
prototype for 		

ed_fm_add_local_force
ed_fm_add_global_force
*/
typedef void (*PFN_FORCE_SOURCE)  (double & x,double &y,double &z,double & pos_x,double & pos_y,double & pos_z);
/*
Pointer to function of moment source in body axis 
prototype for 
ed_fm_add_local_moment
ed_fm_add_global_moment
*/
typedef void (*PFN_MOMENT_SOURCE) (double & x,double &y,double &z);

/*
simulate will be called on each step, all your FM should be evaluated here,
result of simulation will be called later as forces and moment sources

prototype for

ed_fm_simulate
*/
typedef void (*PFN_SIMULATE)	  (double dt);

/*
called before simulation to set up your environment for the next step

prototype for

ed_fm_set_atmosphere
*/
typedef void (*PFN_SET_ATMOSPHERE)(double h,//altitude above sea level
								   double t,//current atmosphere temperature , Kelwins
								   double a,//speed of sound
								   double ro,// atmosphere density
								   double p,// atmosphere pressure
								   double wind_vx,//components of velocity vector, including turbulence in world coordinate system
								   double wind_vy,//components of velocity vector, including turbulence in world coordinate system
								   double wind_vz //components of velocity vector, including turbulence in world coordinate system
								   );
/*
called before simulation to set up your environment for the next step

prototype for

ed_fm_set_current_mass_state
*/
typedef void (*PFN_CURRENT_MASS_STATE) (double mass,
										double center_of_mass_x,
										double center_of_mass_y,
										double center_of_mass_z,
										double moment_of_inertia_x,
										double moment_of_inertia_y,
										double moment_of_inertia_z
										);
/*
called before simulation to set up your environment for the next step

prototype for

ed_fm_set_current_state
*/
typedef void (*PFN_CURRENT_STATE) (double ax,//linear acceleration component in world coordinate system
								   double ay,//linear acceleration component in world coordinate system
								   double az,//linear acceleration component in world coordinate system
								   double vx,//linear velocity component in world coordinate system
								   double vy,//linear velocity component in world coordinate system
								   double vz,//linear velocity component in world coordinate system
								   double px,//center of the body position in world coordinate system
								   double py,//center of the body position in world coordinate system
								   double pz,//center of the body position in world coordinate system
								   double omegadotx,//angular accelearation components in world coordinate system
								   double omegadoty,//angular accelearation components in world coordinate system
								   double omegadotz,//angular accelearation components in world coordinate system
								   double omegax,//angular velocity components in world coordinate system
								   double omegay,//angular velocity components in world coordinate system
								   double omegaz,//angular velocity components in world coordinate system
								   double quaternion_x,//orientation quaternion components in world coordinate system
								   double quaternion_y,//orientation quaternion components in world coordinate system
								   double quaternion_z,//orientation quaternion components in world coordinate system
								   double quaternion_w //orientation quaternion components in world coordinate system
								   );
/*
additional state information in body axis

ed_fm_set_current_state_body_axis
*/
typedef void (*PFN_CURRENT_STATE_BODY_AXIS) (double ax,//linear acceleration component in body coordinate system
											 double ay,//linear acceleration component in body coordinate system
											 double az,//linear acceleration component in body coordinate system
											 double vx,//linear velocity component in body coordinate system
											 double vy,//linear velocity component in body coordinate system
											 double vz,//linear velocity component in body coordinate system
											 double wind_vx,//wind linear velocity component in body coordinate system
											 double wind_vy,//wind linear velocity component in body coordinate system
											 double wind_vz,//wind linear velocity component in body coordinate system

											 double omegadotx,//angular accelearation components in body coordinate system
											 double omegadoty,//angular accelearation components in body coordinate system
											 double omegadotz,//angular accelearation components in body coordinate system
											 double omegax,//angular velocity components in body coordinate system
											 double omegay,//angular velocity components in body coordinate system
											 double omegaz,//angular velocity components in body coordinate system
											 double yaw,  //radians
											 double pitch,//radians
											 double roll, //radians
											 double common_angle_of_attack, //AoA radians
											 double common_angle_of_slide   //AoS radians
	);
/*
input handling

prototype for 

ed_fm_set_command
*/
typedef void (*PFN_SET_COMMAND) (int command,
								 float value);

/*
 Mass handling 

 will be called  after ed_fm_simulate :
 you should collect mass changes in ed_fm_simulate 

 double delta_mass = 0;
 double x = 0;
 double y = 0; 
 double z = 0;
 double piece_of_mass_MOI_x = 0;
 double piece_of_mass_MOI_y = 0; 
 double piece_of_mass_MOI_z = 0;
 
 //
 while (ed_fm_change_mass(delta_mass,x,y,z,piece_of_mass_MOI_x,piece_of_mass_MOI_y,piece_of_mass_MOI_z))
 {
	//internal DCS calculations for changing mass, center of gravity,  and moments of inertia
 }

*/
typedef bool (*PFN_CHANGE_MASS) (double & delta_mass,
							 	 double & delta_mass_pos_x,
								 double & delta_mass_pos_y,
								 double & delta_mass_pos_z,
								 double & delta_mass_moment_of_inertia_x,
								 double & delta_mass_moment_of_inertia_y,
								 double & delta_mass_moment_of_inertia_z
								 );
/*
	set internal fuel volume , init function, called on object creation and for refueling , 
	you should distribute it inside at different fuel tanks
	
	prototype for ed_fm_set_internal_fuel

*/
typedef void   (*PFN_SET_INTERNAL_FUEL)  (double fuel);

/*
	get internal fuel volume 
	
	prototype for ed_fm_get_internal_fuel

*/
typedef double (*PFN_GET_INTERNAL_FUEL)  ();

/*
	set external fuel volume for each payload station , called for weapon init and on reload
	
	prototype for ed_fm_set_external_fuel

*/
typedef void   (*PFN_SET_EXTERNAL_FUEL)  (int	 station,
										  double fuel,
										  double x,
										  double y,
										  double z);
/*
	get external fuel volume 
	
	prototype for ed_fm_get_external_fuel
*/
typedef double (*PFN_GET_EXTERNAL_FUEL)  ();

struct EdDrawArgument
{
	union 
	{
		float f;
		void *p;
		int   i;
	};
};
/*
	update draw arguments for your aircraft 
*/
typedef double (*PFN_SET_DRAW_ARGS)  (EdDrawArgument * array,size_t size);


/*
will be called for your internal configuration
ed_fm_configure
*/
typedef double (*PFN_CONFIGURE)  (const char * cfg_path);


/*
various param call back to proper integrate your FM to DCS , like engine RPM , thrust etc
ed_fm_get_param
*/

enum ed_fm_param_enum
{
	ED_FM_ENGINE_0_RPM = 0,					//NOTE!!!! engine at index 0 is APU
	ED_FM_ENGINE_0_RELATED_RPM,				//NOTE!!!! engine at index 0 is APU
	ED_FM_ENGINE_0_CORE_RPM,				//NOTE!!!! engine at index 0 is APU
	ED_FM_ENGINE_0_CORE_RELATED_RPM,		//NOTE!!!! engine at index 0 is APU
	ED_FM_ENGINE_0_THRUST,					//NOTE!!!! engine at index 0 is APU
	ED_FM_ENGINE_0_RELATED_THRUST,			//NOTE!!!! engine at index 0 is APU
	ED_FM_ENGINE_0_CORE_THRUST,				//NOTE!!!! engine at index 0 is APU
	ED_FM_ENGINE_0_CORE_RELATED_THRUST,		//NOTE!!!! engine at index 0 is APU
	/*RESERVED PLACE FOR OTHER ENGINE PARAM*/
	ED_FM_ENGINE_1_RPM = 100,	
	ED_FM_ENGINE_1_RELATED_RPM,				
	ED_FM_ENGINE_1_CORE_RPM,				
	ED_FM_ENGINE_1_CORE_RELATED_RPM,		
	ED_FM_ENGINE_1_THRUST,					
	ED_FM_ENGINE_1_RELATED_THRUST,			
	ED_FM_ENGINE_1_CORE_THRUST,				
	ED_FM_ENGINE_1_CORE_RELATED_THRUST,		
	//.................................
	ED_FM_ENGINE_2_RPM = 2 * (ED_FM_ENGINE_1_RPM - ED_FM_ENGINE_0_RPM),
	ED_FM_ENGINE_2_RELATED_RPM,				
	ED_FM_ENGINE_2_CORE_RPM,				
	ED_FM_ENGINE_2_CORE_RELATED_RPM,		
	ED_FM_ENGINE_2_THRUST,					
	ED_FM_ENGINE_2_RELATED_THRUST,			
	ED_FM_ENGINE_2_CORE_THRUST,				
	ED_FM_ENGINE_2_CORE_RELATED_THRUST,
	//.................................
	ED_FM_ENGINE_3_RPM = 3 * (ED_FM_ENGINE_1_RPM - ED_FM_ENGINE_0_RPM),
	/*	
		up to 20 engines
	*/
	ED_FM_END_ENGINE_BLOCK = 20 * (ED_FM_ENGINE_1_RPM - ED_FM_ENGINE_0_RPM),
};

typedef double (*PFN_GET_PARAM)  (unsigned param_enum);



/*
prepare your FM for different start conditions:
ed_fm_cold_start
ed_fm_hot_start
ed_fm_hot_start_in_air

next functions will be called before 

ed_fm_set_current_state
ed_fm_set_current_mass_state
ed_fm_set_atmosphere
*/
typedef void (*PFN_COLD_START)  ();
typedef void (*PFN_HOT_START)   ();
typedef void (*PFN_HOT_START_IN_AIR)   ();
