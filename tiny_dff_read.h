// MIT License
// 
// Copyright (c) 2026 webl1ne
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once
#ifndef TINYDFF_READ_TINYDFF_H
#define TINYDFF_READ_TINYDFF_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

// Max Values
#define TINYDFF_READ_MAX_MATERIALS  64
#define TINYDFF_READ_MAX_ATOMICS    256
#define TINYDFF_READ_MAX_FRAMES     256
#define TINYDFF_READ_MAX_VERTICES   65535
#define TINYDFF_READ_MAX_FACES      65535
#define TINYDFF_READ_MAX_UVS        8
#define TINYDFF_READ_MAX_SPLITS     64
#define TINYDFF_READ_MAX_NAME_LEN   32

// Flags Geometry
#define TINYDFF_READ_FLAG_TRISTRIP     0x01
#define TINYDFF_READ_FLAG_POSITIONS    0x02
#define TINYDFF_READ_FLAG_TEXTURED     0x04
#define TINYDFF_READ_FLAG_PRELIT       0x08
#define TINYDFF_READ_FLAG_NORMALS      0x10
#define TINYDFF_READ_FLAG_LIGHT        0x20
#define TINYDFF_READ_FLAG_MODULATE     0x40
#define TINYDFF_READ_FLAG_TEXTURED2    0x80

// Types Material FX
#define TINYDFF_READ_MATFX_NONE            0
#define TINYDFF_READ_MATFX_BUMPMAP         1
#define TINYDFF_READ_MATFX_ENVMAP          2
#define TINYDFF_READ_MATFX_BUMPENVMAP      3
#define TINYDFF_READ_MATFX_DUAL            4
#define TINYDFF_READ_MATFX_UVTRANSFORM     5
#define TINYDFF_READ_MATFX_DUALUVTRANSFORM 6

// Data Types
typedef int8_t   TinyDFF_Read_Int8;
typedef uint8_t  TinyDFF_Read_UInt8;
typedef int16_t  TinyDFF_Read_Int16;
typedef uint16_t TinyDFF_Read_UInt16;
typedef int32_t  TinyDFF_Read_Int32;
typedef uint32_t TinyDFF_Read_UInt32;
typedef float    TinyDFF_Read_Float;

// RW Versions
#define TINYDFF_READ_GTA3_1   0x00000302
#define TINYDFF_READ_GTA3_2   0x00000304
#define TINYDFF_READ_GTA3_3   0x00000310
#define TINYDFF_READ_GTA3_4   0x0800FFFF
#define TINYDFF_READ_VCPS2    0x0C02FFFF
#define TINYDFF_READ_VCPC     0x1003FFFF
#define TINYDFF_READ_SA       0x1803FFFF

// Types
typedef struct TinyDFF_Read_Context* TinyDFF_Read_Handle;

// Callback
typedef void* (*TinyDFF_Read_AllocFunc)(void* user, size_t size);
typedef void  (*TinyDFF_Read_FreeFunc)(void* user, void* memory);
typedef size_t (*TinyDFF_Read_ReadFunc)(void* user, void* buffer, size_t byteCount);
typedef bool   (*TinyDFF_Read_SeekFunc)(void* user, int64_t offset);
typedef int64_t (*TinyDFF_Read_TellFunc)(void* user);
typedef void   (*TinyDFF_Read_ErrorFunc)(void* user, const char* msg);

typedef struct TinyDFF_Read_Callbacks {
    TinyDFF_Read_ErrorFunc errorFn;
    TinyDFF_Read_AllocFunc allocFn;
    TinyDFF_Read_FreeFunc freeFn;
    TinyDFF_Read_ReadFunc readFn;
    TinyDFF_Read_SeekFunc seekFn;
    TinyDFF_Read_TellFunc tellFn;
} TinyDFF_Read_Callbacks;

// Structure Data
typedef struct TinyDFF_Read_Vector3 {
    float x, y, z;
} TinyDFF_Read_Vector3;

typedef struct TinyDFF_Read_Vector2 {
    float u, v;
} TinyDFF_Read_Vector2;

typedef struct TinyDFF_Read_Color {
    uint8_t r, g, b, a;
} TinyDFF_Read_Color;

typedef struct TinyDFF_Read_Face {
    TinyDFF_Read_UInt16 a, b, c;
    TinyDFF_Read_UInt16 materialIndex;
} TinyDFF_Read_Face;

typedef struct TinyDFF_Read_Split {
    TinyDFF_Read_UInt32 matIndex;
    TinyDFF_Read_UInt32 indexCount;
    TinyDFF_Read_UInt16* indices;
} TinyDFF_Read_Split;

typedef struct TinyDFF_Read_BinMesh {
    TinyDFF_Read_UInt32 faceType;
    TinyDFF_Read_UInt32 numIndices;
    TinyDFF_Read_UInt32 splitCount;
    TinyDFF_Read_Split* splits;
} TinyDFF_Read_BinMesh;

typedef struct TinyDFF_Read_Texture {
    char name[TINYDFF_READ_MAX_NAME_LEN];
    char maskName[TINYDFF_READ_MAX_NAME_LEN];
    TinyDFF_Read_UInt16 filterFlags;
    bool hasSkyMipmap;
} TinyDFF_Read_Texture;

typedef struct TinyDFF_Read_MatFx {
    TinyDFF_Read_UInt32 type;
    float bumpCoeff;
    float envCoeff;
    float srcBlend;
    float destBlend;
    bool hasTex1;
    TinyDFF_Read_Texture tex1;
    bool hasTex2;
    TinyDFF_Read_Texture tex2;
    bool hasDualPassMap;
    TinyDFF_Read_Texture dualPassMap;
} TinyDFF_Read_MatFx;

typedef struct TinyDFF_Read_Material {
    TinyDFF_Read_UInt32 flags;
    TinyDFF_Read_Color color;
    TinyDFF_Read_UInt32 unknown;
    bool hasTexture;
    TinyDFF_Read_Texture texture;
    float surfaceProps[3];

    bool hasRightToRender;
    TinyDFF_Read_UInt32 rightToRenderVal1;
    TinyDFF_Read_UInt32 rightToRenderVal2;

    bool hasMatFx;
    TinyDFF_Read_MatFx matFx;

    bool hasReflectionMat;
    float reflectionAmount[4];
    float reflectionIntensity;

    bool hasSpecularMat;
    float specularLevel;
    char specularName[TINYDFF_READ_MAX_NAME_LEN];

    bool hasUVAnim;
    TinyDFF_Read_UInt32 uvVal;
    char uvName[TINYDFF_READ_MAX_NAME_LEN];
} TinyDFF_Read_Material;

typedef struct TinyDFF_Read_Geometry {
    TinyDFF_Read_UInt32 flags;
    TinyDFF_Read_UInt32 numUVs;
    bool hasNativeGeometry;

    TinyDFF_Read_UInt32 vertexCount;
    TinyDFF_Read_Face* faces;
    TinyDFF_Read_Color* vertexColors;
    TinyDFF_Read_Vector2* texCoords[TINYDFF_READ_MAX_UVS];
    TinyDFF_Read_Vector3* vertices;
    TinyDFF_Read_Vector3* normals;
    float boundingSphere[4];

    TinyDFF_Read_UInt32 materialCount;
    TinyDFF_Read_Material* materials;

    TinyDFF_Read_BinMesh binMesh;

    bool hasSkin;
    TinyDFF_Read_UInt32 boneCount;
    TinyDFF_Read_UInt32 specialIndexCount;
    TinyDFF_Read_UInt32* vertexBoneIndices;
    TinyDFF_Read_Float* vertexBoneWeights;
    TinyDFF_Read_Float* inverseMatrices;

    bool hasNightColors;
    TinyDFF_Read_Color* nightColors;

    bool has2dfx;
    TinyDFF_Read_UInt32 twodfxSize;
    TinyDFF_Read_UInt8* twodfxData;

    bool hasMorph;
} TinyDFF_Read_Geometry;

