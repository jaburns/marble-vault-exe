float map( vec2 z, vec2 c )
{
    float i = 0.;
    
    for( ; i < 150.; ++i ) {
        z = vec2(
            z.r*z.r - z.g*z.g + c.r,
            2.*z.r*z.g + c.g
        );  
        if (length(z) > 40.) break;
    }
    
    if (i >= 150.) return 0.;
    
    i = i - log(length(z)) / log(40.);
    
    return( i / 35. );
}

float sampJulia( vec2 uv, vec2 muv, vec2 offset )
{
    return map( uv + offset / vec2(1280,720), muv );
}

float sampMandelbrot( vec2 uv, vec2 offset )
{
    return map( vec2( 0 ), uv + offset / vec2(1280,720) );
}

void main()
{
    vec2 uv  = 2.*( gl_FragCoord.xy/vec2(720) - vec2(.5 * (1280./720.), .5) ) - vec2(.5, 0);
    vec2 muv;
    float t = 10.*gl_Color.x;
    muv = vec2(.9*sin( t ) - .4, .8*cos( t ));
    
    float lm = length(uv - muv) + .01;
    float c = clamp(.0002 / lm /lm, 0., 1.);
    
    float ja = sampJulia(uv, muv, vec2(-.75, .25));
    float jb = sampJulia(uv, muv, vec2(-.25,-.75));
    float jc = sampJulia(uv, muv, vec2( .25, .75));
    float jd = sampJulia(uv, muv, vec2( .75,-.25));
    float j = 0.25*(ja + jb + jc + jd);
    
    float ma = sampMandelbrot(uv, vec2(-.75, .25));
    float mb = sampMandelbrot(uv, vec2(-.25,-.75));
    float mc = sampMandelbrot(uv, vec2( .25, .75));
    float md = sampMandelbrot(uv, vec2( .75,-.25));
    float m = 0.25*(ma + mb + mc + md);

    gl_FragColor = vec4(c, j, m, 1);
}
