#pragma once

/*
MIT License

Copyright (c) 2014-2024 Stephane Cuillerdier (aka aiekick)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

// EzGraph is part od the EzLibs project : https://github.com/aiekick/EzLibs.git

#include <set>
#include <vector>
#include <string>
#include <memory>
#include <cassert>
#include <functional>

namespace ez {

/////////////////////////////////////
///// Utils /////////////////////////
/////////////////////////////////////

namespace Utils {

// if the shared_ptr exit in the container return the iterator
template <typename T>
typename std::vector<std::shared_ptr<T>>::iterator  //
isSharedPtrExistInVector(std::shared_ptr<T> vPtr, std::vector<std::shared_ptr<T>>& vContainer) {
    auto ret = vContainer.end();
    if (vPtr != nullptr) {
        ret = vContainer.begin();
        for (; ret != vContainer.end(); ++ret) {
            if (*ret == vPtr) {
                break;
            }
        }
    }
    return ret;
}

// if the weak_ptr exit in the container return the iterator
template <typename T>
typename std::vector<std::weak_ptr<T>>::iterator  //
isWeakPtrExistInVector(std::weak_ptr<T> vWeak, std::vector<std::weak_ptr<T>>& vContainer) {
    auto ret = vContainer.end();
    if (!vWeak.expired()) {
        auto ptr = vWeak.lock();
        ret = vContainer.begin();
        for (; ret != vContainer.end(); ++ret) {
            if (ret->lock() == ptr) {
                break;
            }
        }
    }
    return ret;
}

}  // namespace Utils

/////////////////////////////////////
///// DEFS //////////////////////////
/////////////////////////////////////

enum class SlotDir {  //
    INPUT = 0,
    OUTPUT,
    Count
};

enum class RetCodes {  //
    SUCCESS = 0,
    FAILED,
    FAILED_SLOT_PTR_NULL,
    FAILED_SLOT_ALREADY_EXIST,
    FAILED_SLOT_NOT_FOUND,
    FAILED_NODE_PTR_NULL,
    FAILED_NODE_ALREADY_EXIST,
    FAILED_NODE_NOT_FOUND,
    FAILED_NODE_NO_FUNCTOR,
    Count
};

class Slot;
typedef std::shared_ptr<Slot> SlotPtr;
typedef std::weak_ptr<Slot> SlotWeak;

class Node;
typedef std::shared_ptr<Node> NodePtr;
typedef std::weak_ptr<Node> NodeWeak;
typedef std::function<RetCodes(const size_t, const std::vector<SlotPtr>&, const SlotWeak&)> NodeFunctor;

typedef uintptr_t Uuid;

typedef void* UserDatas;

/////////////////////////////////////
///// UUID //////////////////////////
/////////////////////////////////////

class UUID {
private:
    Uuid m_Uuid = 0U;

public:
    UUID(void* vPtr) {
        m_Uuid = (Uuid)vPtr;
    }
    virtual ~UUID() = default;
    Uuid getUUID()const {
        return m_Uuid;
    }

    void setUUID(Uuid vUUID) {
        m_Uuid = vUUID;
    }
};

/////////////////////////////////////
///// SLOT //////////////////////////
/////////////////////////////////////

struct SlotDatas {
    std::string name;
    std::string type;
    UserDatas userDatas = nullptr;
    SlotDir dir = SlotDir::INPUT;
};

struct EvalDatas {
    size_t frame = 0U;
};

class Slot : public UUID {
    friend class Node;

protected:
    SlotWeak m_This;
    NodeWeak m_ParentNode;
    SlotDatas m_SlotDatas;
    std::vector<SlotWeak> m_ConnectedSlots;
    EvalDatas m_LastEvaluatedDatas;

public:
    Slot() : UUID(this) {
    }
    virtual ~Slot() = default;

    virtual bool initSlot(const SlotDatas& vDatas, const SlotWeak& vThis) {
        m_SlotDatas = vDatas;
        m_This = vThis;
        return true;
    }
    virtual void unitSlot() {
        m_SlotDatas = {};
        m_ConnectedSlots.clear();
    }
    const SlotDatas& getSlotDatas() {
        return m_SlotDatas;
    }
    SlotDatas& getSlotDatasRef() {
        return m_SlotDatas;
    }
    void setParentNode(NodeWeak vNodeWeak) {
        m_ParentNode = vNodeWeak;
    }
    NodeWeak getParentNode() {
        return m_ParentNode;
    }
    const std::vector<SlotWeak>& m_getConnectedSlots() {
        return m_ConnectedSlots;
    }
    void setLastEvaluatedDatas(const EvalDatas vUserDatas) {
        m_LastEvaluatedDatas = vUserDatas;
    }
    const EvalDatas& getLastEvaluatedDatas() {
        return m_LastEvaluatedDatas;
    }

protected:
    RetCodes m_connectSlot(SlotWeak vSlot) {
        RetCodes ret = RetCodes::FAILED_SLOT_PTR_NULL;
        if (!vSlot.expired()) {
            m_ConnectedSlots.push_back(vSlot);
            ret = RetCodes::SUCCESS;
        }
        return ret;
    }
    RetCodes m_disconnectSlot(SlotWeak vSlot) {
        RetCodes ret = RetCodes::FAILED_SLOT_ALREADY_EXIST;
        auto it = Utils::isWeakPtrExistInVector(vSlot, m_ConnectedSlots);
        if (it != m_ConnectedSlots.end()) {
            m_ConnectedSlots.erase(it);
            ret = RetCodes::SUCCESS;
        }
        return ret;
    }
    void m_disconnect() {
        m_ConnectedSlots.clear();
    }
};

/////////////////////////////////////
///// NODE / GRAPH //////////////////
/////////////////////////////////////

struct NodeDatas {
    std::string name;
    std::string type;
    bool dirty = false;
    UserDatas userDatas = nullptr;
    NodeFunctor functor = nullptr;
};

class Node : public UUID {
private:
    NodeWeak m_This;
    NodeWeak m_ParentNode;
    NodeDatas m_NodeDatas;
    std::vector<NodePtr> m_Nodes;
    std::vector<SlotPtr> m_Inputs;
    std::vector<SlotPtr> m_Outputs;

public:
    Node() : UUID(this) {
    }
    virtual ~Node() = default;

    // CTor / DTor
    virtual bool initNode(const NodeDatas& vNodeDatas, const NodeWeak& vThis) {
        m_NodeDatas = vNodeDatas;
        m_This = vThis;
        return true;
    }
    virtual void unitNode() {
        m_NodeDatas = {};
        m_Inputs.clear();
        m_Outputs.clear();
    }

public:  // Eval
    virtual RetCodes eval(const size_t vFrame, const SlotWeak& vFrom) {
        RetCodes ret = RetCodes::FAILED_NODE_NO_FUNCTOR;
        if (m_NodeDatas.functor != nullptr) {
            ret = m_NodeDatas.functor(vFrame, m_Inputs, vFrom);
        }
        return ret;
    }

public:  // Datas
    void setParentNode(const NodeWeak& vParentNode) {
        m_ParentNode = vParentNode;
    }
    NodeWeak getParentNode() {
        return m_ParentNode;
    }
    const NodeDatas& getNodeDatas() {
        return m_NodeDatas;
    }
    NodeDatas& NodeDatasRef() {
        return m_NodeDatas;
    }
    const std::vector<NodePtr>& getNodes() const {
        return m_Nodes;
    }
    std::vector<NodePtr>& getNodesRef() {
        return m_Nodes;
    }
    NodeWeak m_getThis() {
        return m_This;
    }
    void setDirty(bool vFlag) {
        m_NodeDatas.dirty = vFlag;
    }
    bool isDirty() {
        return m_NodeDatas.dirty;
    }

protected:  // Node
    RetCodes m_addNode(NodePtr vNodePtr) {
        RetCodes ret = RetCodes::FAILED_NODE_PTR_NULL;
        if (vNodePtr != nullptr) {
            vNodePtr->setParentNode(m_This);
            m_Nodes.push_back(vNodePtr);
            ret = RetCodes::SUCCESS;
        }
        return ret;
    }
    RetCodes m_delNode(NodePtr vNodePtr) {
        RetCodes ret = RetCodes::FAILED_NODE_NOT_FOUND;
        auto it = Utils::isSharedPtrExistInVector(vNodePtr, m_Nodes);
        if (it != m_Nodes.end()) {
            (*it)->unitNode();
            m_Nodes.erase(it);
            ret = RetCodes::SUCCESS;
        }
        return ret;
    }

protected:  // Slots
    RetCodes m_addSlot(SlotPtr vSlotPtr) {
        RetCodes ret = RetCodes::FAILED;
        if (vSlotPtr != nullptr) {
            const auto& datas = vSlotPtr->getSlotDatas();
            if (datas.dir == SlotDir::INPUT) {
                auto it = Utils::isSharedPtrExistInVector(vSlotPtr, m_Inputs);
                if (it == m_Inputs.end()) {
                    vSlotPtr->setParentNode(m_This);
                    m_Inputs.push_back(vSlotPtr);
                    ret = RetCodes::SUCCESS;
                } else {
                    ret = RetCodes::FAILED_SLOT_ALREADY_EXIST;
                }
            } else if (datas.dir == SlotDir::OUTPUT) {
                auto it = Utils::isSharedPtrExistInVector(vSlotPtr, m_Outputs);
                if (it == m_Outputs.end()) {
                    vSlotPtr->setParentNode(m_This);
                    m_Outputs.push_back(vSlotPtr);
                    ret = RetCodes::SUCCESS;
                } else {
                    ret = RetCodes::FAILED_SLOT_ALREADY_EXIST;
                }
            }
        }
        return ret;
    }
    SlotPtr m_addSlot(const SlotDatas& vSlotDatas, RetCodes* vOutRetCodes) {
        if (vOutRetCodes != nullptr) {
            *vOutRetCodes = RetCodes::FAILED_NODE_PTR_NULL;
        }
        auto slot_ptr = std::make_shared<Slot>();
        if (!slot_ptr->initSlot(vSlotDatas, slot_ptr)) {
            slot_ptr.reset();
        } else {
            auto ret_code = m_addSlot(slot_ptr);
            if (vOutRetCodes != nullptr) {
                *vOutRetCodes = ret_code;
            }
        }
        return slot_ptr;
    }
    RetCodes m_delSlot(SlotPtr vSlotPtr) {
        RetCodes ret = RetCodes::FAILED_SLOT_NOT_FOUND;
        ret = m_delInputSlot(vSlotPtr);
        if (ret != RetCodes::SUCCESS) {
            ret = m_delOutputSlot(vSlotPtr);
        }
        return ret;
    }
    RetCodes m_connectSlots(SlotWeak vFrom, SlotWeak vTo) {
        RetCodes ret = RetCodes::FAILED_SLOT_PTR_NULL;
        if (!vFrom.expired() && !vTo.expired()) {
            auto fromPtr = vFrom.lock();
            auto toPtr = vTo.lock();
            if (fromPtr != nullptr && toPtr != nullptr) {
                ret = fromPtr->m_connectSlot(vTo);
                if (ret == RetCodes::SUCCESS) {
                    ret = toPtr->m_connectSlot(vFrom);
                    if (ret != RetCodes::SUCCESS) {
                        fromPtr->m_connectSlot(vTo);
                    }
                }
            }
        }
        return ret;
    }
    RetCodes m_disconnectSlot(SlotWeak vFrom) {
        RetCodes ret = RetCodes::FAILED_SLOT_NOT_FOUND;
        // INPUTS
        auto it = Utils::isSharedPtrExistInVector(vFrom.lock(), m_Inputs);
        if (it != m_Inputs.end()) {
            (*it)->m_disconnect();
            ret = RetCodes::SUCCESS;
        }
        // OUTPUTS
        it = Utils::isSharedPtrExistInVector(vFrom.lock(), m_Outputs);
        if (it != m_Outputs.end()) {
            assert(ret != RetCodes::SUCCESS);  // if the slot exist both in INPUTS and PUTPUTS. => FAIL
            (*it)->m_disconnect();
            ret = RetCodes::SUCCESS;
        }
        if (ret != RetCodes::SUCCESS) {
            auto fromPtr = vFrom.lock();
            if (fromPtr != nullptr) {
                fromPtr->m_disconnect();
            }
        }
        return ret;
    }
    RetCodes m_disconnectSlots(SlotWeak vFrom, SlotWeak vTo) {
        RetCodes ret = RetCodes::FAILED_SLOT_PTR_NULL;
        if (!vFrom.expired() && !vTo.expired()) {
            auto fromPtr = vFrom.lock();
            auto toPtr = vTo.lock();
            if (fromPtr != nullptr && toPtr != nullptr) {
                ret = fromPtr->m_disconnectSlot(vTo);
                // meme si le dernier est en erreur on tente la deco
                ret = toPtr->m_disconnectSlot(vFrom);
            }
        }
        return ret;
    }

protected:
    RetCodes m_delInputSlot(SlotPtr vSlotPtr) {
        RetCodes ret = RetCodes::FAILED_SLOT_NOT_FOUND;
        auto it = Utils::isSharedPtrExistInVector(vSlotPtr, m_Inputs);
        if (it != m_Inputs.end()) {
            (*it)->unitSlot();
            m_Inputs.erase(it);
            ret = RetCodes::SUCCESS;
        }
        return ret;
    }
    RetCodes m_delOutputSlot(SlotPtr vSlotPtr) {
        RetCodes ret = RetCodes::FAILED_SLOT_NOT_FOUND;
        auto it = Utils::isSharedPtrExistInVector(vSlotPtr, m_Outputs);
        if (it != m_Outputs.end()) {
            (*it)->unitSlot();
            m_Outputs.erase(it);
            ret = RetCodes::SUCCESS;
        }
        return ret;
    }
    std::vector<SlotPtr>& m_getInputsRef() {
        return m_Inputs;
    }
    std::vector<SlotPtr>& m_getOutputsRef() {
        return m_Outputs;
    }
};

}  // namespace ez
