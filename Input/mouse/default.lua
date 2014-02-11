function layout()
return {

keyCommands = {
{combos = {{key = 'MOUSE_BTN3'}}, down = iCommandViewTransposeModeOn, up = iCommandViewTransposeModeOff, name = 'Camera transpose mode on/off', category = 'View Cockpit'},
},

axisCommands = {
-- mouse axes
{combos = {{key = 'MOUSE_X'}}, action = iCommandPlaneViewHorizontal, name = 'Camera Horizontal View (mouse)'},
{combos = {{key = 'MOUSE_Y'}}, action = iCommandPlaneViewVertical, name = 'Camera Vertical View (mouse)'},
{combos = {{key = 'MOUSE_Z'}}, action = iCommandPlaneZoomView, name = 'Camera Zoom View (mouse)'},

{action = iCommandPlaneSelecterHorizontal	, name = 'I-251 Slew Horizontal (mouse)'},
{action = iCommandPlaneSelecterVertical		, name = 'I-251 Slew Vertical (mouse)'},
},
}
end
