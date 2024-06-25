#ifndef SP_VK_QUEUE_H
#define SP_VK_QUEUE_H

#include "SpVKCommon.h"

namespace spe{
    class SpVKQueue{
    public:
        SpVKQueue(uint32_t queueFamilyIndex, uint32_t index, VkQueue queue, bool canPresent);

        void WaitIdle() const;

        ~SpVKQueue();
    private:
        uint32_t mFamilyIndex;
        uint32_t mIndex;
        VkQueue mQueue;
        bool mCanPresent;
    };
}

#endif