typedef struct TinyDFF_Read_Atomic {
    TinyDFF_Read_Int32 frameIndex;
    TinyDFF_Read_Int32 geometryIndex;

    bool hasRightToRender;
    TinyDFF_Read_UInt32 rightToRenderVal1;
    TinyDFF_Read_UInt32 rightToRenderVal2;

    bool hasParticles;
    TinyDFF_Read_UInt32 particlesVal;

    bool hasPipelineSet;
    TinyDFF_Read_UInt32 pipelineSetVal;

    bool hasMaterialFx;
    TinyDFF_Read_UInt32 materialFxVal;
} TinyDFF_Read_Atomic;

typedef struct TinyDFF_Read_Frame {
    float rotationMatrix[9];
    float position[3];
    TinyDFF_Read_Int32 parent;
    char name[TINYDFF_READ_MAX_NAME_LEN];

    bool hasHAnim;
    TinyDFF_Read_UInt32 hAnimUnknown1;
    TinyDFF_Read_Int32 hAnimBoneId;
    TinyDFF_Read_UInt32 hAnimBoneCount;
    TinyDFF_Read_Int32* hAnimBoneIds;
    TinyDFF_Read_UInt32* hAnimBoneNumbers;
    TinyDFF_Read_UInt32* hAnimBoneTypes;
} TinyDFF_Read_Frame;

typedef struct TinyDFF_Read_Clump {
    TinyDFF_Read_UInt32 version;
    TinyDFF_Read_UInt32 atomicCount;
    TinyDFF_Read_UInt32 geometryCount;
    TinyDFF_Read_UInt32 frameCount;

    TinyDFF_Read_Atomic* atomics;
    TinyDFF_Read_Geometry* geometries;
    TinyDFF_Read_Frame* frames;

    bool hasCollision;
    TinyDFF_Read_UInt32 colDataSize;
    TinyDFF_Read_UInt8* colData;
} TinyDFF_Read_Clump;

struct TinyDFF_Read_Context {
    TinyDFF_Read_Callbacks callbacks;
    void* user;
    TinyDFF_Read_Clump clump;
    bool valid;
};

TinyDFF_Read_Handle TinyDFF_Read_CreateContext(const TinyDFF_Read_Callbacks* callbacks, void* user);
void TinyDFF_Read_DestroyContext(TinyDFF_Read_Handle handle);
void TinyDFF_Read_Reset(TinyDFF_Read_Handle handle);
bool TinyDFF_Read_Read(TinyDFF_Read_Handle handle);

TinyDFF_Read_UInt32 TinyDFF_Read_GetVersion(TinyDFF_Read_Handle handle);
TinyDFF_Read_UInt32 TinyDFF_Read_GetAtomicCount(TinyDFF_Read_Handle handle);
TinyDFF_Read_UInt32 TinyDFF_Read_GetGeometryCount(TinyDFF_Read_Handle handle);
TinyDFF_Read_UInt32 TinyDFF_Read_GetFrameCount(TinyDFF_Read_Handle handle);

bool TinyDFF_Read_GetGeometryInfo(TinyDFF_Read_Handle handle, TinyDFF_Read_UInt32 index,
                             TinyDFF_Read_UInt32* vertexCount,
                             TinyDFF_Read_UInt32* faceCount,
                             TinyDFF_Read_UInt32* flags,
                             TinyDFF_Read_UInt32* numUVs);

const TinyDFF_Read_Vector3* TinyDFF_Read_GetVertices(TinyDFF_Read_Handle handle, TinyDFF_Read_UInt32 geomIndex);
const TinyDFF_Read_Vector3* TinyDFF_Read_GetNormals(TinyDFF_Read_Handle handle, TinyDFF_Read_UInt32 geomIndex);
const TinyDFF_Read_Vector2* TinyDFF_Read_GetUVs(TinyDFF_Read_Handle handle, TinyDFF_Read_UInt32 geomIndex, TinyDFF_Read_UInt32 uvIndex);
const TinyDFF_Read_Color*   TinyDFF_Read_GetVertexColors(TinyDFF_Read_Handle handle, TinyDFF_Read_UInt32 geomIndex);
const TinyDFF_Read_Face*    TinyDFF_Read_GetFaces(TinyDFF_Read_Handle handle, TinyDFF_Read_UInt32 geomIndex);

bool TinyDFF_Read_GetBinMeshInfo(TinyDFF_Read_Handle handle, TinyDFF_Read_UInt32 geomIndex,
                            TinyDFF_Read_UInt32* faceType,
                            TinyDFF_Read_UInt32* numIndices,
                            TinyDFF_Read_UInt32* splitCount);
const TinyDFF_Read_Split* TinyDFF_Read_GetBinMeshSplit(TinyDFF_Read_Handle handle, TinyDFF_Read_UInt32 geomIndex, TinyDFF_Read_UInt32 splitIndex);

TinyDFF_Read_UInt32 TinyDFF_Read_GetMaterialCount(TinyDFF_Read_Handle handle, TinyDFF_Read_UInt32 geomIndex);
const TinyDFF_Read_Material* TinyDFF_Read_GetMaterial(TinyDFF_Read_Handle handle, TinyDFF_Read_UInt32 geomIndex, TinyDFF_Read_UInt32 matIndex);
const char* TinyDFF_Read_GetMaterialName(TinyDFF_Read_Handle handle, TinyDFF_Read_UInt32 geomIndex, TinyDFF_Read_UInt32 matIndex);
const char* TinyDFF_Read_GetMaterialMaskName(TinyDFF_Read_Handle handle, TinyDFF_Read_UInt32 geomIndex, TinyDFF_Read_UInt32 matIndex);
bool TinyDFF_Read_GetMaterialColor(TinyDFF_Read_Handle handle, TinyDFF_Read_UInt32 geomIndex, TinyDFF_Read_UInt32 matIndex, TinyDFF_Read_Color* color);
bool TinyDFF_Read_GetMaterialSurfaceProps(TinyDFF_Read_Handle handle, TinyDFF_Read_UInt32 geomIndex, TinyDFF_Read_UInt32 matIndex, float* ambient, float* specular, float* diffuse);
bool TinyDFF_Read_MaterialHasMatFx(TinyDFF_Read_Handle handle, TinyDFF_Read_UInt32 geomIndex, TinyDFF_Read_UInt32 matIndex);
const TinyDFF_Read_MatFx* TinyDFF_Read_GetMaterialMatFx(TinyDFF_Read_Handle handle, TinyDFF_Read_UInt32 geomIndex, TinyDFF_Read_UInt32 matIndex);
bool TinyDFF_Read_MaterialHasReflection(TinyDFF_Read_Handle handle, TinyDFF_Read_UInt32 geomIndex, TinyDFF_Read_UInt32 matIndex);
bool TinyDFF_Read_GetMaterialReflection(TinyDFF_Read_Handle handle, TinyDFF_Read_UInt32 geomIndex, TinyDFF_Read_UInt32 matIndex, float* amount, float* intensity);
bool TinyDFF_Read_MaterialHasSpecular(TinyDFF_Read_Handle handle, TinyDFF_Read_UInt32 geomIndex, TinyDFF_Read_UInt32 matIndex);
bool TinyDFF_Read_GetMaterialSpecular(TinyDFF_Read_Handle handle, TinyDFF_Read_UInt32 geomIndex, TinyDFF_Read_UInt32 matIndex, float* level, const char** name);

bool TinyDFF_Read_GetFrameMatrix(TinyDFF_Read_Handle handle, TinyDFF_Read_UInt32 frameIndex, float matrix[9], float position[3]);
const char* TinyDFF_Read_GetFrameName(TinyDFF_Read_Handle handle, TinyDFF_Read_UInt32 frameIndex);
TinyDFF_Read_Int32 TinyDFF_Read_GetFrameParent(TinyDFF_Read_Handle handle, TinyDFF_Read_UInt32 frameIndex);

