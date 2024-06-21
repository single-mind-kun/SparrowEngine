#ifndef SP_GRAPHIC_CONTEXT_H
#define SP_GRAPHIC_CONTEXT_H

#include "SpEngine.h"

namespace spe{
    class SpWindow;
    class SpGraphicContext{
    public:
        SpGraphicContext(const SpGraphicContext&) = delete;
        SpGraphicContext& operator=(const SpGraphicContext&) = delete;

        virtual ~SpGraphicContext() = default;
        static std::unique_ptr<SpGraphicContext> Create(SpWindow* window);
    protected:
        SpGraphicContext() = default;
    };
}

#endif
