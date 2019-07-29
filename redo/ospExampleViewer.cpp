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
#include "widgets/imguiViewer.h"

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

    class OSPExampleViewer : public OSPApp
    {
      void render(const std::shared_ptr<sg::Frame> &) override;
      int parseCommandLine(int &ac, const char **&av) override;

      std::string fileName {"./aout.jpeg"};
    };

    void OSPExampleViewer::render(const std::shared_ptr<sg::Frame> &root)
    {
      auto &camera = root->child("camera");
      camera["pos"] = vec3f(0.0f, 3.5f, 10.2f);
      camera["up"] = vec3f(0.0f, 1.0f, 0.0f);
      camera["dir"] = vec3f(0.0f, 0.0f, -1.0f);
      camera["fovy"] = 90.0f;
      camera["aspect"] = 1.0f;
      camera.commit();

      auto &renderer = root->child("renderer");
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

      FILE *f = fdopen(100, "w");

      Foo myfoo;
      int ret = stbi_write_jpg_to_func(write, (void*)&myfoo, fbSize.x, fbSize.y, 3, &rgbAsUnsChars[0], 100);
      if (!ret) {
        printf("bad\n");
      }

      fprintf(f, "%lu:", myfoo.bytes.size());
      fwrite(myfoo.bytes.data(), 1, myfoo.bytes.size(), f);
      fflush(f);
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
