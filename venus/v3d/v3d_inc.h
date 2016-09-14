#pragma once

#if defined _WINDLL && !defined VENUS_NO_EXPORT
#define V3D_API __declspec (dllexport)
#else
#define V3D_API
#endif

#include "v3d_enum.h"
#include "v3d_def.h"
#include "v3d_api.h"

#include "math/intersect.h"
#include "math/ray.h"

#include "math/line.h"
#include "math/bbox.h"
#include "math/plane.h"
#include "math/view_frustum.h"
#include "math/cbuffer.h"
#include "math/v3d_algorithm.h"

#include "engine/XmlParser.h"
#include "engine/Engine.h"
#include "engine/C3DNode.h"
#include "engine/C3DEntity.h"
#include "engine/C3DTerrain.h"

#include "engine/C3DMesh.h"
#include "engine/C3DFile.h"
#include "engine/C3DEffect.h"

#include "engine/C3DFileDriver.h"
#include "engine/C3DExplorer.h"
#include "engine/C3DScene.h"
#include "engine/C3DCameraFly.h"
#include "engine/C3DCameraFP.h"
#include "engine/C3DMaterial.h"

VENUS_BEG

V3D_API I3DFileDriver * Get3DFileDriver();
V3D_API I3DVedioDriver * Get3DVedioDriver();

VENUS_END

