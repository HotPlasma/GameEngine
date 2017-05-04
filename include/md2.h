#ifndef md2_H
#define md2_H


//#include "PreHeader.h"
//
//#include "Scene.h"
//#include "Camera.h"
//#include "SceneReader.h"
//#include "ModelReader.h"
//#include "glslprogram.h"

class md2 
{
   public :
	   void makeCheckImage(void);
	   void init(void);
	   int ReadMD2Model(const char *filename, struct md2_model_t *mdl);
	   void FreeModel(struct md2_model_t *mdl);
	   void RenderFrameItpWithGLCmds(int n, float interp, const struct md2_model_t *mdl);
	   void Animate(int start, int end, int *frame, float *interp);
	   void init(const char *filename);
	   void cleanup();
	   void reshape(int w, int h);
	   void display();
	   void keyboard(unsigned char key, int x, int y);
	   int AnimationMain(int argc, char *argv[]);


};



/* Vector */
typedef float vec3_t[3];

/* MD2 header */
struct md2_header_t
{
	int ident;
	int version;

	int skinwidth;
	int skinheight;

	int framesize;

	int num_skins;
	int num_vertices;
	int num_st;
	int num_tris;
	int num_glcmds;
	int num_frames;

	int offset_skins;
	int offset_st;
	int offset_tris;
	int offset_frames;
	int offset_glcmds;
	int offset_end;
};

/* Texture name */
struct md2_skin_t
{
	char name[64];
};

/* Texture coords */
struct md2_texCoord_t
{
	short s;
	short t;
};

/* Triangle info */
struct md2_triangle_t
{
	unsigned short vertex[3];
	unsigned short st[3];
};

/* Compressed vertex */
struct md2_vertex_t
{
	unsigned char v[3];
	unsigned char normalIndex;
};

/* Model frame */
struct md2_frame_t
{
	vec3_t scale;
	vec3_t translate;
	char name[16];
	struct md2_vertex_t *verts;
};

/* GL command packet */
struct md2_glcmd_t
{
	float s;
	float t;
	int index;
};

/* MD2 model structure */
struct md2_model_t
{
	struct md2_header_t header;

	struct md2_skin_t *skins;
	struct md2_texCoord_t *texcoords;
	struct md2_triangle_t *triangles;
	struct md2_frame_t *frames;
	int *glcmds;

	GLuint tex_id;
};
#endif  
