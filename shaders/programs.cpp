#include "programs.h"

namespace gradient {
    unsigned int default_program;
    unsigned int vignete_program;
    unsigned int gray_program;
    unsigned int chromatic_program;
    unsigned int contrast_program;
    unsigned int radial_program;
    unsigned int gausian_program;
    unsigned int matrix_program;
    unsigned int color_program;
    
    


    void initPrograms() {
        default_program = createShaderProgram(default_vertex_src, default_fragment_src);
        vignete_program = createShaderProgram(default_vertex_src, vignete);
        gray_program = createShaderProgram(default_vertex_src, grayscale);
        chromatic_program = createShaderProgram(default_vertex_src, chromatic);
        contrast_program = createShaderProgram(default_vertex_src, contrast);
        radial_program = createShaderProgram(default_vertex_src, radial_chromatic);
        gausian_program = createShaderProgram(default_vertex_src, gaussian_blur);
        matrix_program = createShaderProgram(default_vertex_src, matrix);
        color_program = createShaderProgram(default_vertex_src, color_adjust);
        program = default_program;
        
    }
}