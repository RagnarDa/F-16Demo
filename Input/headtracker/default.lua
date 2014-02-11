function layout()
return {

axisCommands = {
-- headtracker axes
{combos = {{key = 'HEADTRACKER_PITCH'}}, action = iCommandViewVerticalAbs, name = 'Absolute Camera Vertical View'},
{combos = {{key = 'HEADTRACKER_YAW'}}, action = iCommandViewHorizontalAbs, name = 'Absolute Camera Horizontal View'},
{combos = {{key = 'HEADTRACKER_X'}}, action = iCommandViewHorTransAbs, name = 'Absolute Horizontal Shift Camera View'},
{combos = {{key = 'HEADTRACKER_Y'}}, action = iCommandViewVertTransAbs, name = 'Absolute Vertical Shift Camera View'},
{combos = {{key = 'HEADTRACKER_Z'}}, action = iCommandViewLongitudeTransAbs, name = 'Absolute Longitude Shift Camera View'},
{combos = {{key = 'HEADTRACKER_ROLL'}}, action = iCommandViewRollAbs, name = 'Absolute Roll Shift Camera View'},
{combos = nil, action = iCommandViewZoomAbs, name = 'Zoom View'},
},
}

end


