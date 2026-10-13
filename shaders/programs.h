#pragma once
#include "programs.h"
#include "shaders.h"       
#include "../utils/utils.h"

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
        

        void initPrograms();
    }
     
}