#pragma once
#include "../core/core.h"

namespace gradient {
    namespace shaders {
        extern unsigned int default_program;
        extern unsigned int vignete_program;
        extern unsigned int gray_program;
        extern unsigned int chromatic_program;
        extern unsigned int contrast_program;
        extern unsigned int radial_program;
        extern unsigned int gausian_program;
        extern unsigned int matrix_program;
        extern unsigned int color_program;

        // extra filters
        extern unsigned int brightness_program;
        extern unsigned int saturation_program;
        extern unsigned int invert_program;
        extern unsigned int sepia_program;
        extern unsigned int threshold_program;
        extern unsigned int posterize_program;
        extern unsigned int duotone_program;
        extern unsigned int hue_shift_program;
        extern unsigned int s_curve_program;
        extern unsigned int film_grain_program;
        extern unsigned int pixelate_program;
        extern unsigned int wave_program;
        extern unsigned int barrel_program;
        extern unsigned int swirl_program;
        extern unsigned int mirror_program;
        extern unsigned int glitch_program;
        extern unsigned int sharpen_program;
        extern unsigned int edge_detect_program;
        extern unsigned int emboss_program;
        extern unsigned int bloom_program;
        extern unsigned int halftone_program;
        extern unsigned int turbulence_program;

        extern void initPrograms();
    }
}