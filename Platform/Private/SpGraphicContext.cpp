#include "SpGraphicContext.h"
#include "Graphic/SpVKGraphicContext.h"

namespace spe{
    std::unique_ptr<SpGraphicContext> SpGraphicContext::Create(SpWindow *window) {
#ifdef SP_ENGINE_GRAPHIC_API_VULKAN
    return std::make_unique<spe::SpVKGraphicContext>(window);
#else

#endif

        return nullptr;
    }
}