bool TinyDFF_Read_HasSkin(TinyDFF_Read_Handle handle, TinyDFF_Read_UInt32 geomIndex);
TinyDFF_Read_UInt32 TinyDFF_Read_GetBoneCount(TinyDFF_Read_Handle handle, TinyDFF_Read_UInt32 geomIndex);
const TinyDFF_Read_UInt32* TinyDFF_Read_GetBoneIndices(TinyDFF_Read_Handle handle, TinyDFF_Read_UInt32 geomIndex);
const TinyDFF_Read_Float* TinyDFF_Read_GetBoneWeights(TinyDFF_Read_Handle handle, TinyDFF_Read_UInt32 geomIndex);
const TinyDFF_Read_Float* TinyDFF_Read_GetInverseMatrices(TinyDFF_Read_Handle handle, TinyDFF_Read_UInt32 geomIndex);

#ifdef TINYDFF_READ_IMPLEMENTATION

#include <stdlib.h>

// Functions

static TinyDFF_Read_UInt32 TinyDFF_Read_ReadUInt32(TinyDFF_Read_Context* ctx) {
    TinyDFF_Read_UInt32 val;
    ctx->callbacks.readFn(ctx->user, &val, 4);
    return val;
}

static TinyDFF_Read_UInt16 TinyDFF_Read_ReadUInt16(TinyDFF_Read_Context* ctx) {
    TinyDFF_Read_UInt16 val;
    ctx->callbacks.readFn(ctx->user, &val, 2);
    return val;
}

static TinyDFF_Read_UInt8 TinyDFF_Read_ReadUInt8(TinyDFF_Read_Context* ctx) {
    TinyDFF_Read_UInt8 val;
    ctx->callbacks.readFn(ctx->user, &val, 1);
    return val;
}

static float TinyDFF_Read_ReadFloat(TinyDFF_Read_Context* ctx) {
    float val;
    ctx->callbacks.readFn(ctx->user, &val, 4);
    return val;
}

static bool TinyDFF_Read_ReadHeader(TinyDFF_Read_Context* ctx, TinyDFF_Read_UInt32* type, TinyDFF_Read_UInt32* length, TinyDFF_Read_UInt32* build) {
    TinyDFF_Read_UInt32 buf[3];
    if (ctx->callbacks.readFn(ctx->user, buf, 12) != 12)
        return false;
    if (type) *type = buf[0];
    if (length) *length = buf[1];
    if (build) *build = buf[2];
    return true;
}

static void TinyDFF_Read_Skip(TinyDFF_Read_Context* ctx, TinyDFF_Read_UInt32 bytes) {
    if (bytes) ctx->callbacks.seekFn(ctx->user, bytes);
}

static bool TinyDFF_Read_ReadString(TinyDFF_Read_Context* ctx, char* buf, TinyDFF_Read_UInt32 maxLen) {
    TinyDFF_Read_UInt32 len = 0;
    char c;
    do {
        if (ctx->callbacks.readFn(ctx->user, &c, 1) != 1)
            return false;
        if (len < maxLen - 1)
            buf[len++] = c;
    } while (c != '\0');
    buf[len] = '\0';

    TinyDFF_Read_UInt32 pos = (TinyDFF_Read_UInt32)ctx->callbacks.tellFn(ctx->user);
    TinyDFF_Read_UInt32 aligned = (pos + 3) & ~3;
    if (aligned > pos)
        ctx->callbacks.seekFn(ctx->user, aligned - pos);

    return true;
}

// Parser Bin Mesh

static bool TinyDFF_Read_ParseBinMesh(TinyDFF_Read_Context* ctx, TinyDFF_Read_Geometry* geom, TinyDFF_Read_UInt32 binMeshLen) {
    geom->binMesh.faceType = TinyDFF_Read_ReadUInt32(ctx);
    geom->binMesh.splitCount = TinyDFF_Read_ReadUInt32(ctx);
    geom->binMesh.numIndices = TinyDFF_Read_ReadUInt32(ctx);
    
    if (geom->binMesh.splitCount > 10000) {
        geom->binMesh.splitCount = 0;
        geom->binMesh.splits = NULL;
        TinyDFF_Read_Skip(ctx, binMeshLen - 12);
        return true;
    }
    
    geom->binMesh.splits = (TinyDFF_Read_Split*)ctx->callbacks.allocFn(ctx->user,
        geom->binMesh.splitCount * sizeof(TinyDFF_Read_Split));
    
    if (!geom->binMesh.splits) return false;
    memset(geom->binMesh.splits, 0, geom->binMesh.splitCount * sizeof(TinyDFF_Read_Split));
    
    bool hasData = binMeshLen > 12 + geom->binMesh.splitCount * 8;
    
    for (TinyDFF_Read_UInt32 i = 0; i < geom->binMesh.splitCount; i++) {
        TinyDFF_Read_UInt32 indexCount = TinyDFF_Read_ReadUInt32(ctx);
        geom->binMesh.splits[i].matIndex = TinyDFF_Read_ReadUInt32(ctx);
        geom->binMesh.splits[i].indexCount = indexCount;
        
        if (hasData && indexCount > 0 && indexCount <= 100000) {
            geom->binMesh.splits[i].indices = (TinyDFF_Read_UInt16*)ctx->callbacks.allocFn(ctx->user,
                indexCount * sizeof(TinyDFF_Read_UInt16));
            if (geom->binMesh.splits[i].indices) {
                for (TinyDFF_Read_UInt32 j = 0; j < indexCount; j++) {
                    geom->binMesh.splits[i].indices[j] = TinyDFF_Read_ReadUInt16(ctx);
                }
            }
        }
    }
    
    return true;
}

// Parser Textures

static bool TinyDFF_Read_ParseTexture(TinyDFF_Read_Context* ctx, TinyDFF_Read_Texture* tex) {
    TinyDFF_Read_UInt32 type, length, build;

    if (!TinyDFF_Read_ReadHeader(ctx, &type, &length, &build) || type != 0x06)
        return false;

    if (!TinyDFF_Read_ReadHeader(ctx, &type, &length, &build) || type != 0x01)
        return false;

    tex->filterFlags = TinyDFF_Read_ReadUInt16(ctx);
    TinyDFF_Read_Skip(ctx, 2);

    if (!TinyDFF_Read_ReadHeader(ctx, &type, &length, &build) || type != 0x02)
        return false;
    TinyDFF_Read_ReadString(ctx, tex->name, TINYDFF_READ_MAX_NAME_LEN);

    if (!TinyDFF_Read_ReadHeader(ctx, &type, &length, &build) || type != 0x02)
        return false;
    TinyDFF_Read_ReadString(ctx, tex->maskName, TINYDFF_READ_MAX_NAME_LEN);

    if (!TinyDFF_Read_ReadHeader(ctx, &type, &length, &build) || type != 0x03)
        return false;

    TinyDFF_Read_Skip(ctx, length);
    return true;
}

// Parser Material FX

