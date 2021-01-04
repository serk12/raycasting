#version 330

uniform int qttyText;
uniform float cubeSize = 1.733f; // sqrt(1+sqrt(1+1)^2) ~ 1.733f
uniform float resolution = 2.0f;

uniform int max_iterations = 1500;

uniform vec3 colFocus = vec3(0.6f, 0.6f, 0.6f);
uniform vec3 llumAmbient = vec3(1.0f, 1.0f, 1.0f);
uniform vec3 colorBG = vec3(1.0f);

uniform float fmatshin = 50.0f;

smooth in vec3 init_coords;
smooth in vec3 dir;
smooth in vec4 pos;
smooth in vec3 cam_pos;

uniform sampler3D volume;
out vec4 frag_color;

uniform int size;
uniform vec4[11] color;

vec4 alphaToColor(float a) {
    a *=  100;
    float aux = floor(a);
    int i = int((aux * size) / 100);
    float b = aux - a;
    float c = 1.0f - b;
    vec4 result;
    if (i == (size - 1)) {
        result = color[i] / 100.0f;
    }
    else {
        result = (color[i]*b + color[i+1]*c) / 100.0f;
    }
    result = clamp(result, 0.0f, 1.0f);
    result.a *= 0.5f;
    return result;
}

bool insideCube(vec3 coords) {
    return (coords.x <= 0.5f && coords.x >= -0.5f) &&
           (coords.y <= 0.5f && coords.y >= -0.5f) &&
           (coords.z <= 0.5f && coords.z >= -0.5f);
}

vec3 Lambert(vec3 NormSCO, vec3 L, vec3 color)
{
    vec3 colRes = llumAmbient * color;
    if (dot (L, NormSCO) > 0.0f)
      colRes = colRes + colFocus * color * dot (L, NormSCO);
    return (colRes);
}

vec3 Phong(vec3 vert_norm, vec3 L, vec4 vert_pos, vec3 color)
{
    vec3 colRes = Lambert (vert_norm, L, color);
    if (dot(vert_norm,L) < 0.0f)
        return colRes;

    vec3 R = reflect(-L, vert_norm);
    vec3 V = normalize(-vert_pos.xyz);

    if ((dot(R, V) < 0.0f) || (fmatshin == 0.0f))
        return colRes;
    float shine = pow(max(0.0, dot(R, V)), fmatshin);
    return (colRes + color * colFocus * shine);
}

vec3 RCPhong(vec3 coords, vec3 deltaDir, vec3 color) {
    vec3 ini = coords + deltaDir + vec3(0.5f);
    vec3 end = coords - deltaDir + vec3(0.5f);
    vec3 mid = coords + vec3(0.5f);
    float denX = (texture3D(volume, vec3(ini.x, mid.y, mid.z)).x - texture3D(volume, vec3(end.x, mid.y, mid.z)).x) / 2.0f;
    float denY = (texture3D(volume, vec3(mid.x, ini.y, mid.z)).x - texture3D(volume, vec3(mid.x, end.y, mid.z)).x) / 2.0f;
    float denZ = (texture3D(volume, vec3(mid.x, mid.y, ini.z)).x - texture3D(volume, vec3(mid.x, mid.y, end.z)).x) / 2.0f;
    vec3 N = vec3(denX, denY, denZ);
    vec3 L = normalize(cam_pos - coords);
    return Phong(N, L, pos, color);
}

void main (void) {
    vec3 coords = init_coords;
    if (!insideCube(coords)) {
        discard;
    }

    float stepSize = (cubeSize/float(qttyText))/resolution;
    float powSize = cubeSize;

    vec3 deltaDir = dir * stepSize;

    float alphaAcum = 0.0f;
    vec3 colorAcum = vec3(0.0f);


    for(int i = 0; i <= max_iterations; ++i) {
        float alpha_original = texture3D(volume, coords + vec3(0.5f)).x;
        vec4 color_source = alphaToColor(alpha_original);
        float alpha = alpha_original * color_source.a;
        vec3 color = color_source.rgb;
        if (alpha > 0.0f) {
            alpha = 1.0f - pow(1.0f - alpha, powSize);
            alphaAcum = alphaAcum + (1.0f - alphaAcum) * alpha;
            //alphaAcum = alpha + (1.0f - alpha) * alphaAcum;
            color = RCPhong(coords, deltaDir, color) * alpha;
            colorAcum = colorAcum + (1.0 - alphaAcum) * color;
            //colorAcum = color + (1.0f - color) * colorAcum;
            if (alphaAcum >= 1.0f) {
                alphaAcum = 1.0f;
                colorAcum = clamp(colorAcum, 0.0f, 1.0f);
                break;
            }
        }
        coords += deltaDir;
        if (!insideCube(coords)) {
            colorAcum = colorAcum * alphaAcum + (1.0f - alphaAcum) * colorBG;
            break;
        }
    }
    frag_color = vec4(colorAcum, alphaAcum);
}
