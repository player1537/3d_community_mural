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

    class OSPExampleViewer : public OSPApp
    {
      void render(const std::shared_ptr<sg::Frame> &) override;
      int parseCommandLine(int &ac, const char **&av) override;
      int tapestryLoop(OSPRenderer, OSPCamera);

      std::string fileName {"./aout.jpeg"};
    };

    int OSPExampleViewer::tapestryLoop(OSPRenderer renderer, OSPCamera camera)
    {
      FILE *input, *info, *error, *output;
      OSPFrameBuffer frameBuffer;
      OSPModel boxModel;
      OSPLight light_values[2];
      OSPData lights;
      OSPGeometry posy, negx, posz, posx, negy, negz;
      OSPMaterial *materials1, *materials2, *materials3;
      OSPGeometry *geometries1, *geometries2, *geometries3;
      OSPModel *models1, *models2, *models3;
      osp::vec2i size;
      const void *pixels;
      
      input = fopen("/dev/stdin", "r");
      info = fopen("/dev/stdout", "w");
      error = fopen("/dev/stderr", "w");
      output = fopen("/dev/fd/100", "wb");
    
      /**
      
      fprintf(info, "Creating Box\n");
      posy = makePosYGeometry(1.0, 1.0, 1.0);
      negx = makeNegXGeometry(1.0, 1.0, 1.0);
      posz = makePosZGeometry(1.0, 1.0, 1.0);
      posx = makePosXGeometry(1.0, 1.0, 1.0);
      negy = makeNegYGeometry(1.0, 1.0, 1.0);
      negz = makeNegZGeometry(1.0, 1.0, 1.0);
      
      fprintf(info, "Creating Preloaded Materials\n");
      materials1 = makePreloadedMaterials();
      if (materials1 == NULL) {
        fprintf(error, "Error: failed to load materials\n");
        return 1;
      }
      materials2 = makePreloadedMaterials();
      materials3 = makePreloadedMaterials();
      
      fprintf(info, "Creating Preloaded Geometries\n");
      geometries1 = makePreloadedGeometries(&models1);
      if (geometries1 == NULL) {
        fprintf(error, "Error: failed to load geometries\n");
        return 1;
      }
      geometries2 = makePreloadedGeometries(&models2);
      geometries3 = makePreloadedGeometries(&models3);
      
      fprintf(info, "Creating Box Model\n");
      boxModel = ospNewModel();
    
      **/
    
      /**
    
      fprintf(info, "Creating Point Light\n");
      light_values[0] = ospNewLight3("sphere");
    
      fprintf(info, "Creating Ambient Light\n");
      light_values[1] = ospNewLight3("ambient");
      
      fprintf(info, "Creating Light Dataset\n");
      lights = ospNewData(2, OSP_LIGHT, light_values, 0);
      
      fprintf(info, "Initializing Box Model\n");
      ospAddGeometry(boxModel, posy);
      ospAddGeometry(boxModel, negx);
      ospAddGeometry(boxModel, posz);
      ospAddGeometry(boxModel, posx);
      ospAddGeometry(boxModel, negy);
      ospCommit(boxModel);
    
      **/
      
      fprintf(info, "Initializing Camera\n");
      ospSet3f(camera, "pos", 0.0, 0.0, 0.1);
      ospSet3f(camera, "up", 0.0, 1.0, 0.0);
      ospSet3f(camera, "dir", 0.0, 0.0, 1.0);
      ospSet2f(camera, "imageStart", 0.0, 0.0);
      ospSet2f(camera, "imageEnd", 1.0, 1.0);
      ospSet1f(camera, "height", 1.0);
      ospSet1f(camera, "fovy", 60);
      ospSet1f(camera, "aspect", 1.0);
      ospCommit(camera);
    
      /**
      
      fprintf(info, "Initializing Point Light\n");
      ospSet3f(light_values[0], "position", 0.0, 0.9, 0.5);
      ospSet1f(light_values[0], "radius", 0.0);
      ospSet1f(light_values[0], "intensity", 1.0);
      ospCommit(light_values[0]);
      
      fprintf(info, "Initializing Ambient Light\n");
      ospCommit(light_values[1]);
      
      fprintf(info, "Initializing Light Dataset\n");
      ospCommit(lights);
      
      fprintf(info, "Initializing Renderer\n");
      // skipping model; will set later
      ospSetObject(renderer, "camera", camera);
      ospSetData(renderer, "lights", lights);
      ospSet1i(renderer, "spp", 120);
      ospSet1i(renderer, "maxDepth", 20);
      ospSet1i(renderer, "rouletteDepth", 20);
      ospCommit(renderer);
    
      **/
      
      fprintf(info, "Entering render loop\n");
      for (;;) {
        OSPModel model;
        OSPGeometry boxTrans;
        affine3f boxTransform;
        OSPModel obj1Model, obj2Model, obj3Model;
        OSPGeometry obj1Trans, obj2Trans, obj3Trans;
        OSPGeometry obj1Geometry, obj2Geometry, obj3Geometry;
        OSPMaterial obj1Material, obj2Material, obj3Material;
        affine3f obj1Transform, obj2Transform, obj3Transform;
        OSPMaterial negxMaterial, posxMaterial;
        OSPMaterial negyMaterial, posyMaterial;
        OSPMaterial negzMaterial, poszMaterial;
        float px, py, pz, ux, uy, uz, vx, vy, vz;
        int quality;
        float bx1, by1, bz1, bsx1, bsy1, bsz1;
        int matid1, objid1;
        float bx2, by2, bz2, bsx2, bsy2, bsz2;
        int matid2, objid2;
        float bx3, by3, bz3, bsx3, bsy3, bsz3;
        int matid3, objid3;
        float negxr, negxg, negxb;
        float posxr, posxg, posxb;
        float negyr, negyg, negyb;
        float posyr, posyg, posyb;
        float negzr, negzg, negzb;
        float poszr, poszg, poszb;
        float lightx, lighty, lightz;
        float lightr, lightg, lightb, lightint;
        
        fprintf(info, "Waiting for request...\n");
        
        if (fscanf(input, "%f %f %f %f %f %f %f %f %f %d", &px, &py, &pz, &ux, &uy, &uz, &vx, &vy, &vz, &quality) != 10) {
          fprintf(error, "Error: bad format\n");
          fprintf(output, "9:error arg,");
          fflush(output);
          continue;
        }
        
        if (fscanf(input, "%f %f %f %f %f %f %d %d", &bx1, &by1, &bz1, &bsx1, &bsy1, &bsz1, &matid1, &objid1) != 8) {
          fprintf(error, "Error: bad format\n");
          fprintf(output, "9:error arg,");
          fflush(output);
          continue;
        }
        
        if (fscanf(input, "%f %f %f %f %f %f %d %d", &bx2, &by2, &bz2, &bsx2, &bsy2, &bsz2, &matid2, &objid2) != 8) {
          fprintf(error, "Error: bad format\n");
          fprintf(output, "9:error arg,");
          fflush(output);
          continue;
        }
        
        if (fscanf(input, "%f %f %f %f %f %f %d %d", &bx3, &by3, &bz3, &bsx3, &bsy3, &bsz3, &matid3, &objid3) != 8) {
          fprintf(error, "Error: bad format\n");
          fprintf(output, "9:error arg,");
          fflush(output);
          continue;
        }
        
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
        
        if (fscanf(input, "%f %f %f %f %f %f %f", &lightx, &lighty, &lightz, &lightr, &lightg, &lightb, &lightint) != 7) {
          fprintf(error, "Error: bad format\n");
          fprintf(output, "9:error arg,");
          fflush(output);
          continue;
        }
        
        fprintf(info, "Got request\n");
    
        /**
        
        boxTransform.l.vx = (vec3f){ 1, 0, 0 };
        boxTransform.l.vy = (vec3f){ 0, 1, 0 };
        boxTransform.l.vz = (vec3f){ 0, 0, 1 };
        boxTransform.p = (vec3f){ 0, 0, 0 };
        
        boxTrans = ospNewInstance(boxModel, &boxTransform);
        ospCommit(boxTrans);
        
        obj1Transform.l.vx = (vec3f){ bsx1,    0,    0 };
        obj1Transform.l.vy = (vec3f){    0, bsy1,    0 };
        obj1Transform.l.vz = (vec3f){    0,    0, bsz1 };
        obj1Transform.p = (vec3f){ bx1, by1, bz1 };
        
        obj1Model = models1[objid1];
        obj1Geometry = geometries1[objid1];
        obj1Material = materials1[matid1];
        
        ospSetMaterial(obj1Geometry, obj1Material);
        ospCommit(obj1Geometry);
    
        obj1Trans = ospNewInstance(obj1Model, &obj1Transform);
        ospCommit(obj1Trans);
        
        obj2Transform.l.vx = (vec3f){ bsx2,    0,    0 };
        obj2Transform.l.vy = (vec3f){    0, bsy2,    0 };
        obj2Transform.l.vz = (vec3f){    0,    0, bsz2 };
        obj2Transform.p = (vec3f){ bx2, by2, bz2 };
        
        obj2Model = models2[objid2];
        obj2Geometry = geometries2[objid2];
        obj2Material = materials2[matid2];
        
        ospSetMaterial(obj2Geometry, obj2Material);
        ospCommit(obj2Geometry);
    
        obj2Trans = ospNewInstance(obj2Model, &obj2Transform);
        ospCommit(obj2Trans);
        
        obj3Transform.l.vx = (vec3f){ bsx3,    0,    0 };
        obj3Transform.l.vy = (vec3f){    0, bsy3,    0 };
        obj3Transform.l.vz = (vec3f){    0,    0, bsz3 };
        obj3Transform.p = (vec3f){ bx3, by3, bz3 };
        
        obj3Model = models3[objid3];
        obj3Geometry = geometries3[objid3];
        obj3Material = materials3[matid3];
        
        ospSetMaterial(obj3Geometry, obj3Material);
        ospCommit(obj3Geometry);
    
        obj3Trans = ospNewInstance(obj3Model, &obj3Transform);
        ospCommit(obj3Trans);
        
        negxMaterial = makeBasicMaterial(negxr, negxg, negxb);
        posxMaterial = makeBasicMaterial(posxr, posxg, posxb);
        negyMaterial = makeBasicMaterial(negyr, negyg, negyb);
        posyMaterial = makeBasicMaterial(posyr, posyg, posyb);
        negzMaterial = makeBasicMaterial(negzr, negzg, negzb);
        poszMaterial = makeBasicMaterial(poszr, poszg, poszb);
        
        ospSetMaterial(negx, negxMaterial);
        ospCommit(negx);
        
        ospSetMaterial(posx, posxMaterial);
        ospCommit(posx);
        
        ospSetMaterial(negy, negyMaterial);
        ospCommit(negy);
        
        ospSetMaterial(posy, posyMaterial);
        ospCommit(posy);
        
        ospSetMaterial(negz, negzMaterial);
        ospCommit(negz);
        
        ospSetMaterial(posz, poszMaterial);
        ospCommit(posz);
        
        model = ospNewModel();
        ospAddGeometry(model, obj1Trans);
        ospAddGeometry(model, obj2Trans);
        ospAddGeometry(model, obj3Trans);
        ospAddGeometry(model, boxTrans);
        ospCommit(model);
        
        ospSetObject(renderer, "model", model);
        ospCommit(renderer);
        
        ospRelease(model);
        ospRelease(boxTrans);
        ospRelease(obj1Trans);
        ospRelease(obj2Trans);
        ospRelease(obj3Trans);
        
        ospSet3f(light_values[0], "position", lightx, lighty, lightz);
        ospSet3f(light_values[0], "color", lightr, lightg, lightb);
        ospSet1f(light_values[0], "intensity", lightint);
        ospCommit(light_values[0]);
    
        **/
        
        ospSet3f(camera, "pos", px, py, pz);
        ospSet3f(camera, "up", ux, uy, uz);
        ospSet3f(camera, "dir", vx, vy, vz);
        ospCommit(camera);
        
        fprintf(info, "Creating Frame Buffer\n");
        size.x = size.y = quality;
        frameBuffer = ospNewFrameBuffer(size, OSP_FB_RGBA8, OSP_FB_COLOR);
        
        fprintf(info, "Initializing Frame Buffer\n");
        ospCommit(frameBuffer);
        
        fprintf(info, "Starting Rendering\n");
        ospFrameBufferClear(frameBuffer, OSP_FB_COLOR);
        (void)ospRenderFrame(frameBuffer, renderer, OSP_FB_COLOR);
        fprintf(info, "Finished Rendering\n");
        
        fprintf(info, "Saving render\n");
        pixels = ospMapFrameBuffer(frameBuffer, OSP_FB_COLOR);
        
        fprintf(output, "%lu:", (size_t)4 * (size_t)size.x * (size_t)size.y * sizeof(uint8_t));
        fwrite(pixels, sizeof(uint8_t), 4 * size.x * size.y, output);
        fprintf(output, ",");
        fflush(output);
        
        ospUnmapFrameBuffer(pixels, frameBuffer);
        fprintf(info, "Saved render\n");
      }
      
      fprintf(info, "Shutting down OSPRay\n");
      ospShutdown();
      
      fclose(output);
      
      return 0;
    }

    void OSPExampleViewer::render(const std::shared_ptr<sg::Frame> &root)
    {
      sg::Node &sgCamera = root->child("camera");
      sg::Node &sgRenderer = root->child("renderer");

      OSPCamera camera = sgCamera.valueAs<OSPCamera>();
      OSPRenderer renderer = sgCamera.valueAs<OSPRenderer>();

      tapestryLoop(renderer, camera);

      /**

      camera["pos"] = vec3f(0.0f, 3.5f, 10.2f);
      camera["up"] = vec3f(0.0f, 1.0f, 0.0f);
      camera["dir"] = vec3f(0.0f, 0.0f, -1.0f);
      camera["fovy"] = 90.0f;
      camera["aspect"] = 1.0f;
      camera.commit();

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

      **/
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
