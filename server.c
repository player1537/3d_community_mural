#include <stdio.h>
#include <ospray/ospray.h>

typedef struct {
	int a;
	int b;
	int c;
	int d;
} osp_vec4i;

OSPMaterial
makeMirrorMaterial(void) {
	OSPMaterial material;
	osp_vec3f eta, k;
	float roughness;
	
	eta = (osp_vec3f){ 0.1, 0.8, 1.1 };
	k = (osp_vec3f){ 3.5, 2.5, 2.4 };
	roughness = 0.1;
	
	material = ospNewMaterial2("pathtracer", "Metal");
	ospSet3fv(material, "eta", (float *)&eta);
	ospSet3fv(material, "k", (float *)&k);
	ospSet1f(material, "roughness", roughness);
	ospCommit(material);
	
	return material;
}

OSPGeometry
makeBoxGeometry(float dx, float dy, float dz) {
	OSPGeometry geometry;
	OSPData vertex, index, normal;
	size_t nvertex, nindex, nnormal;
	osp_vec3f *vertex_values, *normal_values;
	osp_vec4i *index_values;
	
	vertex_values = (osp_vec3f[]){
		/* 0 */ { -dx, -dy, -dz },
		/* 1 */ { -dx, -dy, +dz },
		/* 2 */ { -dx, +dy, -dz },
		/* 3 */ { -dx, +dy, +dz },
		/* 4 */ { +dx, -dy, -dz },
		/* 5 */ { +dx, -dy, +dz },
		/* 6 */ { +dx, +dy, -dz },
		/* 7 */ { +dx, +dy, +dz },
	};
	nvertex = 8;
	
	index_values = (osp_vec4i[]){
		/* -X */ { 0, 1, 2, 3 },
		/* +X */ { 4, 5, 6, 7 },
		/* -Y */ { 0, 1, 4, 5 },
		/* +Y */ { 2, 3, 6, 7 },
		/* -Z */ { 0, 2, 4, 6 },
		/* +Z */ { 1, 3, 5, 7 },
	};
	nindex = 6;
	
	normal_values = (osp_vec3f[]){
		{ +1,  0,  0 },
		{ -1,  0,  0 },
		{  0, +1,  0 },
		{  0, -1,  0 },
		{  0,  0, +1 },
		{  0,  0, -1 },
	};
	nnormal = 6;
	
	vertex = ospNewData(nvertex, OSP_FLOAT3, (float *)vertex_values, 0);
	ospCommit(vertex);
	
	index = ospNewData(nindex, OSP_INT4, (int *)index_values, 0);
	ospCommit(index);
	
	normal = ospNewData(nnormal, OSP_FLOAT3, (float *)normal_values, 0);
	ospCommit(normal);

	geometry = ospNewGeometry("quads");
	ospSetData(geometry, "vertex", vertex);
	ospSetData(geometry, "vertex.normal", normal);
	ospSetData(geometry, "index", index);
	ospCommit(geometry);
	
	ospRelease(vertex);
	ospRelease(normal);
	ospRelease(index);
	
	return geometry;
}

int
main(int argc, const char **argv) {
	OSPError err;
	FILE *info, *error, *output;
	OSPFrameBuffer frameBuffer;
	OSPRenderer renderer;
	osp_vec2i size;
	int spp, maxDepth;
	OSPModel model;
	OSPCamera camera;
	OSPLight light;
	OSPData lights;
	osp_vec3f cPos, cDir, cUp, lPos;
	float lRadius;
	osp_vec2f imageStart, imageEnd;
	OSPMaterial mirror;
	OSPGeometry box;
	const void *pixels;
	
	info = stdout;
	error = stderr;
	output = fopen("out.bin", "wb");
	
	fprintf(info, "Initializing OSPRay\n");
	err = ospInit(&argc, argv);
	if (err != OSP_NO_ERROR) {
		fprintf(error, "Error: ospInit %d\n", err);
		return 1;
	}
	
	fprintf(info, "Creating Box\n");
	box = makeBoxGeometry(0.5, 0.5, 0.5);
	
	fprintf(info, "Creating Mirror Material\n");
	mirror = makeMirrorMaterial();
	
	fprintf(info, "Creating Model\n");
	model = ospNewModel();

	fprintf(info, "Creating Camera\n");
	camera = ospNewCamera("perspective");

	fprintf(info, "Creating Point Light\n");
	light = ospNewLight3("sphere");
	
	fprintf(info, "Creating Light Dataset\n");
	lights = ospNewData(1, OSP_LIGHT, &light, 0);

	fprintf(info, "Creating Renderer\n");
	renderer = ospNewRenderer("pathtracer");

	size = (osp_vec2i){ 512, 512 };
	spp = 4;
	maxDepth = 20;
	cPos = (osp_vec3f){ 2.0, 2.0, 2.0 };
	cDir = (osp_vec3f){ -1.0, -1.0, -1.0 };
	cUp = (osp_vec3f){ 0.0, 1.0, 0.0 };
	imageStart = (osp_vec2f){ 0.0, 0.0 };
	imageEnd = (osp_vec2f){ 1.0, 1.0 };
	lPos = (osp_vec3f){ 0.0, 0.5, 0.0 };
	lRadius = 0.5;
	
	fprintf(info, "Initializing Box\n");
	ospSetMaterial(box, mirror);
	ospCommit(box);
	
	fprintf(info, "Initializing Model\n");
	ospAddGeometry(model, box);
	ospCommit(model);
	
	fprintf(info, "Initializing Model\n");
	ospSet3fv(camera, "pos", (float *)&cPos);
	ospSet3fv(camera, "dir", (float *)&cDir);
	ospSet3fv(camera, "up", (float *)&cUp);
	ospSet2fv(camera, "imageStart", (float *)&imageStart);
	ospSet2fv(camera, "imageEnd", (float *)&imageEnd);
	ospCommit(camera);
	
	fprintf(info, "Initializing Point Light\n");
	ospSet3fv(light, "position", (float *)&lPos);
	ospSet1f(light, "radius", lRadius);
	ospCommit(light);
	
	fprintf(info, "Initializing Light Dataset\n");
	ospCommit(lights);
	
	fprintf(info, "Initializing Renderer\n");
	ospSetObject(renderer, "model", model);
	ospSetObject(renderer, "camera", camera);
	ospSetData(renderer, "lights", lights);
	ospSet1i(renderer, "spp", spp);
	ospSet1i(renderer, "maxDepth", maxDepth);
	ospCommit(renderer);
	
	fprintf(info, "Creating Frame Buffer\n");
	frameBuffer = ospNewFrameBuffer(&size, OSP_FB_RGBA8, OSP_FB_COLOR);
	
	fprintf(info, "Initializing Frame Buffer\n");
	ospCommit(frameBuffer);
	
	fprintf(info, "Starting Rendering\n");
	ospFrameBufferClear(frameBuffer, OSP_FB_COLOR);
	(void)ospRenderFrame(frameBuffer, renderer, OSP_FB_COLOR);
	fprintf(info, "Finished Rendering\n");
	
	fprintf(info, "Saving render\n");
	pixels = ospMapFrameBuffer(frameBuffer, OSP_FB_COLOR);
	fwrite(pixels, sizeof(uint8_t), 4 * size.x * size.y, output);
	ospUnmapFrameBuffer(pixels, frameBuffer);
	fprintf(info, "Saved render\n");
	
	fprintf(info, "Shutting down OSPRay\n");
	ospShutdown();
	
	fclose(output);
	
	return 0;
}
