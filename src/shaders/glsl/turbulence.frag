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

in vec2 TexCoord;

// ---- STANDARD UV ----
uniform float rotation;
const float UV_ZOOM = 2.;
const vec3 BG = vec3(.1);

vec2 getUV(vec2 res, out float inside) {
    float aspect = res.x / res.y;
    vec2 st = TexCoord - 0.5;
    st.x *= aspect;
    float c = cos(rotation), s = sin(rotation);
    st = mat2(c, s, -s, c) * st;
    st.x /= aspect;
    st *= UV_ZOOM;
    st += 0.5;
    st.y = 1. - st.y;
    inside = step(0., st.x) * step(st.x, 1.) * step(0., st.y) * step(st.y, 1.);
    return st;
}
// ---------------------

//Number of turbulence waves
#define TURB_NUM 10.0
//Turbulence wave amplitude (scaled by strength)
#define TURB_AMP 0.7
//Turbulence wave speed
#define TURB_SPEED 0.3
//Turbulence frequency
#define TURB_FREQ 2.0
//Turbulence frequency multiplier
#define TURB_EXP 1.4

void mainImage(out vec4 o, in vec2 u) {
    float d = mix(1., step(u.x, mouse.x - viewport.x), preview);

    float inside;
    vec2 st = getUV(R.xy, inside);
    float aspect = R.x / R.y;

    vec3 col = texture(tex, st).rgb;

    // centered, aspect-corrected coordinate in roughly -1..1
    vec2 pos = (st - .5) * 2. * vec2(aspect, 1.);
    vec2 start = pos;

    //Turbulence starting scale
    float freq = TURB_FREQ;

    //Turbulence rotation matrix
    mat2 rot = mat2(0.6, -0.8, 0.8, 0.6);

    //Loop through turbulence octaves
    for (float i = 0.0; i < TURB_NUM; i++)
    {
        //Scroll along the rotated y coordinate
        float phase = freq * (pos * rot).y + TURB_SPEED * iTime + i;
        //Add a perpendicular sine wave offset
        pos += TURB_AMP * strength * rot[0] * sin(phase) / freq;

        //Rotate for the next octave
        rot *= mat2(0.6, -0.8, 0.8, 0.6);
        //Scale down for the next octave
        freq *= TURB_EXP;
    }

    // displacement back into texture space
    vec2 st2 = st + (pos - start) / (2. * vec2(aspect, 1.));

    // mirror at the edges so the warp never samples outside the image
    st2 = 1. - abs(1. - mod(st2, 2.));

    vec3 col2 = texture(tex, st2).rgb;
    col2 = clamp(col2, 0., 1.);

    vec3 final = mix(col, col2, d);
    o = vec4(mix(BG, final, inside), 1.);
}

void main() {
    vec2 FC = gl_FragCoord.xy - viewport;
    mainImage(FragColor, FC);
}