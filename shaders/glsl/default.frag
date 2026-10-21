#version 330 core
    out vec4 FragColor;
    uniform vec3 iResolution;
    uniform vec2 viewport;
    uniform float iTime;
    uniform vec2 mouse;
    uniform sampler2D tex;
    #define R iResolution
    #define T iTime

    in vec2 TexCoord;

   
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
   

    void mainImage(out vec4 o, in vec2 u) {
        float inside;
        vec2 st = getUV(R.xy, inside);

        vec3 col = texture(tex, st).rgb;

        o = vec4(mix(BG, col, inside), 1.);
    }

    void main() {
        vec2 FC = gl_FragCoord.xy - viewport;
        mainImage(FragColor, FC);
}