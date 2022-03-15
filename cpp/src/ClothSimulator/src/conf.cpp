/*
  Copyright ©2013 The Regents of the University of California
  (Regents). All Rights Reserved. Permission to use, copy, modify, and
  distribute this software and its documentation for educational,
  research, and not-for-profit purposes, without fee and without a
  signed licensing agreement, is hereby granted, provided that the
  above copyright notice, this paragraph and the following two
  paragraphs appear in all copies, modifications, and
  distributions. Contact The Office of Technology Licensing, UC
  Berkeley, 2150 Shattuck Avenue, Suite 510, Berkeley, CA 94720-1620,
  (510) 643-7201, for commercial licensing opportunities.

  IN NO EVENT SHALL REGENTS BE LIABLE TO ANY PARTY FOR DIRECT,
  INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING
  LOST PROFITS, ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS
  DOCUMENTATION, EVEN IF REGENTS HAS BEEN ADVISED OF THE POSSIBILITY
  OF SUCH DAMAGE.

  REGENTS SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
  FOR A PARTICULAR PURPOSE. THE SOFTWARE AND ACCOMPANYING
  DOCUMENTATION, IF ANY, PROVIDED HEREUNDER IS PROVIDED "AS
  IS". REGENTS HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT,
  UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
*/

#include "conf.hpp"

#include "io.hpp"
#include "magic.hpp"
#include "mot_parser.hpp"
#include "util.hpp"
#include <cassert>
#include <cfloat>
#include <fstream>
#include "sstream"
using namespace std;
using namespace torch;


/*



*/


void load_json (const string &configFilename, Simulation &sim) {
//    // Gather general data
//    if (!json["frame_time"].empty()) {
//        parse_tn<1>(sim.frame_time, json["frame_time"]);
//        parse(sim.frame_steps, json["frame_steps"], 1);
//        sim.step_time = sim.frame_time/sim.frame_steps;
//        parse_tn<1>(sim.end_time, json["end_time"], infinity);
//        parse(sim.end_frame, json["end_frame"], 2147483647);
//    } else if (!json["timestep"].empty()) {
//        parse_tn<1>(sim.step_time, json["timestep"]);
//        parse(sim.frame_steps, json["save_frames"], 1);
//        sim.frame_time = sim.step_time*sim.frame_steps;
//        parse_tn<1>(sim.end_time, json["duration"], infinity);
//        sim.end_frame = 2147483647;
//    }
//    sim.time = ZERO;
//    parse(sim.cloths, json["cloths"]);
//    parse_motions(sim.motions, json["motions"]);
//    parse_handles(sim.handles, json["handles"], sim.cloths, sim.motions);
//    parse_obstacles(sim.obstacles, json["obstacles"], sim.motions);
//    parse_morphs(sim.morphs, json["morphs"], sim.cloths);
//    parse_tn<3>(sim.gravity, json["gravity"], ZERO3);
//    parse(sim.wind, json["wind"]);
//    parse_tn<1>(sim.friction, json["friction"], 0.6*ONE);
//    parse_tn<1>(sim.obs_friction, json["obs_friction"], 0.3*ONE);
//    string module_names[] = {"proximity", "physics", "strainlimiting",
//                             "collision", "remeshing", "separation",
//                             "popfilter", "plasticity"};
//    for (int i = 0; i < Simulation::nModules; i++) {
//        sim.enabled[i] = true;
//        for (int j = 0; j < json["disable"].size(); j++)
//            if (json["disable"][j] == module_names[i])
//                sim.enabled[i] = false;
//    }
//    parse(::magic, json["magic"]);
//    // disable strain limiting and plasticity if not needed
//    bool has_strain_limits = false, has_plasticity = false;
//    for (int c = 0; c < sim.cloths.size(); c++)
//        for (int m = 0; m < sim.cloths[c].materials.size(); m++) {
//            Cloth::Material *mat = sim.cloths[c].materials[m];
//            if (finite(mat->strain_min.item<double>()) || finite(mat->strain_max.item<double>()))
//                has_strain_limits = true;
//            if (finite(mat->yield_curv.item<double>()))
//                has_plasticity = true;
//        }
//    if (!has_strain_limits)
//        sim.enabled[Simulation::StrainLimiting] = false;
//    if (!has_plasticity)
//        sim.enabled[Simulation::Plasticity] = false;
}
