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
     
    u = u/max(R.x,R.y);
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
uniform vec2 mouse;
uniform sampler2D tex;
#define R iResolution

#define T iTime


void mainImage(out vec4 o, in vec2 u) {
    vec2 uv = (u*2.-iResolution.xy)/max(R.x,R.y);
     
    u = u/max(R.x,R.y);
    u.y=1.-u.y;
    vec3 col = texture(tex,u).xyz;
    col*=(1.-length(uv));
    o = vec4(col,1.);
}

void main() {
    vec2 FC=gl_FragCoord.xy-viewport;
    mainImage(FragColor, FC);
}
)";


static const char* contrast= R"(

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
     
    u = u/max(R.x,R.y);
    u.y=1.-u.y;
    vec3 col = texture(tex,u).xyz;
    col=pow(col,vec3(1.3));
    o = vec4(col,1.);
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
uniform vec2 mouse;
uniform sampler2D tex;
#define R iResolution

#define T iTime


void mainImage(out vec4 o, in vec2 u) {
    vec2 uv = (u*2.-iResolution.xy)/max(R.x,R.y);
     
    u = u/max(R.x,R.y);
    u.y=1.-u.y;
    vec3 col = texture(tex,u).xyz;
    col=vec3(dot(col,vec3(.299,.587,.114)));
    o = vec4(col,1.);
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
uniform vec2 mouse;
uniform sampler2D tex;
#define R iResolution
uniform float abb=.01;

#define T iTime


void mainImage(out vec4 o, in vec2 u) {
    vec2 uv = (u*2.-iResolution.xy)/max(R.x,R.y);
     
    u = u/max(R.x,R.y);   
    u.y=1.-u.y;
    float r = texture(tex,u+vec2(0.,abb)).r;
    float g = texture(tex,u+vec2(abb,0.)).g; 
    float b = texture(tex,u+vec2(0.,-abb)).b;      
    
    vec3 col = vec3(r,g,b);
  
    o = vec4(col,1.);
}

void main() {
    vec2 FC=gl_FragCoord.xy-viewport;
    mainImage(FragColor, FC);
}
)";





