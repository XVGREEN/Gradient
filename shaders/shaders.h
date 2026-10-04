#pragma once


// --- Placeholder fullscreen-quad shader (replace with the real one tomorrow) ---
static const char * default_vertex_src = R"(
#version 330 core
layout (location = 0) in vec2 aPos;
void main() {
    gl_Position = vec4(aPos, 0.0, 1.0);
}
)";


static const char* default_fragment_src = R"(

#version 330 core
out vec4 FragColor;

uniform vec3 iResolution;
uniform vec2 viewport;
uniform float iTime;
uniform vec2 mouse;
uniform sampler2D tex;
#define R iResolution


#define T iTime


void mainImage(out vec4 o, in vec2 u) {
    vec2 uv = (u*2.-iResolution.xy)/max(R.x,R.y);
     
    u = u/R.xy;
    u.y=1.-u.y;
    vec3 col = texture(tex,u).xyz;
    
    o = vec4(col,1.);
    
}

void main() {
    vec2 FC=gl_FragCoord.xy-viewport;
    mainImage(FragColor, FC);
   
}
)";

static const char* vignete = R"(

#version 330 core
out vec4 FragColor;

uniform vec3 iResolution;
uniform vec2 viewport;
uniform float iTime;
uniform float preview;
uniform vec2 mouse;
uniform sampler2D tex;
#define R iResolution
uniform float strength;

#define T iTime

void mainImage(out vec4 o, in vec2 u) {
    float d = mix(1., step(u.x, mouse.x - viewport.x), preview);

    vec2 uv = (u*2.-iResolution.xy)/max(R.x,R.y);

    u = u/max(R.x,R.y);
    u.y=1.-u.y;

    vec3 col = texture(tex,u).xyz;
    vec3 col2 = col*(1.-length(uv*strength));

    vec3 final = mix(col,col2,d);
    o = vec4(final,1.);
}

void main() {
    vec2 FC=gl_FragCoord.xy-viewport;
    mainImage(FragColor, FC);
}
)";

static const char* contrast = R"(

#version 330 core
out vec4 FragColor;

uniform vec3 iResolution;
uniform vec2 viewport;
uniform float iTime;
uniform float preview;
uniform vec2 mouse;
uniform sampler2D tex;
uniform float strength;
#define R iResolution

#define T iTime

void mainImage(out vec4 o, in vec2 u) {
    float d = mix(1., step(u.x, mouse.x - viewport.x), preview);

    u = u/R.xy;
    u.y=1.-u.y;

    vec3 col = texture(tex,u).xyz;
    vec3 col2 = pow(col,vec3(1.+strength));

    vec3 final = mix(col,col2,d);
    o = vec4(final,1.);
}

void main() {
    vec2 FC=gl_FragCoord.xy-viewport;
    mainImage(FragColor, FC);
}
)";

static const char* grayscale = R"(

#version 330 core
out vec4 FragColor;

uniform vec3 iResolution;
uniform vec2 viewport;
uniform float iTime;
uniform float preview;
uniform vec2 mouse;
uniform sampler2D tex;
#define R iResolution
#define T iTime

void mainImage(out vec4 o, in vec2 u) {
    float d = mix(1., step(u.x, mouse.x - viewport.x), preview);

    u = u/max(R.x,R.y);
    u.y=1.-u.y;

    vec3 col = texture(tex,u).xyz;
    vec3 col2 = vec3(dot(col,vec3(.299,.587,.114)));

    vec3 final = mix(col,col2,d);
    o = vec4(final,1.);
}

void main() {
    vec2 FC=gl_FragCoord.xy-viewport;
    mainImage(FragColor, FC);
}
)";

static const char* chromatic = R"(

#version 330 core
out vec4 FragColor;

uniform vec3 iResolution;
uniform vec2 viewport;
uniform float iTime;
uniform float preview;
uniform vec2 mouse;
uniform sampler2D tex;
uniform float strength;
#define R iResolution

#define T iTime

void mainImage(out vec4 o, in vec2 u) {
    float d = mix(1., step(u.x, mouse.x - viewport.x), preview);

    u = u/R.xy;
    u.y=1.-u.y;

    vec3 col = texture(tex,u).xyz;

    float abb=.015*strength;
    float r = texture(tex,u+vec2(0.,abb)).r;
    float g = texture(tex,u+vec2(abb,0.)).g;
    float b = texture(tex,u+vec2(0.,-abb)).b;
    vec3 col2 = vec3(r,g,b);

    vec3 final = mix(col,col2,d);
    o = vec4(final,1.);
}

