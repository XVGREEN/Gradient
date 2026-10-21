
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

    void mainImage(out vec4 o, in vec2 u) {
        float d = mix(1., step(u.x, mouse.x - viewport.x), preview);

        float inside;
        vec2 st = getUV(R.xy, inside);

        vec3 col = texture(tex, st).xyz;

        float abb = .015*strength;
        float r = texture(tex, st + vec2(0., abb)).r;
        float g = texture(tex, st + vec2(abb, 0.)).g;
        float b = texture(tex, st + vec2(0., -abb)).b;
        vec3 col2 = vec3(r, g, b);

        vec3 final = mix(col, col2, d);
        o = vec4(mix(BG, final, inside), 1.);
    }

    void main() {
        vec2 FC = gl_FragCoord.xy - viewport;
        mainImage(FragColor, FC);
    }