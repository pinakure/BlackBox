uniform sampler2D tex; 
uniform vec2 resolution;
vec4 fragColor;
uniform vec2 fragCoord;

void main()
{
    vec2 xy = fragCoord.xy / resolution.xy;//Condensing this into one line
    xy.y = 1.0 - xy.y;
    vec4 texColor = texture2D(tex,xy);//Get the pixel at xy from iChannel0
    //texColor.b = xy.x;
    fragColor = texColor;//Set the screen pixel to that color
}