void main() {
    vec2 FC=gl_FragCoord.xy-viewport;
    mainImage(FragColor, FC);
}
)";

static const char* radial_chromatic = R"(

#version 330 core
out vec4 FragColor;

uniform vec3 iResolution;
uniform vec2 viewport;
uniform float iTime;
uniform float preview;
uniform vec2 mouse;
uniform sampler2D tex;
#define R iResolution
uniform float ab=.001;

#define T iTime

void mainImage(out vec4 o, in vec2 u) {
    float d = mix(1., step(u.x, mouse.x - viewport.x), preview);

    vec2 uv = (u*2.-iResolution.xy)/max(R.x,R.y);

    u = u/R.xy;
    u.y=1.-u.y;

    vec3 col = texture(tex,u).xyz;

    float abb=pow(length(uv),1.001)*.1;
    float r = texture(tex,u+vec2(0.,abb)).r;
    float g = texture(tex,u+vec2(abb,0.)).g;
    float b = texture(tex,u+vec2(0.,-abb)).b;
    vec3 col2 = vec3(r,g,b);

    vec3 final = mix(col,col2,d);
    o = vec4(final,1.);
}

void main() {
    vec2 FC=gl_FragCoord.xy-viewport;
    mainImage(FragColor, FC);
}
)";

static const char* gaussian_blur = R"(

#version 330 core
out vec4 FragColor;

uniform vec3 iResolution;
uniform vec2 viewport;
uniform float iTime;
uniform float preview;
uniform vec2 mouse;
uniform sampler2D tex;
#define R iResolution
uniform float strength;

#define T iTime
#define MAXR 16

float gauss(float x, float s) {
    return exp(-(x*x) / (2.*s*s));
}

void mainImage(out vec4 o, in vec2 u) {
    float d = mix(1., step(u.x, mouse.x - viewport.x), preview);

    float sigma = 0.01+strength;
    float m = max(R.x, R.y);
    vec2 px = vec2(1.) / m;

    u = u / m;
    u.y = 1. - u.y;

    vec3 col = texture(tex,u).rgb;

    int r = min(int(ceil(sigma*3.)), MAXR);

    vec3 sum = vec3(0.);
    float wsum = 0.;

    for (int y = -r; y <= r; y++) {
        for (int x = -r; x <= r; x++) {
            float w = gauss(float(x), sigma) * gauss(float(y), sigma);
            sum  += texture(tex, u + vec2(x, y) * px).rgb * w;
            wsum += w;
        }
    }
    vec3 col2 = sum / wsum;

    vec3 final = mix(col,col2,d);
    o = vec4(final,1.);
}

void main() {
    vec2 FC = gl_FragCoord.xy - viewport;
    mainImage(FragColor, FC);
}
)";

static const char* color_adjust = R"(

#version 330 core
out vec4 FragColor;

uniform vec3 iResolution;
uniform vec2 viewport;
uniform float iTime;
uniform float preview;
uniform vec2 mouse;
uniform sampler2D tex;
#define R iResolution

uniform float red;
uniform float green;
uniform float blue;

#define T iTime

void mainImage(out vec4 o, in vec2 u) {
    float d = mix(1., step(u.x, mouse.x - viewport.x), preview);

    u = u / R.xy;
    u.y=1.-u.y;

    vec3 col = texture(tex,u).rgb;
    vec3 col2 = col+vec3(red/255.,green/255.,blue/255.);

    vec3 final = mix(col,col2,d);
    o = vec4(final,1.);
}

void main() {
    vec2 FC = gl_FragCoord.xy - viewport;
    mainImage(FragColor, FC);
}
)";

static const char* matrix = R"(

#version 330 core
out vec4 FragColor;

uniform vec3 iResolution;
uniform vec2 viewport;
uniform float iTime;
uniform float preview;
uniform vec2 mouse;
uniform sampler2D tex;
#define R iResolution

#define T iTime

void mainImage(out vec4 o, in vec2 u) {
    float d = mix(1., step(u.x, mouse.x - viewport.x), preview);

    u = u/R.xy;
    u.y=1.-u.y;

    vec3 col = texture(tex,u).xyz;
    vec3 col2 = pow(col,vec3(7.0/5.,1,8./5.));

    vec3 final = mix(col,col2,d);
    o = vec4(final,1.);
}

void main() {
    vec2 FC=gl_FragCoord.xy-viewport;
    mainImage(FragColor, FC);
}
)";