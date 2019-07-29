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

void eraseSubStr(std::string &mainStr, const std::string &toErase)
{
  // Search for the substring in string
  size_t pos = mainStr.find(toErase);

  if (pos != std::string::npos) {
    // If found then erase it from string
    mainStr.erase(pos, toErase.length());
  }
}

namespace ospray {
  namespace app {

    struct ContextCaptureStruct
    {
      void write(void *data, int size)
      {
        unsigned char *d = (unsigned char *)data;
        bytes.insert(bytes.end(), d, d + size);
      }

      std::vector<unsigned char> bytes;
    };

    void jpgWriteHelp(void *context, void *data, int size)
    {
      ContextCaptureStruct *myfoo = (ContextCaptureStruct *)context;
      myfoo->write(data, size);
    }

    class OSPExampleViewer : public OSPApp
    {
      void render(const std::shared_ptr<sg::Frame> &) override;
      int parseCommandLine(int &ac, const char **&av) override;

      std::string fileName{"./aout.jpeg"};
    };

    void OSPExampleViewer::render(const std::shared_ptr<sg::Frame> &root)
    {
      // Establish the Camera
      auto &camera     = root->child("camera");
      camera["pos"]    = vec3f(0.0f, 3.5f, 10.2f);
      camera["up"]     = vec3f(0.0f, 1.0f, 0.0f);
      camera["dir"]    = vec3f(0.0f, 0.0f, -1.0f);
      camera["fovy"]   = 90.0f;
      camera["aspect"] = 1.0f;
      camera.commit();

      // Retrieve the Renderer's Children
      auto &renderer = root->child("renderer");
      auto &lights = renderer.child("lights");
      auto &world    = renderer.child("world");
      // Skip the World's Nodes that are not active actors in the scene
      std::vector<std::string> nodesToSkip{
          "bounds", "compactMode", "dynamicScene", "robustMode"};

      // Find the Active actors and store them separately
      std::map<std::string, std::shared_ptr<sg::Node>> sceneActors;
      for (auto &element : world.children()) {
        if ((std::find(
                nodesToSkip.begin(), nodesToSkip.end(), element.first)) ==
            nodesToSkip.end()) {
          printf("%s\n", element.first.c_str());
          sceneActors.emplace(std::make_pair(element.first, element.second));
        }
      }

      /**
       * at this point the actors have all been found.
       * We cannot change the material here. It is fixed at load time.
       * What we can do with them:
       *    change rotation (vec3f)
       *    change position (vec3f)
       *    change scale    (vec3f)
       * We can also duplicate Nodes. Do this by adding them back to the parent,
       * a variable named world, using a different name
       */
      std::map<std::string, std::shared_ptr<sg::Node>> namedActors;
      for (auto &actor : sceneActors) {
        std::string actorName = actor.second->name();
        eraseSubStr(actorName, "transform_");
        eraseSubStr(actorName, "_0_0_0");
        printf("actor: %s as %s\n", actor.first.c_str(), actorName.c_str());
        namedActors.emplace(std::make_pair(actorName, actor.second));
        actor.second->child("position").setValue(vec3f(-999.0f, -999.0f, -999.0f));
      }

      FILE *input, *info, *error, *output;
      input  = fopen("/dev/stdin", "r");
      info   = fopen("/dev/stdout", "w");
      error  = fopen("/dev/stderr", "w");
      output = fopen("/dev/fd/100", "wb");
      while (true) {
        float camPosX, camPosY, camPosZ, camUpX, camUpY, camUpZ, camVuDirX,
            camVuDirY, camVuDirZ;
        int quality;
        float objOnePosX, objOnePosY, objOnePosZ, objOneSklX, objOneSklY, objOneSklZ;
        int matid1;
        float objTwoPosX, objTwoPosY, objTwoPosZ, objTwoSklX, objTwoSklY, objTwoSklZ;
        int matid2;
        float objThreePosX, objThreePosY, objThreePosZ, objThreeSklX, objThreeSklY, objThreeSklZ;
        int matid3;
        char objid1[256], objid2[256], objid3[256];
        float negxr, negxg, negxb;
        float posxr, posxg, posxb;
        float negyr, negyg, negyb;
        float posyr, posyg, posyb;
        float negzr, negzg, negzb;
        float poszr, poszg, poszb;
        float lightx, lighty, lightz;
        float lightr, lightg, lightb, lightint;

        fprintf(info, "Waiting for request...\n");

        if (fscanf(input,
                   "%f %f %f %f %f %f %f %f %f %d",
                   &camPosX,
                   &camPosY,
                   &camPosZ,
                   &camUpX,
                   &camUpY,
                   &camUpZ,
                   &camVuDirX,
                   &camVuDirY,
                   &camVuDirZ,
                   &quality) != 10) {
          fprintf(error, "Error: bad format\n");
          fprintf(output, "9:error arg,");
          fflush(output);
          continue;
        }

        if (fscanf(input,
                   "%f %f %f %f %f %f %d %s",
                   &objOnePosX,
                   &objOnePosY,
                   &objOnePosZ,
                   &objOneSklX,
                   &objOneSklY,
                   &objOneSklZ,
                   &matid1,
                   &objid1[0]) != 8) {
          fprintf(error, "Error: bad format\n");
          fprintf(output, "9:error arg,");
          fflush(output);
          continue;
        }
        std::string objOneName = objid1;

        if (fscanf(input,
                   "%f %f %f %f %f %f %d %s",
                   &objTwoPosX,
                   &objTwoPosY,
                   &objTwoPosZ,
                   &objTwoSklX,
                   &objTwoSklY,
                   &objTwoSklZ,
                   &matid2,
                   &objid2[0]) != 8) {
          fprintf(error, "Error: bad format\n");
          fprintf(output, "9:error arg,");
          fflush(output);
          continue;
        }
        std::string objTwoName = objid2;

        if (fscanf(input,
                   "%f %f %f %f %f %f %d %s",
                   &objThreePosX,
                   &objThreePosY,
                   &objThreePosZ,
                   &objThreeSklX,
                   &objThreeSklY,
                   &objThreeSklZ,
                   &matid3,
                   &objid3[0]) != 8) {
          fprintf(error, "Error: bad format\n");
          fprintf(output, "9:error arg,");
          fflush(output);
          continue;
        }
        std::string objThreeName = objid3;

        /// IGNORE VALS FROM HERE
        /**
         *
         *
         */
        if (fscanf(input, "%f %f %f", &negxr, &negxg, &negxb) != 3) {
          fprintf(error, "Error: bad format\n");
          fprintf(output, "9:error arg,");
          fflush(output);
          continue;
        }

        if (fscanf(input, "%f %f %f", &negyr, &negyg, &negyb) != 3) {
          fprintf(error, "Error: bad format\n");
          fprintf(output, "9:error arg,");
          fflush(output);
          continue;
        }

        if (fscanf(input, "%f %f %f", &negzr, &negzg, &negzb) != 3) {
          fprintf(error, "Error: bad format\n");
          fprintf(output, "9:error arg,");
          fflush(output);
          continue;
        }

        if (fscanf(input, "%f %f %f", &posxr, &posxg, &posxb) != 3) {
          fprintf(error, "Error: bad format\n");
          fprintf(output, "9:error arg,");
          fflush(output);
          continue;
        }

        if (fscanf(input, "%f %f %f", &posyr, &posyg, &posyb) != 3) {
          fprintf(error, "Error: bad format\n");
          fprintf(output, "9:error arg,");
          fflush(output);
          continue;
        }

        if (fscanf(input, "%f %f %f", &poszr, &poszg, &poszb) != 3) {
          fprintf(error, "Error: bad format\n");
          fprintf(output, "9:error arg,");
          fflush(output);
          continue;
        }
        /// TO HERE FOR NOW
        /*
         *
         *
         */

        if (fscanf(input,
                   "%f %f %f %f %f %f %f",
                   &lightx,
                   &lighty,
                   &lightz,
                   &lightr,
                   &lightg,
                   &lightb,
                   &lightint) != 7) {
          fprintf(error, "Error: bad format\n");
          fprintf(output, "9:error arg,");
          fflush(output);
          continue;
        }

        fprintf(info, "Got request\n");

        /**
         * Here we need to make sure the specified objects are inside
         * of the mapped actors. If the object is not, then we skip this request.
         */
        auto objByName = namedActors.find(objOneName);
        if (objByName == namedActors.end()) {
          fprintf(error, "Error: bad format, actor %s not in actor map.\n", objOneName.c_str());
          fflush(output);
          continue;
        }
        // This is the Node linked to the specified object name.
        auto& specObjectOne = objByName->second;
        specObjectOne->child("position").setValue(vec3f(objOnePosX, objOnePosY, objOnePosZ));

        objByName = namedActors.find(objTwoName);
        if (objByName == namedActors.end()) {
          fprintf(error, "Error: bad format, actor %s not in actor map.\n", objTwoName.c_str());
          fflush(output);
          continue;
        }
        auto& specObjectTwo = objByName->second;
        specObjectTwo->child("position").setValue(vec3f(objTwoPosX, objTwoPosY, objTwoPosZ));

        objByName = namedActors.find(objThreeName);
        if (objByName == namedActors.end()) {
          fprintf(error, "Error: bad format, actor %s not in actor map.\n", objThreeName.c_str());
          fflush(output);
          continue;
        }
        auto& specObjectThree = objByName->second;
        specObjectThree->child("position").setValue(vec3f(objThreePosX, objThreePosY, objThreePosZ));

        camera["pos"]    = vec3f(camPosX, camPosY, camPosZ);
        camera["up"]     = vec3f(camUpX, camUpY, camUpZ);
        camera["dir"]    = vec3f(camVuDirX, camVuDirY, camVuDirZ);
        camera["fovy"]   = 90.0f;
        camera["aspect"] = 1.0f;
        camera.commit();

	sg::Node &light = lights.createChild("mylight", "PointLight");
	light["color"] = vec3f(lightr, lightg, lightb);
	light["position"] = vec3f(lightx, lighty, lightz);
	light["intensity"] = lightint;
	light["radius"] = 0.1f;

        // Render a single Frame
        std::shared_ptr<sg::FrameBuffer> fb =
            std::make_shared<sg::FrameBuffer>(vec2i(512, 512));
        root->setChild("frameBuffer", fb);
        root->setChild("navFrameBuffer", fb);
        renderer["spp"]                            = 20;
        std::shared_ptr<sg::FrameBuffer> fbCapture = root->renderFrame(true);
        auto fbSize                                = fbCapture->size();
        auto fbColour = (vec4uc *)fbCapture->map(OSP_FB_COLOR);

        // Capture the rendered frame to a buffer
        std::vector<unsigned char> rgbAsUnsChars;
        int index = 0;
        for (int row = fbSize.y - 1; row >= 0; --row) {
          for (int col = 0; col < fbSize.x; ++col) {
            index = row * fbSize.x + col;
            rgbAsUnsChars.push_back((unsigned char)(fbColour[index].x));
            rgbAsUnsChars.push_back((unsigned char)(fbColour[index].y));
            rgbAsUnsChars.push_back((unsigned char)(fbColour[index].z));
          }
        }

        ContextCaptureStruct captureStruct;
        int ret = stbi_write_jpg_to_func(jpgWriteHelp,
                                         (void *)&captureStruct,
                                         fbSize.x,
                                         fbSize.y,
                                         3,
                                         &rgbAsUnsChars[0],
                                         100);
        if (!ret) {
          printf("bad\n");
          break;
        }

        fprintf(output, "%lu:", captureStruct.bytes.size());
        fwrite(
            captureStruct.bytes.data(), 1, captureStruct.bytes.size(), output);
        fprintf(output, ",");
        fflush(output);
        fbCapture->clear();

        // Reset the Spec Objects positions.
        specObjectOne->child("position").setValue(vec3f(-990.0f, -999.0f, -999.0f));
        specObjectTwo->child("position").setValue(vec3f(-990.0f, -999.0f, -999.0f));
        specObjectThree->child("position").setValue(vec3f(-990.0f, -999.0f, -999.0f));

      }
      fclose(output);
    }  // namespace app

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

  }  // namespace app
}  // namespace ospray

int main(int ac, const char **av)
{
  ospray::app::OSPExampleViewer ospApp;
  return ospApp.main(ac, av);
}