static bool TinyDFF_Read_ParseMatFx(TinyDFF_Read_Context* ctx, TinyDFF_Read_MatFx* matFx, TinyDFF_Read_UInt32 length) {
    matFx->type = TinyDFF_Read_ReadUInt32(ctx);
    
    switch (matFx->type) {
        case TINYDFF_READ_MATFX_BUMPMAP:
            TinyDFF_Read_Skip(ctx, 4);
            matFx->bumpCoeff = TinyDFF_Read_ReadFloat(ctx);
            matFx->hasTex1 = TinyDFF_Read_ReadUInt32(ctx) != 0;
            if (matFx->hasTex1) {
                memset(&matFx->tex1, 0, sizeof(TinyDFF_Read_Texture));
                TinyDFF_Read_ParseTexture(ctx, &matFx->tex1);
            }
            matFx->hasTex2 = TinyDFF_Read_ReadUInt32(ctx) != 0;
            if (matFx->hasTex2) {
                memset(&matFx->tex2, 0, sizeof(TinyDFF_Read_Texture));
                TinyDFF_Read_ParseTexture(ctx, &matFx->tex2);
            }
            TinyDFF_Read_Skip(ctx, 4);
            break;
            
        case TINYDFF_READ_MATFX_ENVMAP:
            TinyDFF_Read_Skip(ctx, 4);
            matFx->envCoeff = TinyDFF_Read_ReadFloat(ctx);
            matFx->hasTex1 = TinyDFF_Read_ReadUInt32(ctx) != 0;
            if (matFx->hasTex1) {
                memset(&matFx->tex1, 0, sizeof(TinyDFF_Read_Texture));
                TinyDFF_Read_ParseTexture(ctx, &matFx->tex1);
            }
            matFx->hasTex2 = TinyDFF_Read_ReadUInt32(ctx) != 0;
            if (matFx->hasTex2) {
                memset(&matFx->tex2, 0, sizeof(TinyDFF_Read_Texture));
                TinyDFF_Read_ParseTexture(ctx, &matFx->tex2);
            }
            TinyDFF_Read_Skip(ctx, 4);
            break;
            
        case TINYDFF_READ_MATFX_BUMPENVMAP:
            TinyDFF_Read_Skip(ctx, 4);
            matFx->bumpCoeff = TinyDFF_Read_ReadFloat(ctx);
            matFx->hasTex1 = TinyDFF_Read_ReadUInt32(ctx) != 0;
            if (matFx->hasTex1) {
                memset(&matFx->tex1, 0, sizeof(TinyDFF_Read_Texture));
                TinyDFF_Read_ParseTexture(ctx, &matFx->tex1);
            }
            TinyDFF_Read_Skip(ctx, 4);
            
            TinyDFF_Read_Skip(ctx, 4);
            matFx->envCoeff = TinyDFF_Read_ReadFloat(ctx);
            TinyDFF_Read_Skip(ctx, 4);
            matFx->hasTex2 = TinyDFF_Read_ReadUInt32(ctx) != 0;
            if (matFx->hasTex2) {
                memset(&matFx->tex2, 0, sizeof(TinyDFF_Read_Texture));
                TinyDFF_Read_ParseTexture(ctx, &matFx->tex2);
            }
            break;
            
        case TINYDFF_READ_MATFX_DUAL:
            TinyDFF_Read_Skip(ctx, 4);
            matFx->srcBlend = TinyDFF_Read_ReadFloat(ctx);
            matFx->destBlend = TinyDFF_Read_ReadFloat(ctx);
            matFx->hasDualPassMap = TinyDFF_Read_ReadUInt32(ctx) != 0;
            if (matFx->hasDualPassMap) {
                memset(&matFx->dualPassMap, 0, sizeof(TinyDFF_Read_Texture));
                TinyDFF_Read_ParseTexture(ctx, &matFx->dualPassMap);
            }
            TinyDFF_Read_Skip(ctx, 4);
            break;
            
        default:
            break;
    }
    
    return true;
}

// Parser Meterials

static bool TinyDFF_Read_ParseMaterial(TinyDFF_Read_Context* ctx, TinyDFF_Read_Material* mat) {
    TinyDFF_Read_UInt32 type, length, build;

    if (!TinyDFF_Read_ReadHeader(ctx, &type, &length, &build) || type != 0x07)
        return false;

    if (!TinyDFF_Read_ReadHeader(ctx, &type, &length, &build) || type != 0x01)
        return false;

    mat->flags = TinyDFF_Read_ReadUInt32(ctx);
    mat->color.r = TinyDFF_Read_ReadUInt8(ctx);
    mat->color.g = TinyDFF_Read_ReadUInt8(ctx);
    mat->color.b = TinyDFF_Read_ReadUInt8(ctx);
    mat->color.a = TinyDFF_Read_ReadUInt8(ctx);
    mat->unknown = TinyDFF_Read_ReadUInt32(ctx);
    mat->hasTexture = TinyDFF_Read_ReadUInt32(ctx) != 0;
    mat->surfaceProps[0] = TinyDFF_Read_ReadFloat(ctx);
    mat->surfaceProps[1] = TinyDFF_Read_ReadFloat(ctx);
    mat->surfaceProps[2] = TinyDFF_Read_ReadFloat(ctx);

    if (mat->hasTexture) {
        memset(&mat->texture, 0, sizeof(TinyDFF_Read_Texture));
        if (!TinyDFF_Read_ParseTexture(ctx, &mat->texture))
            return false;
    }

    if (!TinyDFF_Read_ReadHeader(ctx, &type, &length, &build) || type != 0x03)
        return false;

    TinyDFF_Read_UInt32 extEnd = (TinyDFF_Read_UInt32)(ctx->callbacks.tellFn(ctx->user) + length);
    
    while ((TinyDFF_Read_UInt32)ctx->callbacks.tellFn(ctx->user) < extEnd) {
        if (!TinyDFF_Read_ReadHeader(ctx, &type, &length, &build))
            break;
        
        switch (type) {
            case 0x1F:
                mat->hasRightToRender = true;
                mat->rightToRenderVal1 = TinyDFF_Read_ReadUInt32(ctx);
                mat->rightToRenderVal2 = TinyDFF_Read_ReadUInt32(ctx);
                break;
                
            case 0x120:
                mat->hasMatFx = true;
                memset(&mat->matFx, 0, sizeof(TinyDFF_Read_MatFx));
                TinyDFF_Read_ParseMatFx(ctx, &mat->matFx, length);
                break;
                
            case 0x253F2FC:
                mat->hasReflectionMat = true;
                mat->reflectionAmount[0] = TinyDFF_Read_ReadFloat(ctx);
                mat->reflectionAmount[1] = TinyDFF_Read_ReadFloat(ctx);
                mat->reflectionAmount[2] = TinyDFF_Read_ReadFloat(ctx);
                mat->reflectionAmount[3] = TinyDFF_Read_ReadFloat(ctx);
                mat->reflectionIntensity = TinyDFF_Read_ReadFloat(ctx);
                TinyDFF_Read_Skip(ctx, 4);
                break;
                
            case 0x253F2F6:
                mat->hasSpecularMat = true;
                mat->specularLevel = TinyDFF_Read_ReadFloat(ctx);
                TinyDFF_Read_ReadString(ctx, mat->specularName, TINYDFF_READ_MAX_NAME_LEN);
                TinyDFF_Read_Skip(ctx, 4);
                break;
                
            case 0x135:
                mat->hasUVAnim = true;
                if (TinyDFF_Read_ReadHeader(ctx, &type, &length, &build) && type == 0x01) {
                    mat->uvVal = TinyDFF_Read_ReadUInt32(ctx);
                    TinyDFF_Read_ReadString(ctx, mat->uvName, TINYDFF_READ_MAX_NAME_LEN);
                }
                break;
                
            default:
                TinyDFF_Read_Skip(ctx, length);
                break;
        }
    }

    return true;
}

// Parser Geomettry

