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

#include "io.hpp"
#include "util.hpp"
#include <cassert>
#include <cfloat>
#include <fstream>
#include <sstream>
using namespace std;
using namespace torch;

// OBJ meshes

void get_valid_line (istream &in, string &line) {
    do
        getline(in, line);
    while (in && (line.length() == 0 || line[0] == '#'));
}

void triangle_to_obj (const string &inname, const string &outname) {
    fstream outfile(outname.c_str(), ios::out);
    { // nodes
        string filename = inname + ".node";
        fstream file(filename.c_str(), ios::in);
        string line;
        get_valid_line(file, line);
        stringstream linestream(line);
        int nv, dim, na, nb;
        linestream >> nv >> dim >> na >> nb;
        for (int i = 0; i < nv; i++) {
            get_valid_line(file, line);
            stringstream linestream(line);
            int index;
            linestream >> index;
            vector<double> u0(2);
            linestream >> u0[0] >> u0[1];
            Tensor u = torch::tensor(u0,TNOPT);
            outfile << "v " << u[0] << " " << u[1] << " " << 0 << endl;
        }
    }
    { // eles
        string filename = inname + ".ele";
        fstream file(filename.c_str(), ios::in);
        string line;
        get_valid_line(file, line);
        stringstream linestream(line);
        int nt, nn, na;
        linestream >> nt >> nn >> na;
        for (int i = 0; i < nt; i++) {
            get_valid_line(file, line);
            stringstream linestream(line);
            int index;
            linestream >> index;
            int v0, v1, v2;
            linestream >> v0 >> v1 >> v2;
            outfile << "f " << v0+1 << " " << v1+1 << " " << v2+1 << endl;
        }
    }
}

vector<Face*> triangulate (const vector<Vert*> &verts);

void load_obj (Mesh &mesh, const string &filename) {
    delete_mesh(mesh);
    fstream file(filename.c_str(), ios::in);
    if(!file) {
        cout << "Error: failed to open file " << filename << endl;
        return;
    }
    while (file) {
        string line;
        get_valid_line(file, line);
        stringstream linestream(line);
        string keyword;
        linestream >> keyword;
        if (keyword == "vt") {
            vector<double> u0(2);
            linestream >> u0[0] >> u0[1];
            Tensor u = torch::tensor(u0,TNOPT);
            mesh.add(new Vert(u));
        } else if (keyword == "vl") {
            linestream >> mesh.verts.back()->label;
        } else if (keyword == "v") {
            vector<double> u0(3);
            linestream >> u0[0] >> u0[1] >> u0[2];
            Tensor x = torch::tensor(u0,TNOPT);
            mesh.add(new Node(x, ZERO3));
        } else if (keyword == "ny") {
            Tensor &y = mesh.nodes.back()->y;
            vector<double> u0(3);
            linestream >> u0[0] >> u0[1] >> u0[2];
            y = torch::tensor(u0,TNOPT);
        } else if (keyword == "nv") {
            Tensor &v = mesh.nodes.back()->v;
            vector<double> u0(3);
            linestream >> u0[0] >> u0[1] >> u0[2];
            v = torch::tensor(u0,TNOPT);
        } else if (keyword == "nl") {
            linestream >> mesh.nodes.back()->label;
        } else if (keyword == "e") {
            int n0, n1;
            linestream >> n0 >> n1;
            mesh.add(new Edge(mesh.nodes[n0-1], mesh.nodes[n1-1]));
        } else if (keyword == "ea") {
            linestream >> mesh.edges.back()->theta_ideal;
        } else if (keyword == "ed") {
            linestream >> mesh.edges.back()->damage;
        } else if (keyword == "el") {
            linestream >> mesh.edges.back()->label;
        } else if (keyword == "f") {
            vector<Vert*> verts;
            vector<Node*> nodes;
            string w;
            while (linestream >> w) {
                stringstream wstream(w);
                int v, n;
                char c;
                wstream >> n >> c >> v;
                nodes.push_back(mesh.nodes[n-1]);
                if (wstream)
                    verts.push_back(mesh.verts[v-1]);
                else if (!nodes.back()->verts.empty())
                    verts.push_back(nodes.back()->verts[0]);
                else {
                    verts.push_back(new Vert(nodes.back()->x.slice(0,0,2),
                                             nodes.back()->label));
                    mesh.add(verts.back());
                }
            }
            for (int v = 0; v < verts.size(); v++)
                connect(verts[v], nodes[v]);
            vector<Face*> faces = triangulate(verts);
            for (int f = 0; f < faces.size(); f++)
                mesh.add(faces[f]);
        } else if (keyword == "tl" || keyword == "fl") {
            linestream >> mesh.faces.back()->label;
        } else if (keyword == "ts" || keyword == "fs") {
            Tensor &S = mesh.faces.back()->S_plastic;
            vector<double> s(4);
            linestream >> s[0] >> s[1] >> s[2] >> s[3];
            S = torch::tensor(s, TNOPT).reshape({2,2});
        } else if (keyword == "td" || keyword == "fd") {
            linestream >> mesh.faces.back()->damage;
        }
    }
    mark_nodes_to_preserve(mesh);
    compute_ms_data(mesh);
}

