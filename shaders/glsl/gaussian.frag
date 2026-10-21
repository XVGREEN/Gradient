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

    float gauss(float x, float s) {
        return exp(-(x*x) / (2.*s*s));
    }

    void mainImage(out vec4 o, in vec2 u) {
        float d = mix(1., step(u.x, mouse.x - viewport.x), preview);

        float sigma = 0.01+strength;
        float m = max(R.x, R.y);
        vec2 px = vec2(1.) / m;

        float inside;
        vec2 st = getUV(R.xy, inside);

        vec3 col = texture(tex, st).rgb;

        int r = min(int(ceil(sigma*3.)), MAXR);

        vec3 sum = vec3(0.);
        float wsum = 0.;

        for (int y = -r; y <= r; y++) {
            for (int x = -r; x <= r; x++) {
                float w = gauss(float(x), sigma) * gauss(float(y), sigma);
                sum  += texture(tex, st + vec2(x, y) * px).rgb * w;
                wsum += w;
            }
        }
        vec3 col2 = sum / wsum;

        vec3 final = mix(col, col2, d);
        o = vec4(mix(BG, final, inside), 1.);
    }

    void main() {
        vec2 FC = gl_FragCoord.xy - viewport;
        mainImage(FragColor, FC);
    }