static bool TinyDFF_Read_ParseGeometry(TinyDFF_Read_Context* ctx, TinyDFF_Read_Geometry* geom) {
    TinyDFF_Read_UInt32 type, length, build;
    TinyDFF_Read_UInt32 start, end;

    if (!TinyDFF_Read_ReadHeader(ctx, &type, &length, &build) || type != 0x0F)
        return false;

    start = (TinyDFF_Read_UInt32)(ctx->callbacks.tellFn(ctx->user) - 12);
    end = start + length;

    if (!TinyDFF_Read_ReadHeader(ctx, &type, &length, &build) || type != 0x01)
        return false;

    geom->flags = TinyDFF_Read_ReadUInt16(ctx);
    geom->numUVs = TinyDFF_Read_ReadUInt8(ctx);
    geom->hasNativeGeometry = TinyDFF_Read_ReadUInt8(ctx) != 0;

    TinyDFF_Read_UInt32 triCount = TinyDFF_Read_ReadUInt32(ctx);
    geom->vertexCount = TinyDFF_Read_ReadUInt32(ctx);
    TinyDFF_Read_Skip(ctx, 4);

    if (ctx->clump.version < 0x34000)
        TinyDFF_Read_Skip(ctx, 12);

    if (geom->vertexCount > 1000000) return false;

    if (!geom->hasNativeGeometry) {
        if (geom->flags & TINYDFF_READ_FLAG_PRELIT) {
            geom->vertexColors = (TinyDFF_Read_Color*)ctx->callbacks.allocFn(ctx->user, geom->vertexCount * sizeof(TinyDFF_Read_Color));
            if (geom->vertexColors)
                ctx->callbacks.readFn(ctx->user, geom->vertexColors, geom->vertexCount * sizeof(TinyDFF_Read_Color));
        }

        if (geom->flags & TINYDFF_READ_FLAG_TEXTURED) {
            geom->texCoords[0] = (TinyDFF_Read_Vector2*)ctx->callbacks.allocFn(ctx->user, geom->vertexCount * sizeof(TinyDFF_Read_Vector2));
            if (geom->texCoords[0])
                ctx->callbacks.readFn(ctx->user, geom->texCoords[0], geom->vertexCount * sizeof(TinyDFF_Read_Vector2));
        }

        if (geom->flags & TINYDFF_READ_FLAG_TEXTURED2) {
            for (TinyDFF_Read_UInt32 i = 0; i < geom->numUVs && i < TINYDFF_READ_MAX_UVS; i++) {
                geom->texCoords[i] = (TinyDFF_Read_Vector2*)ctx->callbacks.allocFn(ctx->user, geom->vertexCount * sizeof(TinyDFF_Read_Vector2));
                if (geom->texCoords[i])
                    ctx->callbacks.readFn(ctx->user, geom->texCoords[i], geom->vertexCount * sizeof(TinyDFF_Read_Vector2));
            }
        }

        geom->faces = (TinyDFF_Read_Face*)ctx->callbacks.allocFn(ctx->user, triCount * sizeof(TinyDFF_Read_Face));
        if (geom->faces) {
            for (TinyDFF_Read_UInt32 i = 0; i < triCount; i++) {
                geom->faces[i].a = TinyDFF_Read_ReadUInt16(ctx);
                geom->faces[i].b = TinyDFF_Read_ReadUInt16(ctx);
                geom->faces[i].c = TinyDFF_Read_ReadUInt16(ctx);
                geom->faces[i].materialIndex = TinyDFF_Read_ReadUInt16(ctx);
            }
        }
    }

    for (int i = 0; i < 4; i++)
        geom->boundingSphere[i] = TinyDFF_Read_ReadFloat(ctx);

    TinyDFF_Read_ReadUInt32(ctx);
    TinyDFF_Read_ReadUInt32(ctx);

    if (!geom->hasNativeGeometry) {
        geom->vertices = (TinyDFF_Read_Vector3*)ctx->callbacks.allocFn(ctx->user, geom->vertexCount * sizeof(TinyDFF_Read_Vector3));
        if (geom->vertices)
            ctx->callbacks.readFn(ctx->user, geom->vertices, geom->vertexCount * sizeof(TinyDFF_Read_Vector3));

        if (geom->flags & TINYDFF_READ_FLAG_NORMALS) {
            geom->normals = (TinyDFF_Read_Vector3*)ctx->callbacks.allocFn(ctx->user, geom->vertexCount * sizeof(TinyDFF_Read_Vector3));
            if (geom->normals)
                ctx->callbacks.readFn(ctx->user, geom->normals, geom->vertexCount * sizeof(TinyDFF_Read_Vector3));
        }
    }

    if (!TinyDFF_Read_ReadHeader(ctx, &type, &length, &build) || type != 0x08)
        return false;

    if (!TinyDFF_Read_ReadHeader(ctx, &type, &length, &build) || type != 0x01)
        return false;

    geom->materialCount = TinyDFF_Read_ReadUInt32(ctx);
    TinyDFF_Read_Skip(ctx, geom->materialCount * 4);

    geom->materials = (TinyDFF_Read_Material*)ctx->callbacks.allocFn(ctx->user, geom->materialCount * sizeof(TinyDFF_Read_Material));
    if (geom->materials) {
        for (TinyDFF_Read_UInt32 i = 0; i < geom->materialCount; i++) {
            memset(&geom->materials[i], 0, sizeof(TinyDFF_Read_Material));
            if (!TinyDFF_Read_ParseMaterial(ctx, &geom->materials[i]))
                return false;
        }
    }

    if (!TinyDFF_Read_ReadHeader(ctx, &type, &length, &build) || type != 0x03)
        return false;

    TinyDFF_Read_UInt32 extEnd = (TinyDFF_Read_UInt32)(ctx->callbacks.tellFn(ctx->user) + length);
    
    if (!TinyDFF_Read_ReadHeader(ctx, &type, &length, &build))
        return false;

    if (type == 0x50E) {
        if (!TinyDFF_Read_ParseBinMesh(ctx, geom, length))
            return false;
    } else {
        TinyDFF_Read_Skip(ctx, length);
    }
    
    TinyDFF_Read_UInt32 currentPos = (TinyDFF_Read_UInt32)ctx->callbacks.tellFn(ctx->user);
    if (currentPos < extEnd)
        TinyDFF_Read_Skip(ctx, extEnd - currentPos);

    currentPos = (TinyDFF_Read_UInt32)ctx->callbacks.tellFn(ctx->user);
    if (currentPos < end)
        TinyDFF_Read_Skip(ctx, end - currentPos);

    return true;
}

// Parser Atomic

static bool TinyDFF_Read_ParseAtomic(TinyDFF_Read_Context* ctx, TinyDFF_Read_Atomic* atomic) {
    TinyDFF_Read_UInt32 type, length, build;

    if (!TinyDFF_Read_ReadHeader(ctx, &type, &length, &build) || type != 0x14)
        return false;

    if (!TinyDFF_Read_ReadHeader(ctx, &type, &length, &build) || type != 0x01)
        return false;

    atomic->frameIndex = TinyDFF_Read_ReadUInt32(ctx);
    atomic->geometryIndex = TinyDFF_Read_ReadUInt32(ctx);
    TinyDFF_Read_Skip(ctx, 8);

    if (!TinyDFF_Read_ReadHeader(ctx, &type, &length, &build) || type != 0x03)
        return false;

    TinyDFF_Read_Skip(ctx, length);
    return true;
}

// Parser Frame

static bool TinyDFF_Read_ParseFrame(TinyDFF_Read_Context* ctx, TinyDFF_Read_Frame* frame) {
    for (int i = 0; i < 9; i++)
        frame->rotationMatrix[i] = TinyDFF_Read_ReadFloat(ctx);

    frame->position[0] = TinyDFF_Read_ReadFloat(ctx);
    frame->position[1] = TinyDFF_Read_ReadFloat(ctx);
    frame->position[2] = TinyDFF_Read_ReadFloat(ctx);

    frame->parent = TinyDFF_Read_ReadUInt32(ctx);
    TinyDFF_Read_Skip(ctx, 4);

    TinyDFF_Read_UInt32 type, length, build;
    if (!TinyDFF_Read_ReadHeader(ctx, &type, &length, &build) || type != 0x03)
        return false;

    TinyDFF_Read_UInt32 extEnd = (TinyDFF_Read_UInt32)(ctx->callbacks.tellFn(ctx->user) + length);
    while ((TinyDFF_Read_UInt32)ctx->callbacks.tellFn(ctx->user) < extEnd) {
        if (!TinyDFF_Read_ReadHeader(ctx, &type, &length, &build))
            break;

        if (type == 0x253F2FE) {
            TinyDFF_Read_ReadString(ctx, frame->name, TINYDFF_READ_MAX_NAME_LEN);
        } else {
            TinyDFF_Read_Skip(ctx, length);
        }
    }

    return true;
}

