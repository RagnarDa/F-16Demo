dofile(LockOn_Options.script_path.."RADAR/Indicator/definitions.lua")

local MARK_MATERIAL   = MakeMaterial(nil,{255,120,0,255})

function AddGrid(dx,dy,rows,cols)
	local rows          = rows or 1
	local cols          = cols or 1
	
	local sz_x          = cols * dx
	local sz_y          = rows * dy
	
	local verts       = {{0,0},
						 {sz_x,0},
						 {sz_x,sz_y},
						 {0,sz_y}}
	local indices     = {0,1,2,2,3,0}
	
	for j = 1,cols do
		verts[#verts + 1] =  {dx*j ,   0}	indices[#indices + 1] = #verts - 1	
		verts[#verts + 1] =  {dx*j ,sz_y}	indices[#indices + 1] = #verts - 1
	end
	
	for i = 1,rows do
		verts[#verts + 1] =  {0   ,dy*i}	indices[#indices + 1] = #verts - 1	
		verts[#verts + 1] =  {sz_x,dy*i}	indices[#indices + 1] = #verts - 1
	end

	grid			    = CreateElement "ceMeshPoly"
	grid.name 		    = create_guid_string()
	grid.material	    = MARK_MATERIAL
	grid.primitivetype  = "lines"	
	grid.vertices       = verts
	grid.indices	    = indices
	grid.init_pos		= {-0.5 * sz_x , -0.5 * sz_y}
	Add(grid)
	return grid
end

AddGrid(0.1,0.1,20,20)
