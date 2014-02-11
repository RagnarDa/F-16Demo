function layout()
return {
axisCommands = {
{combos = {{key = 'TRACKIR_PITCH'}}, action = iCommandViewVerticalAbs, name = 'TrackIR Absolute Camera Vertical View'},
{combos = {{key = 'TRACKIR_YAW'}} , action = iCommandViewHorizontalAbs, name = 'TrackIR Absolute Camera Horizontal View'},
{combos = {{key = 'TRACKIR_X'}}   , action = iCommandViewHorTransAbs, name = 'TrackIR Absolute Horizontal Shift Camera View'},
{combos = {{key = 'TRACKIR_Y'}}   , action = iCommandViewVertTransAbs, name = 'TrackIR Absolute Vertical Shift Camera View'},
{								    action = iCommandViewLongitudeTransAbs, name = 'TrackIR Absolute Longitude Shift Camera View'},
{combos = {{key = 'TRACKIR_ROLL'}}, action = iCommandViewRollAbs, name = 'TrackIR Absolute Roll Shift Camera View'},
{combos = {{key = 'TRACKIR_Z'}}   , action = iCommandViewZoomAbs, name = 'Zoom View'},
},
}
end