// Reader

bool TinyDFF_Read_Read(TinyDFF_Read_Handle handle) {
    TinyDFF_Read_Context* ctx = (TinyDFF_Read_Context*)handle;
    if (!ctx) return false;
    
    TinyDFF_Read_UInt32 type, length, build;
    bool foundClump = false;

    while (TinyDFF_Read_ReadHeader(ctx, &type, &length, &build)) {
        if (type == 0x10) {
            foundClump = true;
            break;
        }
        if (type == 0) return false;
        TinyDFF_Read_Skip(ctx, length);
    }

    if (!foundClump) return false;

    ctx->clump.version = build;
    TinyDFF_Read_UInt32 clumpEnd = (TinyDFF_Read_UInt32)(ctx->callbacks.tellFn(ctx->user) + length);

    if (!TinyDFF_Read_ReadHeader(ctx, &type, &length, &build) || type != 0x01)
        return false;

    ctx->clump.atomicCount = TinyDFF_Read_ReadUInt32(ctx);
    
    if (length == 0xC) {
        TinyDFF_Read_ReadUInt32(ctx);
        TinyDFF_Read_Skip(ctx, 4);
    }

    if (!TinyDFF_Read_ReadHeader(ctx, &type, &length, &build) || type != 0x0E)
        return false;

    if (!TinyDFF_Read_ReadHeader(ctx, &type, &length, &build) || type != 0x01)
        return false;

    ctx->clump.frameCount = TinyDFF_Read_ReadUInt32(ctx);
    
    ctx->clump.frames = (TinyDFF_Read_Frame*)ctx->callbacks.allocFn(ctx->user, ctx->clump.frameCount * sizeof(TinyDFF_Read_Frame));

    for (TinyDFF_Read_UInt32 i = 0; i < ctx->clump.frameCount; i++) {
        memset(&ctx->clump.frames[i], 0, sizeof(TinyDFF_Read_Frame));
        if (!TinyDFF_Read_ParseFrame(ctx, &ctx->clump.frames[i]))
            return false;
    }

    if (!TinyDFF_Read_ReadHeader(ctx, &type, &length, &build) || type != 0x1A)
        return false;

    if (!TinyDFF_Read_ReadHeader(ctx, &type, &length, &build) || type != 0x01)
        return false;

    ctx->clump.geometryCount = TinyDFF_Read_ReadUInt32(ctx);
    
    ctx->clump.geometries = (TinyDFF_Read_Geometry*)ctx->callbacks.allocFn(ctx->user, ctx->clump.geometryCount * sizeof(TinyDFF_Read_Geometry));

    for (TinyDFF_Read_UInt32 i = 0; i < ctx->clump.geometryCount; i++) {
        memset(&ctx->clump.geometries[i], 0, sizeof(TinyDFF_Read_Geometry));
        if (!TinyDFF_Read_ParseGeometry(ctx, &ctx->clump.geometries[i]))
            return false;
    }

    ctx->clump.atomics = (TinyDFF_Read_Atomic*)ctx->callbacks.allocFn(ctx->user, ctx->clump.atomicCount * sizeof(TinyDFF_Read_Atomic));
    for (TinyDFF_Read_UInt32 i = 0; i < ctx->clump.atomicCount; i++) {
        memset(&ctx->clump.atomics[i], 0, sizeof(TinyDFF_Read_Atomic));
        if (!TinyDFF_Read_ParseAtomic(ctx, &ctx->clump.atomics[i]))
            return false;
    }

    while ((TinyDFF_Read_UInt32)ctx->callbacks.tellFn(ctx->user) < clumpEnd) {
        if (!TinyDFF_Read_ReadHeader(ctx, &type, &length, &build)) break;
        TinyDFF_Read_Skip(ctx, length);
    }

    ctx->valid = true;
    return true;
}

// Getters

TinyDFF_Read_UInt32 TinyDFF_Read_GetVersion(TinyDFF_Read_Handle handle) {
    TinyDFF_Read_Context* ctx = (TinyDFF_Read_Context*)handle;
    return ctx && ctx->valid ? ctx->clump.version : 0;
}

TinyDFF_Read_UInt32 TinyDFF_Read_GetAtomicCount(TinyDFF_Read_Handle handle) {
    TinyDFF_Read_Context* ctx = (TinyDFF_Read_Context*)handle;
    return ctx && ctx->valid ? ctx->clump.atomicCount : 0;
}

TinyDFF_Read_UInt32 TinyDFF_Read_GetGeometryCount(TinyDFF_Read_Handle handle) {
    TinyDFF_Read_Context* ctx = (TinyDFF_Read_Context*)handle;
    return ctx && ctx->valid ? ctx->clump.geometryCount : 0;
}

TinyDFF_Read_UInt32 TinyDFF_Read_GetFrameCount(TinyDFF_Read_Handle handle) {
    TinyDFF_Read_Context* ctx = (TinyDFF_Read_Context*)handle;
    return ctx && ctx->valid ? ctx->clump.frameCount : 0;
}

bool TinyDFF_Read_GetGeometryInfo(TinyDFF_Read_Handle handle, TinyDFF_Read_UInt32 index,
                             TinyDFF_Read_UInt32* vertexCount,
                             TinyDFF_Read_UInt32* faceCount,
                             TinyDFF_Read_UInt32* flags,
                             TinyDFF_Read_UInt32* numUVs) {
    TinyDFF_Read_Context* ctx = (TinyDFF_Read_Context*)handle;
    if (!ctx || !ctx->valid || index >= ctx->clump.geometryCount)
        return false;

    TinyDFF_Read_Geometry* geom = &ctx->clump.geometries[index];
    if (vertexCount) *vertexCount = geom->vertexCount;
    if (faceCount) *faceCount = 0;
    if (flags) *flags = geom->flags;
    if (numUVs) *numUVs = geom->numUVs;
    return true;
}

const TinyDFF_Read_Vector3* TinyDFF_Read_GetVertices(TinyDFF_Read_Handle handle, TinyDFF_Read_UInt32 geomIndex) {
    TinyDFF_Read_Context* ctx = (TinyDFF_Read_Context*)handle;
    if (!ctx || !ctx->valid || geomIndex >= ctx->clump.geometryCount)
        return NULL;
    return ctx->clump.geometries[geomIndex].vertices;
}

const TinyDFF_Read_Vector3* TinyDFF_Read_GetNormals(TinyDFF_Read_Handle handle, TinyDFF_Read_UInt32 geomIndex) {
    TinyDFF_Read_Context* ctx = (TinyDFF_Read_Context*)handle;
    if (!ctx || !ctx->valid || geomIndex >= ctx->clump.geometryCount)
        return NULL;
    return ctx->clump.geometries[geomIndex].normals;
}

const TinyDFF_Read_Vector2* TinyDFF_Read_GetUVs(TinyDFF_Read_Handle handle, TinyDFF_Read_UInt32 geomIndex, TinyDFF_Read_UInt32 uvIndex) {
    TinyDFF_Read_Context* ctx = (TinyDFF_Read_Context*)handle;
    if (!ctx || !ctx->valid || geomIndex >= ctx->clump.geometryCount || uvIndex >= TINYDFF_READ_MAX_UVS)
        return NULL;
    return ctx->clump.geometries[geomIndex].texCoords[uvIndex];
}

