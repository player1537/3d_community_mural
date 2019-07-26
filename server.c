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
	roughness = 0.5;
	
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
		/* -X */ { 0, 1, 3, 2 },
		/* +X */ { 4, 5, 7, 6 },
		/* -Y */ { 0, 1, 5, 4 },
		/* +Y */ { 2, 3, 7, 6 },
		/* -Z */ { 0, 2, 6, 4 },
		/* +Z */ { 1, 3, 7, 5 },
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


OSPGeometry
makeNegXGeometry(float dx, float dy, float dz) {
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
	};
	nvertex = 4;
	
	index_values = (osp_vec4i[]){
		/* -X */ { 0, 1, 3, 2 },
	};
	nindex = 1;
	
	normal_values = (osp_vec3f[]){
		{ +1,  0,  0 },
	};
	nnormal = 1;
	
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


OSPGeometry
makePosXGeometry(float dx, float dy, float dz) {
	OSPGeometry geometry;
	OSPData vertex, index, normal;
	size_t nvertex, nindex, nnormal;
	osp_vec3f *vertex_values, *normal_values;
	osp_vec4i *index_values;
	
	vertex_values = (osp_vec3f[]){
		/* 4 */ { +dx, -dy, -dz },
		/* 5 */ { +dx, -dy, +dz },
		/* 6 */ { +dx, +dy, -dz },
		/* 7 */ { +dx, +dy, +dz },
	};
	nvertex = 4;
	
	index_values = (osp_vec4i[]){
		{ 0, 1, 3, 2 },
	};
	nindex = 1;
	
	normal_values = (osp_vec3f[]){
		{ -1,  0,  0 },
	};
	nnormal = 1;
	
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


OSPGeometry
makeNegYGeometry(float dx, float dy, float dz) {
	OSPGeometry geometry;
	OSPData vertex, index, normal;
	size_t nvertex, nindex, nnormal;
	osp_vec3f *vertex_values, *normal_values;
	osp_vec4i *index_values;
	
	vertex_values = (osp_vec3f[]){
		/* 0 */ { -dx, -dy, -dz },
		/* 1 */ { -dx, -dy, +dz },
		/* 4 */ { +dx, -dy, -dz },
		/* 5 */ { +dx, -dy, +dz },
	};
	nvertex = 4;
	
	index_values = (osp_vec4i[]){
		/* -X */ { 0, 1, 3, 2 },
	};
	nindex = 1;
	
	normal_values = (osp_vec3f[]){
		{  0, +1,  0 },
	};
	nnormal = 1;
	
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


OSPGeometry
makePosYGeometry(float dx, float dy, float dz) {
	OSPGeometry geometry;
	OSPData vertex, index, normal;
	size_t nvertex, nindex, nnormal;
	osp_vec3f *vertex_values, *normal_values;
	osp_vec4i *index_values;
	
	vertex_values = (osp_vec3f[]){
		/* 2 */ { -dx, +dy, -dz },
		/* 3 */ { -dx, +dy, +dz },
		/* 6 */ { +dx, +dy, -dz },
		/* 7 */ { +dx, +dy, +dz },
	};
	nvertex = 4;
	
	index_values = (osp_vec4i[]){
		{ 0, 1, 3, 2 },
	};
	nindex = 1;
	
	normal_values = (osp_vec3f[]){
		{  0, -1,  0 },
	};
	nnormal = 1;
	
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


OSPGeometry
makeNegZGeometry(float dx, float dy, float dz) {
	OSPGeometry geometry;
	OSPData vertex, index, normal;
	size_t nvertex, nindex, nnormal;
	osp_vec3f *vertex_values, *normal_values;
	osp_vec4i *index_values;
	
	vertex_values = (osp_vec3f[]){
		/* 0 */ { -dx, -dy, -dz },
		/* 2 */ { -dx, +dy, -dz },
		/* 4 */ { +dx, -dy, -dz },
		/* 6 */ { +dx, +dy, -dz },
	};
	nvertex = 4;
	
	index_values = (osp_vec4i[]){
		{ 0, 1, 3, 2 },
	};
	nindex = 1;
	
	normal_values = (osp_vec3f[]){
		{  0,  0, +1 },
	};
	nnormal = 1;
	
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


OSPGeometry
makePosZGeometry(float dx, float dy, float dz) {
	OSPGeometry geometry;
	OSPData vertex, index, normal;
	size_t nvertex, nindex, nnormal;
	osp_vec3f *vertex_values, *normal_values;
	osp_vec4i *index_values;
	
	vertex_values = (osp_vec3f[]){
		/* 1 */ { -dx, -dy, +dz },
		/* 3 */ { -dx, +dy, +dz },
		/* 5 */ { +dx, -dy, +dz },
		/* 7 */ { +dx, +dy, +dz },
	};
	nvertex = 4;
	
	index_values = (osp_vec4i[]){
		{ 0, 1, 3, 2 },
	};
	nindex = 1;
	
	normal_values = (osp_vec3f[]){
		{  0,  0, -1 },
	};
	nnormal = 1;
	
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


OSPGeometry
makeBallGeometry(float cx, float cy, float cz, float radius) {
	OSPGeometry geometry;
	OSPData spheres;
	int bytes_per_sphere;
	int offset_center;
	int offset_radius;
	osp_vec4f *spheres_value;
	size_t nspheres;
	
	spheres_value = (osp_vec4f[]){
		{ cx, cy, cz, radius },
	};
	nspheres = 1;
	bytes_per_sphere = sizeof(osp_vec4f);
	offset_center = 0;
	offset_radius = sizeof(osp_vec3f);
	
	spheres = ospNewData(nspheres, OSP_FLOAT4, (float *)spheres_value, 0);
	ospCommit(spheres);
	
	geometry = ospNewGeometry("spheres");
	ospSetData(geometry, "spheres", spheres);
	ospSet1i(geometry, "bytes_per_sphere", bytes_per_sphere);
	ospSet1i(geometry, "offset_center", offset_center);
	ospSet1i(geometry, "offset_radius", offset_radius);
	ospCommit(geometry);
	
	ospRelease(spheres);
	
	return geometry;
}


OSPMaterial
makeLuminousMaterial(void) {
	OSPMaterial material;
	osp_vec3f color;
	float intensity;
	
	color = (osp_vec3f){ 1.0, 0.0, 1.0 };
	intensity = 0.5;
	
	material = ospNewMaterial2("pathtracer", "Luminous");
	ospSet3fv(material, "color", (float *)&color);
	ospSet1f(material, "intensity", intensity);
	ospCommit(material);
	
	return material;
}


OSPMaterial
makeBasicMaterial(float r, float g, float b) {
	OSPMaterial material;
	osp_vec3f Kd;
	
	Kd = (osp_vec3f){ r, g, b };
	
	material = ospNewMaterial2("pathtracer", "OBJMaterial");
	ospSet3fv(material, "Kd", (float *)&Kd);
	ospCommit(material);
	
	return material;
}


int
main(int argc, const char **argv) {
	FILE *input, *info, *error, *output;
	OSPError err;
	OSPFrameBuffer frameBuffer;
	OSPRenderer renderer;
	OSPModel model;
	OSPCamera camera;
	OSPLight light_values[2];
	OSPData lights;
	OSPMaterial mirror, luminous, white, green, red, blue, yellow;
	OSPGeometry top, left, back, right, bottom, front, ball;
	osp_vec2i size;
	const void *pixels;
	
	input = fopen("/dev/stdin", "r");
	info = fopen("/dev/stdout", "w");
	error = fopen("/dev/stderr", "w");
	output = fopen("/dev/fd/100", "wb");
	
	fprintf(info, "Initializing OSPRay\n");
	err = ospInit(&argc, argv);
	if (err != OSP_NO_ERROR) {
		fprintf(error, "Error: ospInit %d\n", err);
		return 1;
	}
	
	fprintf(info, "Creating Box\n");
	top = makePosYGeometry(1.0, 1.0, 1.0);
	left = makeNegXGeometry(1.0, 1.0, 1.0);
	back = makePosZGeometry(1.0, 1.0, 1.0);
	right = makePosXGeometry(1.0, 1.0, 1.0);
	bottom = makeNegYGeometry(1.0, 1.0, 1.0);
	front = makeNegZGeometry(1.0, 1.0, 1.0);
	
	fprintf(info, "Creating Color Materials\n");
	white = makeBasicMaterial(0.8, 0.8, 0.8);
	green = makeBasicMaterial(0.1, 0.8, 0.1);
	red = makeBasicMaterial(0.8, 0.1, 0.1);
	blue = makeBasicMaterial(0.1, 0.1, 0.8);
	yellow = makeBasicMaterial(0.1, 0.8, 0.8);
	
	fprintf(info, "Creating Mirror Material\n");
	mirror = makeMirrorMaterial();
	
	fprintf(info, "Creating Ball\n");
	ball = makeBallGeometry(0.0, 0.0, 0.5, 0.25);
	
	fprintf(info, "Creating Luminous Material\n");
	luminous = makeLuminousMaterial();
	
	fprintf(info, "Creating Model\n");
	model = ospNewModel();

	fprintf(info, "Creating Camera\n");
	camera = ospNewCamera("perspective");

	fprintf(info, "Creating Point Light\n");
	light_values[0] = ospNewLight3("sphere");

	fprintf(info, "Creating Ambient Light\n");
	light_values[1] = ospNewLight3("ambient");
	
	fprintf(info, "Creating Light Dataset\n");
	lights = ospNewData(2, OSP_LIGHT, light_values, 0);

	fprintf(info, "Creating Renderer\n");
	renderer = ospNewRenderer("pathtracer");
	
	fprintf(info, "Initializing Box\n");
	ospSetMaterial(top, white);
	ospCommit(top);
	
	ospSetMaterial(left, red);
	ospCommit(left);
	
	ospSetMaterial(back, white);
	ospCommit(back);
	
	ospSetMaterial(right, green);
	ospCommit(right);
	
	ospSetMaterial(bottom, white);
	ospCommit(bottom);
	
	ospSetMaterial(front, luminous);
	ospCommit(front);
	(void)yellow;
	(void)blue;
	
	fprintf(info, "Initializing Ball\n");
	ospSetMaterial(ball, mirror);
	ospCommit(ball);
	
	fprintf(info, "Initializing Model\n");
	ospAddGeometry(model, top);
	ospAddGeometry(model, left);
	ospAddGeometry(model, back);
	ospAddGeometry(model, right);
	ospAddGeometry(model, bottom);
	ospAddGeometry(model, ball);
	ospCommit(model);
	
	fprintf(info, "Initializing Camera\n");
	ospSet3f(camera, "pos", 0.0, 0.0, 0.1);
	ospSet3f(camera, "up", 0.0, 1.0, 0.0);
	ospSet3f(camera, "dir", 0.0, 0.0, 1.0);
	ospSet2f(camera, "imageStart", 0.0, 0.0);
	ospSet2f(camera, "imageEnd", 1.0, 1.0);
	ospSet1f(camera, "height", 1.0);
	ospSet1f(camera, "fovy", 120);
	ospSet1f(camera, "aspect", 1.0);
	ospCommit(camera);
	
	fprintf(info, "Initializing Point Light\n");
	ospSet3f(light_values[0], "position", 0.0, 0.9, 0.5);
	ospSet1f(light_values[0], "radius", 0.0);
	ospSet1f(light_values[0], "intensity", 10.0);
	ospCommit(light_values[0]);
	
	fprintf(info, "Initializing Ambient Light\n");
	ospCommit(light_values[1]);
	
	fprintf(info, "Initializing Light Dataset\n");
	ospCommit(lights);
	
	fprintf(info, "Initializing Renderer\n");
	ospSetObject(renderer, "model", model);
	ospSetObject(renderer, "camera", camera);
	ospSetData(renderer, "lights", lights);
	ospSet1i(renderer, "spp", 120);
	ospSet1i(renderer, "maxDepth", 20);
	ospSet1i(renderer, "rouletteDepth", 20);
	ospCommit(renderer);
	
	fprintf(info, "Entering render loop\n");
	for (;;) {
		float px, py, pz, ux, uy, uz, vx, vy, vz;
		int quality;
		
		fprintf(info, "Waiting for request...\n");
		
		if (fscanf(input, "%f %f %f %f %f %f %f %f %f %d", &px, &py, &pz, &ux, &uy, &uz, &vx, &vy, &vz, &quality) != 10) {
			fprintf(error, "Error: bad format\n");
			fprintf(output, "10:error arg,");
			fflush(output);
			continue;
		}
		
		fprintf(info, "Got request\n");
		
		ospSet3f(camera, "pos", px, py, pz);
		ospSet3f(camera, "up", ux, uy, uz);
		ospSet3f(camera, "dir", vx, vy, vz);
		ospCommit(camera);
		
		size = (osp_vec2i){ quality, quality };
		
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
