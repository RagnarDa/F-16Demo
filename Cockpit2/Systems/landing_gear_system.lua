local landing_gear_system = GetSelf()

local update_time_step = 0.1
make_default_activity(update_time_step)

local sensor_data = get_base_data()

print("LOADING LANDING GEAR SYSTEM")
--Creating local variables
local GEAR_COMMAND		=	0				-- COMMANDED GEAR POS 0=UP, 1=DOWN
local GEAR_STATE		=	0				-- ACTUALT GEAR POS 0=UP,1=DOWN
local NOSE_GEAR_POS 	= 	0
local RIGHT_GEAR_POS 	=   0
local LEFT_GEAR_POS		=   0

function SetCommand(command,value)			
	print_message_to_user("SetCommand Triggered")
	
	if command == 3020 then  -- GEAR UP		
		GEAR_COMMAND = 0
		print_message_to_user("Gear Set to UP")
	end
	
	if command == 3019 then  -- GEAR DOWN		
		GEAR_COMMAND = 1
		print_message_to_user("Gear Set to DOWN")
	end
end

function update()		
	--print_message_to_user("Update")
	
	NOSE_GEAR_POS 	=  get_aircraft_draw_argument_value(0)
	RIGHT_GEAR_POS 	=  get_aircraft_draw_argument_value(3)
	LEFT_GEAR_POS 	=  get_aircraft_draw_argument_value(5)
	
	--if GEAR_COMMAND == 1 then
	--	GEAR_STATE = 1
	--end
	
	--if GEAR_COMMAND == 0 then
	--	GEAR_STATE = 0
	--end
	GEAR_STATE = 0
	
	if (sensor_data.getRadarAltitude() * 3.28084) < 500.0 then
		GEAR_STATE = 1
	end
	
	set_aircraft_draw_argument_value(0,GEAR_STATE)
	set_aircraft_draw_argument_value(3,GEAR_STATE)
	set_aircraft_draw_argument_value(5,GEAR_STATE)
	
	--if	sensor_data.getWOW_LeftMainLandingGear()==1 or
	--	sensor_data.getWOW_NoseLandingGear()==1 or
	--	sensor_data.getWOW_RightMainLandingGear()==1 then 
	--	print_message_to_user("TOUCHDOWN!")
	--end
end


