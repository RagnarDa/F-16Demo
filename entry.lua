local self_ID = "F-16 High Fidelity Flight Dynamics Model"

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
---------------------------------------------------------------------------------------
dofile(current_mod_path..'/F16Demo.lua')

local FM = 
{
	[1] = self_ID,
	[2] = "F_16Demo",
	center_of_mass		=	{ 0.183 , 0.261 , 0.0},		-- center of mass position relative to object 3d model center for empty aircraft
	moment_of_inertia  	= 	{12874.0, 85552.1, 75673.6},   	-- moment of inertia of empty aircraft
}

--make_flyable(obj_name,optional_cockpit path,optional_fm = {mod_of_fm_origin,dll_with_fm})
make_flyable('F-16Demo',nil,FM,nil)
--current_mod_path..'/Cockpit/Scripts/'
plugin_done()-- finish declaration , clear temporal data
