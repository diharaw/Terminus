#pragma once

#ifndef MESHFACTORY_H
#define MESHFACTORY_H

#include "Mesh.h"
#include <string>

class MeshFactory 
{
public:
	MeshFactory();
	~MeshFactory();
	Mesh* Create(asset_common::MeshLoadData* _Data);
};

#endif