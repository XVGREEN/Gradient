#include "programs.h"
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <filesystem>

namespace gradient {
    namespace shaders {

        unsigned int default_program;
        unsigned int vignete_program;
        unsigned int gray_program;
        unsigned int chromatic_program;
        unsigned int contrast_program;
        unsigned int radial_program;
        unsigned int gausian_program;
        unsigned int matrix_program;
        unsigned int color_program;
        unsigned int brightness_program;
        unsigned int saturation_program;
        unsigned int invert_program;
        unsigned int sepia_program;
        unsigned int threshold_program;
        unsigned int posterize_program;
        unsigned int duotone_program;
        unsigned int hue_shift_program;
        unsigned int s_curve_program;
        unsigned int film_grain_program;
        unsigned int pixelate_program;
        unsigned int wave_program;
        unsigned int barrel_program;
        unsigned int swirl_program;
        unsigned int mirror_program;
        unsigned int glitch_program;
        unsigned int sharpen_program;
        unsigned int edge_detect_program;
        unsigned int emboss_program;
        unsigned int bloom_program;
        unsigned int halftone_program;
        unsigned int turbulence_program;

        static const std::string SHADER_DIR = "shaders/glsl/";

        std::string load_shader(const std::string& path) {
            std::ifstream file(path);
            if (!file.is_open()) {
                std::cerr << "Cannot open shader: " << path
                    << "\n  CWD: " << std::filesystem::current_path().string() << "\n";
                return "";
            }

            std::stringstream buffer;
            buffer << file.rdbuf();
            return buffer.str();
        }

        
        static unsigned int makeProgram(const std::string& vertexSrc, const std::string& fragFile) {
            std::string fragSrc = load_shader(SHADER_DIR + fragFile);
            if (fragSrc.empty()) {
                std::cerr << "Skipping program, empty or missing: " << fragFile << "\n";
                return 0;
            }
            return createShaderProgram(vertexSrc.c_str(), fragSrc.c_str());
        }

        inline void initPrograms() {   
            std::string vert = load_shader(SHADER_DIR + "default.vert");
            default_program = makeProgram(vert, "default.frag");
            vignete_program = makeProgram(vert, "vignette.frag");
            gray_program = makeProgram(vert, "grayscale.frag");
            chromatic_program = makeProgram(vert, "chromatic.frag");
            contrast_program = makeProgram(vert, "contrast.frag");
            radial_program = makeProgram(vert, "chromatic.frag"); 
            gausian_program = makeProgram(vert, "gaussian.frag");
            matrix_program = makeProgram(vert, "matrix.frag");
            color_program = makeProgram(vert, "color_balance.frag");     
            brightness_program = makeProgram(vert, "brightness.frag");
            saturation_program = makeProgram(vert, "saturation.frag");
            invert_program = makeProgram(vert, "invert.frag");
            sepia_program = makeProgram(vert, "sepia.frag");
            threshold_program = makeProgram(vert, "threshold.frag");
            posterize_program = makeProgram(vert, "posterize.frag");
            duotone_program = makeProgram(vert, "duotone.frag");
            hue_shift_program = makeProgram(vert, "hue_shift.frag");
            s_curve_program = makeProgram(vert, "s_curve.frag");
            film_grain_program = makeProgram(vert, "film_grain.frag");
            pixelate_program = makeProgram(vert, "pixelate.frag");
            wave_program = makeProgram(vert, "wave.frag");
            barrel_program = makeProgram(vert, "barrel.frag");
            swirl_program = makeProgram(vert, "swirl.frag");
            mirror_program = makeProgram(vert, "mirror.frag");
            glitch_program = makeProgram(vert, "glitch.frag");
            sharpen_program = makeProgram(vert, "sharpen.frag");
            edge_detect_program = makeProgram(vert, "edge_detect.frag");
            emboss_program = makeProgram(vert, "emboss.frag");
            bloom_program = makeProgram(vert, "bloom.frag");
            halftone_program = makeProgram(vert, "halftone.frag");
            turbulence_program = makeProgram(vert, "turbulence.frag");

            program = default_program;
        }
    }
}