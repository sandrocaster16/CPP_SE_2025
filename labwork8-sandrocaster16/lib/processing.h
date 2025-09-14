#pragma once

#include "adapters/BaseAdapter.h"
#include "adapters/Dir.h"               //start
#include "adapters/Filter.h"
#include "adapters/OpenFiles.h"
#include "adapters/Out.h"               //terminate
#include "adapters/Split.h"
#include "adapters/AsDataFlow.h"        //start
#include "adapters/Transform.h"
#include "adapters/Write.h"             //terminate
#include "adapters/AggregateByKey.h"
#include "adapters/AsVector.h"          //terminate
#include "adapters/Join.h"
#include "adapters/DropNullopt.h"
#include "adapters/SplitExpected.h"
#include "adapters/JoinResult.h"
#include "adapters/KV.h"