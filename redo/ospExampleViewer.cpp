// ======================================================================== //
// Copyright 2009-2019 Intel Corporation                                    //
//                                                                          //
// Licensed under the Apache License, Version 2.0 (the "License");          //
// you may not use this file except in compliance with the License.         //
// You may obtain a copy of the License at                                  //
//                                                                          //
//     http://www.apache.org/licenses/LICENSE-2.0                           //
//                                                                          //
// Unless required by applicable law or agreed to in writing, software      //
// distributed under the License is distributed on an "AS IS" BASIS,        //
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. //
// See the License for the specific language governing permissions and      //
// limitations under the License.                                           //
// ======================================================================== //

#include "sg/SceneGraph.h"
#include "sg/geometry/TriangleMesh.h"

#include "ospapp/OSPApp.h"
//#include "widgets/imguiViewer.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "sg/3rdParty/stb_image_write.h"

#include "stdlib.h"

namespace ospray {
  namespace app {

    struct Foo {
      void write(void *data, int size) {
        unsigned char *d = (unsigned char *)data;
        bytes.insert(bytes.end(), d, d + size);
      }

      std::vector<unsigned char> bytes;
    };

    void write(void *context, void *data, int size) {
      Foo *myfoo = (Foo *)context;
      myfoo->write(data, size);
    }

    OSPGeometry
    makeOBJGeometry(const char *filename) {
      OSPGeometry geometry;
      OSPData vertex_data, index_data;
      int nvertex, nindex;
      float *vertices;
      int *indices;
      FILE *f;
      char c;
      size_t sz;
      int has_vertex, has_index, has_extra1, has_extra2;
      
      f = fopen(filename, "rb");
      
      (void)fread(&c, sizeof(c), 1, f);
      has_vertex = c == 'V';
      
      (void)fread(&c, sizeof(c), 1, f);
      has_index = c == 'I';
      
      (void)fread(&c, sizeof(c), 1, f);
      has_extra1 = c == '?';
      
      (void)fread(&c, sizeof(c), 1, f);
      has_extra2 = c == '?';
    
      if (!has_vertex) goto error;
      if (!has_index) goto error;
      if (has_extra1) goto error;
      if (has_extra2) goto error;
      
      vertices = NULL;
      if (has_vertex) {
        (void)fread(&nvertex, sizeof(nvertex), 1, f);
        sz = (size_t)nvertex * 3;
        vertices = (float *)malloc(sz * sizeof(*vertices));
        (void)fread(vertices, sizeof(*vertices), sz, f);
        vertex_data = ospNewData(nvertex, OSP_FLOAT3, vertices, OSP_DATA_SHARED_BUFFER);
        ospCommit(vertex_data);
      }
      
      indices = NULL;
      if (has_index) {
        (void)fread(&nindex, sizeof(nindex), 1, f);
        sz = (size_t)nindex * 3;
        indices = (int *)malloc(sz * sizeof(*indices));
        (void)fread(indices, sizeof(*indices), sz, f);
        index_data = ospNewData(nindex, OSP_INT3, indices, OSP_DATA_SHARED_BUFFER);
        ospCommit(index_data);
      }
      
      geometry = ospNewGeometry("triangles");
      if (has_vertex) {
        ospSetData(geometry, "vertex", vertex_data);
        ospRelease(vertex_data);
      }
      if (has_index) {
        ospSetData(geometry, "index", index_data);
        ospRelease(index_data);
      }
      ospCommit(geometry);
      
    error:
      fclose(f);
      
      return geometry;
    }
    
    
    OSPMaterial *
    makePreloadedMaterials(void) {
      OSPMaterial *materials, material;
      int nmaterial;
      char *s, name[32];
      int i;
      float Kdr, Kdg, Kdb, d, Ksr, Ksg, Ksb, Ns;
      
      s = getenv("nmat");
      if (s == NULL) {
        nmaterial = 0;
      } else if (sscanf(s, "%d", &nmaterial) != 1) {
        return NULL;
      }
      
      materials = (OSPMaterial *)malloc(nmaterial * sizeof(*materials));
      
      for (i=0; i<nmaterial; ++i) {
        snprintf(name, sizeof(name), "mat_%d", i);
        s = getenv(name);
        if (sscanf(s, "%*s %f %f %f %f %f %f %f %f", &Kdr, &Kdg, &Kdb, &d, &Ksr, &Ksg, &Ksb, &Ns) != 8) {
          return NULL;
        }
        
        material = ospNewMaterial2("pathtracer", "OBJMaterial");
        ospSet3f(material, "Kd", Kdr, Kdg, Kdb);
        ospSet1f(material, "d", d);
        ospSet3f(material, "Ks", Ksr, Ksg, Ksb);
        ospSet1f(material, "Ns", Ns);
        ospCommit(material);
        
        materials[i] = material;
      }
      
      return materials;
    }
    
    
    OSPGeometry *
    makePreloadedGeometries(OSPModel **out_models) {
      OSPGeometry *geometries, geometry;
      OSPModel *models, model;
      int i, nobj;
      char *s, name[32];
      
      s = getenv("nobj");
      if (s == NULL) {
        nobj = 0;
      } else if (sscanf(s, "%d", &nobj) != 1) {
        return NULL;
      }
      
      geometries = (OSPGeometry *)malloc(nobj * sizeof(*geometries));
      models = (OSPModel *)malloc(nobj * sizeof(*models));
      
      for (i=0; i<nobj; ++i) {
        snprintf(name, sizeof(name), "obj_%d", i);
        s = getenv(name);
        
        // We get a string like "Name Path" and we want the
        // path to load the object
        s = strrchr(s, ' ') + 1;
        
        geometry = makeOBJGeometry(s);
        
        model = ospNewModel();
        ospAddGeometry(model, geometry);
        ospCommit(model);
        
        geometries[i] = geometry;
        models[i] = model;
      }
      
      *out_models = models;
      return geometries;
    }

