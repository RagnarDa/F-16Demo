local self_ID = "F-16 High Fidelity Flight Dynamics Model"

local _spring_force_multiplier = 10
local _static_multiplier = 10
local _friction_multiplier = 1
local _roll_friction_multiplier = 1
local _damage_multiplier = 12

declare_plugin(self_ID,
{
installed 	 = true, -- if false that will be place holder , or advertising
dirName	  	 = current_mod_path,
version		 = "1.2.1",		 
state		 = "installed",
info		 = _("F-16 Technology Demonstration.  High Fidelity Flight Dynamics Model"),
binaries	= { 'F_16Demo', },  -- The DLL of the external flight model 
Skins	= 
	{
		{
			name	= "F-16Demo",
			dir		= "Theme"
		},
	},
Missions =
	{
		{
			name		= _("F-16Demo"),
			dir			= "Missions",
			CLSID		= "{CLSID5456456346CLSID}",	
		},
	},	
LogBook =
	{
		{
			name		= _("F-16Demo"),
			type		= "F-16Demo",
		},
	},	
InputProfiles =
	{
		["F-16Demo"]     = current_mod_path .. '/Input',
	},
})

suspension = 
{
	{
		mass  			  = 200,
		pos   			  = {1,0,0},
		moment_of_inertia = {1000,1000,1000},
		
		
		damage_element	    = 83,
		damage_omega	    = 30.0,
		state_angle_0	    =  6.131341662,
		state_angle_1	    = -2.995164152,
		mount_pivot_x	    = -0.274,
		mount_pivot_y	    = -0.118,
		mount_post_radius   = 0.657,
		mount_length	   	= 0.604555117,
		mount_angle_1	   	= -3.138548523,
		post_length	   		= 1.748,
		wheel_axle_offset 	= 0.05,
		self_attitude	    = false,
		yaw_limit		    = math.rad(89.0),
		damper_coeff	    = 30.0,
		

		amortizer_min_length					= 0.0,
		amortizer_max_length					= 0.397,
		amortizer_basic_length					= 0.397,
		amortizer_spring_force_factor			= 1.6e+13,
		amortizer_spring_force_factor_rate		= 20.0 * _spring_force_multiplier, --20.0,
		amortizer_static_force					= 80000.0 * _static_multiplier,--80000.0,
		amortizer_reduce_length					= 0.377,
		amortizer_direct_damper_force_factor	= 45000.0,
		amortizer_back_damper_force_factor		= 15000.0,


		wheel_radius				   = 0.308,
		wheel_static_friction_factor  = 0.65 * _friction_multiplier, --0.65,
		wheel_side_friction_factor    = 0.65 * _friction_multiplier, --0.65 ,
		wheel_roll_friction_factor    = 0.025 * _roll_friction_multiplier, --0.025,
		wheel_glide_friction_factor   = 0.28  * _friction_multiplier,--0.28,
		wheel_damage_force_factor     = 250.0 * _damage_multiplier,--250.0,
		wheel_damage_speed			   = 150.0,


		 wheel_moment_of_inertia   = 3.6, --wheel moi as rotation body

		 wheel_brake_moment_max = 15000.0, -- maximum value of braking moment  , N*m 
		
		--[[
		args_post	  = {0,3,5};
		args_amortizer = {1,4,6};
		args_wheel	  = {76,77,77};
		args_wheel_yaw = {2,-1,-1};
		--]]

		arg_post			  = 0,
		arg_amortizer		  = 1,
		arg_wheel_rotation    = 76,
		arg_wheel_yaw		  = 2,
		collision_shell_name  = "WHEEL_F",
	},
	{
		mass  			  = 200,
		pos   			  = {0,0,-1},
		moment_of_inertia = {1000,1000,1000},
		
		
		damage_element	    = 83,
		damage_omega	    = 30.0,
		state_angle_0	    =  6.131341662,
		state_angle_1	    = -2.995164152,
		mount_pivot_x	    = -0.274,
		mount_pivot_y	    = -0.118,
		mount_post_radius   = 0.657,
		mount_length	   	= 0.604555117,
		mount_angle_1	   	= -3.138548523,
		post_length	   		= 1.748,
		wheel_axle_offset 	= 0.05,
		self_attitude	    = false,
		yaw_limit		    = math.rad(89.0),
		damper_coeff	    = 30.0,
		

		amortizer_min_length					= 0.0,
		amortizer_max_length					= 0.397,
		amortizer_basic_length					= 0.397,
		amortizer_spring_force_factor			= 1.6e+13,
		amortizer_spring_force_factor_rate		= 20.0 * _spring_force_multiplier, --20.0,
		amortizer_static_force					= 80000.0 * _static_multiplier,--80000.0,
		amortizer_reduce_length					= 0.377,
		amortizer_direct_damper_force_factor	= 45000.0,
		amortizer_back_damper_force_factor		= 15000.0,


		wheel_radius				   = 0.308,
		wheel_static_friction_factor  = 0.65 * _friction_multiplier, --0.65,
		wheel_side_friction_factor    = 0.65 * _friction_multiplier, --0.65 ,
		wheel_roll_friction_factor    = 0.025 * _roll_friction_multiplier, --0.025,
		wheel_glide_friction_factor   = 0.28  * _friction_multiplier,--0.28,
		wheel_damage_force_factor     = 250.0 * _damage_multiplier,--250.0,
		wheel_damage_speed			   = 150.0,
		 wheel_moment_of_inertia   = 3.6, --wheel moi as rotation body

		 wheel_brake_moment_max = 15000.0, -- maximum value of braking moment  , N*m 
		
		--[[
		args_post	  = {0,3,5};
		args_amortizer = {1,4,6};
		args_wheel	  = {76,77,77};
		args_wheel_yaw = {2,-1,-1};
		--]]

		arg_post			  = 3,
		arg_amortizer		  = 4,
		arg_wheel_rotation    = 77,
		arg_wheel_yaw		  = -1,
		collision_shell_name  = "WHEEL_L",
	},
	{
		mass  			  = 200,
		pos   			  = {0,0,1},
		moment_of_inertia = {1000,1000,1000},
		
		
		damage_element	    = 83,
		damage_omega	    = 30.0,
		state_angle_0	    =  6.131341662,
		state_angle_1	    = -2.995164152,
		mount_pivot_x	    = -0.274,
		mount_pivot_y	    = -0.118,
		mount_post_radius   = 0.657,
		mount_length	   	= 0.604555117,
		mount_angle_1	   	= -3.138548523,
		post_length	   		= 1.748,
		wheel_axle_offset 	= 0.05,
		self_attitude	    = false,
		yaw_limit		    = math.rad(89.0),
		damper_coeff	    = 30.0,
		

		amortizer_min_length					= 0.0,
		amortizer_max_length					= 0.397,
		amortizer_basic_length					= 0.397,
		amortizer_spring_force_factor			= 1.6e+13,
		amortizer_spring_force_factor_rate		= 20.0 * _spring_force_multiplier, --20.0,
		amortizer_static_force					= 80000.0 * _static_multiplier,--80000.0,
		amortizer_reduce_length					= 0.377,
		amortizer_direct_damper_force_factor	= 45000.0,
		amortizer_back_damper_force_factor		= 15000.0,


		wheel_radius				   = 0.308,
		wheel_static_friction_factor  = 0.65 * _friction_multiplier, --0.65,
		wheel_side_friction_factor    = 0.65 * _friction_multiplier, --0.65 ,
		wheel_roll_friction_factor    = 0.025 * _roll_friction_multiplier, --0.025,
		wheel_glide_friction_factor   = 0.28  * _friction_multiplier,--0.28,
		wheel_damage_force_factor     = 250.0 * _damage_multiplier,--250.0,
		wheel_damage_speed			   = 150.0,
		 wheel_moment_of_inertia   = 3.6, --wheel moi as rotation body

		 wheel_brake_moment_max = 15000.0, -- maximum value of braking moment  , N*m 
		
		--[[
		args_post	  = {0,3,5};
		args_amortizer = {1,4,6};
		args_wheel	  = {76,77,77};
		args_wheel_yaw = {2,-1,-1};
		--]]

		arg_post			  = 5,
		arg_amortizer		  = 6,
		arg_wheel_rotation    = 77,
		arg_wheel_yaw		  = -1,
		collision_shell_name  = "WHEEL_R",
	},
}

---------------------------------------------------------------------------------------
dofile(current_mod_path..'/F16Demo.lua')

local FM = 
{
	[1] = self_ID,
	[2] = "F_16Demo",
	center_of_mass		=	{ 0.183 , 0.261 , 0.0},		-- center of mass position relative to object 3d model center for empty aircraft
	moment_of_inertia  	= 	{12874.0, 85552.1, 75673.6},   	-- moment of inertia of empty aircraft
	suspension   = suspension, -- gear posts initialization
}

--make_flyable(obj_name,optional_cockpit path,optional_fm = {mod_of_fm_origin,dll_with_fm})
make_flyable('F-16Demo',nil,FM,nil)
--current_mod_path..'/Cockpit/Scripts/'
plugin_done()-- finish declaration , clear temporal data