const TinyDFF_Read_Color* TinyDFF_Read_GetVertexColors(TinyDFF_Read_Handle handle, TinyDFF_Read_UInt32 geomIndex) {
    TinyDFF_Read_Context* ctx = (TinyDFF_Read_Context*)handle;
    if (!ctx || !ctx->valid || geomIndex >= ctx->clump.geometryCount)
        return NULL;
    return ctx->clump.geometries[geomIndex].vertexColors;
}

const TinyDFF_Read_Face* TinyDFF_Read_GetFaces(TinyDFF_Read_Handle handle, TinyDFF_Read_UInt32 geomIndex) {
    TinyDFF_Read_Context* ctx = (TinyDFF_Read_Context*)handle;
    if (!ctx || !ctx->valid || geomIndex >= ctx->clump.geometryCount)
        return NULL;
    return ctx->clump.geometries[geomIndex].faces;
}

bool TinyDFF_Read_GetBinMeshInfo(TinyDFF_Read_Handle handle, TinyDFF_Read_UInt32 geomIndex,
                            TinyDFF_Read_UInt32* faceType,
                            TinyDFF_Read_UInt32* numIndices,
                            TinyDFF_Read_UInt32* splitCount) {
    TinyDFF_Read_Context* ctx = (TinyDFF_Read_Context*)handle;
    if (!ctx || !ctx->valid || geomIndex >= ctx->clump.geometryCount)
        return false;

    TinyDFF_Read_Geometry* geom = &ctx->clump.geometries[geomIndex];
    if (faceType) *faceType = geom->binMesh.faceType;
    if (numIndices) *numIndices = geom->binMesh.numIndices;
    if (splitCount) *splitCount = geom->binMesh.splitCount;
    return true;
}

const TinyDFF_Read_Split* TinyDFF_Read_GetBinMeshSplit(TinyDFF_Read_Handle handle, TinyDFF_Read_UInt32 geomIndex, TinyDFF_Read_UInt32 splitIndex) {
    TinyDFF_Read_Context* ctx = (TinyDFF_Read_Context*)handle;
    if (!ctx || !ctx->valid || geomIndex >= ctx->clump.geometryCount)
        return NULL;

    TinyDFF_Read_Geometry* geom = &ctx->clump.geometries[geomIndex];
    if (splitIndex >= geom->binMesh.splitCount)
        return NULL;

    return &geom->binMesh.splits[splitIndex];
}

TinyDFF_Read_UInt32 TinyDFF_Read_GetMaterialCount(TinyDFF_Read_Handle handle, TinyDFF_Read_UInt32 geomIndex) {
    TinyDFF_Read_Context* ctx = (TinyDFF_Read_Context*)handle;
    if (!ctx || !ctx->valid || geomIndex >= ctx->clump.geometryCount)
        return 0;
    return ctx->clump.geometries[geomIndex].materialCount;
}

const TinyDFF_Read_Material* TinyDFF_Read_GetMaterial(TinyDFF_Read_Handle handle, TinyDFF_Read_UInt32 geomIndex, TinyDFF_Read_UInt32 matIndex) {
    TinyDFF_Read_Context* ctx = (TinyDFF_Read_Context*)handle;
    if (!ctx || !ctx->valid || geomIndex >= ctx->clump.geometryCount)
        return NULL;
    TinyDFF_Read_Geometry* geom = &ctx->clump.geometries[geomIndex];
    if (matIndex >= geom->materialCount)
        return NULL;
    return &geom->materials[matIndex];
}

const char* TinyDFF_Read_GetMaterialName(TinyDFF_Read_Handle handle, TinyDFF_Read_UInt32 geomIndex, TinyDFF_Read_UInt32 matIndex) {
    const TinyDFF_Read_Material* mat = TinyDFF_Read_GetMaterial(handle, geomIndex, matIndex);
    return mat ? mat->texture.name : NULL;
}

const char* TinyDFF_Read_GetMaterialMaskName(TinyDFF_Read_Handle handle, TinyDFF_Read_UInt32 geomIndex, TinyDFF_Read_UInt32 matIndex) {
    const TinyDFF_Read_Material* mat = TinyDFF_Read_GetMaterial(handle, geomIndex, matIndex);
    return mat ? mat->texture.maskName : NULL;
}

bool TinyDFF_Read_GetMaterialColor(TinyDFF_Read_Handle handle, TinyDFF_Read_UInt32 geomIndex, TinyDFF_Read_UInt32 matIndex, TinyDFF_Read_Color* color) {
    const TinyDFF_Read_Material* mat = TinyDFF_Read_GetMaterial(handle, geomIndex, matIndex);
    if (!mat || !color) return false;
    *color = mat->color;
    return true;
}

bool TinyDFF_Read_GetMaterialSurfaceProps(TinyDFF_Read_Handle handle, TinyDFF_Read_UInt32 geomIndex, TinyDFF_Read_UInt32 matIndex, float* ambient, float* specular, float* diffuse) {
    const TinyDFF_Read_Material* mat = TinyDFF_Read_GetMaterial(handle, geomIndex, matIndex);
    if (!mat) return false;
    if (ambient) *ambient = mat->surfaceProps[0];
    if (specular) *specular = mat->surfaceProps[1];
    if (diffuse) *diffuse = mat->surfaceProps[2];
    return true;
}

bool TinyDFF_Read_MaterialHasMatFx(TinyDFF_Read_Handle handle, TinyDFF_Read_UInt32 geomIndex, TinyDFF_Read_UInt32 matIndex) {
    const TinyDFF_Read_Material* mat = TinyDFF_Read_GetMaterial(handle, geomIndex, matIndex);
    return mat ? mat->hasMatFx : false;
}

const TinyDFF_Read_MatFx* TinyDFF_Read_GetMaterialMatFx(TinyDFF_Read_Handle handle, TinyDFF_Read_UInt32 geomIndex, TinyDFF_Read_UInt32 matIndex) {
    const TinyDFF_Read_Material* mat = TinyDFF_Read_GetMaterial(handle, geomIndex, matIndex);
    return (mat && mat->hasMatFx) ? &mat->matFx : NULL;
}

bool TinyDFF_Read_MaterialHasReflection(TinyDFF_Read_Handle handle, TinyDFF_Read_UInt32 geomIndex, TinyDFF_Read_UInt32 matIndex) {
    const TinyDFF_Read_Material* mat = TinyDFF_Read_GetMaterial(handle, geomIndex, matIndex);
    return mat ? mat->hasReflectionMat : false;
}

bool TinyDFF_Read_GetMaterialReflection(TinyDFF_Read_Handle handle, TinyDFF_Read_UInt32 geomIndex, TinyDFF_Read_UInt32 matIndex, float* amount, float* intensity) {
    const TinyDFF_Read_Material* mat = TinyDFF_Read_GetMaterial(handle, geomIndex, matIndex);
    if (!mat || !mat->hasReflectionMat) return false;
    if (amount) memcpy(amount, mat->reflectionAmount, 4 * sizeof(float));
    if (intensity) *intensity = mat->reflectionIntensity;
    return true;
}

bool TinyDFF_Read_MaterialHasSpecular(TinyDFF_Read_Handle handle, TinyDFF_Read_UInt32 geomIndex, TinyDFF_Read_UInt32 matIndex) {
    const TinyDFF_Read_Material* mat = TinyDFF_Read_GetMaterial(handle, geomIndex, matIndex);
    return mat ? mat->hasSpecularMat : false;
}

bool TinyDFF_Read_GetMaterialSpecular(TinyDFF_Read_Handle handle, TinyDFF_Read_UInt32 geomIndex, TinyDFF_Read_UInt32 matIndex, float* level, const char** name) {
    const TinyDFF_Read_Material* mat = TinyDFF_Read_GetMaterial(handle, geomIndex, matIndex);
    if (!mat || !mat->hasSpecularMat) return false;
    if (level) *level = mat->specularLevel;
    if (name) *name = mat->specularName;
    return true;
}

