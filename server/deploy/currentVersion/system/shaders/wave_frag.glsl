#version 110



uniform sampler2D tex; // 0
uniform float dt;
uniform vec2 mouse; // Mouse position
uniform vec2 dim;
uniform float radius;
uniform bool warp;

/*
uniform vec3 p1; // top-left pixel
uniform vec3 p2; // top      pixel
uniform vec3 p3; // top-right pixel
uniform vec3 p4; // mid-left pixel
uniform vec3 p5; // mid pixel (self)
uniform vec3 p6; // mid-right pixel
uniform vec3 p7; // bottom-left pixel
uniform vec3 p8; // bottom pixel
uniform vec3 p9; // bottom-right pixel
*/
void main()
{
   float tau = 3.14159265*2.0;
   vec2 uv = gl_TexCoord[0].xy;
   if (warp) {
      
      vec2 uvP = vec2(uv.x*dim.x , uv.y*dim.y);
      
      vec2 dist = mouse - uvP;
      float theta = atan(dist.y , dist.x);
      float d = distance(dist , vec2(0.0,0.0));

      //if ((d < (radius + 50.0)) && (d > (radius - 50.0))) {
         float diff = d - radius;
         diff = sin((diff + 300.0)*tau/600.0);
         dist *= diff;
         uv = uv + vec2(dist.x/dim.x , dist.y/dim.x);
      //}


   }
   gl_FragColor = texture2D(tex , uv);
}

