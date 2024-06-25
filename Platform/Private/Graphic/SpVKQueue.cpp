#include "Graphic/SpVKQueue.h"

namespace spe{
    SpVKQueue::SpVKQueue(uint32_t queueFamilyIndex, uint32_t index, VkQueue queue, bool canPresent)
    :mFamilyIndex(queueFamilyIndex), mIndex(index), mQueue(queue), mCanPresent(canPresent){
        SpLogTrace("Create a new queue: {0} - {1} - {2}, present: {3}", mFamilyIndex, mIndex, (void*)mQueue, mCanPresent);
    }

    void SpVKQueue::WaitIdle() const {
        CALL_VK(vkQueueWaitIdle(mQueue));
    }

    SpVKQueue::~SpVKQueue() {

    }
}