bool TinyDFF_Read_GetFrameMatrix(TinyDFF_Read_Handle handle, TinyDFF_Read_UInt32 frameIndex, float matrix[9], float position[3]) {
    TinyDFF_Read_Context* ctx = (TinyDFF_Read_Context*)handle;
    if (!ctx || !ctx->valid || frameIndex >= ctx->clump.frameCount)
        return false;
    TinyDFF_Read_Frame* frame = &ctx->clump.frames[frameIndex];
    if (matrix) memcpy(matrix, frame->rotationMatrix, 9 * sizeof(float));
    if (position) memcpy(position, frame->position, 3 * sizeof(float));
    return true;
}

const char* TinyDFF_Read_GetFrameName(TinyDFF_Read_Handle handle, TinyDFF_Read_UInt32 frameIndex) {
    TinyDFF_Read_Context* ctx = (TinyDFF_Read_Context*)handle;
    if (!ctx || !ctx->valid || frameIndex >= ctx->clump.frameCount)
        return NULL;
    return ctx->clump.frames[frameIndex].name;
}

TinyDFF_Read_Int32 TinyDFF_Read_GetFrameParent(TinyDFF_Read_Handle handle, TinyDFF_Read_UInt32 frameIndex) {
    TinyDFF_Read_Context* ctx = (TinyDFF_Read_Context*)handle;
    if (!ctx || !ctx->valid || frameIndex >= ctx->clump.frameCount)
        return -1;
    return ctx->clump.frames[frameIndex].parent;
}

bool TinyDFF_Read_HasSkin(TinyDFF_Read_Handle handle, TinyDFF_Read_UInt32 geomIndex) {
    TinyDFF_Read_Context* ctx = (TinyDFF_Read_Context*)handle;
    if (!ctx || !ctx->valid || geomIndex >= ctx->clump.geometryCount)
        return false;
    return ctx->clump.geometries[geomIndex].hasSkin;
}

TinyDFF_Read_UInt32 TinyDFF_Read_GetBoneCount(TinyDFF_Read_Handle handle, TinyDFF_Read_UInt32 geomIndex) {
    TinyDFF_Read_Context* ctx = (TinyDFF_Read_Context*)handle;
    if (!ctx || !ctx->valid || geomIndex >= ctx->clump.geometryCount)
        return 0;
    return ctx->clump.geometries[geomIndex].boneCount;
}

const TinyDFF_Read_UInt32* TinyDFF_Read_GetBoneIndices(TinyDFF_Read_Handle handle, TinyDFF_Read_UInt32 geomIndex) {
    TinyDFF_Read_Context* ctx = (TinyDFF_Read_Context*)handle;
    if (!ctx || !ctx->valid || geomIndex >= ctx->clump.geometryCount)
        return NULL;
    return ctx->clump.geometries[geomIndex].vertexBoneIndices;
}

const TinyDFF_Read_Float* TinyDFF_Read_GetBoneWeights(TinyDFF_Read_Handle handle, TinyDFF_Read_UInt32 geomIndex) {
    TinyDFF_Read_Context* ctx = (TinyDFF_Read_Context*)handle;
    if (!ctx || !ctx->valid || geomIndex >= ctx->clump.geometryCount)
        return NULL;
    return ctx->clump.geometries[geomIndex].vertexBoneWeights;
}

const TinyDFF_Read_Float* TinyDFF_Read_GetInverseMatrices(TinyDFF_Read_Handle handle, TinyDFF_Read_UInt32 geomIndex) {
    TinyDFF_Read_Context* ctx = (TinyDFF_Read_Context*)handle;
    if (!ctx || !ctx->valid || geomIndex >= ctx->clump.geometryCount)
        return NULL;
    return ctx->clump.geometries[geomIndex].inverseMatrices;
}

static void TinyDFF_Read_ClearGeometry(TinyDFF_Read_Context* ctx, TinyDFF_Read_Geometry* geom) {
    if (!geom) return;
    if (geom->vertices) ctx->callbacks.freeFn(ctx->user, geom->vertices);
    if (geom->normals) ctx->callbacks.freeFn(ctx->user, geom->normals);
    if (geom->faces) ctx->callbacks.freeFn(ctx->user, geom->faces);
    if (geom->vertexColors) ctx->callbacks.freeFn(ctx->user, geom->vertexColors);
    if (geom->nightColors) ctx->callbacks.freeFn(ctx->user, geom->nightColors);
    if (geom->twodfxData) ctx->callbacks.freeFn(ctx->user, geom->twodfxData);
    if (geom->vertexBoneIndices) ctx->callbacks.freeFn(ctx->user, geom->vertexBoneIndices);
    if (geom->vertexBoneWeights) ctx->callbacks.freeFn(ctx->user, geom->vertexBoneWeights);
    if (geom->inverseMatrices) ctx->callbacks.freeFn(ctx->user, geom->inverseMatrices);
    if (geom->materials) ctx->callbacks.freeFn(ctx->user, geom->materials);

    for (TinyDFF_Read_UInt32 i = 0; i < TINYDFF_READ_MAX_UVS; i++) {
        if (geom->texCoords[i]) ctx->callbacks.freeFn(ctx->user, geom->texCoords[i]);
    }

    if (geom->binMesh.splits) {
        for (TinyDFF_Read_UInt32 i = 0; i < geom->binMesh.splitCount; i++) {
            if (geom->binMesh.splits[i].indices)
                ctx->callbacks.freeFn(ctx->user, geom->binMesh.splits[i].indices);
        }
        ctx->callbacks.freeFn(ctx->user, geom->binMesh.splits);
    }
}

TinyDFF_Read_Handle TinyDFF_Read_CreateContext(const TinyDFF_Read_Callbacks* callbacks, void* user) {
    if (!callbacks || !callbacks->allocFn || !callbacks->freeFn ||
        !callbacks->readFn || !callbacks->seekFn || !callbacks->tellFn) {
        return NULL;
    }

    TinyDFF_Read_Context* ctx = (TinyDFF_Read_Context*)callbacks->allocFn(user, sizeof(TinyDFF_Read_Context));
    if (!ctx) return NULL;

    memset(ctx, 0, sizeof(TinyDFF_Read_Context));
    ctx->callbacks = *callbacks;
    ctx->user = user;

    return ctx;
}

void TinyDFF_Read_DestroyContext(TinyDFF_Read_Handle handle) {
    TinyDFF_Read_Context* ctx = (TinyDFF_Read_Context*)handle;
    if (!ctx) return;

    TinyDFF_Read_Reset(handle);
    ctx->callbacks.freeFn(ctx->user, ctx);
}

void TinyDFF_Read_Reset(TinyDFF_Read_Handle handle) {
    TinyDFF_Read_Context* ctx = (TinyDFF_Read_Context*)handle;
    if (!ctx) return;

    for (TinyDFF_Read_UInt32 i = 0; i < ctx->clump.geometryCount; i++) {
        TinyDFF_Read_ClearGeometry(ctx, &ctx->clump.geometries[i]);
    }
    if (ctx->clump.geometries) ctx->callbacks.freeFn(ctx->user, ctx->clump.geometries);
    if (ctx->clump.frames) ctx->callbacks.freeFn(ctx->user, ctx->clump.frames);
    if (ctx->clump.atomics) ctx->callbacks.freeFn(ctx->user, ctx->clump.atomics);
    if (ctx->clump.colData) ctx->callbacks.freeFn(ctx->user, ctx->clump.colData);

    memset(&ctx->clump, 0, sizeof(TinyDFF_Read_Clump));
    ctx->valid = false;
}

#endif // TINYDFF_READ_IMPLEMENTATION

#ifdef __cplusplus
}
#endif

#endif // TINYDFF_READ_TINYDFF_H