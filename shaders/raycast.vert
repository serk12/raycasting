#version 330

layout (location = 0) in vec3 vert;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

smooth out vec3 init_coords;
smooth out vec3 dir;
smooth out vec4 pos;
smooth out vec3 cam_pos;

void main(void)  {
    mat4 modelView = view * model;
    //calculate the camera position
    mat4 iView = inverse(modelView);
    cam_pos = vec3(iView[3])/iView[3][3];
    init_coords = vert;
    dir = normalize(vert - cam_pos);
    pos = modelView * vec4(vert, 1.0f);
    gl_Position = projection * pos;
}
