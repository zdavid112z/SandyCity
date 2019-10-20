#pragma once

#include "basicio.h"

namespace ducky { namespace io {

	class Serializable
	{
	public:
		virtual void Read(IStream*) = 0;
		virtual void Write(OStream*) = 0;
	};

} }