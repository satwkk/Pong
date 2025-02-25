#pragma once

#include "shader.h"
#include "pch.h"
#include "dynlist.h"


typedef struct {
    void* tmp;
} resource_container;

int load_resources(resource_container* resources);
void cleanup_resources(resource_container* resources);