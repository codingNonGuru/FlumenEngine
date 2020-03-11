#pragma once

#include <GL/glew.h>

enum class CameraTypes {ORTHO, PERSPECTIVE};

enum class AttributeElementTypes
{
	INTEGER_1, INTEGER_2, INTEGER_3, INTEGER_4,
	FLOAT_1, FLOAT_2, FLOAT_3, FLOAT_4,
	UINTEGER_1, UINTEGER_2, UINTEGER_3, UINTEGER_4,
	MATRIX_3, MATRIX_4
};

enum class MeshAttributeTypes
{
	POSITION,
	VERTEX_NORMAL,
	FACE_NORMAL,
	TEXTURE_COORDS,
	WEIGHT,
	COLOR
};

enum class ShaderTypes
{
	VERTEX = GL_VERTEX_SHADER,
	FRAGMENT = GL_FRAGMENT_SHADER,
	GEOMETRY = GL_GEOMETRY_SHADER,
	TESSELLATION_CONTROL = GL_TESS_CONTROL_SHADER,
	TESSELLATION_EVALUATION = GL_TESS_EVALUATION_SHADER,
	COMPUTE = GL_COMPUTE_SHADER
};

enum class FrameBufferTypes
{
	SINGLE_SAMPLE,
	MULTI_SAMPLE,
};

enum class FrameBufferAttachments
{
	COLOR,
	DEPTH,
	COLOR_AND_DEPTH
};

enum class TextureFormats
{
	FOUR_FLOAT,
	FOUR_BYTE,
	THREE_FLOAT,
	THREE_BYTE,
	ONE_FLOAT,
	ONE_BYTE,
	ONE_INTEGER,
	DEPTH_FLOAT,
	DEPTH_BYTE
};

enum class ImageFormats
{
	RGB,
	RGBA,
	ALPHA
};

enum class AnimationValueTypes
{
	FLOAT,
	DOUBLE,
	INTEGER,
	LONG,
	BOOLEAN,
	NONE
};

enum class InterfaceElementParameters
{
	POSITION_X,
	POSITION_Y,
	SCALE_X,
	SCALE_Y,
	SIZE_X,
	SIZE_Y,
	DEPTH,
	OPACITY
};

enum class KernelTypes
{
	GAUSS,
	CAUCHY,
	LAPLACE
};

enum class States;
