/* File generated with Shader Minifier 1.1.5
 * http://www.ctrl-alt-test.fr
 */
#ifndef SHADER_H_
# define SHADER_H_

const char *shader_glsl =
 "float h(vec2 v,vec2 h)"
 "{"
   "float l=0.;"
   "for(;l<150.;++l)"
     "{"
       "v=vec2(v.x*v.x-v.y*v.y+h.x,2.*v.x*v.y+h.y);"
       "if(length(v)>40.)"
         "break;"
     "}"
   "if(l>=150.)"
     "return 0.;"
   "l=l-log(length(v))/log(40.);"
   "return l/35.;"
 "}"
 "float h(vec2 v,vec2 l,vec2 y)"
 "{"
   "return h(v+y/vec2(1280,720),l);"
 "}"
 "float v(vec2 v,vec2 l)"
 "{"
   "return h(vec2(0),v+l/vec2(1280,720));"
 "}"
 "void main()"
 "{"
   "vec2 l=2.*(gl_FragCoord.xy/vec2(720)-vec2(.888889,.5))-vec2(.5,0),f;"
   "float y=10.*gl_Color.x;"
   "f=vec2(.9*sin(y)-.4,.8*cos(y));"
   "float r=length(l-f)+.01,g=clamp(.0002/r/r,0.,1.),e=h(l,f,vec2(-.75,.25)),c=h(l,f,vec2(-.25,-.75)),o=h(l,f,vec2(.25,.75)),t=h(l,f,vec2(.75,-.25)),x=.25*(e+c+o+t),a=v(l,vec2(-.75,.25)),n=v(l,vec2(-.25,-.75)),i=v(l,vec2(.25,.75)),u=v(l,vec2(.75,-.25)),C=.25*(a+n+i+u);"
   "gl_FragColor=vec4(g,x,C,1);"
 "}";

#endif // SHADER_H_