void load_objs (vector<Mesh*> &meshes, const string &prefix) {
    for (int m = 0; m < meshes.size(); m++)
        load_obj(*meshes[m], stringf("%s_%02d.obj", prefix.c_str(), m));
}

static Tensor angle (const Tensor &x0, const Tensor &x1, const Tensor &x2) {
    Tensor e1 = normalize(x1 - x0);
    Tensor e2 = normalize(x2 - x0);
    return acos(clamp(dot(e1, e2), -1., 1.));
}

vector<Face*> triangulate (const vector<Vert*> &verts) {
    int n = verts.size();
    Tensor best_min_angle = ZERO;
    int best_root = -1;
    for (int i = 0; i < n; i++) {
        Tensor min_angle = infinity;
        const Vert *vert0 = verts[i];
        for (int j = 2; j < n; j++) {
            const Vert *vert1 = verts[(i+j-1)%n], *vert2 = verts[(i+j)%n];
            min_angle=min(min(min_angle,
                          angle(vert0->node->x,vert1->node->x,vert2->node->x)),
                          min(angle(vert1->node->x,vert2->node->x,vert0->node->x),
                          angle(vert2->node->x,vert0->node->x,vert1->node->x)));
        }
        if ((min_angle > best_min_angle).item<int>()) {
            best_min_angle = min_angle;
            best_root = i;
        }
    }
    int i = best_root;
    Vert* vert0 = verts[i];
    vector<Face*> tris;
    for (int j = 2; j < n; j++) {
        Vert *vert1 = verts[(i+j-1)%n], *vert2 = verts[(i+j)%n];
        tris.push_back(new Face(vert0, vert1, vert2));
    }
    return tris;
}

void save_obj (const Mesh &mesh, const string &filename) {
    fstream file(filename.c_str(), ios::out);
    for (int v = 0; v < mesh.verts.size(); v++) {
        const Vert *vert = mesh.verts[v];
        file << "vt " << vert->u[0].item<double>() << " " << vert->u[1].item<double>() << endl;
        if (vert->label)
            file << "vl " << vert->label << endl;
    }
    for (int n = 0; n < mesh.nodes.size(); n++) {
        const Node *node = mesh.nodes[n];
        file << "v " << node->x[0].item<double>() << " " << node->x[1].item<double>() << " "
             << node->x[2].item<double>() << endl;
        if ((node->x != node->y).any().item<int>())
            file << "ny " << node->y[0].item<double>() << " " << node->y[1].item<double>() << " "
                 << node->y[2].item<double>() << endl;
        if ((node->v!=ZERO3).any().item<int>())
            file << "nv " << node->v[0].item<double>() << " " << node->v[1].item<double>() << " "
                 << node->v[2].item<double>() << endl;
        if (node->label)
            file << "nl " << node->label << endl;
    }
    for (int e = 0; e < mesh.edges.size(); e++) {
        const Edge *edge = mesh.edges[e];
        if ((edge->theta_ideal!=0).item<int>() || edge->label) {
            file << "e " << edge->n[0]->index+1 << " " << edge->n[1]->index+1
                 << endl;
            if ((edge->theta_ideal!=0).item<int>())
                file << "ea " << edge->theta_ideal.item<double>() << endl;
            if ((edge->damage!=0).item<int>())
                file << "ed " << edge->damage.item<double>() << endl;
            if (edge->label)
                file << "el " << edge->label << endl;
        }
    }
    for (int f = 0; f < mesh.faces.size(); f++) {
        const Face *face = mesh.faces[f];
        file << "f " << face->v[0]->node->index+1 << "/" << face->v[0]->index+1
             << " " << face->v[1]->node->index+1 << "/" << face->v[1]->index+1
             << " " << face->v[2]->node->index+1 << "/" << face->v[2]->index+1
             << endl;
        if (face->label)
            file << "tl " << face->label << endl;
        if ((face->S_plastic!=torch::zeros({2,2},TNOPT)).any().item<int>()) {
            const Tensor &S = face->S_plastic;
            file << "ts " << S[0][0].item<double>() << " " << S[0][1].item<double>() << " " << S[1][0].item<double>() << " "
                 << S[1][1].item<double>() << endl;
        }
        if ((face->damage!=ZERO).item<int>())
            file << "td " << face->damage.item<double>() << endl;
    }
}

void save_objs (const vector<Mesh*> &meshes, const string &prefix) {
    for (int m = 0; m < meshes.size(); m++)
        save_obj(*meshes[m], stringf("%s_%02d.obj", prefix.c_str(), m));
}

void save_transformation (const Transformation &tr, const string &filename) {
    FILE* file = fopen(filename.c_str(), "w");
    pair<Tensor, Tensor> axis_angle = tr.rotation.to_axisangle();
    Tensor axis = axis_angle.first;
    Tensor angle = axis_angle.second * 180 / M_PI;
    fprintf(file, "<rotate angle=\"%f\" x=\"%f\" y=\"%f\" z=\"%f\"/>\n",
            angle.item<double>(), axis[0].item<double>(), axis[1].item<double>(), axis[2].item<double>());
    fprintf(file, "<scale value=\"%f\"/>\n", tr.scale.item<double>());
    fprintf(file, "<translate x=\"%f\" y=\"%f\" z=\"%f\"/>\n",
            tr.translation[0].item<double>(), tr.translation[1].item<double>(), tr.translation[2].item<double>());
    fclose(file);
}