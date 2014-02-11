dofile(LockOn_Options.common_script_path.."devices_defs.lua")
indicator_type       = indicator_types.COLLIMATOR----------------------
init_pageID     = 1
purposes 	   = {render_purpose.GENERAL,render_purpose.HUD_ONLY_VIEW}
--subset ids
BASE    = 1
INDICATION = 2

page_subsets  = {
[BASE]    		= LockOn_Options.script_path.."HUD/Indicator/base_page.lua",
[INDICATION]    = LockOn_Options.script_path.."HUD/Indicator/indication_page.lua",
}
pages = 
{
	{
	 BASE,
	 INDICATION
	 },
}