    class OSPExampleViewer : public OSPApp
    {
      void render(const std::shared_ptr<sg::Frame> &) override;
      int parseCommandLine(int &ac, const char **&av) override;

      std::string fileName {"./aout.jpeg"};
    };

    void OSPExampleViewer::render(const std::shared_ptr<sg::Frame> &root)
    {
      FILE *f = fdopen(100, "w");
      sg::Node &camera = root->child("camera");
      sg::Node &renderer = root->child("renderer");
      sg::Node &model = renderer.child("world");
      for (;;) {

        camera["pos"] = vec3f(0.0f, 3.5f, 10.2f);
        camera["up"] = vec3f(0.0f, 1.0f, 0.0f);
        camera["dir"] = vec3f(0.0f, 0.0f, -1.0f);
        camera["fovy"] = 90.0f;
        camera["aspect"] = 1.0f;
        camera.commit();
  
        std::shared_ptr<sg::FrameBuffer> fb = std::make_shared<sg::FrameBuffer>(vec2i(512, 512));
        root->setChild("frameBuffer", fb);
        root->setChild("navFrameBuffer", fb);
  
        renderer["spp"] = 1;
        std::shared_ptr<sg::FrameBuffer> fbCapture = root->renderFrame(true);
        auto fbSize = fbCapture->size();
  
        auto fbColour = (vec4uc*)fbCapture->map(OSP_FB_COLOR);
  
  
        std::vector<unsigned char> rgbAsUnsChars;
        int index = 0;
        for(int row = fbSize.y - 1; row >= 0; --row){
          for(int col = 0; col < fbSize.x; ++col) {
            index = row * fbSize.x + col;
            rgbAsUnsChars.push_back((unsigned char)(fbColour[index].x));
            rgbAsUnsChars.push_back((unsigned char)(fbColour[index].y));
            rgbAsUnsChars.push_back((unsigned char)(fbColour[index].z));
          }
        }
  
  
        Foo myfoo;
        int ret = stbi_write_jpg_to_func(write, (void*)&myfoo, fbSize.x, fbSize.y, 3, &rgbAsUnsChars[0], 100);
        if (!ret) {
          printf("bad\n");
        }
  
        fprintf(f, "%lu:", myfoo.bytes.size());
        fwrite(myfoo.bytes.data(), 1, myfoo.bytes.size(), f);
        fprintf(f, ",");
        fflush(f);
      }
      fclose(f);
    }

    int OSPExampleViewer::parseCommandLine(int &ac, const char **&av)
    {
      for (int i = 1; i < ac; i++) {
        const std::string arg = av[i];
        if (arg == "--outputName") {
          fileName = av[i + 1];
          removeArgs(ac, av, i, 2);
          --i;
        }
      }
      return 0;
    }

  } // ::ospray::app
} // ::ospray

int main(int ac, const char **av)
{
  ospray::app::OSPExampleViewer ospApp;
  return ospApp.main(ac, av);
}
