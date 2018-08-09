#include <cassert>
#include <cstdio>
#include "ExportObj.h"

ExportObj::ExportObj(const std::string& path)
{
  auto err = fopen_s(&out, path.c_str(), "w");
  assert(err == 0);

}

ExportObj::~ExportObj()
{
  if (out) {
    fclose(out);
  }
}


void ExportObj::beginFile(const std::string info, const std::string& note, const std::string& date, const std::string& user, const std::string& encoding)
{
  fprintf(out, "# %s\n", info.c_str());
  fprintf(out, "# %s\n", note.c_str());
  fprintf(out, "# %s\n", date.c_str());
  fprintf(out, "# %s\n", user.c_str());
}

void ExportObj::endFile() {
  fprintf(out, "# End of file\n");
}

void ExportObj::beginModel(const std::string& project, const std::string& name)
{
  fprintf(out, "# Model project=%s, name=%s\n", project.c_str(), name.c_str());
}

void ExportObj::endModel() { }

void ExportObj::beginGroup(const std::string& name, const float* translation, const uint32_t material)
{
  for (unsigned i = 0; i < 3; i++) curr_translation[i] = translation[i];

  fprintf(out, "o %s\n", name.c_str());

}

void ExportObj::EndGroup() { }

void ExportObj::triangles(float* M, float* bbox, std::vector<float>& P, std::vector<float>& N, std::vector<uint32_t>& indices)
{

  assert(P.size() == N.size());
  fprintf(out, "g\n");
  for (size_t i = 0; i < P.size(); i += 3) {
    auto px = P[i + 0];
    auto py = P[i + 1];
    auto pz = P[i + 2];
    auto nx = N[i + 0];
    auto ny = N[i + 1];
    auto nz = N[i + 2];

    float Px, Py, Pz, Nx, Ny, Nz;
    Px = M[0] * px + M[3] * py + M[6] * pz + M[9] + curr_translation[0];
    Py = M[1] * px + M[4] * py + M[7] * pz + M[10] + curr_translation[1];
    Pz = M[2] * px + M[5] * py + M[8] * pz + M[11] + curr_translation[2];
    Nx = M[0] * nx + M[3] * ny + M[6] * nz;
    Ny = M[1] * nx + M[4] * ny + M[7] * nz;
    Nz = M[2] * nx + M[5] * ny + M[8] * nz;

    fprintf(out, "v %f %f %f\n", Px, Py, Pz);
    fprintf(out, "vn %f %f %f\n", Nx, Ny, Nz);

    //fprintf(out, "v %f %f %f\n", px, py, pz);
    //fprintf(out, "vn %f %f %f\n", nx, ny, nz);

  }
  for (size_t i = 0; i < indices.size(); i += 3) {
    fprintf(out, "f %zd//%zd %zd//%zd %zd//%zd\n",
            indices[i + 0] + off, indices[i + 0] + off,
            indices[i + 1] + off, indices[i + 1] + off,
            indices[i + 2] + off, indices[i + 2] + off);
  }
  off += P.size() / 3;
}